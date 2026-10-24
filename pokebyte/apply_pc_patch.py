"""Phase-1 desktop keyboard controls. Only patches the unpacked desktop shim."""
from pathlib import Path
p=Path("pc/desktop_main.cpp")
s=p.read_text(encoding="utf-8")
assert '#include "sdl_host.h"\n' in s
assert '    if(e.type==SDL_QUIT)alive=false;\n' in s
s=s.replace('#include "sdl_host.h"\n','#include "sdl_host.h"\n#include "../pokebyte/pokebyte_scene.h"\n#include "../pokebyte/ambient_fx.h"\n#include "../pokebyte/personality_pack01.h"\n#include "../pokebyte/personality_pack02.h"\n',1)
s=s.replace('    if(e.type==SDL_QUIT)alive=false;\n',
'''    if(e.type==SDL_QUIT)alive=false;
    // SDL_KeyboardEvent.keysym.sym is at byte offset 20 in SDL2.
    if(e.type==0x300){ uint32_t key=0;std::memcpy(&key,e.padding+20,sizeof(key));
      if(key=='b'||key=='B')pokebyteToggleBackground();
      if(key=='a'||key=='A')pokebyteToggleAmbient();
      if(key=='w'||key=='W')pokebyteToggleWeather();
      if(key=='s'||key=='S')pokebyteToggleSnow();
      if(key=='p'||key=='P')pokebyteTogglePersonality(); }
''',1)
s=s.replace('TamaPoke - Original PC Emulator','PokeByte Phase 3 | P: Reactions  B: Scene  A: Ambience  W: Rain  S: Snow',1)

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

# CI and optional manual previews can pick one lighting mode without
# changing the host clock. Normal PLAY-POKEBYTE uses the local PC clock.
needle=' if(SDL_Init(SDL_INIT_VIDEO)!=0)'
assert s.count(needle)==1, "SDL main entry changed"
s=s.replace(needle, ''' for(int i=1;i<argc;i++)if(!strcmp(argv[i],"--time-of-day")&&i+1<argc)
    pokebyteSetPreviewMode(argv[++i]);
''' + needle,1)
assert '--time-of-day' in s
# Developer screenshot runner only: transient weather, never saved to player prefs.
needle=' if(SDL_Init(SDL_INIT_VIDEO)!=0)'
assert s.count(needle)==1
s=s.replace(needle, ''' for(int i=1;i<argc;i++)if(!strcmp(argv[i],"--weather")&&i+1<argc)
    pokebyteSetWeatherForTest(argv[++i]);
''' + needle,1)
assert 'pokebyteSetWeatherForTest' in s



# Preview launchers must NEVER overwrite the user's actual saved Pokémon.
assert 'int scale=2;bool dump=false,testHome=false,testMinigame=false,testSack=false;' in s
s=s.replace('int scale=2;bool dump=false,testHome=false,testMinigame=false,testSack=false;',
            'int scale=2;bool dump=false,testHome=false,testMinigame=false,testSack=false,noSave=false;')
assert 'else if(!strcmp(argv[i],"--test-sack"))testSack=true;' in s
s=s.replace('else if(!strcmp(argv[i],"--test-sack"))testSack=true;',
            'else if(!strcmp(argv[i],"--test-sack"))testSack=true;else if(!strcmp(argv[i],"--no-save"))noSave=true;')
assert 'emuLoadSave("tamapoke_pc_save.dat");' in s
s=s.replace('emuLoadSave("tamapoke_pc_save.dat");','if(!noSave)emuLoadSave("tamapoke_pc_save.dat");',1)
assert 'pet.flushSave();emuSaveSave("tamapoke_pc_save.dat");' in s
s=s.replace('pet.flushSave();emuSaveSave("tamapoke_pc_save.dat");',
            'if(!noSave){pet.flushSave();emuSaveSave("tamapoke_pc_save.dat");}',1)
# Demo creates a fresh temporary Charmander only in RAM (existing test-home was Bulbasaur).
assert 'pet.chooseStarter(1);pet.eggTap();' in s
s=s.replace('pet.chooseStarter(1);pet.eggTap();',
            'pet.chooseStarter(noSave?4:1);pet.eggTap();',1)
needle=' if(SDL_Init(SDL_INIT_VIDEO)!=0)'
assert s.count(needle)==1
s=s.replace(needle, ''' for(int i=1;i<argc;i++)if(!strcmp(argv[i],"--personality-demo")&&i+1<argc)
    pokebyteSetPersonalityDemo(argv[++i]);
''' + needle,1)
assert 'pokebyteSetPersonalityDemo' in s

# Eight temporary previews: 4 temperaments x low/high bond. No pet save reads
# or writes, and no actual FOOD, ENERGY or BOND manipulation.
needle=' if(SDL_Init(SDL_INIT_VIDEO)!=0)'
assert s.count(needle)==1
s=s.replace(needle, ''' for(int i=1;i<argc;i++){
   if(!strcmp(argv[i],"--temperament")&&i+1<argc)
     pokebyteSetTemperamentDemo(argv[++i]);
   else if(!strcmp(argv[i],"--bond-preview")&&i+1<argc)
     pokebyteSetBondDemo(argv[++i]);
 }
''' + needle,1)
assert 'pokebyteSetTemperamentDemo' in s and 'pokebyteSetBondDemo' in s


p.write_text(s,encoding="utf-8")
print("Phase 1 desktop keyboard patch installed")
