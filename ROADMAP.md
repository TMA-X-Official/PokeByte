# PokéByte — Development Roadmap

**Last reviewed:** 24 October 2026  
**Project principle:** Keep the real TamaPoke game, Pokémon sprites, animations, swipe menus, minigames and save behavior intact. Experiment only on separate branches and keep the classic game available.

| Phase | Status | Acceptance |
| --- | --- | --- |
| **Phase 0 — Secure working version** | ✅ **Completed** | Original TamaPoke code and original Windows PC emulator baseline preserved; separate development branches |
| **Phase 1 — Visual upgrade** | ✅ **Completed (Windows PC playtest)** | User approved all five correctly framed remastered environments: morning, day, afternoon, evening and night. Home, ball mini-game and training-bag backgrounds match. Classic scene toggle remains available; original Pokémon/gameplay preserved |
| **Phase 2 — A living environment** | ⬜ Not started | Dynamic touches: ambient animation, weather and habitat polish |
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

## Small UI polish idea — battery charge display

The original firmware already obtains `batPercent()` from the Waveshare AXP2101 PMU. A future optional header update could show `85%` beside the battery icon, preserve the charging bolt, and hide the percentage if the reading is unavailable.

**PC caution:** the Windows emulator does not measure the Waveshare battery. Show a neutral `PC`/unavailable indication there, not a fictitious battery percentage. This is a minor separate follow-up, **not a blocker to completed Phase 1**.

## After each phase

Review the completed features and Windows playtest, update this roadmap, and start the next phase only after the user approves it. Preserve known-good downloads and branches.
