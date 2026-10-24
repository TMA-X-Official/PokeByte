# PokéByte Phase 4 — Pack 01: Classic-Style One-on-One Pokémon Battles

**Status: Windows prototype; pending the user's playtest. Do not merge or flash hardware.**

## User-requested direction change

After viewing the Forest Trail test, the user explicitly rejected Adventure Item, Adventure Quiet and Adventure Wild. The corresponding PR #17 was closed **without merging**. This new battle pack is built directly on the user-approved Phase 3 branch, deliberately **not** from the rejected Adventure implementation. No Adventure button, Forest Trail screen, G:Adventure control, or TEST-ADVENTURE launchers belong in the new ZIP.

## How to test

- Normal game: start PLAY-POKEBYTE.bat, tap BATTLE (if your Pokémon is awake) or press K.
- TEST-BATTLE-BULBASAUR.bat: isolated Charmander vs Bulbasaur (Fire advantage).
- TEST-BATTLE-SQUIRTLE.bat: isolated Charmander vs Squirtle (Fire disadvantage).
- TEST-BATTLE-PIKACHU.bat: isolated Charmander vs Pikachu (neutral).

These three tests use a temporary in-memory Charmander and **never read or write** tamapoke_pc_save.dat. Keep the approved Phase 3 backup intact.

## Familiar Pokémon battle interface

- Separate opposing Pokémon and your Pokémon with names, levels and temporary battle HP bars.
- Existing original PMD sprite files for both Pokémon, with attack/hurt animations if the actions exist. No alternate or AI-drawn Pokémon sprite art.
- **FIGHT / BAG / POKEMON / RUN** main selection (the last returns home).
- **FIGHT:** four moves with independent PP, same-type bonuses, elementary Fire/Grass/Water/Electric strengths/weaknesses and neutral damage, Speed determining turn order, QUICK ATK priority, GROWL weakening the foe.
- **BAG:** a single *demo-only* Potion restores temporary battle HP; it is **not a persistent inventory item**.
- **POKEMON:** explains that the prototype has only your current partner; no party switch implemented.
- **WIN / LOSE:** HP depletion resolves match and offers HOME; you can always leave with HOME.

**Not yet a complete main-series game:** the battle calculations and limited type/move tables are simplified. Full move learning, 18-type chart, party/box, XP, Poké Balls, status conditions, gym fights and item progression can be designed and tested in later packs. There is currently no permanent win reward or penalty, no damage to real care/health, no changes to Pokémon level/evolution/shiny odds and no new save fields.

## Preservation requirements

Keep all twelve approved 466×466 background BMPs and all 303 original Pokémon PMD BIN files **byte-for-byte unchanged** relative to the accepted Phase 3 Windows ZIP. Preserve five lighting modes, weather, all original care/evolution menus, original ball and training minigames and all three approved personality packs, including four one-shot surprise demos. Use a new Windows ZIP in a new folder. Synthetic CI background fixtures are for test automation only and must be replaced with **verified user-approved artwork** before sharing. Phase 4 still requires user acceptance; the physical Waveshare firmware is for Phase 5.
