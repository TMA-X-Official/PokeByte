# PokéByte experimental Waveshare USB test

**Explicitly owner-opt-in beta. Not a stable release.** The owner requested to test actual PokéByte, declines to retain their old TamaPoke internal firmware/save, and wants the existing physical microSD sprite files preserved.

## What the beta does

The GitHub workflow [pokebyte-beta-installer.yml](../../../.github/workflows/pokebyte-beta-installer.yml) builds the exact repo ESP32-S3 source (16 MB flash, OPI PSRAM), checks the partition table and image boundaries, and packages an ESP Web Tools page at **/beta/** that uses targeted flash parts at:
- 0x000000 — bootloader
- 0x008000 — partition table
- 0x00e000 — OTA boot selection
- 0x010000 — app firmware

**No part is written at the internal NVS 0x9000–0xdfff, existing FFat storage or the physical microSD.** A complete 16MB merged image is NOT flashed or published. A targeted flash can nevertheless still cause loss of your old firmware/save or fail to boot. Hardware has not yet been tested.

**Single integrated installer (October 2026 user requirement):** The public beta page now contains **one installation flow**: website pre-verifies the genuine hosted `approved-backgrounds.zip` (all 12 SHA-256s, CRC32s, bytes), unlocks the main `Install PokéByte` button only when the firmware and artwork are both ready, opens ESP Web Tools for the firmware, and after the ESP Web Tools dialog closes automatically reconnects to firmware via the previously-authorized USB port when possible. The safe `PBCAPS/PBCRC/PBPUT` protocol verifies and transfers only **missing** BMPs to `/pokebyte/`; existing approved BMPs are skipped and **different existing files are never overwritten**. There is **no separate background-upload form or user ZIP picker**. If the browser cannot reuse serial permission, a `Continue PokéByte installation` button appears **inside the same installation progress panel**; user interaction is necessary to show the Web Serial device picker, not a second product installer. **The original Pokémon sprite files in `/mons/` are never modified**. The firmware's shorter battery plaque is now 112 px wide (previously 145 px), while keeping the contrast and 2× battery percentage.

**Required one-time owner upload BEFORE this installer can begin:** `web/pokebyte/approved-backgrounds.zip` must exist in the Phase 4 development branch. The website **locks the main Install button if the hosted artwork is missing or fails any checksum**—it does NOT flash only half of PokéByte. The 4.7 MiB ZIP exists in the owner's conversation files, but the current GitHub integration can write text and cannot directly upload these binary files. The owner must upload the exact verified `approved-backgrounds.zip` once, then GitHub Actions automatically publishes it with the website. This is **not an end-user download or repeated step**. Full ZIP SHA-256 is `5f72992ca1c93cbc7e16e42624ba9fdd449e0173ddd369fd7a4904e68db12c81`.

## GitHub Pages deployment permission (failure diagnosed 25 October 2026)

The beta firmware build passed in [workflow #40142288944](https://github.com/TMA-X-Official/PokeByte/actions/runs/40142288944), but GitHub Pages rejected the deployment with:

`Branch "pokebyte-phase4-waveshare-release-prep" is not allowed to deploy to github-pages due to environment protection rules.`

This is **not a firmware build or ESP32 issue**, and 404 is expected until the Pages deployment succeeds. The repository variable `POKEBYTE_BETA_PAGES_ENABLED=true` and the Pages build source `GitHub Actions` are **not sufficient alone**.

Fix as repository owner (no new commits or firmware uploads needed):

1. Open [Settings → Environments](https://github.com/TMA-X-Official/PokeByte/settings/environments) → `github-pages`.
2. Under **Deployment branches and tags**, choose **Selected branches and tags** and **Add deployment branch or tag rule**.
3. Choose **Branch** (not Tag). Enter the exact branch name `pokebyte-phase4-waveshare-release-prep` (no `refs/heads/` prefix), then **Add rule**. Keep the existing `main` branch rule if present.
4. Save any protection changes. Return to [the failed workflow run](https://github.com/TMA-X-Official/PokeByte/actions/runs/40142288944) and choose **Re-run failed jobs** (or re-run the failed deployment job).
5. Only after **both package and deployment jobs** succeed, check the expected website `https://tma-x-official.github.io/PokeByte/beta/`. **Do not flash the board based on a 404 or a partial workflow success.**

GitHub official guidance: https://docs.github.com/en/actions/how-tos/deploy/configure-and-manage-deployments/manage-environments

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

## Approved backgrounds — website install, no card reader (25 October 2026)

The owner reported that the native firmware boots and touch/sprites function, but backgrounds fell back to original TamaPoke. They also requested **no automatic display dimming**, a **50% reduction in synthesized sound waveform amplitude**, and a **high-contrast battery icon plus larger, readable percentage**. These changes are in the native source, with a separate periodic save flush to preserve save reliability while the display stays bright. Manual PWR screen-off still functions.

The beta website now includes the on-page **Install approved backgrounds via USB** button and a checksum-verified uploader. Both scenarios are supported:

**Immediately, for the owner:** On the HTTPS beta website, pick your exact user-approved `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip` in the file selector (the browser verifies its entire ZIP SHA-256 and all twelve BMP SHA-256s/CRC32s, without extracting it to disk). Then click **Install approved backgrounds via USB**, choose Waveshare COM6, and let the browser send missing files to the inserted microSD. It does **not** need an adapter, Python or Arduino IDE. This is a *separate step* from flashing firmware in the current beta.

**For future automatic hosted download (one-time repo-owner publication):**
1. Obtain the verified `approved-backgrounds.zip` bundle containing exactly `assets/meadow_*.bmp`, `assets/minigame_*.bmp`, `assets/minigame.bmp`, `assets/sunset-forest.bmp`. The exact 12 original BMPs and their hashes are pinned in `APPROVED-ASSETS.md`. The owner's archive was packed byte-for-byte from the original approved Phase 3 Windows ZIP. Its ZIP SHA-256 is `5f72992ca1c93cbc7e16e42624ba9fdd449e0173ddd369fd7a4904e68db12c81`.
2. While browsing the **`pokebyte-phase4-waveshare-release-prep`** branch, open [`web/pokebyte/`](https://github.com/TMA-X-Official/PokeByte/tree/pokebyte-phase4-waveshare-release-prep/web/pokebyte) on GitHub, click **Add file → Upload files**, and upload it **with the exact filename `approved-backgrounds.zip`** (not `approved-backgrounds.zip.zip`). Commit to the *development branch*; do not merge into main.
3. The installer GitHub Action automatically inspects every BMP SHA-256/CRC32/byte count against the canonical manifest. It **fails closed** if the pack contains test fixtures or other artwork; otherwise it publishes `/beta/approved-backgrounds.zip` on GitHub Pages. Future visitors then need **no manual ZIP or SD reader**—they just use the website uploader.
4. No approved asset upload occurs merely by viewing the page. A dedicated USB permission prompt remains necessary. The original `/mons` sprite files are never overwritten. A current card with a *different existing* `/pokebyte` file is left untouched, and the installer reports it for diagnosis instead of erasing it.
5. Once the owner confirms all 12 backgrounds and the updated firmware on actual Waveshare, evaluate combining firmware flash and scene sync into a single guided Install flow for the public release.

**Important:** The current private owner **willingly accepts internal TamaPoke save loss**. That decision does not authorize wiping the microSD or a public installer that erases other users' saves. The new no-auto-dim display can use more battery power and may increase AMOLED burn-in; manual PWR off remains available.

## Publishing the owner-opted-in beta

The repository owner enabled GitHub Pages (GitHub Actions source) and reports setting the repository Actions variable `POKEBYTE_BETA_PAGES_ENABLED=true`. This documentation update intentionally triggers a **fresh** beta workflow so the new run reads that variable. Check the entire job result, including the Pages deployment, before sharing the site URL or attempting any flash. No microSD write, microSD format, or consumer-stable release is authorized by publication alone.
