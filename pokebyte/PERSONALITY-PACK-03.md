# PokéByte Phase 3 — Personality Pack 03: Little Surprises (Windows test)

**Status: Human playtest required.** No hardware flash; no merge without approval.

This pack adds four naturally occurring, **infrequent**, non-rewarding personality moments using **only original PMD animation frames**. It builds on the already-approved personality Pack 01 & 02 and the completed five-scene Phase 2 environment.

## What the real game does

1. **Wake-up stretch** — after the Pokémon actually goes from sleeping to awake, a genuine `DeepBreath` stretch followed by a short `Nod` plays on the ground (with safe fallbacks). A 30-minute cooldown prevents repeated wake/sleep abuse.
2. **Victory flourish** — upon returning to the home screen after a *completed* ball game with at least eight points (or a new record of at least four points), it celebrates with a brief original `Hop` followed by `Pose` (with safe fallbacks). Original ball physics, records, scoring, joy rewards and UI are unchanged; exiting the game early triggers nothing. At most once every ten minutes.
3. **Rare explorer** — a brief spontaneous stroll to a safe meadow edge, a look around, then a walk back to the center. It follows the original walking animation and foot-ground positioning. Curious Pokémon are modestly more likely to do this, but other temperaments can too. The ambient selection is deliberately rare, with a further general event cooldown. It never crosses the scene boundaries.
4. **Friendship moment** — when the existing Pokémon has Bond >=70, occasionally petting it can queue a little `Nod` followed by `Pose` with the game's original heart icon. The original caress, Bond increase and heart still work. The check has a 12-minute cooldown and does not add new bonuses.

Surprises only replace a normal idle action when the Pokémon is happy, **not** when hungry/sleepy, asleep, eating, evolving or playing a minigame. The previous Bond/Temperament and mood logic resumes afterward. Pokémon lacking the animation safely skip the special event.

## Easiest way to test each moment

Extract the final Windows ZIP in a **new** folder, then double-click one of these standalone batch files:

| Launcher | What to watch |
| --- | --- |
| `TEST-SURPRISE-WAKE.bat` | Charmander stretches with DeepBreath then acknowledges with Nod with a **Wake-up Stretch** caption |
| `TEST-SURPRISE-VICTORY.bat` | Charmander does a brief Hop then strikes a Pose, labelled **Victory Flourish** |
| `TEST-SURPRISE-WANDER.bat` | Charmander explores an edge, pauses, then walks back, **Curious Explorer** |
| `TEST-SURPRISE-FRIENDSHIP.bat` | Charmander nods and poses with a friendly heart, **Friendship Moment** |

All four preview launchers create an **in-memory Charmander** and **never read or write `tamapoke_pc_save.dat`**. The caption appears only in these previews, not in normal game. **Fixed playtest behavior:** Each surprise preview fires **once** per .bat launch, completes its own distinct two-gesture or out-and-back walk sequence, then settles into real Idle for around 4.5 seconds, and then returns to normal Pack 02 temperament behavior. The caption changes to **Back to normal** when the surprise finishes. To replay the surprise, close the window and double-click the same launcher again. This avoids the earlier 10-second auto-replay that made Wake look stuck. The actual game retains its existing rare events and cooldowns.

After checking all four previews, open `PLAY-POKEBYTE.bat` to test the ordinary game. Press **P** to compare original/classic behavior, **B** classic vs remastered scenery, **A** ambience, **W** rain and **S** snow. All previously approved scenery and interactions should look identical.

## What is preserved

- Five user-approved home scenes, five full-height minigame scene BMPs and two fallback BMPs, all **byte-identical** to Phase 1/2/3 approved downloadable ZIPs
- All 303 original PMD sprite binary files and their exact frames, colors, sizing and positioning
- Original care, bond, saved genes, stats, egg odds, evolution, ball and training mini-games
- Pack 01's Happy/Hungry/Sleepy labels and Pack 02's four temperaments/three Bond stages, interaction reactions
- Windows only: physical Waveshare firmware untouched

The CI build uses synthetic background art **only for automated checks**. Final user ZIP must replace synthetic fixture BMPs with the exact approved artwork. Windows CI tests four no-save preview modes, pure event/cooldown/fallback logic, the former eleven mood/temperament preview modes, all five scene lighting modes, clear/rain/snow, and home/ball/training. This is still a **draft development branch**, never an auto-merge.

## Distinct surprise refinement (Windows playtest only)

The four surprises intentionally use different original-sprite choreography, not just different labels. Wake = DeepBreath then Nod. Victory = Hop then Pose. Wander = a paced walk out, a short Idle pause, a walk back. Friendship = Nod then Pose with the existing heart graphic. Missing optional actions fall back without displaying invented frames; all four transitions end normally, and user saves are untouched. The five approved home scenes and the five matching minigame scenes **must be copied byte-identically from the user's backed-up approved ZIP** when packaging the Windows executable, never replaced with CI-generated fixtures. Phase 3 remains pending the user's visual approval.
