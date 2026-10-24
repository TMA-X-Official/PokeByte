#include "personality_pack01.h"
#include "personality_pack02.h"
#include <cassert>
#include <cstdio>
// Standalone pure planner tests, no mock sprites, keyboard or artificial stats writes.
int main(){
 PokeBytePersonalityPlan p{};
 pokebyteSetSocialSnapshot(101,100,100,85); // Playful high-bond fixture
 const uint16_t full=(1u<<1)|(1u<<2)|(1u<<5)|(1u<<7)|(1u<<9)|(1u<<10);
 assert(pokebytePersonalityPlan(80,90,95,100,full,4,&p));
 assert(p.mode==PB_GESTURE && (p.action==7||p.action==9||p.action==10));
 assert(pokebytePersonalityPlan(80,75,30,100,full,4,&p));
 assert(p.mode==PB_GESTURE && (p.action==10||p.action==9));
 assert(pokebytePersonalityPlan(30,75,80,100,full,4,&p));
 assert(p.mode==PB_GESTURE && p.action==5);
 // The identical context must drive the actual HUD label.
 assert(pokebytePersonalityStatusMood(30,85,90,90)==PB_HUNGRY);
 assert(pokebytePersonalityStatusMood(55,85,30,90)==PB_TIRED);
 assert(pokebytePersonalityStatusMood(80,90,95,90)==PB_HAPPY);
 // Status and scheduler must agree at their threshold boundaries.
 assert(pokebytePersonalityStatusMood(39,85,90,90)==PB_HUNGRY);
 assert(pokebytePersonalityStatusMood(40,85,44,90)==PB_TIRED);
 assert(pokebytePersonalityStatusMood(40,85,45,90)==PB_NORMAL);
 // Preview modes only influence context; stats are not edited.
 pokebyteSetPersonalityDemo("hungry");
 assert(pokebytePersonalityStatusMood(80,90,95,90)==PB_HUNGRY);
 pokebyteSetPersonalityDemo("tired");
 assert(pokebytePersonalityStatusMood(80,90,95,90)==PB_TIRED);
 pokebyteSetPersonalityDemo("happy");
 assert(pokebytePersonalityStatusMood(80,90,95,90)==PB_HAPPY);
 pokebyteSetPersonalityDemo("off");
 assert(pokebytePersonalityStatusMood(80,90,95,90)==PB_HAPPY);
 // Species without optional PMD gestures: graceful idle fallback, not an
 // unsupported, invisible animation.
 const uint16_t basic=(1u<<0)|(1u<<1)|(1u<<2);
 assert(pokebytePersonalityPlan(30,80,80,80,basic,4,&p));
 assert(p.mode==PB_IDLE);
 assert(pokebytePersonalityPlan(80,80,80,80,basic,4,&p));
 assert(p.mode==PB_IDLE || p.mode==PB_WALK);
 // If only one Walk direction is available, never ask for both.
 assert(pokebytePersonalityPlan(80,90,80,90,(1u<<0)|(1u<<1),70,&p));
 assert(p.mode==PB_IDLE);
 std::puts("Phase 3 original-sprite reaction planner: 16 reaction/status/fallback tests passed");
 return 0;
}
