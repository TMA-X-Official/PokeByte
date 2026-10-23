#include "pokebyte_scene.h"
#include "../pc/shim/Arduino_GFX_Library.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

namespace {
constexpr int W=466,H=466;
std::vector<uint16_t> forest;
bool forestEnabled=true;
bool initialized=false;
uint16_t rgb565(uint8_t r,uint8_t g,uint8_t b){return uint16_t(((r>>3)<<11)|((g>>2)<<5)|(b>>3));}
uint16_t le16(const uint8_t *p){return uint16_t(p[0]|(p[1]<<8));}
uint32_t le32(const uint8_t *p){return uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);}
bool loadBMP(const char *path){
 std::ifstream f(path,std::ios::binary);
 if(!f)return false;
 uint8_t hdr[54]{};
 f.read((char*)hdr,sizeof(hdr));
 if(!f||hdr[0]!='B'||hdr[1]!='M'||le32(hdr+14)<40||le16(hdr+26)!=1)return false;
 int w=(int)le32(hdr+18),h=(int)le32(hdr+22);
 if(w!=W||h!=H||le16(hdr+28)!=24||le32(hdr+30)!=0)return false;
 const int rowBytes=((w*3+3)/4)*4;
 std::vector<uint8_t> row(rowBytes);
 std::vector<uint16_t> decoded(size_t(W)*H);
 f.seekg(le32(hdr+10),std::ios::beg);
 for(int y=H-1;y>=0;y--){
  f.read((char*)row.data(),row.size());
  if(!f)return false;
  for(int x=0;x<W;x++){const auto b=&row[3*x];decoded[size_t(y)*W+x]=rgb565(b[2],b[1],b[0]);}
 }
 forest=std::move(decoded);return true;
}
void init(){
 if(initialized)return;
 initialized=true;
 std::ifstream cfg("pokebyte-background.cfg");
 std::string selection;
 if(cfg&&std::getline(cfg,selection)&&selection=="original")forestEnabled=false;
 if(loadBMP("assets/sunset-forest.bmp")||loadBMP("pokebyte/assets/sunset-forest.bmp")){
  std::cout<<"PokeByte: sunset meadow loaded. Press B to change backgrounds.\n";
 }else{
  std::cerr<<"PokeByte: Meadow art missing; original background available.\n";
 }
}
}
bool pokebyteDrawSceneOverride(Arduino_Canvas *canvas){
 init();
 if(!forestEnabled||forest.size()!=size_t(W)*H)return false;
 std::copy(forest.begin(),forest.end(),canvas->frame.begin());
 return true;
}
void pokebyteToggleBackground(){
 init();
 if(forest.empty()){std::cerr<<"Meadow scenery not installed.\n";return;}
 forestEnabled=!forestEnabled;
 std::ofstream cfg("pokebyte-background.cfg",std::ios::trunc);
 if(cfg)cfg<<(forestEnabled?"forest":"original")<<"\n";
 std::cout<<"PokeByte scenery: "<<(forestEnabled?"sunset meadow":"original TamaPoke")<<"\n";
}
