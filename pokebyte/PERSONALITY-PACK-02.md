# Phase 3 — Personality Pack 02: Bonding & Temperament (Windows Playtest)

**Status:** Experimental Windows test pending user acceptance. No device flashing. Preserve the completed Phase 2 and approved Phase 3 Pack 01.2 baseline.

## What is real in this build

1. **Four stable individual temperaments.** Playful, Calm, Curious, Shy. The temperament is derived deterministically from the Pokémon's **existing stored hatch genes** (geneAtk, geneDef, geneSpe) and does **not** depend on species or evolution form. The existing genes are saved with each Pokémon; no new save-file fields are added.
2. **Three friendship stages** from the pre-existing **Bond 0–100** statistic. 0–29 New Friend, 30–69 Growing Bond, 70–100 Close Friend.
3. **Bond affects on-screen behavior.** The temperament and bond together adjust the frequency of genuine existing PMD Pose/Nod/DeepBreath, standing still, or walking. Shy becomes notably more expressive at high Bond; Playful gestures more; Calm relaxes more; Curious explores more. Pokémon missing a PMD gesture use an existing supported animation instead.
4. **Existing mood takes precedence.** Hungry/Sleepy animation priorities from Pack 01 stay intact. Egg odds, medals, stats, care, evolutions and original mini-games are unchanged.
5. **Profile-only text.** The existing Pokémon profile displays a small English label (e.g. `Playful - Growing Bond`) beneath the existing stats. No permanent home-screen overlay or new bars.

## Safe one-click demonstration launchers

Double-click each batch file after extracting the ZIP. It launches a **temporary Charmander in memory**, with the requested temperament and Bond stage purely for **visible demonstration**. It neither loads nor writes `tamapoke_pc_save.dat`. The two Bond preview values are low=15 and high=85; they do NOT change the genuine Pokémon or its Bond.

| Temperament | Low Bond | High Bond |
| --- | --- | --- |
| Playful | `TEST-PLAYFUL-LOW-BOND.bat` | `TEST-PLAYFUL-HIGH-BOND.bat` |
| Calm | `TEST-CALM-LOW-BOND.bat` | `TEST-CALM-HIGH-BOND.bat` |
| Curious | `TEST-CURIOUS-LOW-BOND.bat` | `TEST-CURIOUS-HIGH-BOND.bat` |
| Shy | `TEST-SHY-LOW-BOND.bat` | `TEST-SHY-HIGH-BOND.bat` |

**Suggested first comparisons:** Playful Low vs Calm Low (active vs relaxed); Curious Low vs Shy Low (exploration vs shy restraint); **Shy Low vs Shy High** (largest Bond-related change). Let each demonstration play long enough to see several gestures. Home-screen preview-only label shows which temperament and Bond stage was selected. For clarity, close each game window before launching the next one.

## Normal persistent game

- `PLAY-POKEBYTE.bat` uses the Pokémon's actual saved genes and actual Bond.
- Swipe to the existing Pokémon profile for the current temperament and friendship stage (English Windows UI only in this early test).
- **P** toggles enhanced vs original Classic idle scheduler.
- **B** approved remastered/classic scenery, **A** ambience, **W** rain, **S** snow.
- Only the eight preview .bat files use hypothetical values; the real game does not.
- All five approved 466×466 environment images and their matched minigame images are redistributed **byte-for-byte** from the approved previous Windows build, as are all 303 PMD sprite BIN files.

## Boundaries and verification

This pack does NOT add extra Bond points, change existing Bond progression or rewards, introduce an emotion meter, force gestures the selected Pokémon lacks, add new sprites, or implement Pack 03's rare surprises.

Windows CI validates four temperament policies, three Bond threshold stages, evolution/genetics determinism, graceful sprite fallbacks, eight no-save demo screenshot renderings, the prior three mood previews, five distinct time-of-day scenes, three weather modes, and the original ball/training mini-games.

**Important:** Windows demonstration first. The physical Waveshare remains on its original firmware; release is Phase 5. Draft PR remains unmerged.
