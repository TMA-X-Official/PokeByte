"""Phase-1 desktop keyboard controls. Only patches the unpacked desktop shim."""
from pathlib import Path
p=Path("pc/desktop_main.cpp")
s=p.read_text(encoding="utf-8")
assert '#include "sdl_host.h"\n' in s
assert '    if(e.type==SDL_QUIT)alive=false;\n' in s
s=s.replace('#include "sdl_host.h"\n','#include "sdl_host.h"\n#include "../pokebyte/pokebyte_scene.h"\n#include "../pokebyte/ambient_fx.h"\n',1)
s=s.replace('    if(e.type==SDL_QUIT)alive=false;\n',
'''    if(e.type==SDL_QUIT)alive=false;
    // SDL_KeyboardEvent.keysym.sym is at byte offset 20 in SDL2.
    if(e.type==0x300){ uint32_t key=0;std::memcpy(&key,e.padding+20,sizeof(key));
      if(key=='b'||key=='B')pokebyteToggleBackground();
      if(key=='a'||key=='A')pokebyteToggleAmbient();
      if(key=='w'||key=='W')pokebyteToggleWeather(); }
''',1)
s=s.replace('TamaPoke - Original PC Emulator','PokeByte Phase 2 | B: Scene  A: Ambience  W: Rain',1)

# Test flags exercise the REAL original minigames, preserving their logic.
s=s.replace('int scale=2;bool dump=false,testHome=false;',
            'int scale=2;bool dump=false,testHome=false,testMinigame=false,testSack=false;')
s=s.replace('else if(!strcmp(argv[i],"--test-home"))testHome=true;',
            'else if(!strcmp(argv[i],"--test-home"))testHome=true;else if(!strcmp(argv[i],"--test-minigame"))testMinigame=true;else if(!strcmp(argv[i],"--test-sack"))testSack=true;')
s=s.replace(' if(testHome){pet.chooseStarter(1);pet.eggTap();pet.eggTap();pet.eggTap();}',
''' if(testHome||testMinigame||testSack){pet.chooseStarter(1);pet.eggTap();pet.eggTap();pet.eggTap();}
 if(testMinigame||testSack){
  extern void startGame();extern void startSack();
  if(testMinigame)startGame();else startSack();
 }''')
assert "testMinigame" in s and "startGame();" in s, "Test flags failed to patch desktop main"

p.write_text(s,encoding="utf-8")
print("Phase 1 desktop keyboard patch installed")
