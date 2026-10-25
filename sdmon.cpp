#include "sdmon.h"
#include "pin_config.h"
#include <FS.h>
#include <SD_MMC.h>

bool sdReady = false;
bool sdDirty = false;
SdThumbs thumbs;

bool PmdMon::load(uint8_t dexNum, bool shiny) {
  unload();
  if (!sdReady) return false;

  char path[28];
  snprintf(path, sizeof(path), "/mons/p%s%03u.bin", shiny ? "s" : "", dexNum);
  File f = SD_MMC.open(path, FILE_READ);
  if (!f && shiny) {  // sin shiny PMD: usa el normal
    snprintf(path, sizeof(path), "/mons/p%03u.bin", dexNum);
    f = SD_MMC.open(path, FILE_READ);
  }
  if (!f) return false;

  uint32_t size = f.size();
  if (size < 7 || size > 3UL * 1024 * 1024) { f.close(); return false; }
  blob = (uint8_t *)ps_malloc(size);
  if (!blob || f.read(blob, size) != size || memcmp(blob, "TPK2", 4) != 0) {
    if (blob) { free(blob); blob = nullptr; }
    f.close();
    return false;
  }
  f.close();

  uint8_t nActs = blob[4];
  memcpy(&palCount, blob + 5, 2);
  if (palCount > 256 || (uint32_t)7 + palCount * 2 > size) { unload(); return false; }
  memcpy(pal, blob + 7, palCount * 2);

  const uint8_t *p = blob + 7 + palCount * 2;
  const uint8_t *end = blob + size;
  for (uint8_t i = 0; i < nActs && p + 4 <= end; i++) {
    uint8_t id = p[0], w = p[1], h = p[2], nf = p[3];
    p += 4;
    if (id >= PMD_NACTS || nf > 24) { unload(); return false; }
    // valida que ms[] y los datos del frame caben en el blob (archivo truncado)
    uint32_t bytes = (uint32_t)nf * 2 + (uint32_t)w * h * nf;
    if (w == 0 || h == 0 || nf == 0 || p + bytes > end) { unload(); return false; }
    PmdAct &a = acts[id];
    a.w = w;
    a.h = h;
    a.frames = nf;
    for (uint8_t k = 0; k < nf; k++) {
      a.ms[k] = p[0] | (p[1] << 8);
      if (a.ms[k] == 0) a.ms[k] = 100;  // nunca 0: pmdFrameAt() giraria sin avanzar
      p += 2;
    }
    a.data = p;
    p += (uint32_t)w * h * nf;
    // fila mas baja con contenido en cualquier frame: anclar por los pies
    uint8_t base = 1;
    for (uint8_t f = 0; f < nf; f++) {
      const uint8_t *fr = a.data + (uint32_t)f * w * h;
      for (int r = h - 1; r >= 0; r--) {
        bool any = false;
        for (int c = 0; c < w && !any; c++)
          if (fr[r * w + c] != 0xFF) any = true;
        if (any) { if (r + 1 > base) base = r + 1; break; }
      }
    }
    a.base = base;
  }
  loaded = true;
  Serial.printf("cargado %s (%u KB)\n", path, size / 1024);
  return true;
}

void PmdMon::unload() {
  if (blob) {
    free(blob);
    blob = nullptr;
  }
  for (auto &a : acts) {
    a.w = a.h = a.frames = a.base = 0;
    a.data = nullptr;
  }
  loaded = false;
}

void SdThumbs::unload() {
  if (data) { free(data); data = nullptr; }
  loaded = false;
  count = 0;
  size = 0;
}

bool SdThumbs::load() {
  unload();  // recargar sin fugar el blob anterior
  if (!sdReady) return false;
  File f = SD_MMC.open("/mons/thumbs.bin", FILE_READ);
  if (!f) {
    Serial.println("sin thumbs.bin (galeria sin miniaturas)");
    return false;
  }
  uint32_t sz = f.size();
  // acota el tamano: evita un ps_malloc absurdo con un archivo corrupto
  if (sz < 10 || sz > 2UL * 1024 * 1024) {
    Serial.println("thumbs.bin invalido (tamano)");
    f.close();
    return false;
  }
  data = (uint8_t *)ps_malloc(sz);
  if (!data || f.read(data, sz) != sz || memcmp(data, "TPTH", 4) != 0) {
    Serial.println("thumbs.bin invalido");
    if (data) { free(data); data = nullptr; }
    f.close();
    return false;
  }
  f.close();
  memcpy(&count, data + 4, 2);
  // la tabla de offsets (uno por especie) debe caber entera en lo leido
  if ((uint32_t)6 + 4UL * count > sz) {
    Serial.println("thumbs.bin invalido (tabla de offsets)");
    unload();
    return false;
  }
  size = sz;
  loaded = true;
  Serial.printf("miniaturas cargadas: %u (%u KB)\n", count, sz / 1024);
  return true;
}

