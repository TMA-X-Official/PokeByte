#pragma once
// Phase 4 Waveshare port: exact user-approved BMP art + cloud-free ambience.
// Hardware build only. NEVER formats or modifies the microSD. No gameplay saves.
#if defined(ARDUINO) && !defined(POKEBYTE_PC)
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <FS.h>
#include <SD_MMC.h>
#include <Preferences.h>
#include <esp_heap_caps.h>
#include <cstring>
#include <cstdio>

namespace pokebyte_embedded {
constexpr int W=466,H=466;
static uint16_t* scene=nullptr;  // ONE 434 KiB PSRAM cache, not two scene buffers
static char loaded[42]={0};
static uint8_t currentMode=2;
static bool failed=false, initialized=false, ambientOn=true;
static uint8_t weather=0; // 0 clear, 1 light rain, 2 gentle snow

inline uint16_t rgb(uint8_t r,uint8_t g,uint8_t b){
 return uint16_t((uint16_t(r>>3)<<11)|(uint16_t(g>>2)<<5)|uint16_t(b>>3));
}
inline uint16_t le16(const uint8_t*p){return uint16_t(p[0])|(uint16_t(p[1])<<8);}
inline uint32_t le32(const uint8_t*p){
 return uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);
}
inline uint8_t modeFromHour(int hour){
 // Same 5 day-parts as approved Windows art; normal RTC hour from game.
 return (hour<6||hour>=21)?4:hour<10?0:hour<14?1:hour<18?2:3;
}
inline const char* modeName(uint8_t mode){
 static const char* names[]={"morning","day","afternoon","evening","night"};
 return names[mode<5?mode:1];
}
inline void init(){
 if(initialized)return;
 initialized=true;
 Preferences p;
 if(p.begin("pbvisual",true)){
  ambientOn=p.getBool("ambient",true);
  weather=p.getUChar("weather",0);
  if(weather>2)weather=0;
  p.end();
 }
}
inline void setAmbient(bool on){
 init();ambientOn=on;
 Preferences p;
 if(p.begin("pbvisual",false)){p.putBool("ambient",on);p.end();}
}
inline void setWeather(uint8_t selected){
 if(selected>2)return;
 init();weather=selected;
 Preferences p;
 if(p.begin("pbvisual",false)){p.putUChar("weather",selected);p.end();}
}
inline bool loadApprovedBmp(const char* path) {
 if(!sdReady)return false; // never call begin() here; never autoformat card
 File f=SD_MMC.open(path,FILE_READ);
 if(!f)return false;
 uint8_t head[54];
 if(f.read(head,sizeof(head))!=sizeof(head)||head[0]!='B'||head[1]!='M' ||
    le32(head+14)<40 || le16(head+26)!=1 || le32(head+18)!=W ||
    le32(head+22)!=H || le16(head+28)!=24 || le32(head+30)!=0){
   f.close();return false;
 }
 constexpr uint32_t ROW=((W*3+3)/4)*4;
 const uint32_t offset=le32(head+10);
 if(offset<54||offset+ROW*H>f.size()){f.close();return false;}
 if(!scene){
  scene=(uint16_t*)heap_caps_malloc(size_t(W)*H*sizeof(uint16_t),
                                  MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
  if(!scene){f.close();Serial.println("PokéByte: scene PSRAM allocation failed");return false;}
 }
 if(!f.seek(offset)){f.close();return false;}
 uint8_t row[ROW];
 for(int y=H-1;y>=0;--y){
  if(f.read(row,ROW)!=ROW){f.close();return false;}
  uint16_t* dest=scene+size_t(y)*W;
  for(int x=0;x<W;++x){
    const uint8_t* b=&row[3*x]; // 24-bit uncompressed BGR
    dest[x]=rgb(b[2],b[1],b[0]);
  }
 }
 f.close();
 return true;
}
inline bool ensureScene(uint8_t timeMode,bool mini){
 init();
 if(!sdReady)return false;
 char name[42];
 snprintf(name,sizeof(name),"/pokebyte/%s_%s.bmp",
          mini?"minigame":"meadow",modeName(timeMode));
 if(strcmp(name,loaded)==0)return !failed;
 // Fail closed to original renderer; don't display half-loaded image.
 // Retry only on scene change / new SD mount to avoid hammering broken SD.
 failed=!loadApprovedBmp(name);
 strncpy(loaded,name,sizeof(loaded)-1);
 loaded[sizeof(loaded)-1]=0;
 if(failed)Serial.printf("PokéByte: missing/invalid approved SD scene %s; classic fallback\n",name);
 return !failed;
}
inline uint32_t hash32(uint32_t n){
 n^=n>>16;n*=0x7feb352dU;n^=n>>15;n*=0x846ca68bU;n^=n>>16;return n;
}
inline void pixel(Arduino_Canvas* c,int x,int y,uint16_t color,int w=2,int h=2){
 if(x<0||y<0||x+w>W||y+h>H)return;
 c->fillRect(x,y,w,h,color);
}
inline void pollen(Arduino_Canvas*c,uint32_t t,int bottom){
 for(uint32_t i=0;i<4;++i){
  int x=(i%2?355:35)+int(hash32(100+i)%70)+int((t/850+i*7)%17)-8;
  int y=bottom-70+int(hash32(150+i)%50)-int((t/1600+i*3)%12);
  pixel(c,x&~1,y&~1,rgb(249,228,153),2,2);
 }
}
inline void fireflies(Arduino_Canvas*c,uint32_t t,int bottom){
 for(uint32_t i=0;i<7;++i){
  int x=(i%2?361:24)+int(hash32(210+i)%65)+int((t/520+i*9)%19)-9;
  int y=bottom-88+int(hash32(310+i)%61)+int((t/750+i*5)%9)-4;
  if((t/280+i*5)%11>=7)continue;
  pixel(c,(x-2)&~1,y,rgb(119,134,102),2,2);
  pixel(c,x&~1,y&~1,rgb(255,235,133),2,2);
  if((t/450+i)%4==0)pixel(c,x&~1,y-2,rgb(218,184,99),2,2);
 }
}
inline void grass(Arduino_Canvas*c,uint32_t t,bool mini,bool night){
 int base=mini?402:284;
 uint16_t dark=night?rgb(32,91,88):rgb(67,131,62);
 uint16_t light=night?rgb(67,128,100):rgb(138,187,75);
 for(int i=0;i<7;++i){
  int x=(i%2?369:26)+int(hash32(i+431)%66);
  int y=base-int(hash32(i+902)%13);
  int sway=int((t/800+uint32_t(i*3))%5)-2;
  pixel(c,(x+sway)&~1,y-6,dark,2,4);
  pixel(c,x&~1,y-3,light,2,3);
  pixel(c,(x-sway)&~1,y-5,dark,2,3);
 }
}
inline void rain(Arduino_Canvas*c,uint32_t t,bool mini){
 int bottom=mini?419:302;
 const uint16_t col=rgb(160,207,232);
 for(uint32_t i=0;i<40;++i){
  int x=int((hash32(730+i)%W+t/22)%W);
  int y=36+int((hash32(920+i)%(bottom-36)+t/12)%(bottom-36));
  pixel(c,x,y,col,1,3);pixel(c,x-1,y+3,col,1,2);
 }
 for(uint32_t i=0;i<5;++i){
  uint32_t age=(t/200+i*17)%23;
  if(age>5)continue;
  int x=20+int(hash32(1100+i)%420),y=bottom-14-int(hash32(1120+i)%18);
  if(age<4){
   pixel(c,x-3-int(age),y,col,2,1);
   pixel(c,x+3+int(age),y,col,2,1);
   if(age<3)pixel(c,x,y-2-int(age),col,1,2);
  }
 }
}
inline void snow(Arduino_Canvas*c,uint32_t t,bool mini){
 int bottom=mini?422:299;
 const uint16_t white=rgb(245,250,252),soft=rgb(194,220,237);
 for(uint32_t i=0;i<44;++i){
  uint32_t seed=hash32(1700+i*17);
  int fallRate=(i%3==0)?45:70;
  int x=int((seed%W+int(t/230)+(int(i*7)%19))%W);
  int y=20+int(((seed>>9)%(bottom-20)+t/fallRate)%(bottom-20));
  int drift=int((t/650+int(i*3))%13)-6;
  int sx=(x+drift+W)%W;
  if(i%6==0){pixel(c,sx,y,white,2,3);pixel(c,sx-2,y+1,soft,2,1);}
  else pixel(c,sx,y,(i%3==0)?white:soft,2,2);
 }
 for(uint32_t i=0;i<5;++i){
  uint32_t age=(t/360+i*11)%31;
  if(age>5)continue;
  int x=25+int(hash32(2600+i)%414),y=bottom-10-int(hash32(2800+i)%18);
  pixel(c,x,y,white,2,2);
 }
}
inline bool draw(Arduino_Canvas*c,bool mini,int hour){
 if(!c)return false;
 uint8_t mode=modeFromHour(hour);
 currentMode=mode;
 if(!ensureScene(mode,mini)||!scene)return false;
 c->draw16bitRGBBitmap(0,0,scene,W,H); // cached approved art, no SD read per frame
 uint32_t t=millis();
 bool night=(mode==4);
 int bottom=mini?420:300;
 if(ambientOn){
  grass(c,t,mini,night);
  if(night||mode==3)fireflies(c,t,bottom);
  else pollen(c,t,bottom);
 }
 if(weather==1)rain(c,t,mini);
 else if(weather==2)snow(c,t,mini);
 return true;
}
inline const char* status(){
 init();
 return !sdReady?"SD missing":scene==nullptr?"Scene not loaded":
        failed?"Scene missing":"Ready";
}
} // namespace pokebyte_embedded
#endif
