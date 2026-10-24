# PokéByte Phase 4 — Gameplay Expansion (working plan)

**Project status:** The user likes the proposed direction and confirms **Pack 01 — Adventure Mode first**. The *feature plan* is agreed as a direction; no Phase 4 code or finished screens are approved yet. Date: 24 October 2026.

**Baseline:** [Phase 3 approved Windows source](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-distinct-moments), [known-good artwork and executable manifest](APPROVED-ASSETS.md), and [continuity note](PROJECT-CONTINUITY.md). Preserve the working original hardware firmware on `main`. Windows-first experiments only; Phase 5 is for Waveshare firmware.

## Vision from the user's original conversation

**More gameplay:** adventure mode, wild Pokémon encounters, turn-based battles, extra minigames, collectibles.

**Long-term progression:** gyms, badges, achievements, unlockable habitats, shiny discovery improvements, rewards.

The original conversation described both a simple battle system and long-term Gym/Trainer progression; the later user reiterated these categories explicitly. Our current roadmap numbers these collectively as **Phase 4**, following approved personality work in Phase 3. Specific event probabilities, item economy, Gym difficulty, battle moves, layouts and reward values still need prototype-level decisions and user review.

## Proposed pack structure (separate review / approval gates)

| Pack | Focus | Prototype deliverable / acceptance gate |
| --- | --- | --- |
| **01** | **Adventure Mode** | A safe, playable exploration loop on Windows: enter from home, see a forest/meadow route, trigger a bounded encounter or find, return home without altering saves |
| **02** | **Wild encounters and turn-based battles** | One controlled 1v1 fight using real ATK/DEF/SPD and original PMD attack/hurt/idle sprites; sensible battle UI and type interactions; clear win/loss/flee outcomes |
| **03** | **Collectible items and new minigames** | Small inventory prototype, safe reward logic and one or more optional games; retain original ball and bag training activities |
| **04** | **Gyms, badges, achievements, Trainer Rank** | A replayable Gym challenge, earned badge/trainer-rank progression and clearer long-term goals; distinguish new Gym badges from the eight existing per-Pokémon medals |
| **05** | **Unlockable habitats and rare discoveries** | Additional *new* adventure area choices, carefully bounded shiny-related discovery/reward systems, unlockables while preserving the five original approved home scenes |

Each pack is proposed, not automatically accepted. Review the visible behavior and pacing with the user before expanding scope.

## Pack 01 — suggested first Windows preview

**Primary design constraint:** Adventure is a *new optional mode*, not a modification of the existing care loop.

Suggested first milestone:

1. An **Adventure** entry point that fits the existing 466×466 layout/touch/swipe behavior and is reachable without misclicking feed/sleep/minigames.
2. A single **Forest/Meadow Path** route. Keep the 12 approved home/minigame BMPs unchanged; any adventure-only art must be an additional asset, not a replacement.
3. Authentic PMD pet Idle/Walk actions and existing sprite loader/rendering; no AI-substitute Pokémon art.
4. Short event prototype outcomes: `Nothing unusual`, `Found an item`, `Wild Pokémon spotted`. Encounter displays an appropriate real PMD sprite if available, or a safe text-only fallback; **do not implement incomplete battles or grant permanent items before Pack 02/03 persistence design**.
5. **Return home** action available at all times, restores normal pet UI and scheduler cleanly. No surprise left running and no stranded Pokémon positions.
6. One-click Windows-only preview with `--no-save` and test launchers. Ordinary care/save mode unchanged, existing Pokémon never overwritten.
7. Use configurable/deterministic preview cases so the user can explicitly test each adventure outcome without waiting for randomness.

**User feedback to gather after prototype:** clarity of adventure entry/exit, journey pace and length, feeling of discovery, encounters being exciting but not disruptive, whether touching feels natural on the round display. No selection of art/UI as final until approved.

### Explicit boundaries for Pack 01

- No new persistent inventory schema, Gym badges, party system, overwriting Dex registrations or altering egg/shiny odds.
- No new battle rewards or stats changes before Pack 02 design/testing.
- No hardware flashing or claim of Waveshare compatibility: Windows first.
- Do not alter original level progression, evolution, care routines, existing 8 medals, PMD binary art, save keys or approved backgrounds.
- All 303 original sprite BINs remain bit-identical, and all 12 approved BMPs remain bit-identical in each delivered Windows ZIP.

## Packs 02–05 design safeguards

**Battles:** Original source already supplies `Pet::atkStat()`, `defStat()`, `speStat()`, individual genes, care/energy and the PMD `ATTACK`/`HURT` actions. Battle HP, damage formulas, available moves and type matchups should be deterministic and testable before adding progression; never confuse pet's four 0–100 *care* needs with battle HP. Respect timing, cancelability and no-permanent-penalty preview mode.

**Items/minigames:** Keep reward/accounting and anti-farming rules clear. Original ball minigame and strength training should remain identical (physics, score, rewards and menus); new activities must not silently change them.

**Gym/achievements:** Existing eight medals are per-Pokémon and some player-wide counters persist. Gym badges and Trainer Rank should be separate, with forward/backward-compatible save fields and protected ownership across pet life cycles.

**Habitat/shiny:** User-approved home-scene cycle remains intact and should not be modified. Adventure unlockables add scenes instead of replacing existing five; original egg/shiny roll is preserved unless a later explicit user-approved mechanic specifies a controlled change.

## Windows build validation — mandatory before user ZIP

1. Verify complete packaging: executable, libraries, .bat test launchers, **12 approved 466×466 BMP backgrounds** and **303 unchanged PMD BIN files**; check names **and bytes** against trusted backup or [manifest](APPROVED-ASSETS.md).
2. Ensure no synthetic CI fixture from `test_modes_ci.py` is shipped as approved art. Never replace distinct time-of-day scenes with `sunset-forest.bmp`. The prior grey-square regression arose from shipping synthetic BMP fixture content.
3. Regression test all 5 day-parts, 5 minigame backgrounds, scenery/weather toggles, clear/rain/snow, five approved scene layouts, ball game, strength bag, original care, sleep and evolution, menu/swipes.
4. Regression test 3 personality packs, including all four no-save one-shot surprise animations and their recovery to normal Idle.
5. Assert no actual save file was read or written in demos; no new save-file keys without explicit design review and migration tests.
6. Provide a new Windows ZIP with SHA-256, identify its source commit and list exactly which additional game files changed. Never approve or merge automatically.

## Development checkpoint

**Next action:** implement the smallest possible *Pack 01 forest-route Windows prototype* on a **new Phase 4 development branch** using the Phase 3 approved source as base. Show the user the preview, gather feedback and iterate. **Do not mark Phase 4 complete** until all user-approved packs are delivered and accepted.
