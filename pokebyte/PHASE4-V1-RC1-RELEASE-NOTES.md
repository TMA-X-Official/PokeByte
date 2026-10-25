# PokéByte v1.0 RC1 — DRAFT release notes (Waveshare)

**STATUS: RELEASE-CANDIDATE SOURCE PREPARATION ONLY.** Not an approved public stable firmware or tagged GitHub Release. Latest first-install sprite-packaging CI and separate physical QA are still pending. Existing [beta website](https://tma-x-official.github.io/PokeByte/beta/) remains experimental.

## What's in this candidate
- Real PokéByte port for the round **Waveshare ESP32-S3-Touch-AMOLED-1.75**, 466×466 AMOLED, 16MB ESP32-S3 flash and 8MB PSRAM.
- User-approved **five time-of-day home backgrounds and corresponding minigame scenery**, verified exact originals without the historical grey-square CI fixtures.
- Phase 2 gentle original-scene ambience (balanced grass/pollen/fireflies); optional rain/snow; **no animated clouds**.
- Original Pokémon sprites/action frames, temperament/Bond and all four approved one-shot surprises (wake, victory, wander, friendship).
- Existing original care, food, sleep and two minigames. **Adventure and Battle experiments are intentionally excluded** per owner request.
- AMOLED no longer automatically dims after inactivity; manual PWR-off remains. **Latest owner-requested audio revision:** sound effects now use **20% of the original PCM sample amplitude** (1000 instead of 5000; approximately −14 dB relative to the original), reduced from the previous 50% build. This revision **requires a new firmware installation and has not yet been physically volume-tested by the owner**. High-contrast battery UI has a more compact **112px** plaque and clear percentage.
- Intended one-click browser installation with no Python, Arduino IDE, manual ZIP download or microSD adapter, using HTTPS ESP Web Tools + Web Serial.
- Integrated installer verifies and transfers only **missing** files into `/pokebyte` and `/mons`, after checking exact approved **12 BMP scenes and 303 original PMD sprite files**. Existing different files must never be overwritten. The 303-file first-install feature is freshly coded and **not yet physically validated**.

## Supported / unsupported
- **Supported beta hardware:** ESP32-S3-Touch-AMOLED-1.75 (not 1.75C).
- **Supported browsers:** desktop Chrome and Edge with USB/Web Serial, over HTTPS.
- **Cards:** microSD needed for original sprites and scenery; a fresh microSD setup is being implemented. **Not yet tested with a blank card.** Existing original files should be preserved.
- **Saved games:** existing user save upgrade/backups are **not yet production validated**. The owner waived old TamaPoke internal save backup for **their own** test only. This waiver does not apply to a public release.
- **Noncommercial unofficial fan project.** PMD SpriteCollab animation artwork subject to credit/CC BY-NC 4.0, characters belong to respective holders; review [credits](https://github.com/TMA-X-Official/PokeByte/blob/main/CREDITS.md).

## Before promoting this candidate to v1.0 stable
Complete the [v1.0 release-readiness checklist](PHASE4-V1-RELEASE-READINESS.md): green CI/Pages deployment on final source SHA, original asset packaging verification, **fresh-card first install**, interrupted-transfer recovery, comprehensive hardware regression (all scenes, minigames, surprises, touch/RTC, power, sound, battery, reboot), and a **save-preserving upgrade/restore flow for other users**. Review/merge release branch safely (Phase 4 branch diverged from `main`), then publish versioned firmware checksums and a reviewed single-button installer.

**No stable v1.0 tag, GitHub Release, or merge has been performed.**
