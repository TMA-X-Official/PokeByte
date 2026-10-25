/* PokéByte Phase 4: owner-approved scene installer via browser Web Serial.
   No SD reader or desktop utility; never formats or overwrites existing files.
   Hosted bundle preferred; until approved art is published, choose the owner's
   exact Phase 3 approved Windows ZIP once using the input in the same website.
   Firmware PBCAPS/PBCRC/PBPUT/1 required. */
const $=id=>document.getElementById(id);
const state={files:null,source:"",busy:false};
const encoder=new TextEncoder();
const decoder=new TextDecoder();
let serial=null;
function log(message){
  const node=$("assetLog");
  node.textContent+=String(message)+"\n";
  node.scrollTop=node.scrollHeight;
}
function bytesHex(data){return Array.from(new Uint8Array(data),b=>b.toString(16).padStart(2,"0")).join("");}
async function digest(data){return bytesHex(await crypto.subtle.digest("SHA-256",data));}
const TABLE=(()=>{const a=new Uint32Array(256);for(let n=0;n<256;++n){let c=n;for(let j=0;j<8;++j)c=(c>>>1)^((c&1)?0xedb88320:0);a[n]=c>>>0;}return a})();
function crc32(buffer){let crc=0xffffffff;for(const b of buffer)crc=(crc>>>8)^TABLE[(crc^b)&255];return ((~crc)>>>0).toString(16).padStart(8,"0");}
async function unzipApproved(raw,source,manifest){
  if(!window.fflate?.unzipSync)throw Error("ZIP decoder unavailable. Reload the page in Chrome or Edge.");
  const files=window.fflate.unzipSync(new Uint8Array(raw));
  const wanted=manifest.files;
  const found=new Map();
  for(const entry of wanted){
    const matches=Object.entries(files).filter(([name])=>name==="assets/"+entry.name || name===entry.name);
    if(matches.length!==1)throw Error("Approved ZIP missing or duplicates "+entry.name);
    const bytes=matches[0][1];
    if(bytes.length!==entry.bytes)throw Error("File size does not match approved original: "+entry.name);
    if(crc32(bytes)!==entry.crc32)throw Error("Background differs from approved original (CRC): "+entry.name);
    if(await digest(bytes)!==entry.sha256)throw Error("Background differs from approved original (SHA-256): "+entry.name);
    found.set(entry.name,bytes);
  }
  state.files=found;
  state.source=source;
  $("installScenes").disabled=false;
  log("Verified all "+found.size+" exact user-approved backgrounds from "+source+".");
  $("assetStatus").textContent="Approved backgrounds verified. Ready to install missing scenes.";
}
async function manifest(){
  const r=await fetch("approved-backgrounds.json",{cache:"no-store"});
  if(!r.ok)throw Error("Unable to load the approved-art integrity manifest");
  return r.json();
}
async function tryHosted(){
  const m=await manifest();
  const r=await fetch("approved-backgrounds.zip",{cache:"no-store"});
  if(!r.ok)throw Error("Hosted approved-art pack not published yet");
  const raw=await r.arrayBuffer();
  await unzipApproved(raw,"verified hosted pack",m);
}
async function useOwnerArchive(file){
  const m=await manifest();
  const raw=await file.arrayBuffer();
  const zipSha=await digest(raw);
  if(zipSha!==m.approvedSourceZipSHA256 && zipSha!==m.approvedBundleZipSHA256)
    throw Error("Wrong ZIP. Choose the exact approved Phase 3 Windows ZIP or the verified approved-backgrounds.zip, not a CI/test image.");
  await unzipApproved(raw,"verified owner-approved ZIP",m);
}

