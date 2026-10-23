# PokéByte Phase 1 v2 — Sunset Meadow (Windows playtest)

**What changed:** The previous lakeside background made Pokémon appear to float. This revision places a broad grassy clearing under the original game sprite at ground level (`PET_GROUND=304`) while preserving the sunset, trees and distant lake.

**Original gameplay untouched:** The `POKEBYTE_PC` conditional is only used by the desktop port. The original TamaPoke firmware, sprites, menus, controls, mini-game and Waveshare data remain unchanged. `main` and `pc-original-emulator` remain safe.

**Windows:** Extract the ZIP and double-click `PLAY-POKEBYTE.bat`. **B** toggles Classic / Sunset Meadow; `pokebyte-background.cfg` remembers that choice. Mouse click = tap; mouse drag = swipe. Your PC save (`tamapoke_pc_save.dat`) is separate from the physical Waveshare.

**Committed artwork:** `pokebyte/assets/meadow-v2.png` is the approved complete 466 × 466 image, cropped and vertically reframed from the mockup to give walking Pokémon a proper grass field. `pokebyte/build_meadow_asset.py` uses Python's standard library to reconstruct the full-resolution Windows BMP. `pokebyte/build-windows.sh` automatically packages it as `assets/sunset-forest.bmp` (the legacy internal filename is intentionally retained for compatibility). No art downloads, Python imaging packages, or SD adapters are required.

**Verification:** The local Linux emulator displayed Bulbasaur and Charmander standing on the grass, original classic scenery was restored using the B-toggle configuration, and the source code has a dedicated GitHub Windows build with a startup screenshot test. Windows interaction and animation quality still need user playtesting before Phase 1 is marked complete.

Credits: based on TamaPoke v1.17 by [socquique](https://github.com/socquique/TamaPoke) (MIT); character copyright Nintendo/Game Freak/The Pokémon Company; original sprites from PMD SpriteCollab (CC BY-NC). Non-commercial fan project.
