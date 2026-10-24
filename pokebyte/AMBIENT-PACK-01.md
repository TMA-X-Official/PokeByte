# PokéByte Phase 2 — Ambient Pack 01 (Balanced)

**Status:** Windows playtest candidate. Phase 1 is completed and preserved in the baseline `pokebyte-phase1-minigames-scene` branch.

## What is new

- **Balanced ambience by default:** restrained pixel clouds, grass-edge movement and sparse pollen by day, small fireflies by evening/night, with no gameplay consequences.
- **Optional drizzle:** press **W** to toggle. Default OFF; simulated visual effect only, not local weather.
- **Ambient toggle:** press **A** to disable/enable all particles. Preferences are kept in `pokebyte-ambient.cfg`.
- **Automatic time-of-day changes while playing:** every second the desktop environment checks PC local time. The scene switches when the clock enters Morning (06:00–09:59), Day (10:00–13:59), Afternoon (14:00–17:59), Evening (18:00–20:59), Night (21:00–05:59). No restart required.
- **Manual scene test launchers:** `PLAY-MORNING.bat`, etc. A manual mode is intentionally locked until the normal `PLAY-POKEBYTE.bat` is launched again, which restores automatic switching.
- **Classic mode:** press **B** to return to original TamaPoke scenery. Press again for PokéByte remastered scenes.
- **Battery:** Windows shows **PC**, since the Waveshare battery cannot be read. The physical firmware source can show the real AXP2101 percentage, but no new firmware has been flashed.

## Preserving the approved art

The Windows release ZIP includes the **exact same ten bitmap files** approved in Phase 1: `assets/meadow_{morning,day,afternoon,evening,night}.bmp` and matching `assets/minigame_*.bmp`. Neither the source nor the build scripts modify these files.

The GitHub source branch currently does not include all artwork binary files. GitHub CI generates synthetic temporary fixtures using `pokebyte/test_modes_ci.py` solely to test each selectable mode; these temporary images **are not the approved scenes** and must be replaced with the ten approved originals when packaging the release ZIP.

## Scope and tests

The game continues running the original TamaPoke v1.17 source, original PMD Pokémon sprites, touch/swipe code, all menus, original ball/training game logic and separate save files. The approved art is loaded as a background, before the original UI/sprites are rendered. The PC-only code remains guarded with `POKEBYTE_PC`; Phase 5 will separately optimize and test firmware for the Waveshare.

Windows CI: build + home startup screenshot + five distinct scenes + ball and training screenshots. **Human playtesting is still required** to approve motion, visual balance and the live clock transition. Windows download is a test build, not Phase 2 completion.

TamaPoke originally by socquique (MIT); PMD SpriteCollab artwork CC BY-NC, Pokémon characters Nintendo/Game Freak/The Pokémon Company. Non-commercial fan project.
