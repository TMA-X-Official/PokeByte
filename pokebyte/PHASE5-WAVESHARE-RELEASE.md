# PokéByte Phase 5 — Waveshare ESP32-S3-Touch-AMOLED-1.75 Release

**Status: STARTED — firmware port and hardware preflight, NOT READY TO FLASH.**

## Controlling user decisions (24 October 2026)

- User explicitly approved Windows Phases **1 (distinct five daypart scenes)**, **2 (balanced ambience, no animated clouds)** and **3 (three personality packs and four distinct surprises)**.
- User rejected both Phase 4 experimental alternatives after Windows screenshot/playtests: **Forest Trail / Adventure Quiet–Item–Wild**, and later **classic-style Pokémon battles**.
- **Phase 4 is SKIPPED and removed from the active release roadmap**; rejected PRs [#17](https://github.com/TMA-X-Official/PokeByte/pull/17) and [#18](https://github.com/TMA-X-Official/PokeByte/pull/18) are closed **without merge**. No Adventure or Battle feature ships.
- The next product milestone remains named **Phase 5 — Waveshare release** (not renumbered).
- Preserve original touch/swipes, care, original two minigames, evolution, eggs, save data, audio, original 151 PMD species variants, SD sprites and original hardware firmware fallback.
- **No automatic merge, erasing, formatting, flashing or changing the user's NVS save.** Hardware acceptance is required before any production release.

## Accepted Windows reference — DO NOT SUBSTITUTE

- `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip`
- SHA-256 `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`.
- The exact **12 466×466 approved bitmap backgrounds** and original **303 .bin SD assets (302 PMD animation binaries plus thumbs.bin)** have checksums in [APPROVED-ASSETS.md](https://github.com/TMA-X-Official/PokeByte/blob/main/APPROVED-ASSETS.md).
- The source repository / raw GitHub Actions Windows artifacts may include *synthetic fixture art*. **Never ship fixture scenes, fill grey rectangles with fake art, or replace all dayparts with a single sunset**.
- **Do not copy the Windows .exe or SDL2.dll to a device.** Use correctly compiled ESP32-S3 firmware and a FAT microSD staged with the approved original art and sprite binaries.

## Hardware specification and starting configuration

- **Exactly** Waveshare **ESP32-S3-Touch-AMOLED-1.75** (round 466×466 AMOLED, CO5300 QSPI, CST9217 touch, AXP2101 PMU, PCF85063 RTC, ES8311 audio, TF slot). Verify that the user has the **-1.75**, not the different **-1.75C** model, before choosing an image.
- Arduino CLI core **esp32:esp32@3.3.10** is the repo's currently pinned CI baseline. FQBN: `esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=16M,PSRAM=opi,PartitionScheme=app3M_fat9M_16MB`.
- 466×466×RGB565 display framebuffer ~**434 KiB** of PSRAM. Approved 466×466 uncompressed 24-bit BMP images are ~**652 KiB each** on microSD.
- Original firmware version is **1.17**. Do not relabel the unported baseline as a new PokéByte release. Version numbering and publishable firmware binaries come after successful full hardware port and device validation.
- The current source's `TamaPoke.ino` still includes the Phase 1–3 personality headers only under `#ifdef POKEBYTE_PC`, and the accepted BMP/ambient loaders use `pc/shim/`, SDL-style I/O and PC configuration files. Therefore **an Arduino compile of this branch by itself does NOT implement approved Windows features on the Waveshare.** A real ESP32 port is the next engineering task.

## Milestone gates for a safe Waveshare release

1. **Hardware-independent source audit / safety (in progress):** leave `main` intact; create this Phase 5 branch directly from approved Phase 3 branch, with no Phase 4 code. Disable SD card auto-format on mount failure. Compile original hardware target in GitHub Actions, run logic regressions and no-adventure/no-battle assertions.
2. **Approved SD file staging (implemented tool; not flashed):** run `python tools/phase5_prepare_sd.py --source "PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip" --output ./Pokebyte_SD_STAGING`. Tool verifies full ZIP checksum, each of the 12 approved BMP checksums/dimensions, the exact original PMD-set digest, and stages `pokebyte/*.bmp` plus `mons/*.bin` in a *new* directory. It NEVER formats a card or touches connected devices. User should copy these files to a backed-up microSD only after validating its current contents. The new `/pokebyte/*.bmp` hardware paths are **staging conventions, not yet wired into the firmware**.
3. **Real hardware port (not yet complete):** introduce an embedded SD BMP decoder and frame rendering paths behind an explicit feature flag; port the approved Phase 1 scene clock and minigame backgrounds, Phase 2 balanced pollen / grass / fireflies / manual rain & snow without clouds, and Phase 3 personality/surprise scheduling. Avoid `pc/shim`/SDL or host filesystem includes. Test PSRAM footprint/frame cadence and ensure file errors fall back to classic rendering rather than crash; preserve original SD sprite-loading and save format.
4. **Device backup BEFORE flashing (requires the user's device and explicit action):** record exact model and USB port; save a copy of the currently installed *full flash* plus NVS/partition table, backup the microSD, and verify checksums. **Never use an erase-all upload or format option**, never change partition layout without a separate verified save migration. Do not claim the project can restore lost saves until the actual backup/restore flow is validated.
5. **First development hardware test:** flash the explicit approved firmware only after backup and user confirmation, verify display orientation, touch, SD and shiny sprites, power rails, RTC, charging indicator, sound, sleep/dim, frame rate and no spontaneous reboot; then check 5 home+minigame scenes, ambient modes, ball/sack games, all personality interactions, wake/victory/wander/friendship and a reboot/save retention test.
6. **Release acceptance:** gather user approval from physical hardware, tag/build a reproducible firmware release and approved SD bundle, include rollback guidance with checked backup hashes. Keep the older Phase 3 Windows ZIP forever as the trusted visual target. No rejected Phase 4 features.

## Dedicated PokéByte browser installer (design preview — NOT flashable)

- User requested a PokéByte installation website analogous to the original TamaPoke browser installer.
- Created a separate, responsive, branded static landing page at [`web/pokebyte/index.html`](../web/pokebyte/index.html), documented in [`web/pokebyte/README.md`](../web/pokebyte/README.md).
- The installer button is deliberately **disabled**. There is **no** ESP Web Tools active element, firmware manifest, `.bin` flashing image, Web Serial or USB transfer. Original `web/index.html` and `web/manifest.json` are **unchanged and still install upstream TamaPoke 1.17**.
- [Standalone static safety CI](https://github.com/TMA-X-Official/PokeByte/actions/runs/40134413049) **passed**, asserting the new page cannot initiate flashing, preserves the original installer and names only the accepted Phases 1–3 without Phase 4 features.
- **Not publicly hosted or released**: current GitHub branch link is a source preview. Do not expose it as a working installer or configure Pages until user reviews and signs off.
- After the embedded port, binary validation, physical testing and flash/NVS/microSD backup/rollback are ready, implement genuine [ESP Web Tools](https://esphome.github.io/esp-web-tools/) ESP32-S3 flashing (correct verified manifest offsets) and safe Web Serial approved-asset copying. That must be a separate review; no erase and no user-save loss.

## Remaining risks

- **Missing verified artwork in GitHub:** SHA fingerprints exist but the actual original distinct BMP files are retained privately in the approved ZIP and not automatically accessible to CI. The release process requires that original ZIP explicitly.
- **PC-only implementation:** even a successful `arduino-cli compile` of the initial phase-5 branch outputs the **classic** firmware path until approved features are truly ported.
- **SD auto-format:** on the original firmware, `SD_MMC.begin("/sdcard", true, true)` automatically formats upon some mount errors; the Phase 5 branch changes this to `false` as the first safety patch. Do not apply it to the user's live device until a reviewed firmware build.
- **PSRAM/performance and save:** Sprite blobs, framebuffer and two scenery buffers can conflict. Prefer one framebuffer plus a reusable scene buffer or streaming pipeline and measure actual heap margin. Original `Pet` Preferences/NVS storage and RTC time progression must remain bit-compatible.
- **Waveshare product variants:** wrong target can lead to a blank screen or wrong touch mapping. Verify model before selecting a build or instructions.

**Next action:** complete real ESP32 hardware port plus non-destructive backup method and reproduce a verified firmware binary. No hardware-ready ZIP is available yet. Do not advertise the staged SD art bundle as a flashable firmware.