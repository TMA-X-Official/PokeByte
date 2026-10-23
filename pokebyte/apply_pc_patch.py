"""Phase-1 desktop keyboard controls. Only patches the unpacked desktop shim."""
from pathlib import Path
p=Path("pc/desktop_main.cpp")
s=p.read_text(encoding="utf-8")
assert '#include "sdl_host.h"\n' in s
assert '    if(e.type==SDL_QUIT)alive=false;\n' in s
s=s.replace('#include "sdl_host.h"\n','#include "sdl_host.h"\n#include "../pokebyte/pokebyte_scene.h"\n',1)
s=s.replace('    if(e.type==SDL_QUIT)alive=false;\n',
'''    if(e.type==SDL_QUIT)alive=false;
    // SDL_KeyboardEvent.keysym.sym is at byte offset 20 in SDL2.
    if(e.type==0x300){ uint32_t key=0;std::memcpy(&key,e.padding+20,sizeof(key));
      if(key=='b'||key=='B')pokebyteToggleBackground(); }
''',1)
s=s.replace('TamaPoke - Original PC Emulator','PokeByte - Sunset Meadow | B: Toggle Background',1)
p.write_text(s,encoding="utf-8")
print("Phase 1 desktop keyboard patch installed")
