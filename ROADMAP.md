# PokéByte — Development Roadmap

**Last reviewed:** 24 October 2026  
**Project principle:** Keep the real TamaPoke game, Pokémon sprites, animations, swipe menus, minigames and save behavior intact. Experiment only on separate branches and keep the classic game available.

| Phase | Status | Acceptance |
| --- | --- | --- |
| **Phase 0 — Secure working version** | ✅ **Completed** | Original TamaPoke code and original Windows PC emulator baseline preserved; separate development branches |
| **Phase 1 — Visual upgrade** | ✅ **Completed (Windows PC playtest)** | User approved all five correctly framed remastered environments: morning, day, afternoon, evening and night. Home, ball mini-game and training-bag backgrounds match. Classic scene toggle remains available; original Pokémon/gameplay preserved |
| **Phase 2 — A living environment** | ✅ **Completed (Windows PC playtest)** | User approved Balanced pollen, evening/night fireflies, gentle grass movement, optional rain and snowfall, five approved scenes and original mini-games; no animated cloud overlays |
| **Phase 3 — More Pokémon personality** | 🟡 **In progress — Pack 01 Windows test** | Mood-aware reactions and idle behavior using only available PMD animations; bonding personality and surprises reserved for later Pack 02/03 |
| **Phase 4 — Gameplay expansion** | ⬜ Not started | New gameplay systems developed alongside existing minigames |
| **Phase 5 — Waveshare release** | ⬜ Not started | Hardware optimization, flashable builds, backup/recovery and device validation |

## Phase 1 accepted baseline

