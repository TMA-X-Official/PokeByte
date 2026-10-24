"""Patch only the extracted SDL desktop shim. Never modifies original hardware firmware."""
from pathlib import Path

desktop=Path("pc/desktop_main.cpp")
s=desktop.read_text(encoding="utf-8")
def replace_once(needle,replacement):
    global s
    assert s.count(needle)==1, f"Desktop shim changed: {needle[:60]}"
    s=s.replace(needle,replacement,1)

replace_once('#include "sdl_host.h"\n', '#include "sdl_host.h"\n#include "../pokebyte/remaster_scene.h"\n')
replace_once(' if(SDL_Init(SDL_INIT_VIDEO)!=0)',
''' for(int i=1;i<argc;i++)if(!strcmp(argv[i],"--time-of-day")&&i+1<argc) {
   const char*mode=argv[++i];
   int h=!strcmp(mode,"morning")?7:!strcmp(mode,"day")?12:!strcmp(mode,"afternoon")?16:!strcmp(mode,"evening")?19:!strcmp(mode,"night")?23:-1;
   pokebyteSetPreviewHour(h);
 }
 if(SDL_Init(SDL_INIT_VIDEO)!=0)''')
replace_once('TamaPoke - Original PC Emulator','PokeByte - Original Scene Remaster | B: Classic / Remaster')
replace_once('    if(e.type==SDL_QUIT)alive=false;',
'''    if(e.type==SDL_QUIT)alive=false;
    if(e.type==0x300){ uint32_t key=0;std::memcpy(&key,e.padding+20,sizeof(key));
      if(key=='b'||key=='B')pokebyteToggleRemaster(); }''')
desktop.write_text(s,encoding="utf-8")

# Original emulator's Canvas.fillRect assumes x is within the screen even
# when the original game's clouds drift fully offscreen. Clip empty rectangles
# before creating STL iterator ranges to prevent an out-of-bounds write.
shim=Path("pc/shim/emu_core.cpp")
code=shim.read_text(encoding="utf-8")
old='void Arduino_Canvas::fillRect(int x,int y,int w,int h,uint16_t c){int x0=std::max(0,x),y0=std::max(0,y),x1=std::min(W,x+w),y1=std::min(H,y+h);for(int j=y0;j<y1;j++)std::fill(frame.begin()+j*W+x0,frame.begin()+j*W+x1,c);}'
new='void Arduino_Canvas::fillRect(int x,int y,int w,int h,uint16_t c){if(w<=0||h<=0||x>=W||y>=H||x+w<=0||y+h<=0)return;int x0=std::max(0,x),y0=std::max(0,y),x1=std::min(W,x+w),y1=std::min(H,y+h);for(int j=y0;j<y1;j++)std::fill(frame.begin()+j*W+x0,frame.begin()+j*W+x1,c);}'
assert code.count(old)==1,"SDL clipping shim changed"
shim.write_text(code.replace(old,new),encoding="utf-8")
print("PokéByte original-scene remaster SDL patch applied")
