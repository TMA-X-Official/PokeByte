# PokéByte — Original TamaPoke Scene Remaster (PC Playtest)

**No wallpaper files. No redrawn Pokémon.** This is a desktop-only enhancement to TamaPoke's existing 466×466 procedural sky/horizon/biome renderer using restrained 2-pixel-scale shading, improved color gradients, sparse pixel clouds, distant hills, and five time-of-day lighting palettes.

The original TamaPoke game logic, care rules, minigame, swipe interactions, menus, all Pokémon PMD sprites and animations are compiled and used without replacing their artwork. The `POKEBYTE_PC` compile guard keeps the physical Waveshare firmware behavior unchanged.

**Run on Windows:** `PLAY-POKEBYTE.bat`, then press **B** to switch between original TamaPoke and the remaster. The choice is stored in `pokebyte-background.cfg`. To test specific colors, use `PREVIEW-morning.bat`, `PREVIEW-day.bat`, `PREVIEW-afternoon.bat`, `PREVIEW-evening.bat`, or `PREVIEW-night.bat`. Standard mode follows the computer's local time and changes with the clock while running; sleeping remains night. Preview mode is only for testing and does not change the real clock.

Time modes: Morning 06:00–09:59, Day 10:00–13:59, Afternoon 14:00–17:59, Evening 18:00–19:59, Night 20:00–05:59. Save files remain entirely separate from the physical Waveshare.

**How the desktop build works:** the workflow unpacks the original desktop compatibility layer from `pc-port.tar.xz.b64`, applies `pokebyte/apply_remaster_pc.py`, compiles `pokebyte/remaster_scene.cpp`, tests launch and five distinct screenshots, and uploads a downloadable Windows ZIP. The SDL clipping fix affects only desktop rendering, guarding against offscreen clouds causing memory corruption.

The original-game C++ and Python tests have passed locally. **Phase 1 stays in progress until the user completes the Windows playtest.**

Original TamaPoke project by [socquique](https://github.com/socquique/TamaPoke), MIT licensed. Pokémon by Nintendo/Game Freak/The Pokémon Company; pixel sprites by PMD SpriteCollab (CC BY-NC), non-commercial use only.
