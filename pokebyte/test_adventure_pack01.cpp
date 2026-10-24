#include "adventure_pack01.h"
#include <cassert>
#include <cstring>
#include <cstdio>
int main(){
 assert(!pokebyteAdventureCanEnter(true,false,false,80));
 assert(!pokebyteAdventureCanEnter(false,true,false,80));
 assert(!pokebyteAdventureCanEnter(false,false,true,80));
 assert(!pokebyteAdventureCanEnter(false,false,false,14));
 assert(pokebyteAdventureCanEnter(false,false,false,15));
 PokeByteAdventureState a{};
 pokebyteAdventureOpen(&a);
 assert(a.stage==PB_ADV_READY && a.steps==0);
 pokebyteAdventureStartStep(&a,1000,20);
 assert(a.stage==PB_ADV_WALKING && a.event==PB_ADV_QUIET && a.steps==1);
 pokebyteAdventureStartStep(&a,2000,77);
 assert(a.steps==1);  // cannot stack steps while walking
 pokebyteAdventureUpdate(&a,2699);
 assert(a.stage==PB_ADV_WALKING);
 pokebyteAdventureUpdate(&a,2700);
 assert(a.stage==PB_ADV_RESULT);
 pokebyteAdventureStartStep(&a,3000,40);
 assert(a.event==PB_ADV_ITEM && a.wildDex==0 && a.steps==2);
 pokebyteAdventureUpdate(&a,4700);
 assert(a.stage==PB_ADV_RESULT);
 pokebyteAdventureStartStep(&a,5000,365);
 assert(a.event==PB_ADV_WILD && a.wildDex==43 && a.steps==3);
 pokebyteAdventureUpdate(&a,6700);
 assert(a.stage==PB_ADV_RESULT);
 assert(std::strcmp(pokebyteAdventureEventLabel(a.event),"Wild Pokemon spotted!")==0);
 pokebyteAdventureClose(&a);
 assert(a.stage==PB_ADV_CLOSED && a.wildDex==0 && a.steps==0);
 pokebyteAdventureSetDemo("wild");
 assert(pokebyteAdventureDemoActive() && pokebyteAdventureDemoEvent()==PB_ADV_WILD);
 pokebyteAdventureSetDemo("item");
 assert(pokebyteAdventureDemoEvent()==PB_ADV_ITEM);
 pokebyteAdventureSetDemo("quiet");
 assert(pokebyteAdventureDemoEvent()==PB_ADV_QUIET);
 pokebyteAdventureSetDemo(nullptr);
 assert(!pokebyteAdventureDemoActive());
 std::puts("PASS: Pack 01 forest adventure pure-state / safety / deterministic outcomes");
}
