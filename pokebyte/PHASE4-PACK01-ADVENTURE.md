# PokéByte Phase 4 — Pack 01 Forest Trail (Windows development preview)

**Status:** Draft / awaiting user playtest. Do not merge, flash Waveshare hardware or modify the approved Phase 3 baseline.

**First loop:** From the regular Pokémon home screen, use the small **ADVENTURE** button near the right edge, or press **G**. An additional Forest Trail screen opens without modifying the existing home/background assets. Press **WALK** to stroll down the path; after about 1.7 seconds the Pokémon discovers a **quiet stretch**, **berry sighting**, or **wild Pokémon sighting**. Tap **AGAIN** to explore again or **HOME** to return. You can always select HOME, including during a walk. Normal status, care, egg and evolution logic remain unchanged.

Three deterministic **no-save** Windows launchers test different event results: `TEST-ADVENTURE-QUIET.bat`, `TEST-ADVENTURE-ITEM.bat`, and `TEST-ADVENTURE-WILD.bat`. All show an in-memory Charmander, do not load `tamapoke_pc_save.dat`, and use existing real original PMD sprite data. Event results are a *preview only*; no berries go into an inventory yet, no Pokédex flags change, and no battles/rewards/odds are implemented. The wild sprite is loaded separately from unchanged original PMD binaries. Species with unavailable optional clips fall back to Idle.

**Hard constraints:** Preserve the exact 12 user-approved BMP scene files and 303 original PMD binaries, all existing minigames, five day-parts, all personalities/surprises and the existing save schema. GitHub Actions creates synthetic scene fixtures for CI only: the user-facing Windows ZIP **must** restore original art from the verified approved Phase 3 ZIP and check SHA-256 before release. Phase 5 is for physical hardware.
