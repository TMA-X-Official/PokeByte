# PokéByte Advanced

**Advanced virtual-pet firmware for the actual round Waveshare ESP32-S3-Touch-AMOLED-1.75 (466×466).**

PokéByte Advanced is a **clearly attributed rebrand and adaptation of Cheng Ren's TamaPoke v3.14**. The owner previously tested Cheng Ren's advanced TamaPoke on hardware, prefers its richer functionality, and asked to preserve it before its original website is retired.

> **Creator credits are mandatory:** This project builds on [Cheng Ren's crenberries428/TamaPoke](https://github.com/crenberries428/TamaPoke) (v3.14, 2026-10-05), which is based on [Quique Tortosa's original TamaPoke](https://github.com/socquique/TamaPoke) with contributions from [Dylan Dao's fork](https://github.com/DylanPDao/TamaPoke). All original MIT notices remain in [LICENSE](LICENSE), and the original [CREDITS.md](CREDITS.md) is preserved. **PokéByte branding does not mean those game systems were written from scratch by PokéByte.**

## Features from the advanced upstream edition

- Expanded Pokémon regions and original PMD animated Pokémon sprites, with shiny variants.
- Move-based battles, type matchups, gym challenges, trainer progression and badges.
- Pokédex, party/box collection, happiness/care, eggs, training, daily goals and music.
- RTC day/night environment, steps, sounds, adjustable volume and brightness, multilingual menus.
- HTTPS ESP Web Tools installer and regional sprite packs transferable over USB/Web Serial.

For the original full gameplay documentation and screenshots, see the exact [v3.14 source archive](https://github.com/TMA-X-Official/PokeByte/blob/archive/crenberries428-tamapoke-v3.14-2026-10-25/README.md).

## Hardware and installation

**Exact supported device:** Waveshare **ESP32-S3-Touch-AMOLED-1.75** round AMOLED (not 1.75C). Windows or another desktop with Chrome/Edge, a USB-C data cable, and a microSD card for the regional sprites.

The inherited browser installer sources are in [web/](web/). It flashes firmware using a four-part manifest and then sends regional sprite files to microSD via USB. This is the upstream workflow that the owner already tested.

**RELEASE GATE:** The source repository contains **historical precompiled upstream v3.14 firmware binaries**, not the newly PokéByte-branded firmware. The PokéByte-branded installer must be **rebuilt and verified in GitHub Actions** before public replacement. Use the linked build artifact/PR; do not flash a historic `tamapoke.bin` labelled as PokéByte. [Read the full migration safety and build plan](docs/POKEBYTE-ADVANCED-IMPORT.md).

**Existing Pokémon saves:** Never tick **Erase device** when trying to preserve an existing save. The old PokéByte Phase 4 firmware and this advanced fork might store saves differently, even if both use the `tamapoke` NVS namespace. The installer cannot guarantee data migration between them. Preserve a rollback path and do not overwrite a valuable save until tested.

## Source backups — both safe on your own GitHub

- **Exact upstream source, commit and all original packs:** [archive/crenberries428-tamapoke-v3.14-2026-10-25](https://github.com/TMA-X-Official/PokeByte/tree/archive/crenberries428-tamapoke-v3.14-2026-10-25) — the unmodified upstream snapshot and its history.
- **Our previously working PokéByte Phase 4:** [archive/pokebyte-original-phase4-2026-10-25](https://github.com/TMA-X-Official/PokeByte/tree/archive/pokebyte-original-phase4-2026-10-25).
- **New branded adaptation:** [pokebyte-crenberries-v3.14-import](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-crenberries-v3.14-import).

The firmware source is deliberately still named `TamaPoke.ino` so Arduino sketches build correctly. The internal NVS storage key `tamapoke` is retained for backward-compatibility; these are **not** public-facing project names.

## Original Pokémon sprite assets

The large `web/sprites-*.pak` archives in this fork include PMD SpriteCollab graphics licensed **CC BY-NC 4.0**, with Pokémon-related characters/names belonging to their respective owners. Personal, non-commercial fan use only. The code's MIT license **does not grant commercial permission for the included sprites, music or character artwork**.

## Attribution and licenses

MIT software license: [LICENSE](LICENSE). Authors and third-party components: [CREDITS.md](CREDITS.md). Case design is separately CC BY-NC-SA as described by its original authors. Not affiliated with or endorsed by Nintendo, Game Freak or The Pokémon Company.
