// PokéByte Phase 2 v0.1: restrained, optional ambient pixel effects.
// Only modifies backgrounds; original Pokémon sprites and gameplay are untouched.
#include "ambient_fx.h"
#include "../pc/shim/Arduino.h"
#include "../pc/shim/Arduino_GFX_Library.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

namespace {
constexpr int W=466,H=466;
bool initialized=false, ambientEnabled=true, drizzleEnabled=false;
std::string timeMode="day";
uint16_t rgb(int r,int g,int b){return uint16_t(((r>>3)<<11)|((g>>2)<<5)|(b>>3));}
uint32_t hash(uint32_t x){x^=x>>16;x*=0x7feb352dU;x^=x>>15;x*=0x846ca68bU;x^=x>>16;return x;}
void dot(Arduino_Canvas*c,int x,int y,uint16_t color,int size=2){
 for(int yy=0;yy<size;yy++)for(int xx=0;xx<size;xx++)
   if(x+xx>=0&&y+yy>=0&&x+xx<W&&y+yy<H)c->frame[size_t(y+yy)*W+(x+xx)]=color;
}
void init(){
 if(initialized)return;initialized=true;
 std::ifstream mode("assets/active_meadow_mode.txt");
 if(mode)std::getline(mode,timeMode);
 std::ifstream pref("pokebyte-ambient.cfg");
 if(pref){int a=1,r=0;if(pref>>a>>r){ambientEnabled=(a!=0);drizzleEnabled=(r!=0);}}
}
void save(){
 std::ofstream pref("pokebyte-ambient.cfg",std::ios::trunc);
 if(pref)pref<<(ambientEnabled?1:0)<<" "<<(drizzleEnabled?1:0)<<"\n";
}
}
void pokebyteToggleAmbient(){
 init();ambientEnabled=!ambientEnabled;save();
 std::cout<<"PokéByte ambience "<<(ambientEnabled?"ON":"OFF")<<"\n";
}
void pokebyteToggleWeather(){
 init();drizzleEnabled=!drizzleEnabled;save();
 std::cout<<"PokéByte optional drizzle "<<(drizzleEnabled?"ON":"OFF")<<"\n";
}
void pokebyteApplyAmbient(Arduino_Canvas*c,bool minigame){
 init();if(!ambientEnabled||c->frame.size()!=size_t(W)*H)return;
 const uint32_t ticks=millis();
 const bool night=(timeMode=="night"), evening=(timeMode=="evening");
 const int lowest=minigame?423:298;
 // Night/evening: six tiny fireflies, spaced toward the edges. Deliberately
 // avoid the center where the original Pokémon moves and the ball is played.
 if(night||evening){
  for(uint32_t i=0;i<7;i++){
   const int side=(i%2)?1:0;
   const int baseX=side?348:18;
   const int x=baseX+int(hash(i+73)%84)+(int((ticks/580+i*3)%13)-6);
   const int y=197+int(hash(i+137)%88)+(int((ticks/730+i*7)%9)-4);
   if(y>lowest-8)continue;
   const bool bright=((ticks/270+i*7)%9)<6;
   const uint16_t glow=night?rgb(247,223,118):rgb(255,226,140);
   if(bright){dot(c,x&~1,y&~1,glow,2);}
  }
 }else{
  // Daylight: just three unobtrusive floating pollen motes at the edges.
  for(uint32_t i=0;i<3;i++){
   const int x=((i%2)?366:45)+int(hash(i+17)%55);
   const int y=194+int(hash(i+12)%80)-int((ticks/2100+i*11)%12);
   dot(c,x&~1,y&~1,rgb(244,236,177),2);
  }
 }
 // Rain is never automatic or based on real weather; PC player presses W.
 // Paint fine streaks on the existing pixel-art scene, before all gameplay.
 if(drizzleEnabled){
  const int bottom=minigame?414:297;
  for(uint32_t i=0;i<26;i++){
   const int x=int((hash(i+940)%W+ticks/39)%W);
   const int y=118+int((hash(i+32)%(bottom-118)+ticks/20)%(bottom-118));
   const uint16_t rain=rgb(133,187,216);
   dot(c,x,y,rain,1);dot(c,x-1,y+2,rain,1);
  }
 }
}
