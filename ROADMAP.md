# PokéByte — Development Roadmap

**Last reviewed:** 24 October 2026  
**Project principle:** Keep the real TamaPoke game, Pokémon sprites, animations, swipe menus, minigames and save behavior intact. Experiment only on separate branches and keep the classic game available.

| Phase | Status | Acceptance |
| --- | --- | --- |
| **Phase 0 — Secure working version** | ✅ **Completed** | Original TamaPoke code and original Windows PC emulator baseline preserved; separate development branches |
| **Phase 1 — Visual upgrade** | ✅ **Completed (Windows PC playtest)** | User approved all five correctly framed remastered environments: morning, day, afternoon, evening and night. Home, ball mini-game and training-bag backgrounds match. Classic scene toggle remains available; original Pokémon/gameplay preserved |
| **Phase 2 — A living environment** | ✅ **Completed (Windows PC playtest)** | User approved Balanced pollen, evening/night fireflies, gentle grass movement, optional rain and snowfall, five approved scenes and original mini-games; no animated cloud overlays |
| **Phase 3 — More Pokémon personality** | ✅ **Completed — user-approved Windows PC, 24 October 2026** | ✅ Pack 01 mood reactions/Hungry/Sleepy; ✅ Pack 02 four temperaments and three Bond stages; ✅ Pack 03 distinct Wake, Victory, Wander and Friendship surprises. Approved 12 backgrounds and 303 PMD assets preserved |
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

## Phase 3 — Personality Pack 01: accepted Windows milestone (24 October 2026)

