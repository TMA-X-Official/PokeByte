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
| 4 — Gameplay expansion | **Planning approved; no implementation started** | User likes **Adventure Mode first**, then wild encounters, turn-based battles, extra minigames/items, gyms/badges/ranks, achievements, habitat unlocks and shiny-related progression |
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
- User explicitly **liked the recommendation to begin with Pack 01 — Adventure Mode**. This confirms direction and proposed ordering, **not** approval of a specific finished UI or permission to merge source code.
- Suggested packs: **01 Adventure Mode** → **02 turn-based battles** → **03 items & more minigames** → **04 gyms, badges & progression** → **05 habitats, shiny and rare discovery systems**.
- Avoid breaking existing level progression (one level per hour of real time). **Trainer Rank should be separate** from Pokémon level.
- Existing code already has Gen 1 ATK/DEF/SPD derived from real base stats, genes and training, 8 original medals, shiny egg odds, stages of bond, a ball mini-game and strength-bag training. Reuse rather than duplicate them.
- See [PHASE4-GAMEPLAY-PLAN.md](PHASE4-GAMEPLAY-PLAN.md) for the working proposal and safe acceptance criteria.
- **No Phase 4 gameplay code was implemented as of this checkpoint.** Do not mark Pack 01 completed without a new build and user playtest.

## Practical recovery in a new ChatGPT conversation

1. Start with: **"Continue PokéByte from its approved Phase 3 baseline. Read https://github.com/TMA-X-Official/PokeByte/blob/main/PROJECT-CONTINUITY.md and https://github.com/TMA-X-Official/PokeByte/blob/main/PHASE4-GAMEPLAY-PLAN.md. Work on Phase 4 Pack 01 Adventure Mode, Windows first. Do not touch the original firmware or approved art."**
2. Upload the privately retained **PokéByte Continuity Archive ZIP**, or at minimum the approved Phase 3 Windows ZIP, the backup ZIP, and the previous-chat export.
3. Verify the ZIP and [asset hashes](APPROVED-ASSETS.md) before providing a new Windows build. Source-only CI art is **not** enough. Never invent a local download link.
4. Check the GitHub roadmap and most recent PR; don't assume a new conversation has access to the prior session or uploaded binary files.
5. Develop on a separate Phase 4 branch, verify core gameplay and original PMD fidelity, provide a Windows preview and await the user's explicit playtest approval.

## History coverage and privacy

This public GitHub document captures project decisions and milestones, **not private ChatGPT conversation transcripts**. The user supplied a pasted text export of the **early** original conversation (`Pasted text(20261024-192835).txt`, 843 lines, SHA-256 `7b7f1addce79040a996951e0dbe660f9eaa94b26b0d9037700837c1a2554654d`). The file doesn't cover all later messages; later state was reconstructed from the current chat and GitHub evidence. The original transcript and user uploads belong in the user's **privately stored archive**, not in a public repository.

**Authority order when reconstructing:** explicit recent user approval + verified accepted Windows ZIP and backup hashes → GitHub source/review and passing test workflows → historical exported chat for feature intention → assistant proposals (not automatically approved).
