# PokéByte — Continuity / New-Conversation Handoff

**Last project checkpoint:** 24 October 2026. **Canonical project:** [TMA-X-Official/PokeByte](https://github.com/TMA-X-Official/PokeByte).

> **READ THIS FIRST IN ANY FUTURE CHAT.** This file documents the user-approved source of truth, known-good Windows deliverable and next milestone. It is a **project-state summary**, not a verbatim transcript. The user retains a separate conversation/archive ZIP; do not assume ChatGPT has access to chats in a different thread.

## Exact status

| Phase | Status | Meaning |
| --- | --- | --- |
| 0 — Preserve original TamaPoke | **Completed** | Working baseline and original hardware firmware kept safe |
| 1 — Remastered environments | **Completed, user-approved on Windows** | Five distinct day-part scenes and their matching minigame scenes |
| 2 — Living environment | **Completed, user-approved on Windows** | Balanced pollen, grass movement, evening/night fireflies, manual rain/snow; **no animated overlay clouds** |
| 3 — Personality | **Completed, user-approved on Windows 24 Oct 2026** | Pack 01 moods; Pack 02 temperament/Bond; Pack 03 four distinct one-shot surprises |
| 4 — Gameplay expansion | **In development — Pack 01 classic-style Pokémon battles, awaiting user approval** | User explicitly rejected Adventure Quiet / Item / Wild and wants turn-based 1v1 battles with classic FIGHT/BAG/POKEMON/RUN actions |
| 5 — Waveshare release | **Not started** | Device build, flash, backup and hardware testing reserved for later |

### Non-negotiable preservation

1. **Never modify or automatically merge onto `main` the untested ESP32 hardware/gameplay source.** Documentation-only updates on `main` are fine. Development happens on dedicated branches/PRs. A user's approval of a Windows playtest is **not** authorization to flash hardware.
2. Preserve the real TamaPoke/PMD sprites and animations: exactly **303 original PMD sprite `.bin` files**, without rescaling/repainting/replacing them.
3. Preserve **all 12 approved 466×466 BMP files**, each by byte identity. They include 5 **different** meadow scenes, 5 matching minigame scenes, and 2 fallback files. Do **not** replace them with generic procedural fixtures, a grey placeholder, or a single sunset image.
4. Preserve existing feeding, evolution, egg odds, original level timing, Pokédex, saves, swipe menus, ball/training minigames, weather and Packs 01–03.
5. Every new gameplay feature must use an independent Windows test branch and user acceptance. Demo launchers must never read/write `tamapoke_pc_save.dat`; any persistent Phase 4 data requires an explicit backward-compatible save migration and preservation tests.

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

## Phase 4 — user-accepted direction (next work)

The user provided a previous-chat text export and confirmed focus on **Gameplay Expansion**:

- **More gameplay:** adventure mode, wild Pokémon encounters, **turn-based** battles, additional minigames and collectible items.
- **Long-term progression:** Gym challenges, badges, achievements, unlockable habitats, shiny hunting improvements and rewards.
- User initially supported an Adventure-first plan, **but later inspected the Forest Trail screenshot and explicitly rejected the entire Adventure Quiet/Item/Wild experience**. That decision supersedes Adventure-first: **the current priority is classic-style turn-based Pokémon battles**. Do not recreate/ship the rejected Forest Trail or its three launchers.
- Revised packs: **01 classic-style battle prototype** → **02 deeper battle mechanics/encounters** → **03 items & more minigames** → **04 gyms, badges & progression** → **05 habitats, shiny and rare discovery systems**. Future scope remains subject to user approval.
- Avoid breaking existing level progression (one level per hour of real time). **Trainer Rank should be separate** from Pokémon level.
- Existing code already has Gen 1 ATK/DEF/SPD derived from real base stats, genes and training, 8 original medals, shiny egg odds, stages of bond, a ball mini-game and strength-bag training. Reuse rather than duplicate them.
- See [PHASE4-GAMEPLAY-PLAN.md](PHASE4-GAMEPLAY-PLAN.md) for the working proposal and safe acceptance criteria.
- **The first Forest Trail Pack 01 was explicitly rejected by the user and PR #17 closed without merge.** A fresh Phase 4 Pack 01 Pokémon-style battle prototype is on a **separate branch based directly on accepted Phase 3** and awaits user's hands-on playtest. No persistent battle rewards, inventory or save changes. No hardware changes.

## Phase 4 Pack 01 — REJECTED Forest Trail (historical record)

- **Development branch:** [`pokebyte-phase4-pack01-adventure-prototype`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase4-pack01-adventure-prototype), [draft PR #17](https://github.com/TMA-X-Official/PokeByte/pull/17). Based directly on approved Phase 3 source; not merged.
- **Implemented:** Optional new Adventure screen reached via home **ADVENTURE** pill or PC **G** key, single Forest Trail with original Pokémon walk animation, three outcome kinds (peaceful, berry discovered, wild Gen 1 original PMD sprite), repeatable **WALK/AGAIN**, always-available **HOME** return. No battle, permanent items, extra currency, Pokémon stat/save mutation.
- **Test launchers:** `TEST-ADVENTURE-QUIET.bat`, `TEST-ADVENTURE-ITEM.bat`, `TEST-ADVENTURE-WILD.bat`, all in-memory Charmander no-save previews. Existing Phase 3 launchers and standard `PLAY-POKEBYTE.bat` remain.
- **Successful Windows CI:** [run #40129169216](https://github.com/TMA-X-Official/PokeByte/actions/runs/40129169216), compiled source commit `aa65bce87d572e3bcfcc80d52e322757e6d54ccb`; all previous gameplay/surprise tests and new state/screenshot tests passed.
- **User-facing ZIP filename:** `PokeByte-Phase4-Pack01-Forest-Trail-Approved-Art-Windows.zip`, SHA-256 `d914e79c7942305fe274e7144bdf21887c4b751f9c01ab6bde63f816d8bfb434`. Verified that 12 BMP and 303 PMD BIN source bytes are **exactly the approved Phase 3 ZIP's**; ZIP CRC integrity pass. Do not substitute synthetic scene fixtures from the raw GitHub CI artifact.
- **Acceptance:** **IN TEST / awaiting user hands-on feedback.** Not yet approved. Phase 3 ZIP remains the last **user-approved** Windows baseline. Do not advance to battle development or hardware deployment without this pack's review.
- **New chat instructions:** Forest Trail was definitively **rejected**, PR #17 closed unmerged. **Do not restore Adventure Quiet, Item or Wild.** Use the **current Phase 4 battle PR #18** and this continuity file instead; check whether the replacement battle ZIP was tested before continuing.

## Phase 4 Pack 01 — replacement classic Pokémon battles (24 October 2026)

- **Latest explicit user instruction:** The Forest Trail screenshot was unsatisfactory. **Remove Adventure Quiet / Adventure Item / Adventure Wild. Instead build a battle system that feels like a classic main-series Pokémon game.** This is the controlling instruction; any earlier Adventure-first plan is historical.
- **Rejected PR:** [#17](https://github.com/TMA-X-Official/PokeByte/pull/17), **closed without merge**. The Windows adventure ZIP must not be promoted or used as the source for further development.
- **Replacement branch:** [pokebyte-phase4-pack01-classic-battles](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase4-pack01-classic-battles); [draft PR #18](https://github.com/TMA-X-Official/PokeByte/pull/18), based **directly** on approved Phase 3 source. No Adventure mode or Adventure launchers.
- **Prototype mechanics:** new BATTLE home-screen entry / PC K shortcut, 1v1 menu **FIGHT/BAG/POKEMON/RUN**, four moves and PP, separate temporary battle HP bars, classic-inspired elemental effectiveness, turn order based on existing ATK/DEF/SPEED + move priority, optional one-use **practice-only** potion and safe HOME exit, original PMD Attack/Hurt sprites with available-action fallbacks. A win/faint/escape has **no persistent impact** on the player's saved Pokémon.
- **Demo opponents:** Bulbasaur, Squirtle, Pikachu; safe standalone TEST-BATTLE launchers use an in-memory Charmander and `--no-save`. The user still needs to visually test and approve the replacement.
- **Windows CI successful:** [run #40131289786](https://github.com/TMA-X-Official/PokeByte/actions/runs/40131289786), source commit `fddf23c208357e5c1ec4134c2479caa1c37bd090`; battle state tests, three distinct no-save original-sprite screenshots, and all previous gameplay/personality regressions passed.
- **Current Windows test ZIP:** `PokeByte-Phase4-Pack01-Classic-Battles-Windows-TEST.zip` (SHA-256 `65bb9f55e6fcd55e89d812972974c3ad45ad8f1040df10d2ec3103a7a1f7032e`), 7,989,250 bytes. All **12 approved 466×466 BMPs** and **303 original PMD BINs** exactly match approved Phase 3 ZIP; ZIP CRC pass; the rejected Adventure launchers are absent, along with all CI synthetic fixture BMPs. Do not treat this unapproved battle prototype as the new accepted baseline until user playtests it.
- **Pending:** User's evaluation of classic 1v1 battle mechanics and UI. Do not merge or flash Waveshare; Phase 3 remains accepted backup.
- **Future:** richer battle type chart, learned moves, party/items, Gym progress only if user approves this visual battle direction first.

## Practical recovery in a new ChatGPT conversation

1. Start with: **"Continue PokéByte from the accepted Phase 3 baseline. The user rejected Forest Trail/Adventure Quiet/Item/Wild, PR #17 is closed. Phase 4 Pack 01 now means classic-style Pokémon battles on draft PR #18. Read the GitHub PROJECT-CONTINUITY.md, PHASE4-GAMEPLAY-PLAN.md, and APPROVED-ASSETS.md and preserve the approved artwork/sprite binaries. Do not flash firmware or merge automatically."**
2. Upload the privately retained **PokéByte Continuity Archive ZIP**, or at minimum the approved Phase 3 Windows ZIP, the backup ZIP, and the previous-chat export.
3. Verify the ZIP and [asset hashes](APPROVED-ASSETS.md) before providing a new Windows build. Source-only CI art is **not** enough. Never invent a local download link.
4. Check the GitHub roadmap and most recent PR; don't assume a new conversation has access to the prior session or uploaded binary files.
5. Develop on a separate Phase 4 branch, verify core gameplay and original PMD fidelity, provide a Windows preview and await the user's explicit playtest approval.

## History coverage and privacy

This public GitHub document captures project decisions and milestones, **not private ChatGPT conversation transcripts**. The user supplied a pasted text export of the **early** original conversation (`Pasted text(20261024-192835).txt`, 843 lines, SHA-256 `7b7f1addce79040a996951e0dbe660f9eaa94b26b0d9037700837c1a2554654d`). The file doesn't cover all later messages; later state was reconstructed from the current chat and GitHub evidence. The original transcript and user uploads belong in the user's **privately stored archive**, not in a public repository.

**Authority order when reconstructing:** explicit recent user approval + verified accepted Windows ZIP and backup hashes → GitHub source/review and passing test workflows → historical exported chat for feature intention → assistant proposals (not automatically approved).
