#pragma once
#include <cstdint>
// PC-only, visual/behavioral scheduler. No changes to care stats or save format.
enum : uint8_t { PB_IDLE=0, PB_WALK=1, PB_GESTURE=2 };
enum : uint8_t { PB_NORMAL=0, PB_HAPPY=1, PB_TIRED=2, PB_HUNGRY=3 };
struct PokeBytePersonalityPlan {
  uint8_t mode=PB_IDLE;
  uint8_t action=0;        // existing TPK2/PMD action ID
  uint32_t durationMs=3000;
};
// Returns false for Classic behavior; caller runs original scheduler unchanged.
bool pokebytePersonalityPlan(uint8_t food,uint8_t joy,uint8_t energy,
                             uint8_t hygiene,uint16_t available,
                             uint8_t random100,PokeBytePersonalityPlan* out);
void pokebyteTogglePersonality();
void pokebyteSetPersonalityDemo(const char* name);
const char* pokebytePersonalityContext(uint8_t food,uint8_t joy,uint8_t energy,uint8_t hygiene);

bool pokebytePersonalityDemoActive();
// The same context is used by the visual animation planner AND the home status.
// PB_NORMAL means keep original wording. When P is set to Classic, no override.
uint8_t pokebytePersonalityStatusMood(uint8_t food,uint8_t joy,
                                     uint8_t energy,uint8_t hygiene);
