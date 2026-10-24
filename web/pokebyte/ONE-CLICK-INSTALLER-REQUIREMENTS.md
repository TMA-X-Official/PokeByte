# PokéByte — Browser-only, One-click USB Installer (Phase 4)

**User requirement (25 October 2026):** A visitor must install **their approved PokéByte version**, as easily as the original TamaPoke page. **NO Python, Arduino IDE, command line, manually downloaded firmware ZIP/.bin, separate sprite package, desktop driver installer unless the OS genuinely requires one, or microSD reader.** Visitor opens a hosted HTTPS page in compatible desktop browser, connects Waveshare by USB-C data cable and clicks **Install PokéByte**. Browser device chooser and safety confirmations are unavoidable, but the visitor must not need to manage installation files.

**Important status:** This is the **acceptance contract**, not a released implementation. Current page intentionally shows a **disabled** Install button. The accepted Windows Phase 1–3 features remain partly PC-only and have not been hardware-tested. The original `web/manifest.json` points to **original TamaPoke**, not PokéByte, and must not be reused. No user firmware/device writes are permitted until the gates below pass.

## Finished visitor experience

1. Open the dedicated **PokéByte** HTTPS installer (distinct from original TamaPoke page) in Chrome or Edge desktop.
2. Plug in a verified **Waveshare ESP32-S3-Touch-AMOLED-1.75**; click **Install PokéByte** and approve the browser's USB port chooser. The installer must not assume an arbitrary ESP32-S3 is necessarily this Waveshare board; validate the actual variant using a safe device handshake and/or explicit user verification.
3. Behind the scenes, the page checks firmware compatibility, flash map, save/NVS status, and existing card assets. **On an existing TamaPoke device, complete the necessary browser-guided read-only flash backup first** and provide a safe way to retain the backup privately; halt if it fails. The browser may have to save a private backup file, but the visitor should never have to manually locate, download, extract or install a firmware package. The original microSD may have unique files; never format/erase it.
4. The browser automatically fetches the **verified PokéByte** firmware over HTTPS and writes it through **ESP Web Tools/Web Serial or an equivalently audited browser flasher**, using only documented, tested offsets that do **not** overwrite a saved Pokémon's NVS. **ESP Web Tools' default new-install erase-all behavior is unacceptable for existing devices**; explicitly implement safe fresh-vs-upgrade handling and require recovery confirmation. Do not silently erase, repartition or use the upstream TamaPoke firmware.
5. After reboot the same page reconnects (a second browser device permission may be required) and checks the on-card **303 original Pokémon BIN/thumbnail files and 12 approved scene BMPs** by SHA-256, then automatically uploads only missing/incorrect approved assets through Web Serial. A user doesn't need an SD reader; don't repeat a 303-file upload needlessly.
6. Complete the setup, verify Pokémon and the approved home screen, display **Installation successful**. If anything fails, keep original backup and guide an audited rollback instead of formatting a card.

## Technical deliverables

- **Embedded firmware:** Port all user-approved Phase 1 backgrounds/dayparts, Phase 2 ambience, Phase 3 personality/rare surprises and both original minigames to actual ESP32-S3. Preserve original `Pet` save layout, care, RTC, touch, audio, power management and 151 original species. Exclude **rejected Adventure and battle systems**.
- **Verified release artifact:** Build a genuine bootable ESP32-S3 firmware from the accepted code; use a uniquely named **PokéByte**, source-commit-pinned, checksummed artifact. A `manifest.json` for exactly the supported chip family should only exist after real hardware tests. The binary must not be a relabeled `web/firmware/tamapoke.bin`.
- **Web installer:** Host under HTTPS (e.g. a GitHub Pages subpath), import a pinned ESP Web Tools release only once the verified firmware exists, build a valid `ESP32-S3` manifest and do not rely on the **chip family alone** to distinguish Waveshare submodels. Browser script should sequence backup → firmware → approved assets → verification with progress/error recovery.
- **Safe existing-device backup:** Official Espressif [esptool-js](https://espressif.github.io/esptool-js/docs/classes/ESPLoader.html) implements `readFlash(addr, size)` for Web Serial. Prototype a checked 16 MiB full-flash backup entirely in the browser, validate byte count/SHA-256, allow the user to keep the resulting backup privately and verify restore on a test unit; never publish a user's saved Pokémon. Full flash is a **separate** concern from microSD backup.
- **USB SD asset sync (native groundwork now implemented):** The original TamaPoke `PUT` is legacy and destructive. The new hardware branch adds **`PBCAPS`**, read-only **`PBCRC /mons/...` or `/pokebyte/...`** reporting size+CRC32, and **`PBPUT <approved-path> <size> <crc32hex>`**. `PBPUT` only creates *missing* approved paths (12 exact BMP names or original PMD `.bin` names), checks CRC32 and expected BMP size, writes to a `.pbpart` temporary file, and renames it only upon successful validation. **It never deletes or overwrites an existing original file.** It still needs hardware communication, interrupt/recovery testing, and a matching audited browser asset-transfer client. It has **no full microSD readback/GET** and is **not** a complete card backup; preserve unique card files and implement a separate full-card recovery method before allowing automatic card changes. The user with no card reader is a first-class supported case.
- **Hosted assets:** Publish the validated approved 12 BMP + original 303 PMD BINs as a hosted content-addressed bundle with credit/licensing and a manifest. All files must match [APPROVED-ASSETS.md](https://github.com/TMA-X-Official/PokeByte/blob/main/APPROVED-ASSETS.md). GitHub Actions synthetic BMP backgrounds are unacceptable.
- **Quality gate:** Full device QA (RTC sleep/wake, SD, original all-weather scenes, Bond/temperaments and all four surprise sequences, existing minigames, power/audio/touch, save preservation and reboot/rollback). No flash on user's existing hardware before verifying backups, model and restore plan.
- **Product release:** Unlock the one-click Install button only after the real release manifest is present and checksummed, hardware acceptance and owner approval. Disable flashing again if release checks fail.

## What the visitor does NOT have to do

Install Python, run Arduino IDE, download firmware .bin/ZIP, pick sprite filenames, copy files into SD manually, use a card adapter, change flash partitions, or manually load Pokémon sprites. A browser security prompt and preserving a **private backup file** for existing devices may still be necessary; promising zero confirmation prompts or preserving NVS without verification would be inaccurate.

## Current implementation / protection

- `web/pokebyte/index.html`: branded **locked visual preview**, now expressly describes the one-click workflow.
- `tools/test_pokebyte_installer_preview.py`: asserts no live flash button, upstream TamaPoke manifest, firmware .bin path or scripts are present in the preview.
- `.github/workflows/pokebyte-installer-preview.yml`: CI guards.
- `tools/phase4_prepare_sd.py`: non-destructive preparation of verified approved artwork for engineering only. **It is not a visitor step**.
- `sdmon.cpp`: separate, non-overwriting CRC-gated `PBCAPS/PBCRC/PBPUT` serial protocol for approved game assets; **native implementation only; no browser integration or on-device test yet**.
- `tools/phase4_waveshare_backup.py`: a PC read-only preflight helper for engineering, not a required visitor installation method once the browser-only flow is ready.

**No end-user-ready one-click installer exists yet. Do not relabel the original TamaPoke one as a PokéByte release.**