class PBSerial {
  constructor(port){this.port=port;this.decoder=new TextDecoder();this.lines=[];this.waiting=null;this.buffer="";this.closed=false;}
  async open(){
    await this.port.open({baudRate:115200,bufferSize:8192});
    this.reader=this.port.readable.getReader();
    this.writer=this.port.writable.getWriter();
    this.pump=this.readLoop();
  }
  async readLoop(){
    try{
      for(;;){
        const {value,done}=await this.reader.read();
        if(done)break;
        this.buffer+=this.decoder.decode(value,{stream:true});
        let end;
        while((end=this.buffer.indexOf("\n"))!==-1){
          const line=this.buffer.slice(0,end).trim();
          this.buffer=this.buffer.slice(end+1);
          if(!line)continue;
          if(this.waiting){const x=this.waiting;this.waiting=null;clearTimeout(x.timer);x.resolve(line);}
          else this.lines.push(line);
          if(this.lines.length>120)this.lines.splice(0,this.lines.length-100);
        }
      }
    }catch(error){if(!this.closed)log("Device connection closed: "+error.message);}
    finally{this.closed=true;if(this.waiting){let w=this.waiting;this.waiting=null;clearTimeout(w.timer);w.reject(Error("USB connection closed"));}}
  }
  async line(timeout=15000){
    if(this.lines.length)return this.lines.shift();
    if(this.closed)throw Error("USB disconnected");
    return new Promise((resolve,reject)=>{
      const timer=setTimeout(()=>{if(this.waiting){this.waiting=null;reject(Error("USB timed out"));}},timeout);
      this.waiting={resolve,reject,timer};
    });
  }
  async until(match,timeout=15000){
    const deadline=Date.now()+timeout;
    while(Date.now()<deadline){
      const line=await this.line(Math.max(1,deadline-Date.now()));
      if(match(line))return line;
      if(line.startsWith("ERR") || line==="EXISTS")throw Error("Device refused operation: "+line);
    }
    throw Error("USB response timeout");
  }
  async ascii(message){await this.writer.write(encoder.encode(message+"\n"));}
  async close(){
    this.closed=true;
    try{await this.reader.cancel();}catch(e){}
    try{this.reader.releaseLock();}catch(e){}
    try{this.writer.releaseLock();}catch(e){}
    try{await this.port.close();}catch(e){}
  }
}
async function getDeviceCrc(pb,path){
  await pb.ascii("PBCRC "+path);
  const line=await pb.until(l=>l==="MISSING"||l==="DONE"||l.startsWith("CRC ")||l.startsWith("ERR"),20000);
  if(line==="MISSING")return null;
  if(!line.startsWith("CRC "))throw Error("Unexpected PBCRC answer: "+line);
  const result=line.match(/^CRC (\d+) ([0-9a-fA-F]{8})$/);
  if(!result)throw Error("Invalid PBCRC response: "+line);
  await pb.until(l=>l==="DONE",15000);
  return {bytes:Number(result[1]),crc32:result[2].toLowerCase()};
}
async function putMissing(pb,entry,data){
  await pb.ascii("PBPUT "+entry.path+" "+entry.bytes+" "+entry.crc32.toUpperCase());
  await pb.until(l=>l==="OK"||l==="EXISTS"||l.startsWith("ERR"),15000);
  const chunk=2048;
  for(let off=0;off<data.length;off+=chunk){
    await pb.writer.write(data.subarray(off,Math.min(data.length,off+chunk)));
    await pb.until(l=>l==="#"||l.startsWith("ERR"),20000);
    if((off/chunk)%32===0)$("assetStatus").textContent=
      "Uploading "+entry.name+": "+Math.round(100*off/data.length)+"%";
  }
  await pb.until(l=>l==="DONE"||l.startsWith("ERR"),20000);
  const confirmed=await getDeviceCrc(pb,entry.path);
  if(!confirmed||confirmed.bytes!==entry.bytes||confirmed.crc32!==entry.crc32)
    throw Error("Device checksum didn't match after upload: "+entry.name);
}

