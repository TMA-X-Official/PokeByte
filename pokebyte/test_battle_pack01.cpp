#include "battle_pack01.h"
#include <cassert>
#include <cstdio>
#include <cstring>
static PBBattleState fresh(uint8_t e=PB_BTYPE_GRASS){
 PBBattleState s;
 pbBattleStart(&s,39,45,4,1,10,60,45,65,53,55,40,PB_BTYPE_FIRE,e);
 return s;
}
int main(){
 assert(!pbBattleCanStart(true,false,false,80));
 assert(!pbBattleCanStart(false,true,false,80));
 assert(!pbBattleCanStart(false,false,true,80));
 assert(!pbBattleCanStart(false,false,false,14));
 assert(pbBattleCanStart(false,false,false,15));
 assert(pbBattleTypeFromAccent(0xEA87)==PB_BTYPE_FIRE);
 assert(pbBattleTypeFromAccent(0x4C98)==PB_BTYPE_WATER);
 assert(pbBattleTypeFromAccent(0x3C49)==PB_BTYPE_GRASS);
 assert(pbBattleMove(PB_BTYPE_FIRE,1).type==PB_BTYPE_FIRE);
 assert(pbBattleMove(PB_BTYPE_FIRE,1).power==40);
 assert(std::strcmp(pbBattleMove(PB_BTYPE_FIRE,1).name,"EMBER")==0);
 PBBattleState s=fresh();
 assert(s.stage==PB_BATTLE_MENU && s.playerHp==s.playerMaxHp && s.enemyHp==s.enemyMaxHp);
 assert(s.pp[1]==25);
 pbBattleChooseMove(&s,1,100);
 assert(s.pp[1]==25 && s.turn==0);
 s.stage=PB_BATTLE_FIGHT;
 pbBattleChooseMove(&s,1,100);
 assert(s.pp[1]==24 && s.enemyHp<s.enemyMaxHp && s.turn==1);
 assert(s.playerHp<=s.playerMaxHp && s.playerHp>=0);
 assert(s.stage==PB_BATTLE_MESSAGE || s.stage==PB_BATTLE_FINISHED);
 if(s.stage==PB_BATTLE_MESSAGE){
  pbBattleUpdate(&s,1500);assert(s.stage==PB_BATTLE_MESSAGE);
  pbBattleUpdate(&s,1600);assert(s.stage==PB_BATTLE_MENU);
 }
 PBBattleState water=fresh(PB_BTYPE_WATER);
 water.stage=PB_BATTLE_FIGHT;
 pbBattleChooseMove(&water,1,100);
 assert(s.enemyHp<water.enemyHp);
 PBBattleState x=fresh();x.stage=PB_BATTLE_BAG;x.playerHp-=25;
 pbBattleChoosePotion(&x,200);
 assert(x.potionCount==0 && x.turn==1 && x.playerHp<=x.playerMaxHp);
 PBBattleState y=fresh();y.stage=PB_BATTLE_FIGHT;
 pbBattleChooseMove(&y,3,100);
 assert(y.enemyWeaken>0 || y.result!=PB_BATTLE_NONE);
 PBBattleState win=fresh();
 for(int i=0;i<120 && win.result==PB_BATTLE_NONE;i++){
  win.stage=PB_BATTLE_FIGHT;
  pbBattleChooseMove(&win,i%3==0?1:0,1000+i*2000);
 }
 assert(win.result==PB_BATTLE_WIN || win.result==PB_BATTLE_LOSE);
 assert(win.stage==PB_BATTLE_FINISHED);
 pbBattleRun(&win);assert(win.stage==PB_BATTLE_OFF && win.result==PB_BATTLE_NONE);
 pbBattleSetDemo("bulbasaur");assert(pbBattleDemoActive() && pbBattleDemoOpponent()==1);
 pbBattleSetDemo("squirtle");assert(pbBattleDemoOpponent()==7);
 pbBattleSetDemo("pikachu");assert(pbBattleDemoOpponent()==25);
 pbBattleSetDemo(nullptr);assert(!pbBattleDemoActive());
 std::puts("PASS: Pokemon-style turns, effectiveness, move PP, potion, faint, run and safe demos");
}
