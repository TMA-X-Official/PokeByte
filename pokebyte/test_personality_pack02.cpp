#include "personality_pack02.h"
#include <cassert>
#include <cstdio>
#include <cstring>
int main(){
 // Genes are saved on each Pokémon; training, evolution and bond don't alter
 // the genotype-derived temperament. No species ID or save schema needed.
 for(unsigned a=90;a<=110;a+=5)
  for(unsigned d=90;d<=110;d+=5)
   for(unsigned s=90;s<=110;s+=5){
     uint8_t x=pokebyteTemperament(a,d,s);
     assert(x<4&&x==pokebyteTemperament(a,d,s));
   }
 assert(pokebyteBondStage(0)==0&&pokebyteBondStage(29)==0);
 assert(pokebyteBondStage(30)==1&&pokebyteBondStage(69)==1);
 assert(pokebyteBondStage(70)==2&&pokebyteBondStage(100)==2);
 assert(!std::strcmp(pokebyteTemperamentName(PB_SHY),"Shy"));
 const uint16_t all=(1u<<0)|(1u<<1)|(1u<<2)|(1u<<7)|(1u<<9)|(1u<<10);
 PokeByteSocialPlan p{};
 // At r=40 each temperament has its own observable low/high policy.
 assert(pokebyteSocialHappyPlan(PB_PLAYFUL,15,all,40,&p)&&p.mode==PB_GESTURE);
 assert(pokebyteSocialHappyPlan(PB_CALM,15,all,40,&p)&&p.mode==PB_IDLE);
 assert(pokebyteSocialHappyPlan(PB_CURIOUS,15,all,40,&p)&&p.mode==PB_WALK);
 assert(pokebyteSocialHappyPlan(PB_SHY,15,all,40,&p)&&p.mode==PB_IDLE);
 assert(pokebyteSocialHappyPlan(PB_SHY,85,all,40,&p)&&p.mode==PB_GESTURE);
 assert(pokebyteSocialHappyPlan(PB_CALM,85,all,40,&p)&&p.mode==PB_GESTURE);
 // Missing special animation => never request unavailable Pose/Nod/Breath.
 const uint16_t bare=(1u<<0)|(1u<<1)|(1u<<2);
 assert(pokebyteSocialHappyPlan(PB_SHY,85,bare,40,&p)&&p.mode!=PB_GESTURE);
 // Existing personal genes and bond passed in are *not* modified.
 pokebyteSetSocialSnapshot(101,100,100,45);
 assert(pokebyteSocialVisibleTemperament()==pokebyteTemperament(101,100,100));
 assert(pokebyteSocialVisibleBond()==45);
 pokebyteSetTemperamentDemo("shy");pokebyteSetBondDemo("high");
 assert(pokebyteSocialDemoActive());
 assert(pokebyteSocialVisibleTemperament()==PB_SHY&&pokebyteSocialVisibleBond()==85);
 assert(std::strstr(pokebyteSocialDemoLabel(),"Shy / High Bond"));
 pokebyteSetTemperamentDemo("");pokebyteSetBondDemo("");
 assert(!pokebyteSocialDemoActive());
 assert(pokebyteSocialVisibleBond()==45);
 std::puts("Pack 02: genetics, bond stages, four temperaments, fallback and demo isolation passed");
}
