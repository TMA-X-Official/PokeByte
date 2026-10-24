// Optional forest path prototype. Absolutely no modification to original
// Pokemon data, battle/egg/medal odds, art, stats, save fields or hardware.
#include "adventure_pack01.h"
#include <cstring>
namespace {
uint8_t demoEvent=255;
}
bool pokebyteAdventureCanEnter(bool isEgg,bool sleeping,bool ceremony,uint8_t energy){
 return !isEgg && !sleeping && !ceremony && energy>=15;
}
void pokebyteAdventureOpen(PokeByteAdventureState*s){
 if(!s)return;
 *s=PokeByteAdventureState{};
 s->stage=PB_ADV_READY;
}
void pokebyteAdventureStartStep(PokeByteAdventureState*s,uint32_t now,uint16_t roll){
 if(!s || (s->stage!=PB_ADV_READY && s->stage!=PB_ADV_RESULT))return;
 // One predetermined event per trail step, with no reward or Dex mutation.
 const uint16_t r=roll%100;
 s->event=r<33?PB_ADV_QUIET:r<65?PB_ADV_ITEM:PB_ADV_WILD;
 static const uint16_t common[]={10,16,19,43,69};
 s->wildDex=s->event==PB_ADV_WILD?common[(roll/100)%5]:0;
 s->stage=PB_ADV_WALKING;
 s->stepAt=now;
 s->steps++;
}
void pokebyteAdventureUpdate(PokeByteAdventureState*s,uint32_t now){
 if(s && s->stage==PB_ADV_WALKING && uint32_t(now-s->stepAt)>=1700u)
  s->stage=PB_ADV_RESULT;
}
void pokebyteAdventureClose(PokeByteAdventureState*s){
 if(s)*s=PokeByteAdventureState{};
}
const char* pokebyteAdventureEventLabel(uint8_t event){
 return event==PB_ADV_WILD?"Wild Pokemon spotted!":
        event==PB_ADV_ITEM?"Found a berry!":"A peaceful walk.";
}
void pokebyteAdventureSetDemo(const char* name){
 if(!name){demoEvent=255;return;}
 demoEvent=std::strcmp(name,"quiet")==0?PB_ADV_QUIET:
           std::strcmp(name,"item")==0?PB_ADV_ITEM:
           std::strcmp(name,"wild")==0?PB_ADV_WILD:255;
}
bool pokebyteAdventureDemoActive(){return demoEvent!=255;}
uint8_t pokebyteAdventureDemoEvent(){return demoEvent;}
