#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
OUT="pc/build/windows/PokeByte-Original-Remaster-PC"
mkdir -p "$OUT"
CXX=g++
"$CXX" -std=c++17 -O2 -DPOKEBYTE_PC -Ipc/shim -I. \
  pc/pc_sketch.cpp pet.cpp i18n.cpp sdmon.cpp \
  pc/shim/emu_core.cpp pc/shim/emu_sd.cpp pc/shim/emu_preferences.cpp \
  pc/emu_stubs.cpp pc/desktop_main.cpp pokebyte/remaster_scene.cpp \
  -lSDL2 -static-libgcc -static-libstdc++ -o "$OUT/PokeByte-Remaster-PC.exe"
for dll in SDL2.dll libwinpthread-1.dll; do
  if [ -f "/mingw64/bin/$dll" ]; then cp "/mingw64/bin/$dll" "$OUT/"; fi
done
mkdir -p "$OUT/tools/sdcard"
cp -r tools/sdcard/mons "$OUT/tools/sdcard/mons"
cat > "$OUT/PLAY-POKEBYTE.bat" <<'BAT'
@echo off
cd /d "%~dp0"
start "" "PokeByte-Remaster-PC.exe" --scale 2
BAT
for mode in morning day afternoon evening night; do
 printf '@echo off\r\ncd /d "%%~dp0"\r\nstart "" "PokeByte-Remaster-PC.exe" --scale 2 --time-of-day %s\r\n' "$mode" > "$OUT/PREVIEW-$mode.bat"
done
cat > "$OUT/README.txt" <<'TEXT'
POKEBYTE - ORIGINAL SCENERY REMASTER TEST (Windows 10/11)
1. Extract ZIP into a folder.
2. Double-click PLAY-POKEBYTE.bat to play using the in-game time.
3. Press B to toggle between Classic TamaPoke and the pixel-art remaster.
4. Optionally double-click PREVIEW-morning/day/afternoon/evening/night.bat to
   preview one lighting mode without changing your clock.
5. Click to tap; hold and drag the mouse to swipe the original menus.
6. Save is stored in tamapoke_pc_save.dat, separate from your Waveshare.
   To continue an old PC save, make a backup and copy it to this folder.

The five lighting modes switch automatically using the in-game clock while
the game is running (a redraw follows the clock's regular updates).
The 466x466 backdrop is drawn in matching pixel-art scale; there are NO pasted
wallpaper assets. Original source, real PMD sprites, menus and game mechanics
remain intact. Hardware sound and battery are not emulated on PC.

Original TamaPoke firmware by socquique (MIT); PMD SpriteCollab (CC BY-NC).
Pokemon names and characters Nintendo/Game Freak/The Pokemon Company.
An independent, non-commercial fan project.
TEXT
