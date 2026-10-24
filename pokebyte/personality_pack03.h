#pragma once
#include <cstdint>
// Phase 3 Pack 03: optional, rare, PC-only events using real existing PMD actions.
// No changes to assets, player save, rewards, physics, or care.
enum : uint8_t { PB_SURPRISE_NONE=0, PB_SURPRISE_WAKE=1,
  PB_SURPRISE_VICTORY=2, PB_SURPRISE_WANDER=3, PB_SURPRISE_FRIENDSHIP=4 };
struct PokeByteSurprisePlan {
 uint8_t event=PB_SURPRISE_NONE, action=0, mode=0; // 0/2=existing Idle/Gesture, 1=Walk
 int16_t targetX=233;
 uint32_t durationMs=2800;
};
void pokebyteSurpriseSetDemo(const char* name);
bool pokebyteSurpriseDemoActive();
const char* pokebyteSurpriseDemoLabel();
void pokebyteSurpriseNotify(uint8_t type,uint32_t now,bool eligible);
bool pokebyteSurpriseChoose(uint32_t now,uint8_t temperament,uint8_t bond,
                           uint16_t available,uint16_t roll1000,
                           PokeByteSurprisePlan* out);
void pokebyteSurpriseClear(); // remove pending event on Pokémon change

// A preview is one genuine moment, never a perpetual repeating animation.
// The home label changes to "Back to normal" once the original gesture ends.
void pokebyteSurpriseMarkFinished();
bool pokebyteSurpriseDemoFinished();
