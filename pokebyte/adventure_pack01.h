#pragma once
#include <cstdint>
// Phase 4 Pack 01: a non-persistent, Windows-only adventure preview.
// Pure state machine. No Pet access, save writes, items or battle rewards.
enum : uint8_t { PB_ADV_CLOSED=0, PB_ADV_READY=1, PB_ADV_WALKING=2, PB_ADV_RESULT=3 };
enum : uint8_t { PB_ADV_QUIET=0, PB_ADV_ITEM=1, PB_ADV_WILD=2 };
struct PokeByteAdventureState {
 uint8_t stage=PB_ADV_CLOSED, event=PB_ADV_QUIET;
 uint16_t wildDex=0;
 uint32_t stepAt=0;
 uint16_t steps=0;
};
bool pokebyteAdventureCanEnter(bool isEgg, bool sleeping, bool ceremony, uint8_t energy);
void pokebyteAdventureOpen(PokeByteAdventureState* state);
void pokebyteAdventureStartStep(PokeByteAdventureState* state,uint32_t now,uint16_t roll);
void pokebyteAdventureUpdate(PokeByteAdventureState* state,uint32_t now);
void pokebyteAdventureClose(PokeByteAdventureState* state);
const char* pokebyteAdventureEventLabel(uint8_t event);
void pokebyteAdventureSetDemo(const char* name);
bool pokebyteAdventureDemoActive();
uint8_t pokebyteAdventureDemoEvent();
