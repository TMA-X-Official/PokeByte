# PokéByte — Phase 5 Waveshare Release

**Status: In progress — preparation and hardware-port work; NOT READY TO FLASH.**

**User decision (24 October 2026):** Phases 1–3 completed and approved on Windows. The user rejected both Phase 4 prototypes (Forest Trail and Pokémon-style battles) and explicitly **skipped Phase 4**, asking to proceed directly to **Phase 5 Waveshare release**. Do not ship either Adventure or Battle. Review PRs [#17](https://github.com/TMA-X-Official/PokeByte/pull/17) and [#18](https://github.com/TMA-X-Official/PokeByte/pull/18) were both closed *without merging*, with code history retained for traceability. The active roadmap no longer contains Phase 4.

**Current engineering branch:** [`pokebyte-phase5-waveshare-release-prep`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase5-waveshare-release-prep), created directly from the user-approved Phase 3 source (not from rejected Phase 4 experiments). Detailed engineering gates and safety notes: [Phase 5 release preparation on that branch](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase5-waveshare-release-prep/pokebyte/PHASE5-WAVESHARE-RELEASE.md).

**Trusted source/asset reference:** [`pokebyte-phase3-pack03-distinct-moments`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-distinct-moments), accepted ZIP `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip` (SHA-256 `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`), and [the 12 approved BMP / original PMD checksum manifest](APPROVED-ASSETS.md).

**Immediate tasks:** verify target is specifically Waveshare **ESP32-S3-Touch-AMOLED-1.75** (not 1.75C); compile an ESP32-S3 hardware build without flashing; prepare verified microSD assets; port and test **all** accepted Windows-only scenes, ambience and personality features to embedded hardware; audit flash/NVS/partition safety, physical touch/PMU/RTC/audio/SD, and firmware rollback before any flash instructions.

**Safety boundary:** The existing `main` firmware (TamaPoke v1.17) and user save remain untouched. Never format a user microSD or flash/erase a connected board automatically. The Phase 5 branch explicitly disables auto-formatting microSD on mount failure as its first safeguard. No hardware image should be called "PokéByte Phases 1–3" merely because the Arduino compiler succeeds: current Phase 1–3 enhancements have important `POKEBYTE_PC`-only implementations that must be ported.

**Release status:** no user-device flash, no tested physical release, no merge, no new firmware tag. All release gates require evidence and physical hardware acceptance.
