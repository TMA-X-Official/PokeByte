#!/usr/bin/env python3
"""Read-only Waveshare full-flash backup, opt-in only. No erase/write/format.

Target: ESP32-S3-Touch-AMOLED-1.75. Windows PC attached via USB.
Requires Python 3.10+ and esptool v5 (python -m pip install esptool).
By default merely prints the plan. --execute explicitly starts flash-ID and read.
This script NEVER writes to the device or microSD, or flashes firmware.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
from datetime import datetime, timezone

def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1048576), b""):
            digest.update(block)
    return digest.hexdigest()

def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", required=True, help="Verified Waveshare USB COM port, e.g. COM7")
    parser.add_argument("--out", required=True, type=Path, help="NEW output directory; must not exist")
    parser.add_argument("--execute", action="store_true", help="Explicit consent for read-only full-flash backup")
    args = parser.parse_args()
    if not args.port.strip():
        parser.error("A verified COM port is required")
    if args.out.exists():
        parser.error("Refusing to overwrite an existing directory")
    base = [sys.executable, "-m", "esptool", "--chip", "esp32s3",
            "--port", args.port, "--baud", "115200"]
    print("READ-ONLY original firmware backup; no firmware installation.")
    print("Checking:", " ".join(base + ["flash-id"]))
    print("Backup:", " ".join(base + ["read-flash", "0", "ALL",
                                     str(args.out / "original-full-flash.bin")]))
    print("Also back up the entire microSD separately before any upgrade.")
    if not args.execute:
        print("DRY RUN: device untouched. Add --execute for opt-in read-only backup.")
        return
    result = subprocess.run(base + ["flash-id"], capture_output=True, text=True, check=True)
    print(result.stdout)
    line = next((s for s in result.stdout.splitlines() if "Detected flash size:" in s), "")
    if not line or not any(v in line for v in ("16MB", "16 MB", "16MiB", "16 MiB")):
        raise RuntimeError("Board or flash size unverified; expected 16 MB: " + repr(line))
    args.out.mkdir(parents=True, exist_ok=False)
    target = args.out / "original-full-flash.bin"
    subprocess.run(base + ["read-flash", "0", "ALL", str(target)], check=True)
    actual = target.stat().st_size
    if actual != 16 * 1024 * 1024:
        raise RuntimeError("Unexpected flash image size; do not flash: " + str(actual))
    digest = sha256_file(target)
    manifest = {
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "chip": "esp32s3", "source_port": args.port,
        "flash_size_bytes": actual, "sha256": digest, "file": target.name,
        "warning": "PRIVATE BACKUP: may contain saved Pokemon and NVS; never publish.",
        "note": "Must also back up microSD and validate restore before flashing any firmware.",
    }
    (args.out / "backup-manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print("PASS. Full flash backup:", target)
    print("SHA256:", digest)
    print("Keep an offline copy. Do NOT publish this file.")

if __name__ == "__main__":
    main()
