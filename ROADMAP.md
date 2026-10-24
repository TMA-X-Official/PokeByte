# PokéByte — Development Roadmap

**Last reviewed:** 24 October 2026  
**Project principle:** Keep the real TamaPoke game, Pokémon sprites, animations, swipe menus, minigames and save behavior intact. Experiment only on separate branches and keep the classic game available.

| Phase | Status | Acceptance |
| --- | --- | --- |
| **Phase 0 — Secure working version** | ✅ **Completed** | Original TamaPoke code and original Windows PC emulator baseline preserved; separate development branches |
| **Phase 1 — Visual upgrade** | ✅ **Completed (Windows PC playtest)** | User approved all five correctly framed remastered environments: morning, day, afternoon, evening and night. Home, ball mini-game and training-bag backgrounds match. Classic scene toggle remains available; original Pokémon/gameplay preserved |
| **Phase 2 — A living environment** | 🟡 **In progress — Windows PC prototype** | Subtle fireflies, daylight pollen particles, optional drizzle and evolving habitat atmosphere; awaiting user playtest |
| **Phase 3 — More Pokémon personality** | ⬜ Not started | Bonding reactions and varied behavior without replacing original animations |
| **Phase 4 — Gameplay expansion** | ⬜ Not started | New gameplay systems developed alongside existing minigames |
| **Phase 5 — Waveshare release** | ⬜ Not started | Hardware optimization, flashable builds, backup/recovery and device validation |

## Phase 1 accepted baseline

- **Approved on:** 24 October 2026, following hands-on Windows playtesting.
- **Source development branch:** [`pokebyte-phase1-minigames-scene`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase1-minigames-scene).
- **Draft review:** [Pull request #5](https://github.com/TMA-X-Official/PokeByte/pull/5).
- **Automated PC checks:** [Windows build and home/ball/training screen tests](https://github.com/TMA-X-Official/PokeByte/actions/runs/40103420707).
- **Important:** The approved playable Windows test ZIP includes a set of full-resolution background artwork files. Some artwork files are distributed as part of that ZIP and **have not all been committed to the GitHub source branch**. Before a standalone GitHub release, make the asset packaging fully reproducible. Do not represent a source-only GitHub build as visually identical without these assets.
- **Hardware:** No PokéByte firmware upgrade was flashed to the Waveshare as part of this phase; that is Phase 5. The actual device still runs its existing firmware.

## Battery indicator polish and Phase 2 prototype

- **Battery percentage:** The actual Waveshare AXP2101's existing `batPercent()` value is displayed beside its battery icon by the new development code. Charging bolt and battery colors are retained. In the Windows emulator, show **PC** rather than inventing a charge percentage.
- **Development branch:** [`pokebyte-phase2-ambient-polish`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase2-ambient-polish), separate from the completed Phase 1 working Windows build.
- **First Phase 2 effects:** subtle evening/night fireflies, daytime pollen specks, optional manually toggled light drizzle. The approved remastered artwork and original minigames remain unchanged. **B** = Classic / remastered scenery, **A** = ambient effects on/off, **W** = optional rain.
- **Not real weather:** No internet or forecast integration. Weather is visual-only and defaults off; a hardware release still needs separate testing.
- **Status:** The Phase 2 PC playtest will determine what to refine next, and Phase 2 remains incomplete.

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
- **State:** Tested Windows build prepared. **Awaiting user playtest**, especially automatic time switching, gentle motion, rain toggles, original minigames, and existing PC save behavior. Phase 2 is **not yet complete**.

## After each phase

Review the completed features and Windows playtest, update this roadmap, and start the next phase only after the user approves it. Preserve known-good downloads and branches.
