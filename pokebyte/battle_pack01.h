#pragma once
#include <cstdint>
// Isolated 1v1 battle state: no Pet, NVS, inventory, or new sprite data.
enum : uint8_t { PB_BATTLE_OFF=0, PB_BATTLE_MENU, PB_BATTLE_FIGHT, PB_BATTLE_BAG,
                 PB_BATTLE_PARTY, PB_BATTLE_MESSAGE, PB_BATTLE_FINISHED };
enum : uint8_t { PB_BTYPE_NORMAL=0, PB_BTYPE_GRASS, PB_BTYPE_FIRE,
                 PB_BTYPE_WATER, PB_BTYPE_ELECTRIC };
enum : uint8_t { PB_BATTLE_NONE=0, PB_BATTLE_WIN, PB_BATTLE_LOSE, PB_BATTLE_RAN };
struct PBBattleMove { const char* name; uint8_t type,power,maxPP,priority; };
struct PBBattleState {
 uint8_t stage=PB_BATTLE_OFF, playerType=PB_BTYPE_NORMAL, enemyType=PB_BTYPE_NORMAL;
 uint8_t result=PB_BATTLE_NONE, pp[4]={0,0,0,0};
 uint8_t potionCount=1, enemyWeaken=0, turn=0, lastAction=0;
 uint16_t enemyDex=0, playerLevel=1, enemyLevel=1;
 int16_t playerHp=0, playerMaxHp=0, enemyHp=0, enemyMaxHp=0;
 uint16_t atk=40,def=40,spe=40,enemyAtk=40,enemyDef=40,enemySpe=40;
 uint32_t until=0;
 char message[88]={0};
};
bool pbBattleCanStart(bool egg,bool asleep,bool ceremony,uint8_t energy);
uint8_t pbBattleTypeFromAccent(uint16_t accent);
PBBattleMove pbBattleMove(uint8_t speciesType,uint8_t slot);
void pbBattleStart(PBBattleState* s,uint16_t playerHpBase,uint16_t enemyHpBase,
                   uint16_t playerDex,uint16_t enemyDex,uint16_t level,
                   uint16_t atk,uint16_t def,uint16_t spe,
                   uint16_t enemyAtk,uint16_t enemyDef,uint16_t enemySpe,
                   uint8_t playerType,uint8_t enemyType);
void pbBattleClose(PBBattleState* s);
void pbBattleChooseMove(PBBattleState* s,uint8_t slot,uint32_t now);
void pbBattleChoosePotion(PBBattleState* s,uint32_t now);
void pbBattleUpdate(PBBattleState* s,uint32_t now);
void pbBattleRun(PBBattleState* s);
const char* pbBattleTypeName(uint8_t type);
void pbBattleSetDemo(const char* name);
uint16_t pbBattleDemoOpponent();
bool pbBattleDemoActive();
