# PokéByte Phase 2 — Ambient Pack 02 (Windows Test)

**Status:** Developer preview / human playtest required. **Do not merge with Phase 1 without approval.**

## What changed compared with Ambient Pack 01

- **Proper animated pixel-art clouds:** Two separate multi-lobed cloud sprites, about **88 and 118 pixels wide**, glide slowly across the sky at different speeds. Each has a highlighted crown, cloudy body and soft shaded underside. Their colors adapt to the active Morning / Day / Afternoon / Evening / Night background.
- **Distinct pollen motes:** Four occasional tiny dots float close to the grass. They are explicitly pollen, **not clouds**.
- **Grass sway:** A handful of narrow tufts near scene edges move by only a few pixels; the main grassy ground remains the original bitmap.
- **Evening/night fireflies:** Small golden lights blink, drift slowly and remain mostly near the field edges.
- **Optional drizzle with splashes:** Fine diagonal streaks and tiny near-ground splashes; no effects on gameplay, physics or Pokémon care.
- **Balanced remains the default.** All ambient effects can be disabled, and rain is off until requested.

## Preserved without edits

Five **user-approved** home backgrounds (`assets/meadow_*.bmp`), five matching full-height minigame backgrounds (`assets/minigame_*.bmp`), the two approved fallback BMPs, original TamaPoke Pokémon PMD files, touch and swipe UI, controls, ball physics, training mini-game and saves. The downloadable Windows playtest ZIP copies the exact previously approved BMP bytes and all 303 Pokémon files.

## Controls and local time

- **PLAY-POKEBYTE.bat:** Automatically changes backgrounds based on the PC local clock, even while the game remains open.
- **PLAY-MORNING / PLAY-DAY / PLAY-AFTERNOON / PLAY-EVENING / PLAY-NIGHT:** Locks the selected scene while testing.
- **B:** Classic/remastered background toggle.
- **A:** Ambient effects On/Off.
- **W:** Rain On/Off.
- Windows **PC** battery label is intentionally not a fabricated percentage. Real AXP2101 battery percentage support is source code only pending future hardware testing.

## Verification

Windows CI validates startup, five distinct scene-mode renderings, ball and training minigames, and the original game executable. **The user must test cloud movement and visual blending on Windows.** The GitHub CI scene BMPs are synthetic fixtures only. The final user ZIP is repackaged with exact previously approved Phase 1 artwork.

**Scope:** Desktop (POKEBYTE_PC) only. Hardware flashing belongs to Phase 5.

Original project: TamaPoke by socquique, MIT. Pokémon sprites: PMD SpriteCollab, CC BY-NC. Unofficial noncommercial fan work.
