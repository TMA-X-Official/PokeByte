#include "battle_pack01.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace { uint16_t demoOpponent=0; }
bool pbBattleCanStart(bool egg,bool asleep,bool ceremony,uint8_t energy){
 return !egg && !asleep && !ceremony && energy>=15;
}
uint8_t pbBattleTypeFromAccent(uint16_t a){
 return a==0x3C49?PB_BTYPE_GRASS:a==0xEA87?PB_BTYPE_FIRE:
        a==0x4C98?PB_BTYPE_WATER:a==0xBCA1?PB_BTYPE_ELECTRIC:PB_BTYPE_NORMAL;
}
const char* pbBattleTypeName(uint8_t t){
 static const char* names[]={"NORMAL","GRASS","FIRE","WATER","ELECTRIC"};
 return t<=PB_BTYPE_ELECTRIC?names[t]:"NORMAL";
}
PBBattleMove pbBattleMove(uint8_t t,uint8_t slot){
 if(slot==0)return {"SCRATCH",PB_BTYPE_NORMAL,35,35,0};
 if(slot==1){
  if(t==PB_BTYPE_FIRE)return {"EMBER",t,40,25,0};
  if(t==PB_BTYPE_GRASS)return {"VINE WHIP",t,40,25,0};
  if(t==PB_BTYPE_WATER)return {"WATER GUN",t,40,25,0};
  if(t==PB_BTYPE_ELECTRIC)return {"THUNDER SHOCK",t,40,25,0};
  return {"TACKLE",PB_BTYPE_NORMAL,40,35,0};
 }
 if(slot==2)return {"QUICK ATK",PB_BTYPE_NORMAL,30,30,1};
 return {"GROWL",PB_BTYPE_NORMAL,0,30,0};
}
static int effectiveness(uint8_t attack,uint8_t target){
 if((attack==PB_BTYPE_FIRE && target==PB_BTYPE_GRASS)||
    (attack==PB_BTYPE_WATER && target==PB_BTYPE_FIRE)||
    (attack==PB_BTYPE_GRASS && target==PB_BTYPE_WATER)||
    (attack==PB_BTYPE_ELECTRIC && target==PB_BTYPE_WATER))return 2;
 if((attack==PB_BTYPE_GRASS && target==PB_BTYPE_FIRE)||
    (attack==PB_BTYPE_FIRE && target==PB_BTYPE_WATER)||
    (attack==PB_BTYPE_WATER && target==PB_BTYPE_GRASS)||
    (attack==PB_BTYPE_ELECTRIC && target==PB_BTYPE_GRASS))return -2;
 return 1;
}
static int damage(uint16_t level,uint16_t attack,uint16_t defense,
                  uint8_t attackType,uint8_t ownerType,uint8_t targetType,
                  uint8_t power){
 int core=((2*int(std::min<uint16_t>(level,100))/5+2)*int(power)*
           int(attack)/std::max(1,int(defense)))/50+2;
 if(attackType==ownerType)core=core*5/4;
 int eff=effectiveness(attackType,targetType);
 if(eff==2)core*=2;
 if(eff==-2)core=std::max(1,core/2);
 return std::max(1,core);
}
void pbBattleStart(PBBattleState*s,uint16_t playerHpBase,uint16_t enemyHpBase,
                   uint16_t /*playerDex*/,uint16_t enemyDex,uint16_t level,
                   uint16_t atk,uint16_t def,uint16_t spe,
                   uint16_t enemyAtk,uint16_t enemyDef,uint16_t enemySpe,
                   uint8_t playerType,uint8_t enemyType){
 if(!s)return;
 *s=PBBattleState{};
 s->stage=PB_BATTLE_MENU;s->enemyDex=enemyDex;
 s->playerLevel=std::max<uint16_t>(1,level);
 s->enemyLevel=std::max<uint16_t>(1,level);
 s->playerType=playerType;s->enemyType=enemyType;
 s->atk=std::max<uint16_t>(1,atk);s->def=std::max<uint16_t>(1,def);
 s->spe=std::max<uint16_t>(1,spe);
 s->enemyAtk=std::max<uint16_t>(1,enemyAtk);
 s->enemyDef=std::max<uint16_t>(1,enemyDef);
 s->enemySpe=std::max<uint16_t>(1,enemySpe);
 s->playerMaxHp=std::min<int>(300,std::max<int>(30,int(playerHpBase)/2+2*int(level)+10));
 s->enemyMaxHp=std::min<int>(300,std::max<int>(30,int(enemyHpBase)/2+2*int(level)+10));
 s->playerHp=s->playerMaxHp;s->enemyHp=s->enemyMaxHp;
 for(uint8_t i=0;i<4;i++)s->pp[i]=pbBattleMove(playerType,i).maxPP;
 std::snprintf(s->message,sizeof(s->message),"A wild Pokemon appeared!");
}
void pbBattleClose(PBBattleState*s){if(s)*s=PBBattleState{};}
static void enemyTurn(PBBattleState*s){
 if(s->enemyHp<=0)return;
 PBBattleMove move=pbBattleMove(s->enemyType,(s->turn%3)==0?1:0);
 int dealt=damage(s->enemyLevel,s->enemyAtk,s->def,move.type,
                       s->enemyType,s->playerType,move.power);
 if(s->enemyWeaken){dealt=std::max(1,dealt*3/4);s->enemyWeaken--;}
 s->playerHp=std::max<int>(0,s->playerHp-dealt);
 if(!s->playerHp){
  s->result=PB_BATTLE_LOSE;s->stage=PB_BATTLE_FINISHED;
  std::snprintf(s->message,sizeof(s->message),"Your Pokemon fainted!");
 }
}
void pbBattleChooseMove(PBBattleState*s,uint8_t slot,uint32_t now){
 if(!s||s->stage!=PB_BATTLE_FIGHT||slot>=4)return;
 if(s->pp[slot]==0){std::snprintf(s->message,sizeof(s->message),"No PP left for that move!");return;}
 const PBBattleMove move=pbBattleMove(s->playerType,slot);
 s->pp[slot]--;s->turn++;s->lastAction=1;
 bool first=move.priority || s->spe>=s->enemySpe;
 if(!first)enemyTurn(s);
 if(s->playerHp<=0){s->until=now+1500;return;}
 if(move.power){
  int dealt=damage(s->playerLevel,s->atk,s->enemyDef,move.type,
                   s->playerType,s->enemyType,move.power);
  s->enemyHp=std::max<int>(0,s->enemyHp-dealt);
  int eff=effectiveness(move.type,s->enemyType);
  const char* tail=eff==2?" Super effective!":eff==-2?" Not very effective.":"";
  std::snprintf(s->message,sizeof(s->message),"Used %s!%s",move.name,tail);
 }else {
  s->enemyWeaken=std::min<int>(3,s->enemyWeaken+2);
  std::snprintf(s->message,sizeof(s->message),"GROWL weakened the foe!");
 }
 if(s->enemyHp<=0){
  s->result=PB_BATTLE_WIN;s->stage=PB_BATTLE_FINISHED;
  std::snprintf(s->message,sizeof(s->message),"You won the battle!");
 }else{
  if(first)enemyTurn(s);
  if(s->stage!=PB_BATTLE_FINISHED)s->stage=PB_BATTLE_MESSAGE;
 }
 s->until=now+1500;
}
void pbBattleChoosePotion(PBBattleState*s,uint32_t now){
 if(!s||s->stage!=PB_BATTLE_BAG)return;
 if(s->potionCount==0){std::snprintf(s->message,sizeof(s->message),"No demo potions left.");return;}
 if(s->playerHp>=s->playerMaxHp){std::snprintf(s->message,sizeof(s->message),"Your HP is already full.");return;}
 s->potionCount--;s->turn++;s->lastAction=2;
 s->playerHp=std::min<int>(s->playerMaxHp,s->playerHp+20);
 enemyTurn(s);
 if(s->stage!=PB_BATTLE_FINISHED)s->stage=PB_BATTLE_MESSAGE;
 s->until=now+1500;
 if(s->result!=PB_BATTLE_LOSE)
  std::snprintf(s->message,sizeof(s->message),"Demo POTION restored HP!");
}
void pbBattleUpdate(PBBattleState*s,uint32_t now){
 if(s&&s->stage==PB_BATTLE_MESSAGE && now>=s->until)
  s->stage=PB_BATTLE_MENU;
}
void pbBattleRun(PBBattleState*s){
 if(!s)return;
 s->result=PB_BATTLE_RAN;
 pbBattleClose(s);
}
void pbBattleSetDemo(const char* name){
 demoOpponent=!name?0:std::strcmp(name,"bulbasaur")==0?1:
              std::strcmp(name,"squirtle")==0?7:
              std::strcmp(name,"pikachu")==0?25:0;
}
uint16_t pbBattleDemoOpponent(){return demoOpponent;}
bool pbBattleDemoActive(){return demoOpponent!=0;}
