// Phase 3 Pack 02: stable genetics-based temperament and visible bond effects.
// Render-only; original feeding, egg odds, achievements, evolution and saves untouched.
#include "personality_pack02.h"
#include <cstring>
#include <cstdio>
namespace {
uint8_t genes[3]={100,100,100};
uint8_t actualBond=0;
int8_t demoTemperament=-1,demoBond=-1;
bool has(uint16_t mask,uint8_t id){return (mask&uint16_t(1u<<id))!=0;}
}
uint8_t pokebyteTemperament(uint8_t a,uint8_t d,uint8_t s){
 // Stable across evolution: hatch genes are already persisted and not
 // modified by training or by species changes.
 return uint8_t((uint32_t(a)*13u+uint32_t(d)*7u+uint32_t(s)*19u+3u)%4u);
}
uint8_t pokebyteBondStage(uint8_t bond){return bond<30?0:bond<70?1:2;}
const char* pokebyteTemperamentName(uint8_t v){
 static const char* names[]={"Playful","Calm","Curious","Shy"};
 return names[v%4];
}
const char* pokebyteBondStageName(uint8_t v){
 static const char* names[]={"New Friend","Growing Bond","Close Friend"};
 return names[v%3];
}
void pokebyteSetSocialSnapshot(uint8_t a,uint8_t d,uint8_t s,uint8_t bond){
 genes[0]=a;genes[1]=d;genes[2]=s;actualBond=bond;
}
void pokebyteSetTemperamentDemo(const char* v){
 demoTemperament= v&&!std::strcmp(v,"playful")?PB_PLAYFUL:
   v&&!std::strcmp(v,"calm")?PB_CALM:
   v&&!std::strcmp(v,"curious")?PB_CURIOUS:
   v&&!std::strcmp(v,"shy")?PB_SHY:-1;
}
void pokebyteSetBondDemo(const char* v){
 demoBond=v&&!std::strcmp(v,"low")?15:
          v&&!std::strcmp(v,"medium")?50:
          v&&!std::strcmp(v,"high")?85:-1;
}
bool pokebyteSocialDemoActive(){return demoTemperament>=0||demoBond>=0;}
uint8_t pokebyteSocialVisibleTemperament(){
 return demoTemperament>=0?uint8_t(demoTemperament):
   pokebyteTemperament(genes[0],genes[1],genes[2]);
}
uint8_t pokebyteSocialVisibleBond(){return demoBond>=0?uint8_t(demoBond):actualBond;}
const char* pokebyteSocialDemoLabel(){
 if(demoTemperament<0)return "";
 static char label[48];
 const char* bond=demoBond>=70?"High Bond":demoBond>=30?"Mid Bond":"Low Bond";
 std::snprintf(label,sizeof(label),"%s / %s",
               pokebyteTemperamentName(pokebyteSocialVisibleTemperament()),bond);
 return label;
}
bool pokebyteSocialHappyPlan(uint8_t temperament,uint8_t bond,
                             uint16_t actions,uint8_t roll,PokeByteSocialPlan* out){
 if(!out)return false;
 const uint8_t r=roll%100,t=temperament%4,stage=pokebyteBondStage(bond);
 // The same Pokémon behaves visibly differently at low/high bond.
 // Mood always takes priority in Pack 01 (hungry/sleepy override us).
 static const uint8_t gestureChances[4][3]={{
   47,64,80   // Playful: becomes enthusiastic quickly.
 },{
   17,28,42   // Calm: prefers peaceful idle.
 },{
   17,31,46   // Curious: prefers wandering the field.
 },{
    4,20,53   // Shy: becomes affectionate with care.
 }};
 static const uint8_t walkChances[4][3]={{
   37,29,16
 },{
   14,14,13
 },{
   64,52,42
 },{
   23,25,24
 }};
 uint8_t gesture=gestureChances[t][stage],walk=walkChances[t][stage];
 // Only original PMD Pose, Nod, Breath, or idle/walk. No Hop/Sit.
 static const uint8_t playful[]={7,9,10};
 static const uint8_t calm[]={10,9,7};
 static const uint8_t curious[]={9,7,10};
 static const uint8_t shy[]={9,10,7};
 const uint8_t* choices=t==PB_PLAYFUL?playful:t==PB_CALM?calm:t==PB_CURIOUS?curious:shy;
 uint8_t valid[3],n=0;
 for(uint8_t i=0;i<3;i++)if(has(actions,choices[i]))valid[n++]=choices[i];
 if(n>0&&r<gesture){
  out->mode=PB_GESTURE;
  // A predictable preference per temperament; still varies with time/roll.
  out->action=valid[(r/16)%n];out->durationMs=0;
  return true;
 }
 bool canWalk=has(actions,1)&&has(actions,2);
 if(canWalk&&r<gesture+walk){
  out->mode=PB_WALK;out->action=0;
  out->durationMs=t==PB_CURIOUS?10800:t==PB_PLAYFUL?7300:t==PB_CALM?5900:6600;
  return true;
 }
 out->mode=PB_IDLE;out->action=0;
 out->durationMs=t==PB_CALM?6000:t==PB_SHY?4800:t==PB_CURIOUS?2500:2800;
 return true;
}

bool pokebyteSocialGreeting(uint8_t temperament,uint8_t bond,
                             uint16_t available,uint8_t roll,uint8_t* action){
 if(!action)return false;
 const uint8_t t=temperament%4,stage=pokebyteBondStage(bond);
 static const uint8_t chance[4][3]={
  {20,55,90}, // Playful
  {13,35,65}, // Calm
  {10,45,70}, // Curious
  {0,22,85}   // Shy: strongly grows closer at high Bond
 };
 if(roll%100>=chance[t][stage])return false;
 static const uint8_t preferred[4][3]={
  {7,9,10},{10,9,7},{9,7,10},{9,10,7}
 };
 for(uint8_t i=0;i<3;i++)if(has(available,preferred[t][i])){
  *action=preferred[t][i];return true;
 }
 return false; // species lacks optional gesture, original heart still works
}
