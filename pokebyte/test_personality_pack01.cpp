#include "personality_pack01.h"
#include <cassert>
#include <cstdio>
// Standalone pure planner tests, no mock sprites, keyboard or artificial stats writes.
int main(){
 PokeBytePersonalityPlan p{};
 const uint16_t full=(1u<<1)|(1u<<2)|(1u<<5)|(1u<<7)|(1u<<9)|(1u<<10);
 assert(pokebytePersonalityPlan(80,90,95,100,full,4,&p));
 assert(p.mode==PB_GESTURE && (p.action==7||p.action==9||p.action==10));
 assert(pokebytePersonalityPlan(80,75,30,100,full,4,&p));
 assert(p.mode==PB_GESTURE && (p.action==10||p.action==9));
 assert(pokebytePersonalityPlan(30,75,80,100,full,4,&p));
 assert(p.mode==PB_GESTURE && p.action==5);
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
 std::puts("Phase 3 original-sprite reaction planner: 6 policy/fallback tests passed");
 return 0;
}
