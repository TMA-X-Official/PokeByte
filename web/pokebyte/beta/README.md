# PokéByte experimental Waveshare USB test

**Explicitly owner-opt-in beta. Not a stable release.** The owner requested to test actual PokéByte, declines to retain their old TamaPoke internal firmware/save, and wants the existing physical microSD sprite files preserved.

## What the beta does

The GitHub workflow [pokebyte-beta-installer.yml](../../../.github/workflows/pokebyte-beta-installer.yml) builds the exact repo ESP32-S3 source (16 MB flash, OPI PSRAM), checks the partition table and image boundaries, and packages an ESP Web Tools page at **/beta/** that uses targeted flash parts at:
- 0x000000 — bootloader
- 0x008000 — partition table
- 0x00e000 — OTA boot selection
- 0x010000 — app firmware

**No part is written at the internal NVS 0x9000–0xdfff, existing FFat storage or the physical microSD.** A complete 16MB merged image is NOT flashed or published. A targeted flash can nevertheless still cause loss of your old firmware/save or fail to boot. Hardware has not yet been tested.

**The first beta does not copy sprites or background BMPs to microSD.** It uses the /mons files already on the card, and shows the fallback original background if /pokebyte approved BMPs are not present yet. The beta does not require a microSD reader. Existing files on the inserted card must not be formatted or overwritten; current SD mount uses autoformat=false.

## One-time project-owner GitHub Pages setup

This setup is needed only by the **repository owner**, not ordinary visitors.

1. In GitHub, open **TMA-X-Official/PokeByte → Settings → Pages → Build and deployment → Source: GitHub Actions**.
2. Open **Settings → Secrets and variables → Actions → Variables → New repository variable**. Enter name **POKEBYTE_BETA_PAGES_ENABLED**, value **true**.
3. Trigger the **PokéByte Phase 4 experimental browser installer bundle** workflow by pushing another commit to this feature branch or rerunning a suitably fresh build after enabling the variable (ask in chat to trigger if preferred). Wait for green compilation, manifest verification and Pages deploy steps.
4. The beta page should then be at **https://tma-x-official.github.io/PokeByte/beta/**. **Do not assume it exists until the deployment succeeds**. The public production installer at the Pages root remains locked.
5. In desktop Chrome/Edge, visit beta with your Waveshare connected on COM6. Confirm *exactly* ESP32-S3-Touch-AMOLED-1.75, accept the beta data-risk box and click **Test-install PokéByte**. If offered an **Erase device** choice, select **do not erase all flash**. Watch the device reboot. Do not run any sprite upload, and do not click TamaPoke's original Install page.
6. Report screen appearance, whether sprites load from microSD, touch/care responses, any USB errors and serial `PBSTATUS` output. Test more modes/surprises only after the initial hardware can boot safely.

**Never publish the beta as an ordinary consumer release.** A chip-family check identifies ESP32-S3 but cannot verify a particular Waveshare display variant. Initial boot failure may require reentering the ESP32 ROM bootloader; do not promise effortless rollback without a tested recovery route.

For the final public one-click installer, original game save backups and vetted automatic BMP asset transfer remain separate requirements. The beta is specifically for an owner who opted to accept internal data loss, and their preferences must not be generalized to other users.

## Publishing the owner-opted-in beta

The repository owner enabled GitHub Pages (GitHub Actions source) and reports setting the repository Actions variable `POKEBYTE_BETA_PAGES_ENABLED=true`. This documentation update intentionally triggers a **fresh** beta workflow so the new run reads that variable. Check the entire job result, including the Pages deployment, before sharing the site URL or attempting any flash. No microSD write, microSD format, or consumer-stable release is authorized by publication alone.
