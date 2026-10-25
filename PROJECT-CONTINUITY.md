# PokéByte — Continuity / New-Conversation Handoff

**Last project checkpoint:** 25 October 2026. **Canonical project:** [TMA-X-Official/PokeByte](https://github.com/TMA-X-Official/PokeByte).

> **READ THIS FIRST IN ANY FUTURE CHAT.** This file documents the user-approved source of truth, known-good Windows deliverable and next milestone. It is a **project-state summary**, not a verbatim transcript. The user retains a separate conversation/archive ZIP; do not assume ChatGPT has access to chats in a different thread.

## Exact status

| Phase | Status | Meaning |
| --- | --- | --- |
| 0 — Preserve original TamaPoke | **Completed** | Working baseline and original hardware firmware kept safe |
| 1 — Remastered environments | **Completed, user-approved on Windows** | Five distinct day-part scenes and their matching minigame scenes |
| 2 — Living environment | **Completed, user-approved on Windows** | Balanced pollen, grass movement, evening/night fireflies, manual rain/snow; **no animated overlay clouds** |
| 3 — Personality | **Completed, user-approved on Windows 24 Oct 2026** | Pack 01 moods; Pack 02 temperament/Bond; Pack 03 four distinct one-shot surprises |
| 4 — Waveshare release | ✅ **Completed — user-approved on physical Waveshare, 25 Oct 2026** | Owner successfully installed PokéByte with the integrated HTTPS beta installer; genuine ESP32-S3 firmware, approved original artwork, ambient/personality features and UI refinements run on hardware. Public stable v1.0 testing is separate |

### Phase 4 completion and public-release distinction (25 October 2026)

- **User explicitly approved:** Label **Phase 4 — Waveshare release DONE** on GitHub. Phases 1, 2, 3 and 4 are all complete in the [ROADMAP](ROADMAP.md). The user successfully installed and tested PokéByte on an actual Waveshare ESP32-S3-Touch-AMOLED-1.75 and reported that it appears to work.
- **Live owner-tested beta:** https://tma-x-official.github.io/PokeByte/beta/ with true native firmware and the exact original 12-background artwork bundle. No separate art ZIP/card reader required; device tested with preexisting authentic original Pokémon sprites on microSD. Owner requested auto-dimming off, 50%-lower audio waveform amplitude and shorter readable 112px battery percentage display.
- **Release hardening outside numbered Phase 4:** 303 original PMD files are already tracked at `tools/sdcard/mons/`. More recent source changes extend the same browser installation to missing sprites on fresh cards but require their own green deployment and *blank-card* hardware test. Save-preserving upgrades and interrupted-transfer/recovery also remain before any **public stable v1.0** tag.
- **Branch and preservation:** [Phase 4 draft PR #20](https://github.com/TMA-X-Official/PokeByte/pull/20) remains **unmerged**; `main` source still preserves upstream firmware. Updating roadmap/docs is authorized, but **do not merge, retag stable v1.0, or overwrite the original installer** without separately reviewed user approval. Owner's waiver of backup on their personal device must not become a universal public upgrade policy.
- **Reference:** [Phase 4 acceptance record](PHASE4-WAVESHARE-RELEASE.md), [Release readiness checklist](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/pokebyte/PHASE4-V1-RELEASE-READINESS.md).

### Non-negotiable preservation

1. **Never modify or automatically merge onto `main` the untested ESP32 hardware/gameplay source.** Documentation-only updates on `main` are fine. Development happens on dedicated branches/PRs. A user's approval of a Windows playtest is **not** authorization to flash hardware.
2. Preserve the real TamaPoke/PMD sprites and animations: exactly **303 original PMD sprite `.bin` files**, without rescaling/repainting/replacing them.
3. Preserve **all 12 approved 466×466 BMP files**, each by byte identity. They include 5 **different** meadow scenes, 5 matching minigame scenes, and 2 fallback files. Do **not** replace them with generic procedural fixtures, a grey placeholder, or a single sunset image.
4. Preserve existing feeding, evolution, egg odds, original level timing, Pokédex, saves, swipe menus, ball/training minigames, weather and Packs 01–03.
5. Every new gameplay feature must use an independent Windows test branch and user acceptance. Demo launchers must never read/write `tamapoke_pc_save.dat`; any future gameplay data would require an explicit backward-compatible save migration and preservation tests.

## Approved known-good Phase 3 Windows release (PIN THIS)

- **User-approved Windows filename:** `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip`
- **SHA-256:** `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`
- Contains all 12 original approved backgrounds (match supplied backup), 303 unchanged sprite BIN files, Windows executable, and the four `TEST-SURPRISE-*.bat` launchers.
- **User-provided scenery/source backup:** `PokeByte-Phase3-Personality-Pack-03-LITTLE-SURPRISES-WINDOWS(1).zip` (SHA-256 `afb21867c8375543acdc800c8ed8c6eb5285aac6874e9a27e2d1b79441d4577c`). It is not the final approved Pack 03 animation executable; **use it to independently verify the approved scene files**.
- **Approved Phase 3 code branch:** [`pokebyte-phase3-pack03-distinct-moments`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-distinct-moments).
- **Final review:** [draft PR #16](https://github.com/TMA-X-Official/PokeByte/pull/16); prior Pack 03 evolution [PR #14](https://github.com/TMA-X-Official/PokeByte/pull/14) → [PR #15](https://github.com/TMA-X-Official/PokeByte/pull/15) → #16.
- **Passing Windows CI:** [run #40123935906](https://github.com/TMA-X-Official/PokeByte/actions/runs/40123935906), compiled source commit `e0ffd70a629505f3fb0a0d8b9def5b89df3c2102`. Later commits on the same source branch updated documentation only.
- **Critical caveat:** The GitHub Actions **raw** CI ZIP includes synthetic fixture backgrounds and **must not be handed to the user** as the approved art build. The tested executable and exact approved background files were combined into the separate known-good ZIP above. GitHub repository itself does not have all of the user-approved artwork committed as authoritative binary assets. See [APPROVED-ASSETS.md](APPROVED-ASSETS.md).

### Phase 3 details

- Pack 01: original sprite mood-aware happy/hungry/sleepy/normal responses; short **Sleepy** status text.
- Pack 02: four hatch-gene-derived temperaments: Playful, Calm, Curious and Shy. Bond stages New Friend (0–29), Growing Bond (30–69), Close Friend (70–100). Original bond and genetics saved unchanged.
- Pack 03: rare one-shot surprises with original PMD clips, no invented sprite frames:
  - **Wake** = DeepBreath then Nod; actual wake event when suitable.
  - **Victory** = Hop then Pose; after a successful completed ball minigame when eligible.
  - **Wander** = walk to safe meadow edge, pause, walk back.
  - **Friendship** = Nod then Pose + original heart at high Bond.
  - Each separate no-save Charmander demo plays **once**, shows `Back to normal`, settles ~4.5 seconds in Idle, then normal temperament/mood resumes. The *real* game keeps rare event/cooldowns, whereas previews trigger on demand.
  - User tested and explicitly **approved** the distinct performances on 24 October 2026.

### Lessons from the actual playtests

- The first wake preview *looked stuck* because a timer automatically replayed it every ten seconds. Fix was a one-shot preview and explicit normal Idle recovery.
- The first CI test scene files contained an opaque **grey square**. They were *fixture images*, not approved art.
- A mistaken attempt to cure the grey square copied `sunset-forest.bmp` over every scene, obliterating the user-approved different morning/day/evening/night versions. **Never do this.**
- The user provided their earlier approved Windows ZIP as backup. The corrected approved-art Windows ZIP was assembled using that backup's 12 exact BMPs and all 303 unchanged PMD sprites and was approved.
- Preserve the user's known-good backups before any trial replacement. Always compare file-by-file and test all five scenes, mini-games, surprise demos, and saves.

## Cancelled gameplay-expansion experiments — hardware release renumbered Phase 4

- After viewing the Forest Trail screen, the user explicitly rejected **Adventure Quiet, Adventure Item, Adventure Wild**. The Adventure PR [#17](https://github.com/TMA-X-Official/PokeByte/pull/17) was closed **without merge**.
- After personally viewing the classic battle screen, the user explicitly rejected the **battle system** as well. The battle PR [#18](https://github.com/TMA-X-Official/PokeByte/pull/18) was closed **without merge**.
- User says: **remove the rejected gameplay-expansion stage** and **renumber the Waveshare hardware release as Phase 4**. This supersedes every older proposed gameplay pack, Adventure-first plan and battle-first plan.
- The active [ROADMAP.md](ROADMAP.md) now lists only **Phases 1–4**, with **Phase 4 as Waveshare Release**. The abandoned proposal is kept **only** in [archive/REJECTED-GAMEPLAY-HISTORY.md](archive/REJECTED-GAMEPLAY-HISTORY.md) and closed PR history. **Neither rejected feature is allowed in a release or rebuilt test baseline.**
- **Approved source remains Phase 3:** [`pokebyte-phase3-pack03-distinct-moments`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-distinct-moments), approved ZIP SHA-256 `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`.
- **Active Phase 4 engineering source branch:** [`pokebyte-phase4-waveshare-release-prep`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase4-waveshare-release-prep) branched **directly from approved Phase 3**, not from either rejected gameplay prototype. Its first code safety fix disables `SD_MMC` auto-format on failed mount. Its [`tools/phase4_prepare_sd.py`](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/tools/phase4_prepare_sd.py) validates/stages the exact approved BMP/PMD SD data without touching hardware. [PHASE4-WAVESHARE-RELEASE.md](PHASE4-WAVESHARE-RELEASE.md) has release gates.
- **Critical engineering gap:** existing approved Phase 1/2/3 scenery/ambient/personality code still contains desktop-only `#ifdef POKEBYTE_PC` paths and host SDL/file APIs. **A firmware compile of the current Phase 4 branch will still run much of the original game, not all PokéByte-approved Windows features.** It is a preparation branch, not a release candidate.
- **Protect physical Pokémon:** Don't flash a firmware, repartition, wipe Preferences/NVS or format any SD card until complete user flash/NVS/microSD backups and physical board model/port verification. **No hardware flash done.** A no-save Windows test is not hardware validation.
- **Next work:** real embedded feature port, compile/test ESP32-S3 firmware, stage exact approved background BMPs and 303 original SD `.bin` assets, verify performance/RTC/PMU/touch/audio/SD, then safe backup/flash with explicit user approval. No new gameplay features.

## Hardware-release recovery files (historical filenames retain Phase 5) (24 October 2026)

- **Last accepted executable:** `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip` — exact sha256 `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`.
- **Exact approved SD data staging (not firmware):** `PokeByte-Phase5-Verified-MicroSD-Assets-STAGING.zip` — SHA-256 `d70a674139e0e5a7a7acce4a508f7b8b2c1e9a33250c04d19ef3d9078e861442`. Includes 12 466x466 BMP scenes in `pokebyte/` plus 303 original BIN data assets in `mons/`, with an internal checksum manifest. Stage paths need real embedded firmware support; no SD flash/copy done.
- **Private new-chat checkpoint:** `PokeByte-Phase5-Waveshare-Release-Checkpoint.zip` — SHA-256 `077fa8fb2d623f094185235a279554e3a2bc7f9f3b97eb9f6815896531d86c99`. Contains approved Phase 3 executable, original background backup ZIP, early conversation text upload, verified SD staging archive and a Phase 5 README. The user should retain this ZIP locally and upload it to a new chat when actual binary files are required.
- **Review:** [draft PR #19](https://github.com/TMA-X-Official/PokeByte/pull/19), [CI #40132890760 passed compile/regression](https://github.com/TMA-X-Official/PokeByte/actions/runs/40132890760). **No device tested and no hardware-ready approved firmware generated yet.**

## Phase 4 branding and first-connection checkpoint — 24 October 2026

- **Current visible milestones:** Phase 1 Visual, Phase 2 Ambience, Phase 3 Personality (all user-approved on Windows), **Phase 4 Waveshare Hardware Release** (engineering only). The original TamaPoke baseline is an **unnumbered** historical foundation. The canceled gameplay experiments are archived, not active.
- **GitHub code review:** [active Phase 4 draft PR #20](https://github.com/TMA-X-Official/PokeByte/pull/20) on `pokebyte-phase4-waveshare-release-prep`, based on the accepted Phase 3 branch. Former Phase 5 PR #19 closed unmerged after renumbering.
- **User-provided brand art:** Original PokéByte logo uploaded in the chat as a 1774×887 transparent PNG. An **optimized thumbnail made from those exact image pixels** is committed to `main/branding/pokebyte-logo-thumbnail.webp` and to the active review branch, plus `web/pokebyte/assets/pokebyte-logo.webp` for the locked installer site. This is not a generated/reimagined logo. A high-resolution source copy should be kept in the owner's private branding archive.
- **Current installer:** [`web/pokebyte/index.html`](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/web/pokebyte/index.html), labeled Phase 4, using the real user logo thumbnail. Installer button is **disabled**; no manifest, ESP Web Tools active script, USB/Web Serial writes, or PokéByte firmware binary. The old `web/index.html` is still the original TamaPoke installer and must not be used to install the custom PokéByte upgrade. [Installer safety CI](https://github.com/TMA-X-Official/PokeByte/actions/runs/40136314345) passed.
- **Windows hardware preflight:** [`pokebyte/PHASE4-FIRST-CONNECTION-WINDOWS.md`](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/pokebyte/PHASE4-FIRST-CONNECTION-WINDOWS.md) and a **read-only, opt-in** full flash backup helper `tools/phase4_waveshare_backup.py` on that branch. Backup the entire SD separately; do not publish full NVS/flash dumps. Require 16 MB ESP32-S3, verified USB COM port and explicit `--execute` for read-only backup; no erase/write/flash occurs in the helper.
- **Critical remaining task:** port the approved PC-only art/ambient/personality logic to embedded ESP32, then hardware validation on the genuine Waveshare round 1.75 device, save restoration testing, and *then* explicit user approval before flashing. **NO DEVICE FLASH HAS OCCURRED; NO FLASHABLE POKÉBYTE PHASE 4 BIN EXISTS YET.**

## Required release experience — one-click PokéByte website (25 October 2026)

- **Explicit user instruction:** Install PokéByte **exactly as easily as TamaPoke**: ordinary visitors open the hosted PokéByte website, connect their Waveshare ESP32-S3-Touch-AMOLED-1.75 by USB-C and click **Install**. The browser automatically fetches firmware and installs/checks all sprite and scene assets. **No desktop installer, manual download, Python/Arduino IDE, microSD card reader or separate sprite upload is acceptable for a normal end user.** Browser USB chooser and safe backup prompts are expected.
- The user has **no microSD adapter**. The 303 original PMD files are verified in the accepted Phase 3 private ZIP; the physical card may have other unique data, so the installer must not automatically erase or format it.
- [Phase 4 one-click release contract](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/web/pokebyte/ONE-CLICK-INSTALLER-REQUIREMENTS.md) is the controlling implementation specification; updated [branded page preview](https://github.com/TMA-X-Official/PokeByte/blob/pokebyte-phase4-waveshare-release-prep/web/pokebyte/index.html) and [draft PR #20](https://github.com/TMA-X-Official/PokeByte/pull/20) record it. [Installer safety CI passed](https://github.com/TMA-X-Official/PokeByte/actions/runs/40137839132).
- **Technical blockers as of checkpoint:** accepted Windows-only art/ambient/personality behavior has not all been ported to actual ESP32-S3, no verified genuine PokéByte hardware binary, no safe automatic in-browser existing flash/NVS backup, no safe `/pokebyte` BMP upload/readback/asset-hash Web Serial protocol, no approved hosted release bundle, and no physical board acceptance. The original TamaPoke installer remains untouched and installs original TamaPoke, not PokéByte.
- **Do not mark installed or unlock Install yet.** ESP Web Tools' default new-install erase behavior may endanger existing saves; require an audited existing-device upgrade path first. The final UX is simple, but the engineering prerequisites are non-negotiable.

## First physical PokéByte beta accepted; owner-requested display/sound/background improvements (25 October 2026)

- **User installed experimental firmware via the working browser GitHub Pages beta installer** on their Waveshare ESP32-S3-Touch-AMOLED-1.75, COM6, 16 MB flash and 8 MB PSRAM. User reported that the game boots and works, including touch and original sprite loading from the existing microSD.
- **Observed issue:** The device still showed **original TamaPoke scenery**, not the user-approved Phase 1 scenes. This is expected because the initial firmware-only installer intentionally did not transfer the 12 approved BMPs. Their microSD has the original sprites, but there is no microSD reader.
- **New explicit requests:** (1) install approved backgrounds directly via the PokéByte website, (2) no automatic dimming at ~30s or any idle delay, (3) 50%-lower sound amplitude, (4) much brighter/readable battery icon and percentage on all backgrounds.
- **Developed on [Phase 4 draft PR #20](https://github.com/TMA-X-Official/PokeByte/pull/20):** `TamaPoke.ino` disables automatic idle/sleep dimming while preserving physical PWR blanking; moves periodic `Pet::flushSave()` to a once-per-minute pending-save path independent of dimming; adds an opaque cream high-contrast 2x battery-percent banner within the circular display. `audio.cpp` lowers the synthesized PCM amplitude `5000 → 2500` (half waveform amplitude, -6dB).
- **Browser background uploader implemented:** `web/pokebyte/beta/background-sync.js` + beta page UI/approved JSON manifest. The page can accept the exact approved Phase 3 Windows ZIP or a verified `approved-backgrounds.zip` bundle, checks **all 12 BMP SHA-256 and CRC32 fingerprints**, checks the current SD via read-only `PBCRC`, then uses the already-flashed **non-overwriting PBPUT** protocol to send only missing `/pokebyte/*.bmp` scenes through USB. No physical SD adapter, extraction, manually copied BMP, SD formatting or overwrite of existing sprite/data files. If existing target path differs, stop safely; restart the device after upload.
- **Automatic artwork hosting is supported but one-time owner input pending:** GitHub Actions packaging can host `web/pokebyte/approved-backgrounds.zip`, but the exact binary art isn't otherwise in the public repository. The locally prepared `approved-backgrounds.zip` is **byte-for-byte extracted from the user's accepted Phase 3 ZIP**, 4.70 MB, SHA-256 `5f72992ca1c93cbc7e16e42624ba9fdd449e0173ddd369fd7a4904e68db12c81`. Owner can upload the exact pack once to the **development branch** under `web/pokebyte/approved-backgrounds.zip`; CI verifies all 12 hashes before publication. Thereafter browsers fetch it automatically. Until then, the owner can select the existing approved ZIP inside the beta website.
- **Testing gates:** Native source UX/preservation regression and browser JS syntax CI were added. The newest firmware needs its ESP32-S3 build and GitHub Pages deployment confirmed, then **user's physical validation** of scene transfer, brightness, sound and battery. Do not treat source changes as already installed on the device or merge this untested iteration. The user's existing sprite files and SD card must remain protected. No rejected Adventure or Battle features.
- **Tradeoff:** No automatic AMOLED dimming can increase battery consumption and static-image burn-in. Manual PWR off still works; a configurable burn-in safeguard can be considered later with user approval.

## Practical recovery in a new ChatGPT conversation

**Current status supersedes the historical steps below:** Phase 4 is user-approved **COMPLETE** on the working physical Waveshare. Do not restart the ESP32 port as if nothing had been flashed; preserve the working beta. Public stable v1.0 upgrade/fresh-card QA remains optional follow-up work.

**Numbering update:** This project formerly labeled Waveshare deployment as Phase 5 because a Phase 4 Gameplay Expansion had been proposed. Both experimental gameplay implementations were rejected, and the user explicitly requested continuous public numbering: **Phases 1–3 approved, Phase 4 = Waveshare Release.** Legacy archive filenames with `Phase5` are preserved for checksum provenance only.


1. Start with: **"Continue PokéByte at Phase 4 — Waveshare Release. Phases 1–3 are the approved Windows baseline; the former gameplay-expansion experiments were rejected and archived. Read https://github.com/TMA-X-Official/PokeByte/blob/main/PROJECT-CONTINUITY.md and https://github.com/TMA-X-Official/PokeByte/blob/main/PHASE4-WAVESHARE-RELEASE.md and continue pokebyte-phase4-waveshare-release-prep. Never flash or format without a verified full flash/NVS/microSD backup and user approval. Preserve original sprites and approved artwork."**
2. Upload the privately retained **PokéByte Continuity Archive ZIP**, or at minimum the approved Phase 3 Windows ZIP, the backup ZIP, and the previous-chat export.
3. Verify the ZIP and [asset hashes](APPROVED-ASSETS.md) before providing a new Windows build. Source-only CI art is **not** enough. Never invent a local download link.
4. Check the GitHub roadmap and most recent PR; don't assume a new conversation has access to the prior session or uploaded binary files.
5. Continue Phase 4 on its isolated Waveshare prep branch. Port the PC-only approved features to embedded hardware before claiming any upgrade; build and test, make verified backups, and only then plan a physical flash with explicit user approval.

## History coverage and privacy

This public GitHub document captures project decisions and milestones, **not private ChatGPT conversation transcripts**. The user supplied a pasted text export of the **early** original conversation (`Pasted text(20261024-192835).txt`, 843 lines, SHA-256 `7b7f1addce79040a996951e0dbe660f9eaa94b26b0d9037700837c1a2554654d`). The file doesn't cover all later messages; later state was reconstructed from the current chat and GitHub evidence. The original transcript and user uploads belong in the user's **privately stored archive**, not in a public repository.

**Authority order when reconstructing:** explicit recent user approval + verified accepted Windows ZIP and backup hashes → GitHub source/review and passing test workflows → historical exported chat for feature intention → assistant proposals (not automatically approved).
