#!/usr/bin/env bash
# Build PokeByte Phase 1 from the untouched original game modules plus desktop-specific shim.
set -euo pipefail
cd "$(dirname "$0")/.."
OUT=pc/build/windows/PokeByte-Phase1-PC
mkdir -p "$OUT"
x86_64-w64-mingw32-g++ --version >/dev/null 2>&1 && CXX=x86_64-w64-mingw32-g++ || CXX=g++
"$CXX" -std=c++17 -O2 -DPOKEBYTE_PC -Ipc/shim -I. \
 pc/pc_sketch.cpp pet.cpp i18n.cpp sdmon.cpp \
 pc/shim/emu_core.cpp pc/shim/emu_sd.cpp pc/shim/emu_preferences.cpp \
 pc/emu_stubs.cpp pc/desktop_main.cpp pokebyte/pokebyte_scene.cpp pokebyte/ambient_fx.cpp \
 -lSDL2 -static-libgcc -static-libstdc++ \
 -o "$OUT/PokeByte-Phase1-PC.exe"
for dll in SDL2.dll libwinpthread-1.dll; do
  if [ -f "/mingw64/bin/$dll" ]; then cp "/mingw64/bin/$dll" "$OUT/"; fi
done
mkdir -p "$OUT/tools/sdcard" "$OUT/assets"
cp -r tools/sdcard/mons "$OUT/tools/sdcard/mons"
# Rebuild the exact approved 466x466 grass meadow directly from tracked PNG artwork.
if [ -f pokebyte/assets/meadow-v2.png ]; then
  python pokebyte/build_meadow_asset.py pokebyte/assets/meadow-v2.png "$OUT/assets/sunset-forest.bmp"
elif [ -f pokebyte/assets/sunset-forest.bmp ]; then
  cp pokebyte/assets/sunset-forest.bmp "$OUT/assets/"
else
  echo "ERROR: PokéByte meadow artwork is missing" >&2
  exit 1
fi
# Use a fallback scene for automated build tests. The Windows playtest bundle
# supplies all five correctly framed full-screen minigame scenes.
cp "$OUT/assets/sunset-forest.bmp" "$OUT/assets/minigame.bmp"
# Normal launcher writes 'auto': the running game checks the local PC clock
# each second and changes approved backgrounds without a restart.
cat > "$OUT/PLAY-POKEBYTE.bat" <<'BAT'
@echo off
cd /d "%~dp0"
if "%~1"=="" (
  >"pokebyte-time-mode.cfg" echo auto
) else (
  >"pokebyte-time-mode.cfg" echo %~1
)
start "" "PokeByte-Phase1-PC.exe" --scale 2
BAT
for mode in morning day afternoon evening night; do
  upper=$(printf '%s' "$mode" | tr '[:lower:]' '[:upper:]')
  printf '@echo off\r\ncall "%%~dp0PLAY-POKEBYTE.bat" %s\r\n' "$mode" > "$OUT/PLAY-${upper}.bat"
done
cat > "$OUT/README.txt" <<'TEXT'
POKEBYTE PHASE 2 - CLOUD-FREE WEATHER & SNOW / BALANCED
Run PLAY-POKEBYTE.bat on Windows 10/11 (64-bit). Changes between the five approved time-of-day scenes AUTOMATICALLY while running. PLAY-MORNING/DAY/AFTERNOON/EVENING/NIGHT launchers lock the scene for quick previews.
Press B to switch original/remastered scenery. Press A to toggle BALANCED pollen, grass and fireflies (ON by default). Press W for light rain, S for falling snow. Rain and snow are mutually exclusive and both start OFF. ALL animated clouds removed.
Click to tap; hold and drag the mouse to swipe the original menus.
The game remembers background and ambient preferences in .cfg files. Rain and snow are manual, visual-only weather and do not use real forecasts.
The original TamaPoke v1.17 game logic, interactions and sprites remain intact.
Saves: tamapoke_pc_save.dat in this folder. These are separate from your Waveshare saves.
Battery hardware and sound are not emulated on PC.
Code: based on TamaPoke by socquique (MIT).
Pokémon characters: Nintendo / Game Freak / The Pokémon Company.
PMD artwork: SpriteCollab community (CC BY-NC). Non-commercial use only.
TEXT
