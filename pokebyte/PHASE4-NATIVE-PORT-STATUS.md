# Phase 4 PokéByte: ESP32-S3 native port status

**Status as of 25 October 2026:** The first native hardware code is implemented and has compiled for ESP32-S3 in GitHub CI; **no physical Waveshare test has occurred and no firmware is safe to install yet**. The original TamaPoke firmware, user Pokémon save and installed microSD remain untouched. The public one-click installer stays disabled.

## Source now in development branch

- **Five approved home backgrounds and five minigame backgrounds:** `pokebyte/embedded_scene.h` implements 466×466 24-bit BMP decoding from `/pokebyte/meadow_<mode>.bmp` / `minigame_<mode>.bmp`, converted to RGB565 in one dedicated 434 KiB PSRAM cache. It follows the original local RTC hour (night/morning/day/afternoon/evening) and restores the old renderer if SD art is unavailable.
- **Exact approved art only:** Each scene's raw CRC32 is pinned to the original user-approved ZIP; invalid/synthetic CI background files (including the former grey rectangle fixtures) are rejected. Canonical SHA256 fingerprints for all 12 background BMPs and 303 original `.bin` files are in `APPROVED-ASSETS.md`. User-approved Windows ZIP remains the source of truth. No scene art has been repainted or replaced.
- **Phase 2 ambient rendering:** Existing approved limited pollen/grass/fireflies, optional rain and snow are implemented on the physical Arduino Canvas **without animated clouds**. Controls `FX ON/OFF` and `CLEAR/RAIN/SNOW` have been added to the existing time/settings screen. These FX settings use independent `pbvisual` NVS keys, not saved Pokémon fields.
- **Phase 3 personalities:** Original approved Pack 01/02/03 C++ planners compile as separate Arduino translation units, wired into `TamaPoke.ino` under Arduino build; four short one-shot surprise choreographies and normal Idle return preserved. Original PMD frame files, Bond, temperament genes, care, evolution and save layout are untouched. On embedded, Pack 01 settings use independent Preferences instead of desktop text files.
- **RAM-only test controls:** `PBSTATUS`, `PBMODE morning/day/afternoon/evening/night/auto`, `PBDEMO wake/victory/wander/friendship/off`, and `PBWEATHER clear/rain/snow` allow deterministic physical verification without changing Pet levels/care/saves. Not required for normal players. The five-mode scene override resets in RAM when the board reboots.
- **Safety:** `SD_MMC.begin(...,false)` disables automatic microSD formatting on mount failure. No Adventure or Battle code from rejected PRs #17/#18. Port source branch remains separate from original `main` firmware and from the public installer.

## Verification

- GitHub ESP32-S3 build against Arduino core **esp32:esp32@3.3.10** has passed on a native-port commit, using the original 16MB+OPI PSRAM FQBN and the repository's board libraries. Later additions require their own CI confirmation before calling the latest branch build green.
- Existing 102 Pet/dex/save/mood host game-logic tests and i18n tools tests have passed. `tools/test_waveshare_port.py` and `.github/workflows/pokebyte-native-audit.yml` exercise the native port source assumptions and all three original PMD personality policy suites without hardware.
- **Compilation is not proof of operation on the physical board.** Required pending checks: actual 466×466 display and touch, PSRAM/caching, weather frame rate, all ten approved scenery images, minigame graphics/physics, 151 species PMD sprites, all four gestures, RTC/dayparts, battery/PMU, audio, sleep, reset/save retention and safe rollback.

## User's first USB hardware-identification check (October 2026)

- Windows reported serial port **COM6** for the connected Waveshare, and the user ran the browser-based official Espressif `esptool-js` connection diagnostic.
- Browser detected **ESP32-S3 (QFN56), revision 0**, with **8 MB embedded PSRAM**. The stub loaded and ran; SPI flash ID was **`184020`** (raw 24-bit ID; reverse-order JEDEC `20 40 18`, commonly XMC 128 Mbit = **16 MB flash**). The attached log also showed a `921600`-baud change and the generic recommendation to use a lower rate if commands become unreliable.
- **This was a read-only chip identification, not a complete 16 MB flash/NVS backup**. Do not store the user's hardware MAC address in the public repository.
- **Next gate:** implement/verify a browser-only, private, read-only full-flash backup with a saved checksum, preferably using **115200 baud** for reliability. Preserve the inserted microSD without writes or formatting. The ESP Tool's public `Program` panel is for flashing, not backup. Never click Program or Erase for this step.
- Identification of the ESP32-S3 chip and flash does **not independently prove exact Waveshare 1.75 versus 1.75C display/touch hardware variant**. Physical-device release tests are still required. No PokéByte firmware has been installed on this user's board.

## Remaining blockers before the one-click installer can be enabled

1. **Exact board verification and read-only full-flash/NVS backup**, and a verified restore plan. User has no microSD adapter, so keep the original physical card inserted and untouched; known approved original sprites exist in the verified private Windows ZIP, but unknown unique on-card files might remain.
2. **Browser USB transfer protocol:** the original TamaPoke `PUT` command overwrites sprites, and cannot read back microSD files or upload backgrounds. Need a versioned, checksum-verified, interruption-safe serial asset sync and a browser backup flow; this is not implemented/validated yet.
3. **Hardware test on the user's actual ESP32-S3-Touch-AMOLED-1.75** (not the 1.75C). Physically back up first; don't flash an arbitrary CI binary or re-label original TamaPoke.
4. **One-click website:** after physical QA and asset handling succeed, host a real firmware manifest with checksums, identify the right device, preserve flash/NVS and automate missing assets. The [branded site](../web/pokebyte/index.html) stays explicitly locked until then.

This document supersedes any older statement that Windows-only Phases 1–3 have not begun porting. **Source port is underway and compiles, physical release remains pending.**
