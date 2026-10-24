# PokéByte Phase 4 — first Waveshare USB connection (Windows)

**READ-ONLY PREPARATION. Do not flash PokéByte yet.** The current Waveshare development firmware has not been proven to include all approved Windows Phase 1–3 features. A green Arduino compile is not a release.

## Supported target

The intended device is the **round Waveshare ESP32-S3-Touch-AMOLED-1.75 (466 × 466 pixels; 16 MB flash; 8 MB PSRAM)**, not the separately sold 1.75C board. Before connecting, check the printed model/SKU. The existing working TamaPoke installation and its stored Pokémon must be preserved.

## Step 1 — protect the existing microSD (adapter optional for now)

**User hardware limitation:** No PC microSD adapter is available; the installed card contains the original Pokémon sprite files. **You do not need to buy an adapter before the read-only USB flash-backup step.**

- **Leave the existing microSD in the device and do not modify it.** Never accept a format prompt or click TamaPoke's original **Load sprites** button during the hardware preflight.
- The private **approved Phase 3 Windows ZIP** and the verified **PokéByte recovery archive** already include **303 original `.bin` sprite/thumbnail files** and checksums. They can restore the **known approved original sprite set** to a replacement card later, but this **does not prove the user's physical card has no additional/unique files**.
- The current installed TamaPoke serial protocol exposes `LS` (list names/sizes in `/mons`), `SDINFO` (capacity/usage), and `PUT` (writes files under `/mons`). It has **no `GET` or readback command**. Therefore a USB cable **cannot make a full microSD backup using the current firmware**. Listing does not back up file contents.
- A **physical full card copy is still recommended before any card write, formatting or SD file replacement**. Alternatives: a borrowed USB microSD reader, another PC/tablet/phone that can read microSD, or an additional blank card staged later when a reader is available. Do not borrow an unverified SD image to replace the known-good card.
- Until then we can safely **inspect the board model, identify the correct USB COM port and read the 16 MB internal flash into a Windows backup file** using Step 3. The internal ESP32 flash and the external microSD are different storage devices. **Read-only flashing tools do not copy the microSD**, and a full flash dump is **not** an SD backup.
- Keep both the working card and the user-approved Windows archive preserved. **No firmware flashing or microSD modifications yet.**

## Step 2 — determine the correct USB port

1. Connect the board to Windows over a **data-capable USB-C cable**, directly rather than through an unreliable hub.
2. Open **Device Manager → Ports (COM & LPT)**, observe which COM port appears on connection, and note its name. Use that exact port in the next step; do not guess or connect to an unrelated serial device.
3. Save a photo or description of the board model and what currently appears on its screen. Keep the approved asset archive and the untouched original microSD accessible.

## Step 3 — make a full 16 MB flash/NVS backup before ANY upload

Install Python 3.10+ and the official **esptool v5** on Windows, then use a PowerShell terminal in a **new, empty** work folder. The backed-up image can contain **private saved Pokémon data** and must never be published or uploaded to a public GitHub issue.

```powershell
py -m pip install "esptool>=5,<6"
py -m esptool --chip esp32s3 --port COM7 flash-id
```

**Replace `COM7` with the COM port you actually confirmed.** Verify `ESP32-S3` and `16 MB` in the output. If the device is different, a serial connection fails, flash encryption/secure boot prevents a reliable backup, or flash size is not 16 MB, **stop**.

The repository includes a read-only helper `tools/phase4_waveshare_backup.py`. Run it **without** `--execute` first to review its plan:

```powershell
py tools/phase4_waveshare_backup.py --port COM7 --out .\private-original-flash-backup
```

When the COM port and capacity are confirmed and you are ready to **read only** your existing firmware, explicitly opt in:

```powershell
py tools/phase4_waveshare_backup.py --port COM7 --out .\private-original-flash-backup --execute
```

The helper calls only `flash-id` and `read-flash 0 ALL`, checks 16 MB and records SHA-256 in `backup-manifest.json`. This may require using BOOT/RESET to enter the serial bootloader. It never calls write/erase. Reading flash is **not** a substitute for validating a restore path, and the microSD backup remains a separate requirement before any SD modification. Store the private full-flash dump securely.

Official Espressif documentation: https://docs.espressif.com/projects/esptool/en/latest/esp32s3/esptool/basic-commands.html

## Step 4 — stop here for now

**Do not use TamaPoke's original browser installer to install a PokéByte upgrade. Do not press Erase, Format, Install, or Flash.** Our separate PokéByte installer preview is intentionally disabled until embedded scene rendering, balanced ambience, all three personality packs, physical screen/touch/SD/audio/battery checks, and backup/restore tests are complete.

The next development deliverable is a genuinely ported ESP32-S3 PokéByte binary for **hardware tests**, which will be reviewed and tested separately. The board is not ready for flashing merely because a build passes.

## Share only non-sensitive diagnostics

For the next review, share the **exact board model**, the **COM port**, whether the original microSD remains untouched (no reader available) and whether the **read-only internal-flash backup finished successfully**. Avoid sharing full-flash images, NVS dumps, secret keys or personal serial numbers publicly.

**Phase 4 release status:** in engineering preparation; original hardware remains untouched.
