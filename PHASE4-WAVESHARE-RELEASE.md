# PokéByte — Phase 4 Waveshare Release

**✅ PHASE 4 COMPLETED — user-approved on Waveshare, 25 October 2026.**

The project owner confirms that PokéByte's integrated browser installation succeeded on their real **Waveshare ESP32-S3-Touch-AMOLED-1.75** and reports that the game appears to work. They explicitly instructed that **Phase 4 — Waveshare release** be marked **DONE** on GitHub. This is acceptance of the numbered hardware-delivery milestone. Phases **1, 2, 3 and 4 are now complete** in the [project roadmap](ROADMAP.md).

## Delivered and accepted hardware milestone

- **Native device:** ESP32-S3 on the correct round Waveshare 1.75-inch 466×466 AMOLED display (16 MB flash, 8 MB PSRAM), connected to Windows via USB-C.
- **Browser install:** Genuine PokéByte firmware installed from the project's HTTPS [owner-tested beta installer](https://tma-x-official.github.io/PokeByte/beta/). GitHub Actions built the firmware and deployed the integrated firmware-and-approved-backgrounds workflow successfully ([reference run](https://github.com/TMA-X-Official/PokeByte/actions/runs/40181033046)).
- **Original artwork:** Approved five meadow dayparts and matching minigame backgrounds, twelve exact original BMP files hosted at `web/pokebyte/approved-backgrounds.zip` on the Phase 4 source branch; checksum-checked against [APPROVED-ASSETS.md](APPROVED-ASSETS.md). Original 151 Pokémon and authentic sprite animation files retained, with the approved temperament/Bond/mood/surprise system.
- **Atmosphere and user refinements:** Gentle pollen, fireflies and grass, optional rain/snow, no new animated clouds; no automatic AMOLED dimming; 50% lower sound-sample amplitude; stronger-contrast, more compact 112px battery percentage display.
- **No rejected gameplay:** The owner cancelled Adventure/Forest Trail and Pokémon battles; rejected [PR #17](https://github.com/TMA-X-Official/PokeByte/pull/17) and [PR #18](https://github.com/TMA-X-Official/PokeByte/pull/18) remain closed without merging.

## Separate follow-up: general-audience stable v1.0

**Completed Phase 4 does not assert an unrestricted public stable release or successful testing of every recovery path.** The latest development code includes preparations for the complete 303-file original Pokémon asset set on fresh microSD cards; these and save-preserving upgrades, interrupted-transfer recovery, first-time/blank-card testing, cross-browser/board checking and broad physical regressions are tracked in the [v1.0 release-readiness checklist](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/pokebyte/PHASE4-V1-RELEASE-READINESS.md).

- **Phase 4 owner delivery:** ✅ complete.
- **Public stable v1.0 certification/tag:** not yet issued.
- **Code branch and review:** [pokebyte-phase4-waveshare-release-prep](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase4-waveshare-release-prep), [PR #20](https://github.com/TMA-X-Official/PokeByte/pull/20), retained **unmerged** for review and stable hardening. The original upstream `main` firmware and original TamaPoke web installer are not silently overwritten.
- **Release policy:** Owner's private consent to skip backing up an old TamaPoke save was specific to their test. Do not apply that waiver to new players; never format or overwrite unique microSD files without verified safeguards.

## Historical Phase 4 engineering record

The following pre-hardware-test context is retained **as history**, not as the current roadmap status. Source notes and archives were written before the owner installed the working beta. See the [Phase 4 branch engineering record](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/pokebyte/PHASE4-WAVESHARE-RELEASE.md), [native-port report](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/pokebyte/PHASE4-NATIVE-PORT-STATUS.md), and [GitHub project continuity](PROJECT-CONTINUITY.md).