- **Development branch:** [`pokebyte-phase3-personality-pack-01`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-personality-pack-01). [Draft PR #10](https://github.com/TMA-X-Official/PokeByte/pull/10); do not merge until the user approves.
- **Feature scope:** Optional, mood-aware original PMD Idle / Walk / Pose / Nod / DeepBreath / Hurt selection on Windows based on existing FOOD, JOY, ENERGY and HYGIENE. No artificial emotion bars, new frames, invented sprite movement, care logic changes or save format changes.
- **Safe visuals to evaluate:** `TEST-HAPPY-CHARMANDER.bat`, `TEST-TIRED-CHARMANDER.bat`, `TEST-HUNGRY-CHARMANDER.bat`. These temporary demos launch genuine Charmander and **do not read or write a Pokémon save**. Preview moods change only animation selection, not the Pokémon's real needs. Press **P** in the regular game to toggle enhanced or Classic idle/reactions.
- **Action safety:** The behavior planner checks each PMD animation's existence per species. Pokémon without a special gesture fall back to real idle/walk frames. Existing HOP/SIT remain excluded from normal idle transitions, consistent with TamaPoke's original sprite alignment logic.
- **Previous work preserved:** Approved Phase 2 sky/biome assets, five day modes and matching original minigame art, rain/snow, pollen/fireflies, all original PMD files and game code.
- **Validation:** [Windows build and home/ball/training/mood-preview tests](https://github.com/TMA-X-Official/PokeByte/actions/runs/40112695971) passed, as did six independent reaction-scheduler tests. Final ZIP copied all **12** previously approved background BMPs and all **303** Pokémon PMD files byte-identically from the completed Phase 2 Windows build.
- **Acceptance:** The user tested the subsequent status corrections and Sleepy label, confirmed the build works, and explicitly approved **completing Personality Pack 01**. Pack 02 individual temperament/bond personality and Pack 03 rare surprises remain **unimplemented**. Phase 3 is still in progress, and Waveshare firmware remains unchanged.

## Phase 3 — Personality Pack 01.1 status consistency (Windows playtest)

- **User-reported issue:** Tired and Hungry Charmander preview modes showed the original "It's Happy" label even while the PMD animation planner simulated Tired or Hungry. The original game stat thresholds (25) did not match the enhanced animation thresholds (FOOD below 40, ENERGY below 45).
- **Corrected:** The PC-only enhanced status renderer now uses the **same classification function** as the original-frame animation planner. Tired/Hungry demo labels now match their reactions without changing actual stats or persistent saves. The genuine original localized Hungry/Exhausted strings are used.
- **Classic toggle:** Press **P** to restore original behavior **and original status thresholds**. Highest-priority evolution, bath, sleep, eating and heart messages are preserved.
- **Safe demos:** `TEST-HAPPY-CHARMANDER.bat`, `TEST-TIRED-CHARMANDER.bat`, `TEST-HUNGRY-CHARMANDER.bat` launch temporary Charmander previews without loading or writing `tamapoke_pc_save.dat`.
- **Dev branch:** [`pokebyte-phase3-pack01-status-fix`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack01-status-fix), [draft PR #11](https://github.com/TMA-X-Official/PokeByte/pull/11). Not merged.
- **Validation:** [Successful Windows workflow #40116224178](https://github.com/TMA-X-Official/PokeByte/actions/runs/40116224178) passed 16 mood/status/fallback assertions, all three distinct preview screenshots without creating a save, all five lighting scenes, rain/snow, and both original mini-games.
- **Approved baseline unchanged:** The Windows ZIP preserves all **12** approved environment BMPs and all **303** original PMD animation BIN files byte-for-byte from the previous user-tested version; hardware firmware is untouched.
- **Historical status:** Status consistency correction was followed by the accepted short Sleepy label in Pack 01.2. Phase 3 remains in progress, with individual temperament and bond extensions reserved for later packs.

## Phase 3 — Personality Pack 01.2: Accepted “Sleepy” label

- **User screenshot feedback:** English tired status `Worn out...` appeared clipped in the Windows emulator. User requested the simpler `Sleepy` wording.
- **Solution:** In the Windows PC build, both the test-mode Tired Charmander status and regular low-energy status now display **Sleepy**. All other languages retain the original localized exhausted translations; Waveshare firmware text is unchanged. No Pokémon stats or save-file formats were changed.
- **Development:** [`pokebyte-phase3-pack01-sleepy-label`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack01-sleepy-label), [draft PR #12](https://github.com/TMA-X-Official/PokeByte/pull/12) (unmerged).
- **Build checks:** [Windows workflow #40116963570](https://github.com/TMA-X-Official/PokeByte/actions/runs/40116963570) compiled successfully, verified the Sleepy text and fallback, 16 behavior/status/fallback tests, all three mood preview renders with no save writes, five time modes, rain/snow, home and both original mini-games.
- **Asset preservation:** Final ZIP contains exactly the 12 approved Phase 2 bitmaps and all 303 original Pokémon PMD animations unchanged, with the new tested executable.
- **Status:** ✅ **User-tested and accepted on 24 October 2026.** The user confirmed the corrected **Sleepy** text works and approved marking this Phase 3 step as complete. This closes **Personality Pack 01** on Windows; the larger Phase 3 remains **in progress**.

## Phase 3 — Pack 01 acceptance and next milestone

- **Pack 01:** ✅ **Completed and user-approved** on 24 October 2026 (Windows PC). Enhanced idle responses use only existing PMD animations, safely fall back where animations are unavailable, match the displayed Hungry/Sleepy/Happy status, and allow switching to Classic behavior with **P**.
- **Accepted test build:** [Phase 3 Pack 01.2 Sleepy-label Windows build](https://github.com/TMA-X-Official/PokeByte/actions/runs/40116963570). User explicitly tested the shorter Sleepy label and said it works.
- **Preservation:** Completed Phase 2 scenery/weather, the 12 approved art files, 303 original Pokémon animation files, menus, and mini-games remain untouched. The Windows preview launchers do not touch the player's save.
- **Code review:** [Draft PR #12](https://github.com/TMA-X-Official/PokeByte/pull/12), based on the preceding Phase 3 Pack 01 drafts; experimental branches and draft PRs remain separate and **unmerged** pending explicit merge approval.
- **Next:** **Personality Pack 02** — stable per-Pokémon temperaments and observable bond-based behaviors, using existing care/bond systems and original PMD frames. **Personality Pack 03** — occasional context-aware surprises. These are not yet implemented and must be tested before Phase 3 can be marked complete.
- **Hardware:** None of these Phase 3 changes have been deployed to the Waveshare; hardware release remains Phase 5.

## Phase 3 — Personality Pack 02: Bonding & Temperament Windows test (24 October 2026)

- **Status:** ✅ **Completed and user-approved on 24 October 2026 (Windows PC)**. The user confirmed Test A (Shy low/high Bond), Test B (Playful/Calm), Test C (Curious/Shy) and Test D (petting reactions) are working well. Phase 3 itself remains in progress until Pack 03 is accepted.
- **Source:** [`pokebyte-phase3-personality-pack-02`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-personality-pack-02); [draft PR #13](https://github.com/TMA-X-Official/PokeByte/pull/13). Not merged into the approved Pack 01 baseline.
- **Four individual temperaments:** Playful, Calm, Curious, Shy. A stable deterministic temperament is derived from the original Pokémon's already-saved hatch attack/defense/speed genes, with no new persistent fields; evolution does not alter the genes.
- **Three friendship stages:** New Friend (Bond 0–29), Growing Bond (30–69), Close Friend (70–100). The original 0–100 Bond value and rewards remain intact.
- **Behavioral significance:** Temperament and Bond influence the rate of existing genuine PMD gestures versus walking/resting. A Shy Pokémon grows markedly more affectionate at high Bond. Petting can occasionally trigger an original real PMD Nod/Pose/DeepBreath greeting, with stage-dependent probabilities and cooldown. Hunger/tiredness priority still wins over happy behavior.
- **Visibility:** The existing PC Pokémon profile adds a discreet English temperament/stage label; no extra home status bars. In temporary demos, a small label and the displayed Bond bar reflect the simulated preview values without modifying care stats.
- **Eight explicit demos:** `TEST-PLAYFUL/CALM/CURIOUS/SHY-LOW/HIGH-BOND.bat`. Each is a temporary, non-saving Charmander. The original Happy, Tired and Hungry demo launchers remain.
- **Safety:** Preserve all 12 approved Phase 2 scenery BMPs and 303 original Pokémon binary art files in the final packaged Windows ZIP. No new animations, altered sprites, mini-game changes, firmware flashing or save-field additions.
- **Validation:** [Windows CI #40118448261](https://github.com/TMA-X-Official/PokeByte/actions/runs/40118448261) passed save-safe previews, pure policy and fallback tests, original mood demonstrations, five time-of-day modes, clear/rain/snow, and home/ball/training screen rendering. **User additionally confirmed successful hands-on Tests A, B, C and D.**

## Phase 3 — Pack 02 acceptance and Pack 03 design direction

- **Acceptance:** 24 October 2026 — user confirmed that all four manual test groups work correctly: **A** Shy low vs high Bond, **B** Playful vs Calm, **C** Curious vs Shy, **D** interaction-triggered positive reactions. **Personality Pack 02 is completed** on Windows PC.
- **Accepted baseline:** The user-playtested [Pack 02 development branch](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-personality-pack-02), [draft PR #13](https://github.com/TMA-X-Official/PokeByte/pull/13), and [successful Windows regression build](https://github.com/TMA-X-Official/PokeByte/actions/runs/40118448261). Do not merge without explicit merge authorization. Keep working playable Windows ZIP and all 12 approved backgrounds / 303 Pokémon sprites byte-for-byte intact.
- **Next planned milestone, Pack 03 — Little Surprises:** rare, context-aware flavor moments grounded in existing PMD actions and UI effects. Candidates for Windows testing: a short wake-up stretch (DeepBreath or available fallback), an extra victory flourish after a strong original mini-game performance (Pose/Nod, without changing rewards), and an occasional temperament-sensitive idle moment near the meadow edge. No invented Pokémon frames, bonus currency, random gifts, added needs, background modifications or interruption of original care/menus.
- **Acceptance criteria for Pack 03:** clearly noticeable but uncommon moments, no overlap with sleep/eat/evolution/minigames, per-species animation availability checks and safe fallbacks, no new save-format fields, preserve original mechanisms and save data, separate Windows ZIP with automated regression and user hands-on approval. Start only after user agrees to the proposal.
- **Historical note (after Pack 02):** Phase 3 was still in progress pending Pack 03. **Superseded by the user approval of all three packs on 24 October 2026** recorded below; Waveshare flashing remains Phase 5.

## Phase 3 — Personality Pack 03: Little Surprises Windows tester (24 October 2026)

- **Historical status (initial Pack 03 tester):** Developed and automatically tested; user approval was still pending at this stage. **The final improved Pack 03 was approved on 24 October 2026; see Phase 3 completion record below.**
- **Development branch:** [`pokebyte-phase3-personality-pack-03`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-personality-pack-03) and [draft PR #14](https://github.com/TMA-X-Official/PokeByte/pull/14). Not merged into the approved earlier baseline.
- **Four moments using only real PMD actions:** (1) a wake-up breathing/nod/pose gesture after actually waking, subject to a 30-minute cooldown; (2) victory pose/nod after a *completed* ball minigame scoring at least 8, or a new record >=4, subject to a 10-minute cooldown; (3) a naturally rare safe walk toward the meadow edge, slightly likelier for Curious temperaments; (4) a rare high-Bond caress greeting, subject to an interaction chance and 12-minute cooldown. Existing mechanics and original sprite frames are unchanged.
- **How to test:** Four no-save Charmander preview launchers `TEST-SURPRISE-WAKE.bat`, `TEST-SURPRISE-VICTORY.bat`, `TEST-SURPRISE-WANDER.bat`, `TEST-SURPRISE-FRIENDSHIP.bat`; each simulates an event, shows an explanatory label only in the test mode, and leaves the actual save untouched. In normal play, events are rare and have no captions.
- **Safety:** Normal happy/neutral behavior is resumed afterward; no surprise during sleep, eating, evolution, hunger/tiredness, menu/minigame, or when the relevant original PMD action is missing. No new rewards, stats, save fields or artwork. The accepted Phase 2 12 BMPs and 303 PMD files are **byte-identical** in the packaged Windows playtest ZIP.
- **Automated validation:** [GitHub Windows workflow #40119620419](https://github.com/TMA-X-Official/PokeByte/actions/runs/40119620419) passed rare event/cooldown tests, four save-safe screenshot previews, previous 11 personality previews, five scenery modes, clear/rain/snow, and original home/ball/training render checks. Final downloadable Windows ZIP was repackaged with the approved original art and validated for integrity.
- **Historical next step:** User playtest was required for this first Pack 03 iteration. **That acceptance was later obtained for the final distinct-surprises build, 24 October 2026; see final milestone below.** Physical Waveshare firmware remains untouched and is reserved for Phase 5.

## Phase 3 — Pack 03.1 surprise one-shot recovery fix (24 October 2026)

- **Reported issue:** User observed `TEST-SURPRISE-WAKE.bat` repeatedly yawning/stretching/jumping, apparently stuck, and asked that all four surprises be reviewed. Investigation found all four **preview modes automatically replayed their events every 10 seconds**. Actual intended single-gesture behavior was therefore obscured.
- **Corrected Windows tester:** All four demo surprises — Wake, Victory, Wander, Friendship — now play **once per launcher invocation**, then return to genuine original PMD Idle for about **4.5 seconds**, followed by normal approved Pack 02 behavior. The preview-only caption changes to **Back to normal**. Reopening the launcher repeats the demo. In ordinary gameplay, original rare-event/cooldown logic remains.
- **Safeguards:** Hunger, tiredness, sleep, and evolution take priority; an existing Pack 02 caress reaction cannot interrupt the surprise. Missing gestures safely fall back to normal sprite behavior. No game stats, saves, care rewards, minigame logic, hardware firmware or original pixel-art assets changed.
- **Source:** [`pokebyte-phase3-pack03-one-shot-fix`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-one-shot-fix), [draft PR #15](https://github.com/TMA-X-Official/PokeByte/pull/15), left unmerged pending user acceptance.
- **Proof:** [Windows workflow #40121120854](https://github.com/TMA-X-Official/PokeByte/actions/runs/40121120854) passed four actual screenshot before/after (~14 seconds) tests and no-save checks, surprise cooldown and action fallbacks, all prior personality tests, five time modes, weather modes and both minigames. The downloadable ZIP was verified to contain all **12 approved BMPs** and **303 original Pokémon PMD files** byte-identical to the preceding user-provided test ZIP.
- **Historical acceptance status (Pack 03.1):** This intermediate one-shot tester was awaiting review. The user subsequently tested all four previews, requested more distinct movements, and approved the refined Pack 03.2 build on 24 October 2026; see final milestone below.

## Phase 3 — Final accepted milestone: Personality Pack 03.2 (24 October 2026)

- **Status:** ✅ **Phase 3 COMPLETE — user-approved Windows PC playtest, 24 October 2026.** Packs 01 (moods), 02 (temperament and Bond), and 03 (little surprises) are all accepted. Phase 4 has **not** started.
- **Final user feedback:** All four special event launchers were tested hands-on. The user found the first implementations too similar, requested distinct performances, then tested the revised version and explicitly approved marking **Phase 3 done**.
- **Approved original-animation choreography:**
  - **Wake:** genuine original PMD DeepBreath then Nod, or available original-sprite fallbacks.
  - **Victory:** original PMD Hop then Pose; only after a successful completed ball minigame, with normal game rewards unchanged.
  - **Wander:** paced walk to a safe meadow edge, short pause, then walk back to the original clearing.
  - **Friendship:** Nod then Pose, using the existing original heart graphic at high Bond.
- **Timing and safeguards:** Each preview is a **one-shot** and does not auto-replay; the Pokémon returns to genuine Idle for about **4.5 seconds** and resumes its temperament/mood planner. Normal-play event rarity and cooldowns remain; sleep, hunger, tiredness, evolution, and game activities keep priority. Existing 151 species, art frames, mini-games, care, save fields, and hardware firmware remain unchanged.
- **Approved source branch:** [`pokebyte-phase3-pack03-distinct-moments`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-distinct-moments); [review PR #16](https://github.com/TMA-X-Official/PokeByte/pull/16), building on earlier Pack 03 [PR #14](https://github.com/TMA-X-Official/PokeByte/pull/14) and one-shot fix [PR #15](https://github.com/TMA-X-Official/PokeByte/pull/15). [Windows build and regressions #40123935906](https://github.com/TMA-X-Official/PokeByte/actions/runs/40123935906) **passed**. Previous Phase 3 approval history and source branches remain preserved.
- **Accepted Windows ZIP:** `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip`; **SHA-256: `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`**. Final ZIP was assembled using the tested CI executable and **all 12 original approved 466×466 background BMPs and 303 original Pokémon BINs verified byte-for-byte against the user's backup**. This also fixes a temporary **grey square** caused by faulty synthetic CI background bitmaps; never replace the five distinct approved day-part scenes with a single sunset image.
- **Packaging caveat:** GitHub Actions produces temporary **synthetic** scene BMPs strictly for CI. The user-approved final ZIP with correct artwork was packaged separately; **do not distribute the raw GitHub Actions artifact as the accepted visual release**. Before a reproducible public GitHub release, package the approved assets with the executable and recheck their hashes. Preserve the source branch and known-good accepted ZIP.
- **Integration boundary:** The review PR remains isolated, not merged into the original `main` hardware firmware. Completion means **Windows PC user acceptance**, **not** ESP32/Waveshare firmware release. No device flash or changes to saved Pokémon occurred. **Phase 4 — gameplay expansion** is the next planning step, requiring a separate proposal and approval; **Phase 5 — hardware deployment** remains deferred.

## After each phase

Review the completed features and Windows playtest, update this roadmap, and start the next phase only after the user approves it. Preserve known-good downloads and branches.
