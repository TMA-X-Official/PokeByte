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
 pc/emu_stubs.cpp pc/desktop_main.cpp pokebyte/pokebyte_scene.cpp pokebyte/ambient_fx.cpp pokebyte/personality_pack01.cpp pokebyte/personality_pack02.cpp pokebyte/personality_pack03.cpp \
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
# Isolated visual demos start a fresh Charmander without reading or writing
# tamapoke_pc_save.dat; existing Pokémon remain completely safe.
for mood in happy tired hungry; do
  upper=$(printf '%s' "$mood" | tr '[:lower:]' '[:upper:]')
  printf '@echo off\r\ncd /d "%%~dp0"\r\nstart "" "PokeByte-Phase1-PC.exe" --scale 2 --test-home --no-save --personality-demo %s\r\n' "$mood" > "$OUT/TEST-${upper}-CHARMANDER.bat"
done
# Compare four individual temperaments at LOW and HIGH bond, without touching
# any player's Pokémon save. Demo images label their visual-only overrides.
for temperament in playful calm curious shy; do
  upper=$(printf '%s' "$temperament" | tr '[:lower:]' '[:upper:]')
  for stage in low high; do
    bst=$(printf '%s' "$stage" | tr '[:lower:]' '[:upper:]')
    printf '@echo off\r\ncd /d "%%~dp0"\r\nstart "" "PokeByte-Phase1-PC.exe" --scale 2 --test-home --no-save --temperament %s --bond-preview %s\r\n' "$temperament" "$stage" > "$OUT/TEST-${upper}-${bst}-BOND.bat"
  done
done
# Safe one-click previews for four little surprises. All use the original
# Charmander PMD and NEVER load or write the real player's Pokémon save.
for event in wake victory wander friendship; do
  upper=$(printf '%s' "$event" | tr '[:lower:]' '[:upper:]')
  printf '@echo off\r\ncd /d "%%~dp0"\r\nstart "" "PokeByte-Phase1-PC.exe" --scale 2 --test-home --no-save --surprise-demo %s\r\n' "$event" > "$OUT/TEST-SURPRISE-${upper}.bat"
done
cat > "$OUT/README.txt" <<'TEXT'
POKEBYTE PHASE 3 - PERSONALITY PACK 03 / LITTLE SURPRISES
Run PLAY-POKEBYTE.bat for the real persistent game on Windows 10/11 (64-bit). Changes between the five approved time-of-day scenes AUTOMATICALLY while running. PLAY-MORNING/DAY/AFTERNOON/EVENING/NIGHT launchers lock the scene for quick previews.
Press P to toggle enhanced mood-aware reactions / original idle behavior. Press B to switch original/remastered scenery. Press A to toggle BALANCED pollen, grass and fireflies (ON by default). Press W for light rain, S for falling snow. Rain and snow are mutually exclusive and both start OFF. ALL animated clouds removed.
Click to tap; hold and drag the mouse to swipe the original menus.
The game remembers background and ambient preferences in .cfg files. Rain and snow are manual, visual-only weather and do not use real forecasts.
Eight temperament comparison previews TEST-PLAYFUL/CALM/CURIOUS/SHY-LOW/HIGH-BOND.bat: an isolated Charmander with real existing animations and simulated LOW/HIGH bond. These ONLY change demo behavior and a small label, not the saved Pokemon or actual bond. The profile screen shows the real Pokemon temperament and friendship stage.\nTEST-HAPPY-CHARMANDER.bat, TEST-TIRED-CHARMANDER.bat, TEST-HUNGRY-CHARMANDER.bat: safe standalone visual demos. They DO NOT read or change your real saved Pokemon and always use the original Charmander PMD animations. Real game behavior reflects actual needs, not the preview modes. Missing gestures safely fall back to Idle/Walk. The original TamaPoke v1.17 care, evolution, stats, interactions and sprites remain intact.
Phase 3 PACK 03: TEST-SURPRISE-WAKE.bat, TEST-SURPRISE-VICTORY.bat, TEST-SURPRISE-WANDER.bat and TEST-SURPRISE-FRIENDSHIP.bat launch isolated no-save Charmander previews. Each test plays ONCE, shows Back to normal upon completion, settles into Idle, and then resumes regular behavior. Relaunch to see the surprise again. Normal game has NO event labels: after waking, following a sufficiently strong completed ball minigame, rarely while exploring, and occasionally after a caress with strong bond. Existing animations only; no changes to ball rewards or pet stats.
Saves: tamapoke_pc_save.dat in this folder. These are separate from your Waveshare saves.
Battery hardware and sound are not emulated on PC.
Code: based on TamaPoke by socquique (MIT).
Pokémon characters: Nintendo / Game Freak / The Pokémon Company.
PMD artwork: SpriteCollab community (CC BY-NC). Non-commercial use only.
TEXT
