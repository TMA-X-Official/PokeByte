# PokéByte v1.0 — Waveshare release readiness and candidate notes

**Checkpoint:** 25 October 2026. **Status: functional owner-tested hardware BETA / v1.0 RELEASE CANDIDATE, not a publicly supported stable release yet.** The owner reports that the complete current PokéByte web installation succeeded and the device "seems to work all." This is strong positive physical smoke-test evidence, **not** exhaustive acceptance of every screen, card state and upgrade route.

**Target hardware:** Waveshare **ESP32-S3-Touch-AMOLED-1.75**, 466×466 round display, 16 MB internal flash, 8 MB PSRAM. **Not the 1.75C variant.** Chrome / Edge desktop via HTTPS and Web Serial. Device previously identified on Windows COM6 (port number can change).

## Confirmed by source/CI

- Actual PokéByte ESP32-S3 firmware, not relabeled original TamaPoke. ESP32-S3 native build and original game-logic regression checks passed, including CI workflow [40181033046](https://github.com/TMA-X-Official/PokeByte/actions/runs/40181033046) for the uploaded approved-art integrated installer.
- Github Pages installation bundle was packaged/deployed from development commit `35122241054e97526995409a8032d32e6f79e28e`.
- User-approved **12 unique source BMP filenames** (five home dayparts, five minigame dayparts, minigame.bmp and sunset-forest.bmp) bundled in `web/pokebyte/approved-backgrounds.zip`, with expected full ZIP SHA-256 `5f72992ca1c93cbc7e16e42624ba9fdd449e0173ddd369fd7a4904e68db12c81`; individual SHA-256 and CRC32s in `web/pokebyte/beta/approved-backgrounds.json`.
- One browser installation journey: verify hosted firmware/art, flash ESP32-S3, then auto-reconnect/ask browser for serial permission when necessary to transmit only missing checksum-matched BMPs over the approved `PBCAPS/PBCRC/PBPUT` protocol. Existing sprites/SD files are not deliberately overwritten. **This is a workflow intent with source-level safety checks; physical failure-path testing remains outstanding.**
- Accepted visual/personality changes: original 151 Pokémon PMD action frames; approved daypart scenes and minigame backgrounds; limited grass/pollen/fireflies and optional rain/snow without new clouds; Bond/temperament and four one-shot surprises; existing feeding/care/minigames (no rejected Adventure/Battle mode).
- Requested owner UX changes: no automatic screen dim, PCM sound effect amplitude halved (5000→2500), high-contrast battery percentage, shorter **112px** battery badge. Manual power-off remains, but battery life/burn-in tradeoffs must be explained.

## Confirmed by owner (NOT a complete test report)

- Windows computer connected through USB to actual Waveshare and successfully installed PokéByte beta.
- Subsequent owner message: latest installation **completed successfully and appears to work**.
- Owner **does not require backup of their personal old TamaPoke internal save**, but requires preserving original Pokémon sprite files already on their microSD.
- **Not yet independently documented:** all twelve landscape variants shown on physical screen, all four original PMD surprise animations, touch/RTC/power/SD/long idle tests after the latest firmware, reboot with saves intact, interrupted/resent SD uploads and a first-time installation on a blank card.

## Blocking for a general-audience "stable v1.0"

- [ ] **Fresh-device assets:** a newcomer with no TamaPoke card has no guarantee of the **303 original Pokémon sprites/thumbnail .bin files** under `/mons/`. The website currently only installs the **12 BMP backgrounds**. Decide on a legally appropriate distribution/attribution route and implement checksum-gated, missing-only installation for the original sprite set (never destructive `PUT`). Prove actual behavior using a fresh blank test microSD without formatting the owner's only existing card.
- [ ] **Save safety for strangers:** the owner specifically waived a backup for their own testing; that waiver must **not** become a universal stable-installer default. Provide a verified browser-side flash/NVS backup/restore or a clear fresh-vs-upgrade flow with tested save-preserving update and deliberate user consent. No unexpected chip erase or NVS/FFat write, and no SD formatting.
- [ ] **End-to-end physical regression:** exercise each of the five home + five minigame backgrounds, the two original minigames, all four one-shot reactions, weather/grass/fireflies, approved original sprites/frames, touch navigation, charging + 0–100% battery display, manual PWR / wake, 30 sec+ no dim, volume, RTC scene selection, sleeping, reboot and power recovery.
- [ ] **Failure-mode SD tests:** verify both new and already-present scene files; remove USB during a *test-card* upload, then resume safely without corrupting existing files; reject changed/existing BMPs without overwrites; test no SD and SD full. Preserve original `/mons/` and unrelated files.
- [ ] **Installer handoff/identity:** establish that ESP Web Tools dialog-close event reliably advances the single guided installation (including cancelled flash vs successful flash), browser Web Serial permissions, exactly one compatible connected board, and no mixing 1.75 vs 1.75C models. Test Chrome + Edge with appropriate serial disconnect/reboot recovery. Update UI so a failed/partial installation is never reported as complete.
- [ ] **Public package and support:** pin/check external installer + ZIP libraries, publish reproducible versioned firmware artifacts/SHA hashes and a short install/update/recovery guide, test a fresh Pages deployment, add hardware-compatibility and known-limitations notices, verify non-commercial sprite artwork attribution/licensing. Retain source and upstream copyright credits.
- [ ] **Release branch:** the development branch is substantially diverged from `main` (at this checkpoint GitHub compare reports >200 ahead and ~69 behind). Resolve and review intended commits and ensure clean green CI on the **actual** release commit/merge target. Do not force merge or overwrite the public original TamaPoke root installer.

## Release stages

1. **Current:** owner-testable hardware beta at <https://tma-x-official.github.io/PokeByte/beta/>. This is real PokéByte, already used by the owner, but must remain clearly experimental for other users.
2. **Candidate RC1:** test/ship the public candidate limited to **existing correctly prepared microSD cards** with explicit caveat that sprite files must already be present, tested save-risk notice and rollback; use versioned commit and document known limits. It is **not** full one-click new-user setup yet.
3. **Stable v1.0:** complete every above release blocker, achieve owner acceptance and green CI at release SHA, freeze hashes, merge the reviewed release PR, publish a versioned release/installer, and only then label the official main PokéByte installer stable.

## Lightweight owner acceptance test on current Waveshare

After a successful **complete** beta install (including scenery, not just firmware):

- [ ] Normal home: confirm morning/day/afternoon/evening/night scenes switch correctly; verify original character sprite remains authentic and centered.
- [ ] Both minigames render with correct approved background and still work.
- [ ] After 30 seconds+ without tapping, screen remains bright; manual power button still blanks/wakes.
- [ ] Sound is noticeably softer than original; battery label and percentage are easy to read and have the preferred shorter outline.
- [ ] Weather/ambience, care and one-shot surprises work without repeating indefinitely.
- [ ] Save survives reboot; Pokémon original sprites remain on microSD.
- [ ] No grey square in any approved scene and no SD write/format/data loss.

**Release policy:** Owner's "it seems to work all" validates functional direction and supports candidate preparation, but does not replace explicit upgrade, fresh-install and failure-case checks for a full public stable label.
