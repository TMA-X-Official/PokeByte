/* PokéByte integrated beta installer, firmware -> original scenery setup.
 * The webpage fetches the exact approved scene bundle automatically.
 * ESP Web Tools flashes firmware; this controller then reconnects the device
 * (with already-granted browser permission where possible) and writes missing
 * original BMPs through the non-overwriting PBCAPS/PBCRC/PBPUT protocol.
 * No separate background-installer action, ZIP download or SD card reader.
 */
const pb$=id=>document.getElementById(id);
const PB={files:null,manifest:null,ready:false,busy:false,flashStarted:false,complete:false,
  error:null,port:null,device:null};
const pbEncoder=new TextEncoder();
const pbDecoder=new TextDecoder();
const crcTable=(()=>{const a=new Uint32Array(256);for(let n=0;n<256;++n){let c=n;for(let j=0;j<8;++j)c=(c>>>1)^((c&1)?0xedb88320:0);a[n]=c>>>0;}return a;})();
function pbCrc32(buffer){
  let crc=0xffffffff;
  for(const b of buffer)crc=(crc>>>8)^crcTable[(crc^b)&255];
  return ((~crc)>>>0).toString(16).padStart(8,"0");
}
function pbHex(data){return Array.from(new Uint8Array(data),b=>b.toString(16).padStart(2,"0")).join("");}
async function pbSha256(data){return pbHex(await crypto.subtle.digest("SHA-256",data));}
function pbLog(message){
  const el=pb$("setupLog");
  if(el){el.textContent+=String(message)+"\n";el.scrollTop=el.scrollHeight;}
}
function pbProgress(message){
  const el=pb$("setupStatus");if(el)el.textContent=message;
}
function pbResume(enabled,text){
  const el=pb$("resumeSetup");
  if(!el)return;
  el.hidden=!enabled;
  if(text)el.textContent=text;
}
function pbDispatchReady(){window.dispatchEvent(new CustomEvent("pokebyte-scenes-ready",{detail:{ready:PB.ready}}));}
async function pbLoadArt(){
  pbProgress("Preparing the official approved scenery pack (12 files)...");
  const res=await fetch("./approved-backgrounds.json",{cache:"no-store"});
  if(!res.ok)throw Error("Approved scenery manifest unavailable on this website");
  const m=await res.json();
  if(m.files?.length!==12||!m.approvedBundleZipSHA256)throw Error("Incomplete approved art manifest");
  const zip=await fetch("./approved-backgrounds.zip",{cache:"no-store"});
  if(!zip.ok)throw Error("The 12 original approved backgrounds have not been published to GitHub Pages yet");
  const raw=await zip.arrayBuffer();
  if(await pbSha256(raw)!==m.approvedBundleZipSHA256)
    throw Error("Website artwork package does not match the owner-approved original bundle");
  if(!window.fflate?.unzipSync)throw Error("ZIP decoder unavailable. Reload the installer in Chrome/Edge");
  const entries=window.fflate.unzipSync(new Uint8Array(raw));
  const files=new Map();
  for(const art of m.files){
    const keys=Object.keys(entries).filter(name=>name==="assets/"+art.name);
    if(keys.length!==1)throw Error("Missing or duplicate approved artwork: "+art.name);
    const bytes=entries[keys[0]];
    if(bytes.length!==art.bytes||pbCrc32(bytes)!==art.crc32||
       await pbSha256(bytes)!==art.sha256)
      throw Error("Wrong background bytes; refused: "+art.name);
    files.set(art.name,bytes);
  }
  PB.files=files;PB.manifest=m;PB.ready=true;PB.error=null;
  pbProgress("Approved background bundle verified (12/12). Ready for the full PokéByte installation.");
  pbLog("Official hosted BMPs verified byte-for-byte. Original sprites on microSD will not be touched.");
  pbDispatchReady();
}
class PBDeviceSerial{
  constructor(port){this.port=port;this.decoder=new TextDecoder();this.lines=[];this.waiting=null;this.buf="";this.closed=false;}
  async open(){
    await this.port.open({baudRate:115200,bufferSize:8192});
    this.reader=this.port.readable.getReader();
    this.writer=this.port.writable.getWriter();
    this.pump=this.loop();
  }
  async loop(){
    try{
      for(;;){
        const {value,done}=await this.reader.read();
        if(done)break;
        this.buf+=this.decoder.decode(value,{stream:true});
        let i;
        while((i=this.buf.indexOf("\n"))!==-1){
          const line=this.buf.slice(0,i).trim();
          this.buf=this.buf.slice(i+1);
          if(!line)continue;
          if(this.waiting){const w=this.waiting;this.waiting=null;clearTimeout(w.timer);w.resolve(line);}
          else this.lines.push(line);
          if(this.lines.length>150)this.lines.splice(0,100);
        }
      }
    }catch(e){if(!this.closed)pbLog("USB disconnected: "+e.message);}
    finally{
      this.closed=true;
      if(this.waiting){const w=this.waiting;this.waiting=null;clearTimeout(w.timer);w.reject(Error("USB connection closed"));}
    }
  }
  async line(timeout=15000){
    if(this.lines.length)return this.lines.shift();
    if(this.closed)throw Error("USB connection closed");
    return new Promise((resolve,reject)=>{
      const timer=setTimeout(()=>{if(this.waiting){this.waiting=null;reject(Error("USB response timed out"));}},timeout);
      this.waiting={resolve,reject,timer};
    });
  }
  async until(match,timeout=15000){
    const deadline=Date.now()+timeout;
    while(Date.now()<deadline){
      const line=await this.line(Math.max(1,deadline-Date.now()));
      if(line.startsWith("ERR")||line==="EXISTS")
        throw Error("Device refused operation: "+line);
      if(match(line))return line;
    }
    throw Error("Device did not respond");
  }
  async command(cmd){await this.writer.write(pbEncoder.encode(cmd+"\n"));}
  async close(){
    this.closed=true;
    try{await this.reader?.cancel();}catch{}
    try{this.reader?.releaseLock();}catch{}
    try{this.writer?.releaseLock();}catch{}
    try{await this.port.close();}catch{}
  }
}
async function pbReadCrc(dev,path){
  await dev.command("PBCRC "+path);
  const line=await dev.until(l=>l==="MISSING"||l.startsWith("CRC "),20000);
  if(line==="MISSING")return null;
  const match=line.match(/^CRC (\d+) ([a-fA-F0-9]{8})$/);
  if(!match)throw Error("Unrecognized device checksum response");
  await dev.until(l=>l==="DONE",15000);
  return {size:Number(match[1]),crc32:match[2].toLowerCase()};
}
async function pbInstallOne(dev,art,bytes){
  await dev.command("PBPUT "+art.path+" "+art.bytes+" "+art.crc32.toUpperCase());
  await dev.until(l=>l==="OK",15000);
  for(let off=0;off<bytes.length;off+=2048){
    await dev.writer.write(bytes.subarray(off,Math.min(off+2048,bytes.length)));
    // Firmware now acks once per complete 2048-byte frame, not short USB packets.
    await dev.until(l=>l==="#",20000);
    if(off%(2048*16)===0)pbProgress("Installing "+art.name+" — "+Math.round(off*100/bytes.length)+"%");
  }
  await dev.until(l=>l==="DONE",15000);
  const verified=await pbReadCrc(dev,art.path);
  if(!verified||verified.size!==art.bytes||verified.crc32!==art.crc32)
    throw Error("Device verification failed for "+art.name);
}
async function pbSyncWithPort(port){
  if(PB.busy||PB.complete)return;
  if(!PB.ready)throw Error("Approved artwork not yet hosted or verified. No SD changes made.");
  PB.busy=true;pbResume(false);
  let dev;
  try{
    pbProgress("Connecting to PokéByte after firmware installation...");
    dev=new PBDeviceSerial(port);
    await dev.open();
    // Native USB CDC may restart the board; let SD_MMC mount first.
    await new Promise(resolve=>setTimeout(resolve,1900));
    await dev.command("PBCAPS");
    await dev.until(l=>l.startsWith("POKEBYTE-ASSETS/1"),15000);
    await dev.until(l=>l==="DONE",15000);
    let copied=0,skipped=0;
    for(const art of PB.manifest.files){
      pbProgress("Checking approved scenes "+(copied+skipped+1)+"/12: "+art.name);
      const existing=await pbReadCrc(dev,art.path);
      if(existing){
        if(existing.size!==art.bytes||existing.crc32!==art.crc32)
          throw Error("An existing SD file differs from approved "+art.name+". Refusing to overwrite it.");
        skipped++;pbLog("Existing original verified: "+art.name);continue;
      }
      pbLog("Copying missing original BMP "+art.name);
      await pbInstallOne(dev,art,PB.files.get(art.name));
      copied++;pbLog("Installed and verified: "+art.name);
    }
    PB.complete=true;
    pbProgress("PokéByte setup complete: "+copied+" backgrounds installed, "+skipped+" verified. Restart the Waveshare to show the new scenery.");
    pbLog("SUCCESS — original microSD sprites preserved. No existing file overwritten.");
    pbResume(false);
  }finally{
    try{await dev?.close();}catch{}
    PB.busy=false;
  }
}
async function pbAutomaticFinish(){
  if(PB.busy||PB.complete)return;
  if(!PB.ready){pbProgress("Cannot finish setup: approved scenery not hosted. No SD changes made.");return;}
  if(!navigator.serial){pbProgress("Use desktop Chrome or Edge with Web Serial support.");return;}
  // Run only after ESP Web Tools has released the port.
  await new Promise(resolve=>setTimeout(resolve,1400));
  const granted=await navigator.serial.getPorts();
  if(granted.length===1){
    try{
      await pbSyncWithPort(granted[0]);
      return;
    }catch(e){pbLog("Automatic reconnect could not finish: "+e.message);}
  }
  pbProgress("Firmware window closed. To finish this same installation, reconnect your Waveshare using the browser's USB permission dialog.");
  pbResume(true,"Continue PokéByte installation");
}
async function pbUserFinish(){
  if(PB.busy||PB.complete)return;
  if(!PB.ready){pbProgress("Original artwork must be available before continuing.");return;}
  try{
    // Web Serial's first-time chooser requires a *direct user click*.
    const port=await navigator.serial.requestPort();
    await pbSyncWithPort(port);
  }catch(e){
    pbLog("Setup paused without overwriting SD content: "+e.message);
    pbProgress("Setup not completed: "+e.message);
    pbResume(true,"Retry PokéByte setup");
  }
}
function pbInit(){
  pb$("resumeSetup")?.addEventListener("click",pbUserFinish);
  pb$("install")?.addEventListener("click",()=>{
    PB.flashStarted=true;pbResume(false);
    pbProgress("Firmware installer opened. Complete flashing; approved backgrounds follow automatically.");
  });
  // ESP Web Tools fires 'closed' on its install dialog, after both success
  // and cancellation. Never infer success from this; PBCAPS verifies that
  // PokéByte firmware actually booted before we access the microSD.
  document.addEventListener("closed",event=>{
    if(event.target?.tagName==="EWT-INSTALL-DIALOG" && PB.flashStarted){
      PB.flashStarted=false;
      pbAutomaticFinish().catch(e=>{
        pbLog("Unable to reconnect after flashing: "+e.message);
        pbProgress("Firmware setup could not be verified. No scenery changed.");
        pbResume(true,"Continue PokéByte installation");
      });
    }
  },true);
  pbLoadArt().catch(e=>{
    PB.ready=false;PB.error=e.message;
    pbProgress("Installation preparation stopped: "+e.message);
    pbLog("ERROR: "+e.message);
    pbDispatchReady(); // disables new installation until assets hosted
  });
}
if(document.readyState==="loading")document.addEventListener("DOMContentLoaded",pbInit,{once:true});
else pbInit();