const uint8_t *SdThumbs::get(int16_t dex) const {
  if (!loaded || dex < 1 || dex > count) return nullptr;
  uint32_t off;
  memcpy(&off, data + 6 + 4 * (dex - 1), 4);
  // el offset viene del fichero sin comprobar: sin esto, un thumbs.bin truncado
  // o corrupto hacia leer fuera de la reserva de PSRAM
  if (off > size - 3) return nullptr;  // la cabecera w,h,palCount debe caber (size >= 10 siempre)
  uint32_t need = 3 + (uint32_t)data[off + 2] * 2 + (uint32_t)data[off] * data[off + 1];
  if (need > size || off > size - need) return nullptr;  // y el blob entero tambien
  return data + off;
}

bool sdBegin() {
  SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_DATA);
  sdReady = SD_MMC.begin("/sdcard", true /* 1-bit SDMMC */, false /* NEVER auto-format user SD */);
  if (sdReady) {
    Serial.printf("SD montada: %llu MB\n", SD_MMC.cardSize() / (1024ULL * 1024ULL));
    SD_MMC.mkdir("/mons");
  } else {
    Serial.println("SD no disponible; NO se formatea. Revisar FAT/archivos y preservar backup.");
  }
  return sdReady;
}

bool SdMon::load(uint8_t dexNum, bool shiny) {
  unload();
  if (!sdReady) return false;

  char path[24];
  snprintf(path, sizeof(path), "/mons/%s%03u.bin", shiny ? "s" : "", dexNum);
  File f = SD_MMC.open(path, FILE_READ);
  if (!f && shiny) {  // sin variante shiny: usa la normal
    snprintf(path, sizeof(path), "/mons/%03u.bin", dexNum);
    f = SD_MMC.open(path, FILE_READ);
  }
  if (!f) {
    Serial.printf("no existe %s\n", path);
    return false;
  }

  char magic[4];
  uint16_t header[4];
  if (f.read((uint8_t *)magic, 4) != 4 || memcmp(magic, "TPK1", 4) != 0 ||
      f.read((uint8_t *)header, 8) != 8) {
    f.close();
    return false;
  }
  w = header[0];
  h = header[1];
  frames = header[2];
  frameMs = header[3];
  // acota dimensiones: evita size desbordado o absurdo con archivo corrupto
  if (f.read((uint8_t *)&palCount, 2) != 2 || palCount > 256 ||
      w == 0 || w > 256 || h == 0 || h > 256 || frames == 0 || frames > 64) {
    f.close();
    return false;
  }
  if (f.read((uint8_t *)pal, palCount * 2) != palCount * 2) {
    f.close();
    return false;
  }

  uint32_t size = (uint32_t)w * h * frames;
  data = (uint8_t *)ps_malloc(size);
  if (!data) {
    Serial.println("sin PSRAM para el sprite");
    f.close();
    return false;
  }
  uint32_t got = f.read(data, size);
  f.close();
  if (got != size) {
    Serial.printf("%s truncado (%u de %u)\n", path, got, size);
    unload();
    return false;
  }

  // zoom entero para que el bicho mida ~200 px de alto en pantalla
  scale = 200 / h;
  if (scale < 2) scale = 2;
  if (scale > 5) scale = 5;

  Serial.printf("cargado %s: %ux%u x%u frames @%ums, escala %u\n",
                path, w, h, frames, frameMs, scale);
  loaded = true;
  return true;
}

void SdMon::unload() {
  if (data) {
    free(data);
    data = nullptr;
  }
  loaded = false;
}

// ---------------------------------------------------------------------------
// Protocolo de carga por USB (para llenar la SD sin sacarla de la placa):
//   PUT <ruta> <bytes>\n  + datos crudos   -> "OK" ... "DONE"
//   LS\n                                   -> listado de /mons
// Usar con tools/send_sd.py
// ---------------------------------------------------------------------------

