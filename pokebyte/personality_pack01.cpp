// Phase 3 Personality Pack 01 — use ONLY existing PMD sprite animations.
// PC-only scheduler: never changes Pokémon stats, save fields, game physics or art.
#include "personality_pack01.h"
#include "personality_pack02.h"
#include <cstring>
#if defined(ARDUINO) && !defined(POKEBYTE_PC)
#include <Preferences.h>
#else
#include <fstream>
#include <iostream>
#include <string>
#endif
namespace {
bool ready=false, enabled=true;
uint8_t demo=PB_NORMAL;
constexpr uint8_t PMD_WALKL_ID=1,PMD_WALKR_ID=2,PMD_HURT_ID=5;
constexpr uint8_t PMD_POSE_ID=7,PMD_NOD_ID=9,PMD_BREATH_ID=10;
bool has(uint16_t mask,uint8_t act){return (mask & uint16_t(1u<<act))!=0;}
void init(){
 if(ready)return;ready=true;
#if defined(ARDUINO) && !defined(POKEBYTE_PC)
 Preferences cfg;
 if(cfg.begin("pbvisual",true)){
   enabled=cfg.getBool("personality",true);
   cfg.end();
 }
#else
 std::ifstream f("pokebyte-personality.cfg");
 std::string value;
 if(f&&std::getline(f,value)&&(value=="classic"||value=="off"))enabled=false;
#endif
}
uint8_t context(uint8_t food,uint8_t joy,uint8_t energy,uint8_t hygiene){
 if(demo!=PB_NORMAL)return demo;  // Safe visual preview; never mutates pet.
 if(food<40)return PB_HUNGRY;
 if(energy<45)return PB_TIRED;
 if(joy>=70&&food>=55&&energy>=60&&hygiene>=40)return PB_HAPPY;
 return PB_NORMAL;
}
void idle(PokeBytePersonalityPlan*p,uint32_t duration){
 p->mode=PB_IDLE;p->action=0;p->durationMs=duration;
}
void walk(PokeBytePersonalityPlan*p,uint32_t duration){
 p->mode=PB_WALK;p->action=0;p->durationMs=duration;
}
bool gesture(PokeBytePersonalityPlan*p,uint16_t mask,
             const uint8_t* acts,uint8_t count,uint8_t roll){
 uint8_t eligible[3],n=0;
 for(uint8_t i=0;i<count;i++)if(has(mask,acts[i]))eligible[n++]=acts[i];
 if(!n)return false; // Some Pokémon have no optional gesture art.
 p->mode=PB_GESTURE;p->action=eligible[roll%n];p->durationMs=0;
 return true;
}
}
void pokebyteTogglePersonality(){
 init();enabled=!enabled;
#if defined(ARDUINO) && !defined(POKEBYTE_PC)
 Preferences cfg;
 if(cfg.begin("pbvisual",false)){cfg.putBool("personality",enabled);cfg.end();}
#else
 std::ofstream f("pokebyte-personality.cfg",std::ios::trunc);
 if(f)f<<(enabled?"on":"classic")<<"\n";
 std::cout<<"PokeByte existing-sprite reactions "<<(enabled?"ON":"CLASSIC")<<"\n";
#endif
}
void pokebyteSetPersonalityDemo(const char* value){
 const char* s=value?value:"";
 demo=!std::strcmp(s,"happy")?PB_HAPPY:!std::strcmp(s,"tired")?PB_TIRED:
      !std::strcmp(s,"hungry")?PB_HUNGRY:PB_NORMAL;
}
bool pokebytePersonalityDemoActive(){return demo!=PB_NORMAL;}
bool pokebytePersonalityEnabled(){init();return enabled;}
uint8_t pokebytePersonalityStatusMood(uint8_t food,uint8_t joy,
                                     uint8_t energy,uint8_t hygiene){
 init();
 return enabled?context(food,joy,energy,hygiene):PB_NORMAL;
}
const char* pokebytePersonalityContext(uint8_t f,uint8_t j,uint8_t e,uint8_t h){
 const uint8_t c=context(f,j,e,h);
 return c==PB_HAPPY?"Happy":c==PB_TIRED?"Tired":c==PB_HUNGRY?"Hungry":"Normal";
}
bool pokebytePersonalityPlan(uint8_t food,uint8_t joy,uint8_t energy,
                             uint8_t hygiene,uint16_t available,
                             uint8_t random100,PokeBytePersonalityPlan* out){
 init();if(!enabled||!out)return false;
 const uint8_t c=context(food,joy,energy,hygiene);
 const uint8_t r=random100%100;
 const bool walking=has(available,PMD_WALKL_ID)&&has(available,PMD_WALKR_ID);
 static const uint8_t cheerful[]={PMD_POSE_ID,PMD_NOD_ID,PMD_BREATH_ID};
 static const uint8_t relaxed[]={PMD_BREATH_ID,PMD_NOD_ID};
 static const uint8_t anxious[]={PMD_HURT_ID};
 if(c==PB_HUNGRY){
  // Use original Hurt sparingly as a sign of discomfort; no fake feeding art.
  if(r<44&&gesture(out,available,anxious,1,r))return true;
  if(r>=92&&walking){walk(out,6000);return true;}
  idle(out,3900+((r%4)*400));return true;
 }
 if(c==PB_TIRED){
  // Rest more; genuine DeepBreath/Nod only if the selected sprite has it.
  if(r<48&&gesture(out,available,relaxed,2,r))return true;
  if(r>=96&&walking){walk(out,4900);return true;}
  idle(out,4900+((r%4)*350));return true;
 }
 if(c==PB_HAPPY){
  // Phase 3 Pack 02 uses existing gene-derived temperament + real bond.
  // No modification to care, egg odds, medals or save structures.
  PokeByteSocialPlan social{};
  if(pokebyteSocialHappyPlan(pokebyteSocialVisibleTemperament(),
                            pokebyteSocialVisibleBond(),available,r,&social)){
   out->mode=social.mode;out->action=social.action;
   out->durationMs=social.durationMs;
   return true;
  }
  if(r<59&&gesture(out,available,cheerful,3,r))return true;
  if(r<88&&walking){walk(out,6800);return true;}
  idle(out,2200+((r%4)*450));return true;
 }
 // Neutral pet: retain the same three kinds of behavior, tuned conservatively.
 if(r<30&&walking){walk(out,8000);return true;}
 if(r<57&&gesture(out,available,cheerful,3,r))return true;
 idle(out,3000+((r%4)*500));return true;
}
