// PokéByte Phase 2 Ambient Pack 01.
// Load the user's APPROVED five environments unchanged and switch them live.
// The original game renders all Pokemon, HUD, mechanics and menus afterward.
#include "pokebyte_scene.h"
#include "ambient_fx.h"
#include "../pc/shim/Arduino_GFX_Library.h"
#include "../pc/shim/Arduino.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <ctime>

namespace {
constexpr int W=466,H=466;
std::vector<uint16_t> homeScene, gameScene;
bool remastered=true, initialized=false;
std::string selectedMode, loadedMode;
std::string overrideMode; // CI & launch-preview command-line overrides only
uint32_t lastPoll=0;

uint16_t rgb565(uint8_t r,uint8_t g,uint8_t b){
 return uint16_t(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}
uint16_t le16(const uint8_t *p){return uint16_t(p[0]|(p[1]<<8));}
uint32_t le32(const uint8_t *p){
 return uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);
}
bool loadBMP(const std::string &path,std::vector<uint16_t>& target){
 std::ifstream f(path,std::ios::binary);if(!f)return false;
 uint8_t hdr[54]{};
 f.read((char*)hdr,sizeof(hdr));
 if(!f||hdr[0]!='B'||hdr[1]!='M'||le32(hdr+14)<40||le16(hdr+26)!=1)return false;
 int w=(int)le32(hdr+18), h=(int)le32(hdr+22);
 if(w!=W||h!=H||le16(hdr+28)!=24||le32(hdr+30)!=0)return false;
 constexpr int rowBytes=((W*3+3)/4)*4;
 std::vector<uint8_t> row(rowBytes);
 std::vector<uint16_t> decoded(size_t(W)*H);
 f.seekg(le32(hdr+10),std::ios::beg);
 for(int y=H-1;y>=0;y--){
  f.read((char*)row.data(),rowBytes);if(!f)return false;
  for(int x=0;x<W;x++){
   const uint8_t*b=&row[3*x];
   decoded[size_t(y)*W+x]=rgb565(b[2],b[1],b[0]);
  }
 }
 target.swap(decoded);return true;
}
bool validMode(const std::string &s){
 return s=="morning"||s=="day"||s=="afternoon"||s=="evening"||s=="night";
}
std::string clockMode(){
 std::time_t current=std::time(nullptr);
 std::tm *local=std::localtime(&current);
 if(!local)return "day";
 int hour=local->tm_hour;
 return (hour<6||hour>=21)?"night":hour<10?"morning":
        hour<14?"day":hour<18?"afternoon":"evening";
}
void init(){
 if(initialized)return;initialized=true;
 std::ifstream f("pokebyte-background.cfg");std::string s;
 if(f&&std::getline(f,s)&&(s=="original"||s=="classic"))remastered=false;
 std::ifstream choice("pokebyte-time-mode.cfg");
 if(choice&&std::getline(choice,s)&&validMode(s))selectedMode=s;
}
void pollMode(){
 init();
 uint32_t now=millis();
 if(lastPoll && (uint32_t)(now-lastPoll)<1000)return;
 lastPoll=now?now:1;
 std::string requested=validMode(overrideMode)?overrideMode:
    validMode(selectedMode)?selectedMode:clockMode();
 if(requested==loadedMode)return;
 std::vector<uint16_t> nextHome,nextGame;
 const bool homeOK=loadBMP("assets/meadow_"+requested+".bmp",nextHome);
 const bool gameOK=loadBMP("assets/minigame_"+requested+".bmp",nextGame);
 if(homeOK&&gameOK){
  homeScene.swap(nextHome);gameScene.swap(nextGame);
  loadedMode=requested;
  std::cout<<"PokéByte: live scene "<<loadedMode<<"\n";
  return;
 }
 // Source-only builds may lack the five user-approved binary backgrounds.
 // Preserve the classic fallback behavior, never show an empty/broken canvas.
 if(homeScene.empty())loadBMP("assets/sunset-forest.bmp",homeScene);
 if(gameScene.empty())loadBMP("assets/minigame.bmp",gameScene);
 loadedMode=requested;
 std::cerr<<"PokéByte: five-scene art missing; using fallback. " <<requested<<"\n";
}
}

void pokebyteSetPreviewMode(const char* mode){
 overrideMode=(mode&&validMode(mode))?mode:"";
 lastPoll=0;loadedMode.clear();
}
const char* pokebyteCurrentSceneMode(){
 pollMode();
 return loadedMode.empty()?"day":loadedMode.c_str();
}
bool pokebyteDrawSceneOverride(Arduino_Canvas *canvas){
 pollMode();
 if(!remastered||homeScene.size()!=size_t(W)*H||canvas->frame.size()!=size_t(W)*H)return false;
 std::copy(homeScene.begin(),homeScene.end(),canvas->frame.begin());
 pokebyteApplyAmbient(canvas,false);
 return true;
}
bool pokebyteDrawMinigameSceneOverride(Arduino_Canvas *canvas){
 pollMode();
 if(!remastered||gameScene.size()!=size_t(W)*H||canvas->frame.size()!=size_t(W)*H)return false;
 std::copy(gameScene.begin(),gameScene.end(),canvas->frame.begin());
 pokebyteApplyAmbient(canvas,true);
 return true;
}
void pokebyteToggleBackground(){
 init();remastered=!remastered;
 std::ofstream f("pokebyte-background.cfg",std::ios::trunc);
 if(f)f<<(remastered?"forest":"original")<<"\n";
 std::cout<<"PokéByte scenery: "<<(remastered?"remastered":"classic")<<"\n";
}
