# PokéByte Advanced — import, provenance, build and migration record

**Owner request (25 October 2026):** Preserve and use Cheng Ren's advanced TamaPoke v3.14 fork under the PokéByte brand because the original creator may close their hosted site. The owner already tested upstream firmware on their exact Waveshare.

## Complete source archives (verified and created)

- Upstream author: **Cheng Ren** — [crenberries428/TamaPoke](https://github.com/crenberries428/TamaPoke), forked from [socquique/TamaPoke](https://github.com/socquique/TamaPoke) and incorporating [DylanPDao/TamaPoke](https://github.com/DylanPDao/TamaPoke).
- Version: **3.14**; exact upstream Git commit: `289a456feb48b19bbc974809397216431c890468`.
- Unmodified original preserved at `archive/crenberries428-tamapoke-v3.14-2026-10-25`. The exact imported Git history, **all tracked source files, original prebuilt firmware and regional sprite packs** remain on this archive branch. The archive does not depend on the original live website.
- Our previous working hardware release preserved at `archive/pokebyte-original-phase4-2026-10-25`.
- Current rebrand code at `pokebyte-crenberries-v3.14-import`.
- License `LICENSE` and detailed `CREDITS.md` preserved unmodified. Credit must remain with original authors. Sprite/data rights differ from code license.

## Rebranding boundaries

- Display labels, user-facing HTML, serial welcome line, boot splash/About title and installer manifest are now **PokéByte**.
- **Do NOT rename the Arduino source `TamaPoke.ino` nor firmware NVS namespace `tamapoke`**. The sketch name and persisted key are technical compatibility interfaces, and renaming the key would break saved settings/data.
- Rebrand does not modify the underlying gameplay, battle logic, original sprites, gyms, music, regional files or user saves.
- New installs without a saved volume setting default to **20% sound amplitude** (the previous PokéByte owner preferred this). The advanced edition still provides an in-game 0–100 volume slider; previously saved preferences are respected.

## Release safety (must pass before replacing the production installer)

1. A new **native ESP32-S3 branded firmware build** must succeed. Original upstream `web/firmware/*.bin` are retained for provenance but **must never be passed off as the freshly rebranded build**. See `.github/workflows/pokebyte-advanced-candidate.yml`.
2. The installer should flash **four separate parts**, not the merged image. Offsets are 0, 32768, 57344 and 65536. **Do not write into NVS** beginning at 0x9000. `new_install_prompt_erase:true` must remain true so users can decline destructive erase.
3. Use the exact Waveshare **ESP32-S3-Touch-AMOLED-1.75**, not `1.75C`.
4. The existing PokéByte Phase 4 NVS save and advanced fork v3.14 save formats have not been confirmed interoperable. **Do not guarantee save preservation across this replacement**, regardless of safe flash offset. Ask users to back up important progress before switching.
5. Verify that the public Pages installer actually loads the freshly compiled code, labels, sound and sprite packs after deployment. Ensure the original contributor credits remain visible.
6. Keep the original release and upstream archives available. Do not force-update a working hardware unit until the new branded edition has passed an actual hardware smoke test and the owner explicitly accepts it.

**Importing the source is complete; production release promotion remains subject to firmware build and device validation.**
