#include "personality_pack03.h"
#include "personality_pack02.h"
#include <cassert>
#include <cstdio>
#include <cstring>
int main(){
 const uint16_t all=(1u<<0)|(1u<<1)|(1u<<2)|(1u<<7)|(1u<<8)|(1u<<9)|(1u<<10);
 PokeByteSurprisePlan p{};
 pokebyteSurpriseSetDemo("wake");
 assert(pokebyteSurpriseDemoActive());
 assert(std::strcmp(pokebyteSurpriseDemoLabel(),"Wake-up Stretch")==0);
 assert(pokebyteSurpriseChoose(1000,PB_CALM,15,all,999,&p));
 assert(p.event==PB_SURPRISE_WAKE && p.mode==2 && p.action==10 && p.followupAction==9);
 assert(!pokebyteSurpriseChoose(1001,PB_CALM,15,all,999,&p));
 // Regression for reported bug: do NOT restart at 10 s or even 10 minutes.
 assert(!pokebyteSurpriseChoose(11020,PB_CALM,15,all,999,&p));
 assert(!pokebyteSurpriseChoose(601000,PB_CALM,15,all,999,&p));
 assert(!pokebyteSurpriseDemoFinished());
 pokebyteSurpriseMarkFinished();
 assert(pokebyteSurpriseDemoFinished());
 assert(std::strcmp(pokebyteSurpriseDemoLabel(),"Back to normal")==0);
 pokebyteSurpriseSetDemo("victory");
 assert(pokebyteSurpriseChoose(12000,PB_PLAYFUL,85,all,999,&p));
 assert(p.event==PB_SURPRISE_VICTORY&&p.action==8&&p.followupAction==7);
 assert(!pokebyteSurpriseChoose(43000,PB_PLAYFUL,85,all,0,&p));
 pokebyteSurpriseMarkFinished();
 assert(pokebyteSurpriseDemoFinished());
 pokebyteSurpriseSetDemo("wander");
 assert(pokebyteSurpriseChoose(13000,PB_CURIOUS,15,all,999,&p));
 assert(p.event==PB_SURPRISE_WANDER&&p.mode==1&&p.followupAction==255);
 assert(p.targetX>=150&&p.targetX<=326);
 assert(!pokebyteSurpriseChoose(34000,PB_CURIOUS,15,all,0,&p));
 pokebyteSurpriseMarkFinished();
 assert(pokebyteSurpriseDemoFinished());
 pokebyteSurpriseSetDemo("friendship");
 assert(pokebyteSurpriseChoose(14000,PB_SHY,85,all,999,&p));
 assert(p.event==PB_SURPRISE_FRIENDSHIP&&p.mode==2&&p.action==9&&p.followupAction==7);
 assert(!pokebyteSurpriseChoose(44000,PB_SHY,85,all,0,&p));
 pokebyteSurpriseMarkFinished();
 assert(pokebyteSurpriseDemoFinished());
 // Original PMD actions may be missing on some species.
 const uint16_t onlyBreath=(1u<<0)|(1u<<10);
 pokebyteSurpriseSetDemo("wake");
 assert(pokebyteSurpriseChoose(14600,PB_CALM,15,onlyBreath,999,&p));
 assert(p.action==10 && p.followupAction==255);
 const uint16_t onlyPose=(1u<<0)|(1u<<7);
 pokebyteSurpriseSetDemo("victory");
 assert(pokebyteSurpriseChoose(14700,PB_CALM,15,onlyPose,999,&p));
 assert(p.action==7 && p.followupAction==255);
 // Missing all optional gestures: never request imaginary frames.
 const uint16_t basic=(1u<<0)|(1u<<1)|(1u<<2);
 pokebyteSurpriseSetDemo("wake");
 assert(!pokebyteSurpriseChoose(15000,PB_CALM,30,basic,999,&p));
 pokebyteSurpriseSetDemo("wander");
 assert(pokebyteSurpriseChoose(16000,PB_CURIOUS,30,basic,999,&p));
 assert(!pokebyteSurpriseChoose(26050,PB_CURIOUS,30,(1u<<0)|(1u<<1),999,&p));
 // Real events require actual successful context; no arbitrary victory.
 pokebyteSurpriseSetDemo("");
 pokebyteSurpriseClear();
 pokebyteSurpriseNotify(PB_SURPRISE_VICTORY,30000,false);
 assert(!pokebyteSurpriseChoose(30010,PB_CALM,30,all,999,&p));
 pokebyteSurpriseNotify(PB_SURPRISE_VICTORY,30020,true);
 assert(pokebyteSurpriseChoose(30030,PB_CALM,30,all,999,&p));
 assert(p.event==PB_SURPRISE_VICTORY);
 // Event cooldown prevents spamming the same mini-game flourish.
 pokebyteSurpriseNotify(PB_SURPRISE_VICTORY,40000,true);
 assert(!pokebyteSurpriseChoose(215000,PB_CALM,30,all,999,&p));
 // At real play, high-Bond friendship is guarded; absent gestures degrade.
 pokebyteSurpriseNotify(PB_SURPRISE_FRIENDSHIP,250000,true);
 assert(!pokebyteSurpriseChoose(250010,PB_SHY,30,all,999,&p));
 pokebyteSurpriseNotify(PB_SURPRISE_FRIENDSHIP,1000000,true);
 assert(pokebyteSurpriseChoose(1000020,PB_SHY,85,all,999,&p));
 assert(p.event==PB_SURPRISE_FRIENDSHIP);
 // Ambient wander remains very rare; use 1000-sided rarity sample.
 assert(!pokebyteSurpriseChoose(1400000,PB_CURIOUS,85,all,900,&p));
 assert(pokebyteSurpriseChoose(1400020,PB_CURIOUS,85,all,1,&p));
 assert(p.event==PB_SURPRISE_WANDER);
 std::puts("Pack 03: all four surprise previews play ONCE, return-to-normal state, original PMD fallbacks, real cooldowns, rare idle tests passed");
}