// PokéByte browser-asset protocol v1 (future Web Serial transport).
// PBCRC is READ-ONLY; PBPUT never replaces a pre-existing original file.
// This intentionally does NOT reuse the legacy destructive PUT command.
static uint32_t pbCrc32(uint32_t crc,const uint8_t* b,size_t n){
 for(size_t i=0;i<n;++i){
  crc^=b[i];
  for(uint8_t j=0;j<8;++j)
   crc=(crc>>1)^(0xedb88320U & (uint32_t(0U)-(crc&1U)));
 }
 return crc;
}
static uint32_t pbArtCrc(const String& name){
 const char* files[]={
  "/pokebyte/meadow_morning.bmp","/pokebyte/meadow_day.bmp",
  "/pokebyte/meadow_afternoon.bmp","/pokebyte/meadow_evening.bmp",
  "/pokebyte/meadow_night.bmp","/pokebyte/minigame_morning.bmp",
  "/pokebyte/minigame_day.bmp","/pokebyte/minigame_afternoon.bmp",
  "/pokebyte/minigame_evening.bmp","/pokebyte/minigame_night.bmp",
  "/pokebyte/minigame.bmp","/pokebyte/sunset-forest.bmp"
 };
 static const uint32_t digests[]={
  0x8416f26dU,0x8bd4737fU,0xae3973fbU,0x5e9eec1bU,
  0xae38373bU,0xc3ef1747U,0x2b4a34a0U,0x1295529fU,
  0x1817ea5eU,0xd527026bU,0x1817ea5eU,0x5e9eec1bU
 };
 for(int i=0;i<12;++i)if(name==files[i])return digests[i];
 return 0;
}
static bool pbPermittedPath(const String& path){
 if(path.indexOf("..")>=0 || path.indexOf('\\')>=0 ||
    path.indexOf("//")>=0)return false;
 if(path.startsWith("/pokebyte/"))return pbArtCrc(path)!=0;
 if(!path.startsWith("/mons/")||!path.endsWith(".bin"))return false;
 String name=path.substring(6);
 if(name=="thumbs.bin")return true;
 // p001.bin ... p151.bin; ps001.bin ... ps151.bin.
 int prefix=name.startsWith("ps")?2:1;
 if(name[0]!='p'||name.length()!=(prefix==2?9:8))return false;
 for(int i=prefix;i<prefix+3;++i)
  if(name[i]<'0'||name[i]>'9')return false;
 int n=name.substring(prefix,prefix+3).toInt();
 return n>=1 && n<=151;
}
static uint32_t pbFileCRC(const String& path,uint32_t* size){
 if(size)*size=0;
 File f=SD_MMC.open(path,FILE_READ);
 if(!f)return 0;
 uint32_t len=f.size(),crc=0xffffffffU;
 uint8_t buf[2048];
 while(f.available()){
  int n=f.read(buf,sizeof(buf));
  if(n<=0){f.close();return 0;}
  crc=pbCrc32(crc,buf,size_t(n));
 }
 f.close();
 if(size)*size=len;
 return ~crc;
}
bool sdSerialCommand(const String &line) {
  if(line == "PBCAPS"){
    Serial.println("POKEBYTE-ASSETS/1 CRC32 READ-ONLY-NONDESTRUCTIVE");
    Serial.println("PATHS /mons/*.bin /pokebyte/*.bmp");
    Serial.println("COMMANDS PBCRC PBPUT PBCAPS");
    Serial.println("DONE");return true;
  }
  if(line.startsWith("PBCRC ")){
    String path=line.substring(6);
    if(!sdReady||!pbPermittedPath(path)){
      Serial.println("ERR PATH");return true;
    }
    if(!SD_MMC.exists(path)){Serial.println("MISSING");return true;}
    uint32_t size=0,digest=pbFileCRC(path,&size);
    if(size==0){Serial.println("ERR READ");return true;}
    Serial.printf("CRC %u %08lX\n",(unsigned)size,(unsigned long)digest);
    Serial.println("DONE");
    return true;
  }
  if(line.startsWith("PBPUT ")){
    // Non-destructive new-file transfer. Verify before rename; NEVER replace
    // existing files; the client must first use PBCRC and compare its manifest.
    int first=line.indexOf(' ',6);
    int last=line.lastIndexOf(' ');
    if(first<7||last<=first||!sdReady){
      Serial.println("ERR ARGS");return true;
    }
    String path=line.substring(6,first);
    uint32_t size=(uint32_t)line.substring(first+1,last).toInt();
    String crcText=line.substring(last+1);
    if(!pbPermittedPath(path)||size==0||size>1024*1024||
       crcText.length()!=8){
      Serial.println("ERR PATH");return true;
    }
    for(size_t k=0;k<8;++k)
      if(!isxdigit((unsigned char)crcText[k])){
        Serial.println("ERR CRC");return true;
      }
    uint32_t claimed=strtoul(crcText.c_str(),nullptr,16);
    uint32_t approved=pbArtCrc(path);
    if(approved && (size!=652454 || claimed!=approved)){
      Serial.println("ERR ART");return true; // no synthetic CI scenes allowed
    }
    if(SD_MMC.exists(path)){Serial.println("EXISTS");return true;}
    if(path.startsWith("/pokebyte/")&&!SD_MMC.exists("/pokebyte"))
      if(!SD_MMC.mkdir("/pokebyte")){
        Serial.println("ERR SD");return true;
      }
    String temp=path+".pbpart";
    if(SD_MMC.exists(temp))SD_MMC.remove(temp); // only our scratch file
    File f=SD_MMC.open(temp,FILE_WRITE);
    if(!f){Serial.println("ERR SD");return true;}
    Serial.println("OK");
    static uint8_t chunk[2048];
    uint32_t remaining=size,crc=0xffffffffU;
    Serial.setTimeout(5000);
    while(remaining){
      size_t want=remaining>sizeof(chunk)?sizeof(chunk):remaining;
      // Web Serial streams may arrive in several short packets. The host
      // sends one 2048-byte logical frame and waits for exactly one '#'.
      // Read that entire frame before acknowledging, or the host/device
      // can become desynchronized and stall with a half-written BMP.
      size_t got=0;
      uint32_t lastData=millis();
      while(got<want && millis()-lastData<5000u){
        size_t n=Serial.readBytes(chunk+got,want-got);
        if(n){got+=n;lastData=millis();}
      }
      if(got!=want||f.write(chunk,got)!=got)break;
      crc=pbCrc32(crc,chunk,got);
      remaining-=got;
      Serial.println("#");
    }
    f.close();
    Serial.setTimeout(1000);
    if(remaining||(~crc)!=claimed){
      SD_MMC.remove(temp);
      Serial.println("ERR INCOMPLETE");return true;
    }
    if(!SD_MMC.rename(temp,path)){
      SD_MMC.remove(temp);
      Serial.println("ERR COMMIT");return true;
    }
    if(path.startsWith("/mons/"))sdDirty=true;
    Serial.println("DONE");return true;
  }

  if (line.startsWith("PUT ")) {
    int sp = line.lastIndexOf(' ');
    String path = line.substring(4, sp);
    uint32_t size = line.substring(sp + 1).toInt();
    if (!sdReady || size == 0 || size > 4 * 1024 * 1024) {
      Serial.println("ERR");
      return true;
    }
    if (!path.startsWith("/")) path = "/" + path;
    // acota la escritura a /mons/: la ruta llega tal cual de la linea serie, sin
    // sanear, asi que un PUT manipulado podria escribir en cualquier sitio de la
    // tarjeta. Los dos clientes reales (tools/send_sd.py y web/index.html) ya
    // mandan nombres con el prefijo mons/.
    if (!path.startsWith("/mons/") || path.indexOf("..") >= 0) {
      Serial.println("ERR");
      return true;
    }
    // FILE_WRITE ANADE al final si el fichero ya existe, asi que reintentar uno
    // que quedo a medias lo alargaba en vez de reemplazarlo: quedaba un sprite
    // corrupto y mas grande que el original. Importa mas desde que el instalador
    // reanuda transferencias cortadas, porque reintenta justo los que fallaron.
    if (SD_MMC.exists(path)) SD_MMC.remove(path);
    File f = SD_MMC.open(path, FILE_WRITE);
    if (!f) {
      Serial.println("ERR");
      return true;
    }
    Serial.println("OK");
    static uint8_t buf[2048];
    uint32_t remaining = size;
    Serial.setTimeout(5000);
    while (remaining > 0) {
      size_t want = remaining > sizeof(buf) ? sizeof(buf) : remaining;
      size_t n = Serial.readBytes(buf, want);
      if (n == 0) break;  // timeout
      // sin mirar el retorno, una tarjeta llena o con fallo de escritura daba
      // DONE igualmente y dejaba el fichero truncado en la SD: justo la entrada
      // corrupta contra la que hay que protegerse luego al cargar el sprite
      if (f.write(buf, n) != n) break;
      remaining -= n;
      Serial.println("#");  // ack: listo para el siguiente bloque
    }
    f.close();
    Serial.setTimeout(1000);
    sdDirty = (remaining == 0);
    Serial.println(remaining == 0 ? "DONE" : "ERR");
    return true;
  } else if (line == "SDINFO") {  // diagnostico remoto de "no me reconoce la SD"
    Serial.printf("total=%llu used=%llu\n", SD_MMC.totalBytes(), SD_MMC.usedBytes());
    Serial.println("DONE");
    return true;
  } else if (line == "LS") {
    File dir = SD_MMC.open("/mons");
    if (dir) {
      File e;
      while ((e = dir.openNextFile())) {
        Serial.printf("%s %u\n", e.name(), (uint32_t)e.size());
        e.close();
      }
      dir.close();
    }
    Serial.println("DONE");
    return true;
  }
  return false;
}
