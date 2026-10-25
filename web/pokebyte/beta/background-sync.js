/* PokéByte integrated beta installer, firmware -> original scenery setup.
 * The webpage fetches the exact approved scene bundle automatically.
 * ESP Web Tools flashes firmware; this controller then reconnects the device
 * (with already-granted browser permission where possible) and writes missing
 * original BMPs through the non-overwriting PBCAPS/PBCRC/PBPUT protocol.
 * No separate background-installer action, ZIP download or SD card reader.
 */
const pb$=id=>document.getElementById(id);
const PB={files:null,manifest:null,spriteManifest:null,ready:false,busy:false,flashStarted:false,complete:false,
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
  // The first-install sprite manifest is assembled and fingerprinted in CI
  // from the 303 original .bin files. Only missing approved sprites will
  // subsequently be downloaded/uploaded, so existing cards stay fast.
  const spriteRes=await fetch("./original-sprites.json",{cache:"no-store"});
  if(!spriteRes.ok)throw Error("Original 303 Pokémon sprite manifest has not been published");
  const spriteManifest=await spriteRes.json();
  if(spriteManifest.count!==303||
     spriteManifest.combinedSha256!=="45624145dab29c46c045c44fea7625884e25507af9d6cac352c317345286294c"||
     spriteManifest.files?.length!==303)
    throw Error("Original Pokémon sprite manifest does not match the accepted PMD set");
  const seen=new Set();
  for(const pm of spriteManifest.files){
    if(!(/^(p\d{3}|ps\d{3}|thumbs)\.bin$/.test(pm.name))||
       pm.path!=="/mons/"+pm.name||seen.has(pm.name)||
       !Number.isInteger(pm.bytes)||pm.bytes<=0||pm.bytes>1024*1024||
       !/^[0-9a-f]{64}$/.test(pm.sha256)||!/^[0-9a-f]{8}$/.test(pm.crc32))
      throw Error("Invalid original Pokémon asset manifest: "+pm.name);
    seen.add(pm.name);
  }
  PB.files=files;PB.manifest=m;PB.spriteManifest=spriteManifest;
  PB.ready=true;PB.error=null;
  pbProgress("12 original landscapes + 303 authentic Pokémon files verified in the release manifest. Ready to install.");
  pbLog("Official BMPs verified byte-for-byte; original 303 PMD sprite manifest verified. Existing SD files will be skipped.");
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
    // Same installation progress continues with the authentic PMD sprites.
    // If the owner's card already has them, no download/write is performed.
    let spritesInstalled=0,spritesSkipped=0;
    for(const art of PB.spriteManifest.files){
      pbProgress("Checking original Pokémon sprite files "+(spritesInstalled+spritesSkipped+1)+"/303");
      const existing=await pbReadCrc(dev,art.path);
      if(existing){
        if(existing.size!==art.bytes||existing.crc32!==art.crc32)
          throw Error("Different existing Pokémon sprite "+art.name+"; refusing overwrite. Your original file stays intact.");
        spritesSkipped++;
        continue;
      }
      pbProgress("Downloading approved missing sprite "+art.name+" ("+(spritesInstalled+spritesSkipped+1)+"/303)");
      const resp=await fetch("./mons/"+art.name,{cache:"no-store"});
      if(!resp.ok)throw Error("Original sprite is missing from the official website: "+art.name);
      const bytes=new Uint8Array(await resp.arrayBuffer());
      if(bytes.length!==art.bytes||pbCrc32(bytes)!==art.crc32||
         await pbSha256(bytes)!==art.sha256)
        throw Error("Host-provided Pokémon sprite differs from the approved original: "+art.name);
      await pbInstallOne(dev,art,bytes);
      spritesInstalled++;
      pbLog("Original PMD installed and verified: "+art.name);
    }
    PB.complete=true;
    pbProgress("PokéByte installation complete: "+copied+" backgrounds + "+spritesInstalled+
      " Pokémon files installed; "+(skipped+spritesSkipped)+" existing files verified. Restart your Waveshare.");
    pbLog("SUCCESS — all 12 approved scenes and all 303 original Pokémon files verified. No existing file overwritten.");
    pbResume(false);
  }finally{
    try{await dev?.close();}catch{}
    PB.busy=false;
  }
}
async function pbAutomaticFinish(){
  if(PB.busy||PB.complete)return;
  if(!PB.ready){pbProgress("Cannot finish setup: approved scenery/sprites not hosted. No SD changes made.");return;}
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
    pbProgress("Firmware installer opened. Approved scenery and original Pokémon sprites are part of this same setup.");
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
