// PokéByte Phase 2 Ambient Pack 03 — cloud-free balanced ambience + optional snow.
// Drawn on top of approved bitmap backgrounds, BEFORE Pokémon/UI.
// No changes to original TamaPoke care, touch, minigames or sprite renderer.
#include "ambient_fx.h"
#include "pokebyte_scene.h"
#include "../pc/shim/Arduino.h"
#include "../pc/shim/Arduino_GFX_Library.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

namespace {
constexpr int W=466,H=466;
bool initialized=false, ambienceOn=true, rainOn=false, snowOn=false;
uint16_t rgb(int r,int g,int b){
 return uint16_t(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}
uint32_t hash32(uint32_t n){
 n^=n>>16;n*=0x7feb352dU;n^=n>>15;n*=0x846ca68bU;n^=n>>16;return n;
}
void pixel(Arduino_Canvas*c,int x,int y,uint16_t col,int w=2,int h=2){
 if(x<0||y<0||x+w>W||y+h>H)return;
 for(int py=y;py<y+h;py++)for(int px=x;px<x+w;px++)
  c->frame[size_t(py)*W+px]=col;
}
void initialize(){
 if(initialized)return;initialized=true;
 std::ifstream f("pokebyte-ambient.cfg");
 if(f){
  int a=1,r=0,s=0;
  if(f>>a>>r){
   if(!(f>>s))s=0; // Backwards-compatible with existing two-value saves.
   ambienceOn=a!=0;rainOn=r!=0;snowOn=s!=0;
   if(snowOn)rainOn=false; // Never show snow and rain simultaneously.
  }
 }
}
void store(){
 std::ofstream f("pokebyte-ambient.cfg",std::ios::trunc);
 if(f)f<<(ambienceOn?1:0)<<" "<<(rainOn?1:0)<<" "<<(snowOn?1:0)<<"\n";
}
void pollen(Arduino_Canvas*c,uint32_t t,bool night,int bottom){
 // Completely separate from CLOUDS: only a few motes down near the grass.
 if(night)return;
 for(uint32_t i=0;i<4;i++){
  int x=(i%2?355:35)+int(hash32(100+i)%70)+int((t/850+i*7)%17)-8;
  int y=bottom-70+int(hash32(150+i)%50)-int((t/1600+i*3)%12);
  pixel(c,x&~1,y&~1,rgb(249,228,153),2,2);
 }
}
void fireflies(Arduino_Canvas*c,uint32_t t,int bottom){
 for(uint32_t i=0;i<7;i++){
  const int x=(i%2?361:24)+int(hash32(210+i)%65)+int((t/520+i*9)%19)-9;
  const int y=bottom-88+int(hash32(310+i)%61)+int((t/750+i*5)%9)-4;
  bool bright=(t/280+i*5)%11<7;
  if(!bright)continue;
  pixel(c,(x-2)&~1,y,rgb(119,134,102),2,2);
  pixel(c,x&~1,y&~1,rgb(255,235,133),2,2);
  if((t/450+i)%4==0)pixel(c,x&~1,y-2,rgb(218,184,99),2,2);
 }
}
void grass(Arduino_Canvas*c,uint32_t t,bool minigame,bool night){
 const int base=minigame?402:284;
 uint16_t dark=night?rgb(32,91,88):rgb(67,131,62);
 uint16_t light=night?rgb(67,128,100):rgb(138,187,75);
 for(int i=0;i<7;i++){
  int x=(i%2?369:26)+int(hash32(i+431)%66);
  int y=base-int(hash32(i+902)%13);
  int sway=int((t/800+uint32_t(i*3))%5)-2;
  pixel(c,(x+sway)&~1,y-6,dark,2,4);
  pixel(c,x&~1,y-3,light,2,3);
  pixel(c,(x-sway)&~1,y-5,dark,2,3);
 }
}
void rain(Arduino_Canvas*c,uint32_t t,bool minigame){
 const int bottom=minigame?419:302;
 for(uint32_t i=0;i<40;i++){
  const int x=int((hash32(730+i)%W+t/22)%W);
  const int y=36+int((hash32(920+i)%(bottom-36)+t/12)%(bottom-36));
  const uint16_t col=rgb(160,207,232);
  pixel(c,x,y,col,1,3);
  pixel(c,x-1,y+3,col,1,2);
 }
 // Ground-level splashes with no collision/gameplay effects.
 for(uint32_t i=0;i<5;i++){
  uint32_t age=(t/200+i*17)%23;
  if(age>5)continue;
  int x=20+int(hash32(1100+i)%420);
  int y=bottom-14-int(hash32(1120+i)%18);
  uint16_t col=rgb(162,198,210);
  if(age<4){
   pixel(c,x-3-int(age),y,col,2,1);
   pixel(c,x+3+int(age),y,col,2,1);
   if(age<3)pixel(c,x,y-2-int(age),col,1,2);
  }
 }
}
// Optional, quiet snowfall: true pixel-sized drifting snowflakes, NOT clouds.
// Rendered on the approved scene without repainting or modifying the art.
// Two depths give gentle parallax; a few flakes briefly settle near the field.
void snow(Arduino_Canvas*c,uint32_t t,bool minigame){
 const int bottom=minigame?422:299;
 const uint16_t white=rgb(245,250,252),soft=rgb(194,220,237);
 for(uint32_t i=0;i<44;i++){
  const uint32_t seed=hash32(1700+i*17);
  const int fallRate=(i%3==0)?45:70;
  const int x=int((seed%W+int(t/230)+(int(i*7)%19))%W);
  const int y=20+int(((seed>>9)%(bottom-20)+t/fallRate)%(bottom-20));
  const int drift=int((t/650+int(i*3))%13)-6;
  const int sx=(x+drift+W)%W;
  if(i%6==0){
    pixel(c,sx,y,white,2,3);
    pixel(c,sx-2,y+1,soft,2,1);
  }else{
    pixel(c,sx,y,(i%3==0)?white:soft,2,2);
  }
 }
 for(uint32_t i=0;i<5;i++){
  const uint32_t age=(t/360+i*11)%31;
  if(age>5)continue;
  int x=25+int(hash32(2600+i)%414);
  int y=bottom-10-int(hash32(2800+i)%18);
  pixel(c,x,y,white,2,2);
 }
}
} // namespace
void pokebyteToggleAmbient(){
 initialize();ambienceOn=!ambienceOn;store();
 std::cout<<"PokeByte Balanced ambience "<<(ambienceOn?"ON":"OFF")<<"\n";
}
void pokebyteToggleWeather(){
 initialize();rainOn=!rainOn;if(rainOn)snowOn=false;store();
 std::cout<<"PokeByte optional light rain "<<(rainOn?"ON":"OFF")<<"\n";
}
void pokebyteToggleSnow(){
 initialize();snowOn=!snowOn;if(snowOn)rainOn=false;store();
 std::cout<<"PokeByte optional snow "<<(snowOn?"ON":"OFF")<<"\n";
}
// CI or explicit launch argument can set visuals for one run without saving.
void pokebyteSetWeatherForTest(const char* mode){
 initialize();
 const std::string choice=mode?mode:"";
 if(choice=="snow"){snowOn=true;rainOn=false;}
 else if(choice=="rain"){rainOn=true;snowOn=false;}
 else if(choice=="clear"){rainOn=false;snowOn=false;}
}
void pokebyteApplyAmbient(Arduino_Canvas*c,bool minigame){
 initialize();
 if(c->frame.size()!=size_t(W)*H)return;
 const uint32_t ticks=millis();
 const std::string mode=pokebyteCurrentSceneMode();
 const bool night=mode=="night";
 const int floor=minigame?420:300;
 if(ambienceOn){
  grass(c,ticks,minigame,night);
  if(night||mode=="evening")fireflies(c,ticks,floor);
  else pollen(c,ticks,false,floor);
 }
 // Rain can be controlled independently from the ambience switch.
 if(rainOn)rain(c,ticks,minigame);
 else if(snowOn)snow(c,ticks,minigame);
}
