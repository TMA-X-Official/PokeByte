#pragma once
#include <cstdint>
#include "personality_pack01.h"
// Phase 3 Pack 02: visual-only temperament derived from persisted hatch genes.
// No extra storage, gameplay-stat adjustment or new sprite assets.
enum : uint8_t { PB_PLAYFUL=0, PB_CALM=1, PB_CURIOUS=2, PB_SHY=3 };
struct PokeByteSocialPlan { uint8_t mode=PB_IDLE,action=0;uint32_t durationMs=3500; };
uint8_t pokebyteTemperament(uint8_t geneAtk,uint8_t geneDef,uint8_t geneSpe);
uint8_t pokebyteBondStage(uint8_t bond); // 0: 0-29, 1: 30-69, 2: 70-100
const char* pokebyteTemperamentName(uint8_t temperament);
const char* pokebyteBondStageName(uint8_t stage);
void pokebyteSetSocialSnapshot(uint8_t geneAtk,uint8_t geneDef,uint8_t geneSpe,uint8_t bond);
void pokebyteSetTemperamentDemo(const char* name);
void pokebyteSetBondDemo(const char* stage);
bool pokebyteSocialDemoActive();
uint8_t pokebyteSocialVisibleTemperament();
uint8_t pokebyteSocialVisibleBond();
const char* pokebyteSocialDemoLabel(); // empty outside special 8 demo modes
bool pokebyteSocialHappyPlan(uint8_t temperament,uint8_t bond,uint16_t actions,uint8_t roll,PokeByteSocialPlan* out);