async function sync(){
  if(state.busy||!state.files) return;
  if(!navigator.serial)throw Error("Web Serial requires desktop Chrome/Edge over HTTPS.");
  state.busy=true;$("installScenes").disabled=true;
  try{
    const m=await manifest();
    log("Requesting Waveshare COM port. Choose the same ESP32-S3 you flashed.");
    const port=await navigator.serial.requestPort();
    serial=new PBSerial(port);
    await serial.open();
    // Opening USB CDC may reset the board; give the SD stack time to mount.
    await new Promise(r=>setTimeout(r,1900));
    await serial.ascii("PBCAPS");
    const caps=await serial.until(l=>l.startsWith("POKEBYTE-ASSETS/1")||l.startsWith("ERR"),15000);
    if(!caps.startsWith("POKEBYTE-ASSETS/1"))throw Error("Device doesn't have the safe PokeByte background-transfer protocol");
    await serial.until(l=>l==="DONE",12000);
    let installed=0,skipped=0;
    for(const entry of m.files){
      $("assetStatus").textContent="Checking "+entry.name+" ("+(installed+skipped+1)+"/12)";
      const have=await getDeviceCrc(serial,entry.path);
      if(have){
        if(have.bytes!==entry.bytes||have.crc32!==entry.crc32){
          throw Error("The SD card already contains a DIFFERENT file at "+entry.path+". Refusing to overwrite it. Existing card files remain untouched.");
        }
        skipped++;log("Already correct: "+entry.name);continue;
      }
      log("Installing approved "+entry.name+"…");
      await putMissing(serial,entry,state.files.get(entry.name));
      installed++;log("Verified on microSD: "+entry.name);
    }
    $("assetStatus").textContent="Success: "+installed+" installed, "+skipped+" already correct. Restart your Waveshare to load the new scenes.";
    log("SUCCESS. All twelve approved backgrounds verified on microSD. No existing file was overwritten.");
  }finally{
    if(serial)await serial.close();
    serial=null;state.busy=false;
    $("installScenes").disabled=!(state.files&&state.files.size===12);
  }
}
function initBackgroundInstaller(){
  // Work whether script loaded before or after DOMContentLoaded.
  const file=$("approvedZip");
  if(!file || !$("assetStatus"))return;
  $("assetStatus").textContent="Preparing approved PokéByte backgrounds…";
  file.addEventListener("change",async()=>{
    try{
      if(!file.files?.length)return;
      $("assetStatus").textContent="Checking original ZIP integrity…";
      await useOwnerArchive(file.files[0]);
    }catch(err){log("Rejected: "+err.message);$("assetStatus").textContent=err.message;}
  });
  $("installScenes").addEventListener("click",async()=>{
    try{await sync();}catch(e){log("Stopped safely: "+e.message);$("assetStatus").textContent="Stopped: "+e.message;}
  });
  $("detectHostedScenes").addEventListener("click",async()=>{
    $("assetStatus").textContent="Checking the official hosted artwork…";
    try{
      await tryHosted();
      const picker=$("approvedZipWrapper");
      if(picker)picker.hidden=true;
      const note=$("assetSourceNote");
      if(note)note.textContent="Approved scenery package loaded automatically—no ZIP selection required.";
    }catch(err){
      $("assetStatus").textContent="Approved scenes have not been uploaded to the website yet. Once the repository owner adds them, this button will work automatically.";
      log("No hosted pack: "+err.message);
    }
  });
  tryHosted().then(()=>{
    const picker=$("approvedZipWrapper");
    if(picker)picker.hidden=true;
    const note=$("assetSourceNote");
    if(note)note.textContent="All 12 original backgrounds downloaded and verified automatically. Connect the Waveshare and click Install backgrounds via USB.";
  }).catch(err=>{
    log("Official scenery package not published yet: "+err.message);
    $("assetStatus").textContent=
      "Approved backgrounds are not hosted yet. The repository owner must upload approved-backgrounds.zip once. In the meantime, the approved ZIP on your PC can be used.";
  });
}
if(document.readyState==="loading")document.addEventListener("DOMContentLoaded",initBackgroundInstaller,{once:true});
else initBackgroundInstaller();
