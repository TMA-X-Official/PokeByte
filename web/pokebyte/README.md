# PokéByte browser installer — SAFE PREVIEW

**State: design prototype on Phase 4 development branch. Not hosted, not released, not flashable.**

The user wants a dedicated PokéByte installation website similar to TamaPoke's browser installer. This directory is the separately branded **PokéByte-only** landing page. It is a safe, static preview, **not** a copy of TamaPoke's active firmware installer.

### What exists now

- `index.html`: standalone responsive installer-preview webpage with a clearly **disabled install button**.
- No ESP Web Tools JavaScript import, no `<esp-web-install-button>`, no Web Serial/USB calls, no firmware manifest, no firmware .bin file and no sprite-transfer button.
- No Adventure Mode or classic battle feature. Only approved Windows Phases 1–3 are listed.
- Original upstream-compatible `web/index.html` and `web/manifest.json` are **left untouched**; they still point to **original TamaPoke firmware 1.17** and must **never** be presented as a PokéByte update.

### Brand image

The uploaded PokéByte logo is represented by an optimized thumbnail at `assets/pokebyte-logo.webp`. The original full-resolution PNG remains the design source; the web thumbnail is deliberately smaller for fast display. Never generate or substitute fake Pokémon art.

### Preview on your PC

```bash
python3 -m http.server 8000
# open http://localhost:8000/web/pokebyte/ in a web browser
```

The page is also viewable by opening `index.html` directly. Neither route accesses devices or uses remote scripts. The GitHub branch is not configured as a public live installer automatically; any Pages deployment requires explicit approval.

### Release requirements — all must be satisfied before enabling installation

1. Finish the embedded hardware port of **all** accepted Phase 1 (five distinct backgrounds), Phase 2 (ambience and optional weather, no animated cloud overlays) and Phase 3 (temperaments, Bond and distinct surprises). The current approved Windows feature source has `POKEBYTE_PC`-only code, and compiling the old firmware is insufficient.
2. Confirm the device model **ESP32-S3-Touch-AMOLED-1.75**, not the **1.75C** variant. Use matching flash size, PSRAM and partition layout. Review actual device hardware.
3. Build a *genuine* Waveshare ESP32-S3 firmware from this approved source without the rejected Adventure/Battle changes and test on device: display/touch, power, audio, RTC, SD, scenes, 303 original PMD assets, all mini-games, saves and every personality behavior. Do not call the existing original v1.17 `web/firmware/tamapoke.bin` a PokéByte upgrade.
4. Produce and verify full flash + NVS + partition table backup AND microSD backup, with a tested restore/rollback path. **No user SD formatting, no automatic erase, no partition layout changes that endanger saved Pokémon.**
5. Stage and verify exact 12 approved BMPs and the 303 .bin sprite/thumbnail assets against the user-approved Phase 3 ZIP and [APPROVED-ASSETS.md](https://github.com/TMA-X-Official/PokeByte/blob/main/APPROVED-ASSETS.md). The current `/pokebyte` art path is staging only until firmware reads it.
6. Then add an **ESP Web Tools** element wired to an actual manifest and explicitly verified firmware files, with correct ESP32-S3 chip family, offsets, and checksum/release provenance. Require Chrome/Edge desktop over HTTPS or localhost. Reject unsupported hardware; do not automatically choose an erase-all option.
7. Design and validate separately the Web Serial microSD asset transfer, with file hash checks and safe handling of interruptions. Do **not** silently auto-format a card upon mount failure or use a stale resume cache that might skip unverified files.
8. Preview the now-active installer with real hardware using a test board and a backed-up device; publish as a separate GitHub Pages page/release only after the user's explicit approval. Enable the install button in a **separate reviewed code change**; don't remove the guard by default.

**Web architecture reference:** The existing upstream TamaPoke `web/` uses ESP Web Tools and Web Serial. Current vendor docs: https://esphome.github.io/esp-web-tools/ . The first PokéByte preview intentionally ships *no* executable installer.

**Phase 4 handoff:** See [PHASE4-WAVESHARE-RELEASE.md](https://github.com/TMA-X-Official/PokeByte/blob/main/PHASE4-WAVESHARE-RELEASE.md). This site does not overwrite existing firmware, user saves or device files.