- **Approved on:** 24 October 2026, following hands-on Windows playtesting.
- **Source development branch:** [`pokebyte-phase1-minigames-scene`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase1-minigames-scene).
- **Draft review:** [Pull request #5](https://github.com/TMA-X-Official/PokeByte/pull/5).
- **Automated PC checks:** [Windows build and home/ball/training screen tests](https://github.com/TMA-X-Official/PokeByte/actions/runs/40103420707).
- **Important:** The approved playable Windows test ZIP includes a set of full-resolution background artwork files. Some artwork files are distributed as part of that ZIP and **have not all been committed to the GitHub source branch**. Before a standalone GitHub release, make the asset packaging fully reproducible. Do not represent a source-only GitHub build as visually identical without these assets.
- **Hardware:** No PokéByte firmware upgrade was flashed to the Waveshare as part of this phase; that is Phase 5. The actual device still runs its existing firmware.

## Battery indicator polish and Phase 2 development history

- **Battery percentage:** The actual Waveshare AXP2101's existing `batPercent()` value is displayed beside its battery icon by the new development code. Charging bolt and battery colors are retained. In the Windows emulator, show **PC** rather than inventing a charge percentage.
- **Development branch:** [`pokebyte-phase2-ambient-polish`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase2-ambient-polish), separate from the completed Phase 1 working Windows build.
- **First Phase 2 effects:** subtle evening/night fireflies, daytime pollen specks, optional manually toggled light drizzle. The approved remastered artwork and original minigames remain unchanged. **B** = Classic / remastered scenery, **A** = ambient effects on/off, **W** = optional rain.
- **Not real weather:** No internet or forecast integration. Weather is visual-only and defaults off; a hardware release still needs separate testing.
- **Historical status:** This was the initial Phase 2 prototype. See the accepted Milestone 03 release below; **Phase 2 is now complete for Windows PC**.

## Background battery-display details

The original firmware already obtains `batPercent()` from the Waveshare AXP2101 PMU. A future optional header update could show `85%` beside the battery icon, preserve the charging bolt, and hide the percentage if the reading is unavailable.

**PC caution:** the Windows emulator does not measure the Waveshare battery. Show a neutral `PC`/unavailable indication there, not a fictitious battery percentage. This is a minor separate follow-up, **not a blocker to completed Phase 1**.

## Phase 2 Milestone 01 — Balanced ambience

- **Build:** <https://github.com/TMA-X-Official/PokeByte/actions/runs/40107135299> — successful Windows compilation with five distinct lighting-mode screenshot tests and original home, ball, and training minigame regressions.
- **Development branch:** [`pokebyte-phase2-ambient-pack-01`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase2-ambient-pack-01).
- **Draft review:** [Pull request #7](https://github.com/TMA-X-Official/PokeByte/pull/7). Not merged.
- **Defaults:** Balanced ambience ON; manual rain OFF.
- **Live time:** Normal Windows launcher automatically selects and changes the five approved scenes using local PC time while the game stays open. Five manual preview launchers lock the chosen scene, and normal launch restores automatic selection.
- **Art:** User-approved 10 full-resolution home + mini-game scene BMPs and 2 legacy fallback BMPs are **unchanged** relative to the completed Phase 1 Windows playtest. The downloadable Windows test ZIP bundles those images. GitHub's synthetic mode images are CI testing fixtures only.
- **Battery:** PC shows an accurate **PC** label rather than a fabricated battery percentage. Firmware-source changes for the physical AXP2101 percentage have *not* been installed on the Waveshare.
- **Historical status:** Milestone 01 was a preliminary test build; superseded by the user's approved cloud-free Milestone 03.

## Phase 2 Milestone 02 — Moving cloud playtest

- **Feature:** Real 88–118-pixel multi-lobed clouds with mode-specific shading, drifting across the sky; separate ground-level pollen, swaying grass highlights, edge fireflies and optional drizzle with small splashes.
- **Default:** Balanced ambience ON, rain OFF; retained B/A/W switches and automatic live local-time mode changes from Pack 01.
- **Approved scenery:** Five home + five mini-game backgrounds and two fallback BMP files are carried into the Windows test package **byte-for-byte unchanged** from the user-approved previous version. All 303 PMD files verified unchanged.
- **Development source:** [`pokebyte-phase2-ambient-pack-02`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase2-ambient-pack-02), [draft PR #8](https://github.com/TMA-X-Official/PokeByte/pull/8). Do not merge before user approval.
- **Windows build/regressions:** [Automated screenshot tests](https://github.com/TMA-X-Official/PokeByte/actions/runs/40108800518) for five distinct lighting scenes, normal home, ball minigame and training activity; CI was successful. Temporary CI artwork was replaced in the final ZIP with the approved bitmaps.
- **Historical status:** Moving cloud sprites were rejected in the following user playtest and permanently removed in the accepted Milestone 03. Waveshare has not been flashed.

## Phase 2 Milestone 03 — Approved cloud-free rain and snow release

- **User feedback:** Animated cloud sprites in Ambient Pack 02 were rejected. Remove the animated overlays entirely. The user's original painted pixel-art clouds stay static in all approved scenes.
- **Development branch:** [`pokebyte-phase2-weather-snow-no-clouds`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase2-weather-snow-no-clouds), reviewed separately in [draft PR #9](https://github.com/TMA-X-Official/PokeByte/pull/9). **Do not merge without user acceptance.**
- **Still preserved:** Balanced pollen, grass animation, evening/night fireflies and light rain with ground splashes.
- **New:** Optional light snowfall and small ground-level flakes. **S** toggles snow, **W** toggles rain, and they are mutually exclusive. Both are disabled by default on a fresh install. **A** and **B** controls are unchanged. The existing two-number ambient preferences format is backwards compatible.
- **Windows smoke tests:** [Build #40109452665](https://github.com/TMA-X-Official/PokeByte/actions/runs/40109452665) compiled successfully, rendered five distinct time modes, and verified clear/rain/snow visuals have different image hashes; original home, ball and training minigame render checks passed.
- **Asset fidelity:** The ZIP distributed to the user contains all 12 approved phase 1 background BMPs **byte-for-byte unchanged**, all 303 Pokémon animation BIN files unchanged, plus a newly compiled Windows executable.
- **Acceptance:** User confirmed on **24 October 2026** that they love the resulting cloud-free living environment and explicitly approved marking **Phase 2 complete**. This is Windows PC acceptance; Phase 1 and Waveshare firmware remain unchanged.

## Phase 2 accepted Windows baseline

- **Approved:** 24 October 2026, after hands-on Windows testing.
- **Chosen visual direction:** Retain all five approved Phase 1 pixel-art backgrounds **including their original static painted clouds**. No additional animated clouds.
- **Balanced atmospheric effects:** daytime pollen, gentle edge grass motion, evening/night fireflies, manually toggled rainfall and snowfall. Rain and snow are exclusive and default to off in a fresh installation.
- **Controls:** `B` Classic / Remastered, `A` balanced ambience on/off, `W` rain on/off, `S` snowfall on/off. Auto time-of-day switching and individual time preview launchers are retained.
- **Source of approved development changes:** [`pokebyte-phase2-weather-snow-no-clouds`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase2-weather-snow-no-clouds), [draft PR #9](https://github.com/TMA-X-Official/PokeByte/pull/9). The draft remains unmerged while the baseline is preserved.
- **Verified Windows build:** [GitHub Action #40109452665](https://github.com/TMA-X-Official/PokeByte/actions/runs/40109452665), with original home, ball, and training game startup/render tests, five distinct time modes, and clear/rain/snow output tests.
- **Reproducibility note:** The actual download ZIP also includes user-approved full-resolution artwork files that are not all available in the GitHub source tree. Before a source-only reproducible release, commit or otherwise package those approved assets using appropriate attribution and permissions.
- **Not yet deployed to hardware:** Waveshare code/firmware has **not** been flashed or device-tested. Physical hardware release still belongs to Phase 5. Completion here does **not** assert hardware readiness.

## Phase 3 — Personality Pack 01: user playtest candidate (24 October 2026)

- **Development branch:** [`pokebyte-phase3-personality-pack-01`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-personality-pack-01). [Draft PR #10](https://github.com/TMA-X-Official/PokeByte/pull/10); do not merge until the user approves.
- **Feature scope:** Optional, mood-aware original PMD Idle / Walk / Pose / Nod / DeepBreath / Hurt selection on Windows based on existing FOOD, JOY, ENERGY and HYGIENE. No artificial emotion bars, new frames, invented sprite movement, care logic changes or save format changes.
- **Safe visuals to evaluate:** `TEST-HAPPY-CHARMANDER.bat`, `TEST-TIRED-CHARMANDER.bat`, `TEST-HUNGRY-CHARMANDER.bat`. These temporary demos launch genuine Charmander and **do not read or write a Pokémon save**. Preview moods change only animation selection, not the Pokémon's real needs. Press **P** in the regular game to toggle enhanced or Classic idle/reactions.
- **Action safety:** The behavior planner checks each PMD animation's existence per species. Pokémon without a special gesture fall back to real idle/walk frames. Existing HOP/SIT remain excluded from normal idle transitions, consistent with TamaPoke's original sprite alignment logic.
- **Previous work preserved:** Approved Phase 2 sky/biome assets, five day modes and matching original minigame art, rain/snow, pollen/fireflies, all original PMD files and game code.
- **Validation:** [Windows build and home/ball/training/mood-preview tests](https://github.com/TMA-X-Official/PokeByte/actions/runs/40112695971) passed, as did six independent reaction-scheduler tests. Final ZIP copied all **12** previously approved background BMPs and all **303** Pokémon PMD files byte-identically from the completed Phase 2 Windows build.
- **Still to do:** User review of Charmander animations and actual game pacing; Pack 02 individual temperament/bond personality and Pack 03 rare surprises are **not implemented**. Phase 3 is **in progress**, and Waveshare firmware remains unchanged.

## After each phase

Review the completed features and Windows playtest, update this roadmap, and start the next phase only after the user approves it. Preserve known-good downloads and branches.
