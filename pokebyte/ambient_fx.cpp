// PokéByte Phase 2 Ambient Pack 02 — Balanced living scenery.
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
bool initialized=false, ambienceOn=true, rainOn=false;
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
uint16_t blend(uint16_t old,uint16_t newcol,int a){
 int br=((old>>11)&31)*255/31, bg=((old>>5)&63)*255/63, bb=(old&31)*255/31;
 int nr=((newcol>>11)&31)*255/31,ng=((newcol>>5)&63)*255/63,nb=(newcol&31)*255/31;
 return rgb((br*(255-a)+nr*a)/255,
            (bg*(255-a)+ng*a)/255,
            (bb*(255-a)+nb*a)/255);
}
void translucentPixel(Arduino_Canvas*c,int x,int y,uint16_t col,int alpha){
 if(x<0||y<0||x+1>=W||y+1>=H)return;
 for(int j=0;j<2;j++)for(int i=0;i<2;i++){
  uint16_t&dst=c->frame[size_t(y+j)*W+x+i];
  dst=blend(dst,col,alpha);
 }
}
bool oval(int x,int y,int cx,int cy,int rx,int ry){
 if(rx==0||ry==0)return false;
 int dx=x-cx,dy=y-cy;
 return (int64_t)dx*dx*ry*ry+(int64_t)dy*dy*rx*rx<=(int64_t)rx*rx*ry*ry;
}
// Carefully shaded, many-lobed pixel art: each logical pixel = 2x2 on screen.
// 44-59 logical columns = 88-118 real screen pixels, clearly a CLOUD,
// not the three tiny lines from the first ambient prototype.
void cloud(Arduino_Canvas*c,int x0,int y0,bool small,
           uint16_t light,uint16_t body,uint16_t shadow){
 const int cw=small?44:59,ch=small?16:21;
 for(int y=0;y<ch;y++){
  for(int x=0;x<cw;x++){
   bool inside;
   if(small){
    inside=oval(x,y,10,11,10,5)||oval(x,y,19,8,12,8)||
           oval(x,y,29,6,9,6)||oval(x,y,36,12,8,4)||
           oval(x,y,23,12,20,4);
   }else{
    inside=oval(x,y,11,15,11,6)||oval(x,y,24,11,14,10)||
           oval(x,y,38,7,12,8)||oval(x,y,50,15,8,5)||
           oval(x,y,30,16,28,5);
   }
   if(!inside)continue;
   // Strong top-edge pixels and blended shaded underbelly.
   const bool above= y==0|| !(small?
      (oval(x,y-1,10,11,10,5)||oval(x,y-1,19,8,12,8)||
       oval(x,y-1,29,6,9,6)||oval(x,y-1,36,12,8,4)||
       oval(x,y-1,23,12,20,4)):
      (oval(x,y-1,11,15,11,6)||oval(x,y-1,24,11,14,10)||
       oval(x,y-1,38,7,12,8)||oval(x,y-1,50,15,8,5)||
       oval(x,y-1,30,16,28,5)));
   uint16_t col=(y>ch-5)?shadow:(above||y<ch/3)?light:body;
   int opacity=(y>ch-4)?185:above?236:208;
   // A few shadowed grooves make clouds read as pixel-art forms.
   if(y>ch/2 && (x==22||x==39) && y<ch-4){col=shadow;opacity=155;}
   translucentPixel(c,x0+x*2,y0+y*2,col,opacity);
  }
 }
}
void initialize(){
 if(initialized)return;initialized=true;
 std::ifstream f("pokebyte-ambient.cfg");
 if(f){int a=1,r=0;if(f>>a>>r){ambienceOn=a!=0;rainOn=r!=0;}}
}
void store(){
 std::ofstream f("pokebyte-ambient.cfg",std::ios::trunc);
 if(f)f<<(ambienceOn?1:0)<<" "<<(rainOn?1:0)<<"\n";
}
struct Palette {uint16_t light,body,shade;};
Palette colors(const std::string&m){
 if(m=="morning")return {rgb(255,232,191),rgb(249,196,177),rgb(167,150,189)};
 if(m=="afternoon")return {rgb(255,225,162),rgb(246,178,124),rgb(167,121,133)};
 if(m=="evening")return {rgb(255,174,119),rgb(231,113,130),rgb(122,70,142)};
 if(m=="night")return {rgb(119,169,222),rgb(76,130,195),rgb(38,67,120)};
 return {rgb(250,252,245),rgb(226,240,250),rgb(132,185,224)};
}
void clouds(Arduino_Canvas*c,uint32_t t,const std::string&m){
 Palette p=colors(m);
 // Two separate drifting multi-lobe sprites: prominent and smaller/distant.
 // Slow 1-2 pixels/sec. Wrap through side edges after leaving the screen.
 const int largeX=int((int64_t(t/650)+105)%600)-85;
 const int smallX=397-int((int64_t(t/870)+30)%560);
 cloud(c,largeX,58,false,p.light,p.body,p.shade);
 cloud(c,smallX,139,true,p.light,p.body,p.shade);
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
} // namespace
void pokebyteToggleAmbient(){
 initialize();ambienceOn=!ambienceOn;store();
 std::cout<<"PokeByte Balanced ambience "<<(ambienceOn?"ON":"OFF")<<"\n";
}
void pokebyteToggleWeather(){
 initialize();rainOn=!rainOn;store();
 std::cout<<"PokeByte optional light rain "<<(rainOn?"ON":"OFF")<<"\n";
}
void pokebyteApplyAmbient(Arduino_Canvas*c,bool minigame){
 initialize();
 if(c->frame.size()!=size_t(W)*H)return;
 const uint32_t ticks=millis();
 const std::string mode=pokebyteCurrentSceneMode();
 const bool night=mode=="night";
 const int floor=minigame?420:300;
 if(ambienceOn){
  clouds(c,ticks,mode);
  grass(c,ticks,minigame,night);
  if(night||mode=="evening")fireflies(c,ticks,floor);
  else pollen(c,ticks,false,floor);
 }
 // Rain can be controlled independently from the ambience switch.
 if(rainOn)rain(c,ticks,minigame);
}
