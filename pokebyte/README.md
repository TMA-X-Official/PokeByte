# PokéByte — Phase 1: Visual upgrade

The game keeps the original TamaPoke v1.17 gameplay, Pokémon sprite files, menus, minigame and swipe logic. The optional scene hook is guarded with `POKEBYTE_PC`: ESP32 firmware builds use the original renderer unchanged.

**Windows:** Double-click `PLAY-POKEBYTE.bat`. Press **B** to switch between Classic and Sunset Forest. Click to tap and hold-drag to swipe. Your scene choice is stored in `pokebyte-background.cfg`, while `tamapoke_pc_save.dat` stores the local save.

**Artwork:** The approved 466×466 sunset forest background is a separate 24-bit BMP included in the assembled Windows testing package at `assets/sunset-forest.bmp`. For a source build, place the asset at `pokebyte/assets/sunset-forest.bmp` before running `pokebyte/build-windows.sh`. If the image is missing, the game automatically shows the original background instead. No SD card files or hardware firmware are changed.

**Tests:** Local Linux startup test confirmed that the full-resolution forest renders behind the original game controls and character. The original-background mode and swipe smoke test also passed. Windows CI builds the code and performs a headless startup test. A human Windows playtest is required before Phase 1 is declared finished.

Original game: [TamaPoke by socquique](https://github.com/socquique/TamaPoke); original MIT license and PMD SpriteCollab credits are retained.
