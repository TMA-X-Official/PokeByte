# Archived — rejected Phase 4 gameplay experiments

**Cancelled by user on 24 October 2026. Not part of the active roadmap or Waveshare release.** Both Forest Trail and classic 1v1 battles were tested and rejected. [PR #17](https://github.com/TMA-X-Official/PokeByte/pull/17) and [PR #18](https://github.com/TMA-X-Official/PokeByte/pull/18) were closed **without merging**. Phase 5 hardware release must be derived directly from the accepted Phase 3 source. The following is preserved **only for historical traceability**, not a plan to implement.

---

# PokéByte Phase 4 — Gameplay Expansion (working plan)

**Project status:** The user **rejected Forest Trail Adventure Quiet/Item/Wild** and explicitly requested classic Pokémon-style battles. PR #17 closed unmerged. Replacement **Pack 01 Battle Windows tester** on [draft PR #18](https://github.com/TMA-X-Official/PokeByte/pull/18) passed [CI #40131289786](https://github.com/TMA-X-Official/PokeByte/actions/runs/40131289786). Final user-facing ZIP `PokeByte-Phase4-Pack01-Classic-Battles-Windows-TEST.zip` (SHA-256 `65bb9f55e6fcd55e89d812972974c3ad45ad8f1040df10d2ec3103a7a1f7032e`) includes **all exact approved 12 scene BMPs and 303 original PMD sprite files**. **User's gameplay/visual acceptance still pending.**

**Baseline:** [Phase 3 approved Windows source](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase3-pack03-distinct-moments), [known-good artwork and executable manifest](APPROVED-ASSETS.md), and [continuity note](PROJECT-CONTINUITY.md). Preserve the working original hardware firmware on `main`. Windows-first experiments only; Phase 5 is for Waveshare firmware.

## Vision from the user's original conversation

**More gameplay:** adventure mode, wild Pokémon encounters, turn-based battles, extra minigames, collectibles.

**Long-term progression:** gyms, badges, achievements, unlockable habitats, shiny discovery improvements, rewards.

The original conversation described both a simple battle system and long-term Gym/Trainer progression; the later user reiterated these categories explicitly. Our current roadmap numbers these collectively as **Phase 4**, following approved personality work in Phase 3. Specific event probabilities, item economy, Gym difficulty, battle moves, layouts and reward values still need prototype-level decisions and user review.

## Proposed pack structure (separate review / approval gates)

| Pack | Focus | Prototype deliverable / acceptance gate |
| --- | --- | --- |
| **01** | **Classic-style 1v1 Pokémon battles (revised after user feedback)** | FIGHT/BAG/POKEMON/RUN, 4 moves + PP, original PMD Attack/Hurt, HP, simplified types, 3 save-safe opponent previews; awaiting user assessment |
| **02** | **Deeper battle mechanics / opponents** | Expand type/move fidelity, challenge variety and meaningful progression after the Pack 01 basic 1v1 UI is accepted |
| **03** | **Collectible items and new minigames** | Small inventory prototype, safe reward logic and one or more optional games; retain original ball and bag training activities |
| **04** | **Gyms, badges, achievements, Trainer Rank** | A replayable Gym challenge, earned badge/trainer-rank progression and clearer long-term goals; distinguish new Gym badges from the eight existing per-Pokémon medals |
| **05** | **Unlockable habitats and rare discoveries** | Additional *new* adventure area choices, carefully bounded shiny-related discovery/reward systems, unlockables while preserving the five original approved home scenes |

Each pack is proposed, not automatically accepted. Review the visible behavior and pacing with the user before expanding scope.

## Rejected Pack 01 Forest Trail — historical specification only

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

## Current Pack 01 — Classic Pokémon-style battle test

**Direct user decision:** Remove Adventure Quiet, Adventure Item and Adventure Wild completely from current gameplay and preview ZIPs, and replace Adventure Mode with battles modeled on familiar classic Pokémon 1v1 combat. This overrides the old Forest Trail proposal above. The previous Adventure pack [PR #17](https://github.com/TMA-X-Official/PokeByte/pull/17) is **rejected and closed unmerged**.

The replacement [battle branch](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase4-pack01-classic-battles) and [PR #18](https://github.com/TMA-X-Official/PokeByte/pull/18) start directly from the **approved Phase 3 code** with no adventure code carried over. Planned initial visible 1v1 behavior: HP panels, two original PMD Pokémon sprites, type-aware attack damage, four moves and PP, turn-order Speed, FIGHT/BAG/POKEMON/RUN, safe win/faint/run and always-available HOME. Three deterministic no-save Charmander battle demos versus Bulbasaur, Squirtle and Pikachu. Preserve the 12 exact BMP and 303 exact PMD files. This is still an **unapproved Windows prototype**, and no full party/inventory/EXP/badges system exists yet.

**Battle fidelity scope:** The new prototype emulates the broad classic battle experience in original code and UI; it is not a copy of Nintendo's engine/assets. We will tune detailed mechanics from user playtesting. The original virtual pet save and all approved personality/environment features remain unchanged.

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

**Latest decision:** Forest Trail implementation rejected; PR #17 closed without merge. Replacement classic battle interface is under Windows testing on [draft PR #18](https://github.com/TMA-X-Official/PokeByte/pull/18). **Next action:** visual review of the new battle ZIP and user feedback. Never ship previous Adventure/Forest Trail launchers. **Do not mark Pack 01 or Phase 4 complete** without explicit user acceptance.
