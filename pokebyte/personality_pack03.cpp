// Rare, nonintrusive moments: zero added sprites, game stats, save fields.
#include "personality_pack03.h"
#include "personality_pack02.h"
#include <cstring>
namespace {
uint8_t pending=PB_SURPRISE_NONE, demo=PB_SURPRISE_NONE;
uint32_t lastEvent=0, lastWake=0, lastVictory=0, lastFriendship=0;
uint32_t demoLast=0;
bool has(uint16_t mask,uint8_t act){return (mask & (uint16_t(1u)<<act))!=0;}
bool chooseGesture(uint8_t kind,uint16_t mask,uint8_t* out){
 // A short genuine stretch / flourish / greeting in PMD action table.
 const uint8_t* order=nullptr;
 static const uint8_t stretch[]={10,9,7},victory[]={7,9,10},
                      friendly[]={9,7,10};
 if(kind==PB_SURPRISE_WAKE)order=stretch;
 else if(kind==PB_SURPRISE_VICTORY)order=victory;
 else order=friendly;
 for(unsigned i=0;i<3;i++)if(has(mask,order[i])){
   *out=order[i];return true;
 }
 return false;
}
}
void pokebyteSurpriseSetDemo(const char* name){
 const char*s=name?name:"";
 demo=!std::strcmp(s,"wake")?PB_SURPRISE_WAKE:
      !std::strcmp(s,"victory")?PB_SURPRISE_VICTORY:
      !std::strcmp(s,"wander")?PB_SURPRISE_WANDER:
      !std::strcmp(s,"friendship")?PB_SURPRISE_FRIENDSHIP:PB_SURPRISE_NONE;
 demoLast=0;pending=PB_SURPRISE_NONE;
}
bool pokebyteSurpriseDemoActive(){return demo!=PB_SURPRISE_NONE;}
const char* pokebyteSurpriseDemoLabel(){
 if(demo==PB_SURPRISE_WAKE)return "Wake-up Stretch";
 if(demo==PB_SURPRISE_VICTORY)return "Victory Flourish";
 if(demo==PB_SURPRISE_WANDER)return "Curious Explorer";
 if(demo==PB_SURPRISE_FRIENDSHIP)return "Friendship Moment";
 return "";
}
void pokebyteSurpriseClear(){pending=PB_SURPRISE_NONE;}
void pokebyteSurpriseNotify(uint8_t type,uint32_t now,bool eligible){
 if(!eligible||demo!=PB_SURPRISE_NONE||type==PB_SURPRISE_NONE)return;
 if(pending!=PB_SURPRISE_NONE)return;
 // Throttle each event. Wrap-safe unsigned elapsed comparisons.
 if(type==PB_SURPRISE_WAKE) {
  if(lastWake&&uint32_t(now-lastWake)<30u*60000u)return;
  lastWake=now;
 }else if(type==PB_SURPRISE_VICTORY){
  if(lastVictory&&uint32_t(now-lastVictory)<10u*60000u)return;
  lastVictory=now;
 }else if(type==PB_SURPRISE_FRIENDSHIP){
  if(lastFriendship&&uint32_t(now-lastFriendship)<12u*60000u)return;
  lastFriendship=now;
 }else return;
 pending=type;
}
bool pokebyteSurpriseChoose(uint32_t now,uint8_t temperament,uint8_t bond,
                           uint16_t available,uint16_t roll1000,
                           PokeByteSurprisePlan*out){
 if(!out)return false;
 const bool isDemo=demo!=PB_SURPRISE_NONE;
 if(isDemo&&demoLast&&uint32_t(now-demoLast)<10000u)return false;
 if(!isDemo&&lastEvent&&uint32_t(now-lastEvent)<180000u)return false;
 uint8_t type=isDemo?demo:pending;
 if(!type){
  // Naturally very rare. Curious is slightly likelier to wander.
  if(roll1000 >= (temperament==PB_CURIOUS?6:2))return false;
  type=PB_SURPRISE_WANDER;
 }
 if(type==PB_SURPRISE_FRIENDSHIP && !isDemo && bond<70) {
   pending=PB_SURPRISE_NONE;return false;
 }
 uint8_t act=0;
 if(type==PB_SURPRISE_WANDER){
  if(!(has(available,1)&&has(available,2))){
   if(!isDemo)pending=PB_SURPRISE_NONE;
   return false;
  }
  out->mode=1;out->action=0;
  out->targetX=(roll1000&1)?161:304; // grounded inside original clearing
  out->durationMs=12000;
 }else{
  if(!chooseGesture(type,available,&act)){
   if(!isDemo)pending=PB_SURPRISE_NONE;
   return false;
  }
  out->mode=2;out->action=act;out->targetX=233;
  out->durationMs=0; // caller uses genuine original animation's full frame timing
 }
 out->event=type;
 if(isDemo)demoLast=now?now:1;
 else {lastEvent=now?now:1;pending=PB_SURPRISE_NONE;}
 return true;
}
