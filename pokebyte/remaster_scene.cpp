// PokéByte: desktop-only remaster of TamaPoke's ORIGINAL procedural scene.
// Keeps the original 466x466 game, PMD animations, 6 biomes, menus and input.
#include "remaster_scene.h"
#include "../pc/shim/Arduino_GFX_Library.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
namespace {
constexpr int W=466, H=466, HORIZON=232, HUD=312;
bool enabled=true, initialized=false;
int hourOverride=-1;
uint16_t rgb(int r,int g,int b){return uint16_t(((r>>3)<<11)|((g>>2)<<5)|(b>>3));}
struct Color {int r,g,b;};
Color mix(Color a,Color b,int numerator,int denominator){return {a.r+(b.r-a.r)*numerator/denominator,a.g+(b.g-a.g)*numerator/denominator,a.b+(b.b-a.b)*numerator/denominator};}
uint16_t to565(Color c){return rgb(c.r,c.g,c.b);}
int hash(int x,int y){uint32_t v=(uint32_t)x*1973u+(uint32_t)y*9277u+26699u;v=(v^(v>>13))*1274126177u;return int((v^(v>>16))&255);}
void block(Arduino_Canvas *c,int x,int y,uint16_t color,int sz=2){
 if(x<0||y<0||x+sz>W||y+sz>HUD)return;
 for(int yy=y;yy<y+sz;yy++)for(int xx=x;xx<x+sz;xx++)c->frame[size_t(yy)*W+xx]=color;
}
void line(Arduino_Canvas *c,int x,int y,int width,uint16_t color){for(int xx=x;xx<x+width;xx+=2)block(c,xx,y,color);}
void pine(Arduino_Canvas *c,int x,int bottom,int height,Color pineColor){
 uint16_t t=to565(pineColor);int center=x;
 for(int dy=0;dy<height;dy+=2){
  int w=(dy*14/height)+2; if(dy>height/3)w+=3;
  if(dy>height*2/3)w+=4;
  for(int xx=center-w;xx<center+w;xx+=2)block(c,xx,bottom-height+dy,t);
 }
 block(c,center,bottom-2, rgb(64,60,62));
}
void initialize(){
 if(initialized)return;initialized=true;
 std::ifstream f("pokebyte-background.cfg");std::string x;
 if(f&&std::getline(f,x)&&(x=="original"||x=="classic"))enabled=false;
}
}
void pokebyteSetPreviewHour(int h){hourOverride=(h>=0&&h<24)?h:-1;}
void pokebyteToggleRemaster(){
 initialize();enabled=!enabled;
 std::ofstream cfg("pokebyte-background.cfg",std::ios::trunc);
 if(cfg)cfg<<(enabled?"remastered":"classic")<<"\n";
 std::cout<<"PokéByte scene: "<<(enabled?"enhanced original":"classic TamaPoke")<<"\n";
}
void pokebyteEnhanceOriginalScene(Arduino_Canvas*c,uint8_t biome,uint32_t now,bool gameNight,int gameHour){
 initialize();if(!enabled||c->frame.size()!=size_t(W)*H)return;
 int h=hourOverride>=0?hourOverride:gameHour;
 bool night=(gameNight&&hourOverride<0)||h<6||h>=20;
 int mode=night?4:h<10?0:h<14?1:h<18?2:3;
 const Color top[5]={{123,125,187},{78,157,215},{90,160,204},{128,86,142},{16,25,56}};
 const Color mid[5]={{231,177,165},{151,209,227},{238,192,153},{221,120,104},{44,63,103}};
 const Color low[5]={{255,217,155},{211,235,224},{251,212,154},{252,179,112},{63,83,111}};
 const Color far[5]={{129,152,166},{97,164,176},{157,155,145},{137,107,145},{44,66,98}};
 const Color near[5]={{95,140,141},{79,131,137},{111,137,117},{104,96,114},{34,67,83}};
 const Color soils[6]={{124,177,104},{222,192,141},{76,136,82},{148,102,83},{162,150,119},{218,229,238}};
 Color soil=soils[biome<6?biome:0];
 if(mode==0)soil=mix(soil,{187,177,137},1,6);
 if(mode==2)soil=mix(soil,{228,178,108},1,6);
 if(mode==3)soil=mix(soil,{178,112,88},2,9);
 if(mode==4)soil=mix(soil,{30,54,83},8,13);
 // Same horizon and ground positions as TamaPoke, improved at 2-pixel art resolution.
 for(int y=0;y<HUD;y+=2){
  Color base;
  if(y<HORIZON){
   base=(y<116)?mix(top[mode],mid[mode],y,116):mix(mid[mode],low[mode],y-116,116);
  }else{
   base=mix(soil,(mode==4?Color{18,36,64}:Color{80,99,61}),std::min(y-HORIZON,80),500);
  }
  for(int x=0;x<W;x+=2){
   int n=hash(x/2,y/2);
   Color pix=base;
   if(y<HORIZON){
    // A pair of subtle distant ridgelines; original meadow remains open to walk.
    int farY=196 + ((x/34)%3)*3 - ((x>110&&x<195)?21-std::abs(x-152)/2:0)
                 - ((x>300&&x<398)?17-std::abs(x-348)/3:0);
    int nearY=219 + ((x/28)%3)*2 - ((x>42&&x<128)?9-std::abs(x-85)/6:0);
    if(y>=farY)pix=far[mode];
    if(y>=nearY)pix=near[mode];
    if(y<farY&&n>245)pix=mix(pix,{255,255,255},1,17);
   }else{
    if(n>218)pix=mix(pix,{243,229,161},1,6);
    if(n<36)pix=mix(pix,{31,72,43},1,9);
    if(y>288&&x>130&&x<340)pix=mix(pix,{126,130,89},1,13);
   }
   block(c,x,y,to565(pix));
  }
 }
 // Original-style pixel sun and moon: no photorealistic artwork.
 if(mode==4){
  int sx=357,sy=72;
  for(int y=sy-13;y<sy+14;y+=2)for(int x=sx-13;x<sx+14;x+=2){
   int dx=x-sx,dy=y-sy;
   if(dx*dx+dy*dy<13*13){
    bool cut=((x-sx-6)*(x-sx-6)+(y-sy-5)*(y-sy-5)<12*12);
    if(!cut)block(c,x,y,rgb(230,232,208));
   }
  }
  for(int i=0;i<28;i++){
   int x=18+(i*83)%426,y=18+(i*47)%168;
   if(hash(i,i)>68)block(c,x&~1,y&~1,rgb(199,219,236));
  }
 }else{
  int sx=(mode==0?362:mode==1?352:mode==2?355:229);
  int sy=(mode==0?90:mode==1?72:mode==2?110:178);
  int r=(mode==3?19:15);
  for(int y=sy-r;y<=sy+r;y+=2)for(int x=sx-r;x<=sx+r;x+=2)
   if((x-sx)*(x-sx)+(y-sy)*(y-sy)<r*r)block(c,x&~1,y&~1,to565(mode==0?Color{255,227,157}:mode==3?Color{255,217,144}:Color{255,242,192}));
  // Small, slow-moving 2px-pixel clouds, drawn away from the text and Pokémon.
  for(int i=0;i<4;i++){
   int cx=(i*131+int(now/250))%550-40;
   int cy=110+i*29;
   Color cloud=mix(mid[mode],{248,245,226},mode==3?2:5,8);
   for(int j=0;j<5;j++)line(c,cx+j*6,cy+((j%3)==0?2:0),12,to565(cloud));
  }
 }
 // Sparse silhouette pines for forest biome; others retain original habitat palette.
 if(biome==2){
  for(int i=0;i<8;i++){
   int tx=20+i*62;
   pine(c,tx,230,28+(i%3)*8,mix(near[mode],{26,57,63},1,3));
  }
 }else if(biome==4){
  for(int i=0;i<4;i++)pine(c,25+i*140,230,24,mix(near[mode],{45,77,70},1,3));
 }else if(biome==5&&mode!=4){
  for(int i=0;i<16;i++){int x=(i*79+int(now/150))%466,y=(i*39+int(now/250))%224;block(c,x&~1,y&~1,rgb(245,249,254));}
 }
 // A restrained grass texture, always beneath the original Pokémon foot line (304).
 if(biome==0||biome==2||biome==4){
  for(int i=0;i<65;i++){
   int x=(i*97+23)%462,y=246+(i*37)%64;
   if(x>155&&x<303&&y>284)continue;  // clean area under walking sprite
   Color blade=mix(soil,mode==4?Color{110,141,128}:Color{41,104,45},2,5);
   block(c,x&~1,y&~1,to565(blade));
   if(i%4==0)block(c,(x+2)&~1,(y-2)&~1,to565(blade));
  }
 }
}
