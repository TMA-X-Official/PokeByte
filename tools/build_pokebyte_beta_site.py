#!/usr/bin/env python3
"""Package an explicitly experimental, non-SD-writing ESP32-S3 web tester.

Never uses or publishes a 16 MB merged image: that would overwrite NVS/FFat.
Requires exact expected original partition layout. Generates 4 specific images
at offsets 0x0,0x8000,0xE000,0x10000; never targets NVS 0x9000–0xDFFF.
Does not deploy, flash, erase, or alter connected devices.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import zipfile
import zlib

EXPECTED = [
    ("nvs", 0x01, 0x02, 0x009000, 0x005000),
    ("otadata", 0x01, 0x00, 0x00e000, 0x002000),
    ("app0", 0x00, 0x10, 0x010000, 0x300000),
    ("app1", 0x00, 0x11, 0x310000, 0x300000),
    ("ffat", 0x01, 0x81, 0x610000, 0x9e0000),
    ("coredump", 0x01, 0x03, 0xff0000, 0x010000),
]
PARTS = [
    ("bootloader.bin", 0x000000),
    ("partitions.bin", 0x008000),
    ("otadata.bin", 0x00e000),
    ("pokebyte.bin", 0x010000),
]

def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()

def layout(partitions: bytes) -> None:
    values = []
    for i in range(0, len(partitions), 32):
        if partitions[i:i+2] != b"\xaa\x50":
            break
        typ, sub, offset, size = struct.unpack_from("<BBII", partitions, i+2)
        name = partitions[i+12:i+28].split(b"\0")[0].decode("ascii")
        values.append((name, typ, sub, offset, size))
    if values != EXPECTED:
        raise ValueError(f"Flash partition mismatch, STOP: {values!r}")
    # Never put a flash part in the user's original NVS or FFat storage.
    for _, address in PARTS:
        if address in range(0x9000, 0xE000) or address>=0x310000:
            raise ValueError("Part would write user NVS/FFat — STOP")

def main() -> None:
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("--firmware-dir",required=True,type=Path)
    p.add_argument("--site-dir",required=True,type=Path)
    p.add_argument("--commit",required=True)
    args=p.parse_args()
    if args.site_dir.exists():
        raise ValueError("Site output directory already exists; won't overwrite")
    f=args.firmware_dir
    original = {
        "bootloader.bin": (f/"TamaPoke.ino.bootloader.bin").read_bytes(),
        "partitions.bin": (f/"TamaPoke.ino.partitions.bin").read_bytes(),
        "pokebyte.bin": (f/"TamaPoke.ino.bin").read_bytes(),
    }
    merged=(f/"TamaPoke.ino.merged.bin").read_bytes()
    if len(merged)!=16*1024*1024:
        raise ValueError("Not a 16 MB merged binary, STOP")
    if original["bootloader.bin"][0]!=0xe9 or original["pokebyte.bin"][0]!=0xe9:
        raise ValueError("Invalid ESP image header")
    if len(original["bootloader.bin"])>0x8000:
        raise ValueError("Bootloader too large")
    if len(original["partitions.bin"])>0x1000:
        raise ValueError("Partition header too large")
    if len(original["pokebyte.bin"])>0x300000:
        raise ValueError("Sketch larger than primary app partition")
    layout(original["partitions.bin"])
    original["otadata.bin"] = merged[0xE000:0x10000]
    if len(original["otadata.bin"])!=8192:
        raise ValueError("OTA boot-selector image invalid")
    for name, offset in PARTS:
        if merged[offset:offset+len(original[name])] != original[name]:
            raise ValueError(f"{name} doesn't match verified combined source image")
    # Extra check: only the boot select region may be included from "merged".
    # The entire merged binary is intentionally NOT published.
    out=args.site_dir
    (out/"beta"/"firmware").mkdir(parents=True)
    logo_dir=Path("web/pokebyte/assets")
    shutil.copytree(logo_dir, out/"assets")
    shutil.copyfile("web/pokebyte/index.html", out/"index.html")
    shutil.copyfile("web/pokebyte/beta/index.html", out/"beta"/"index.html")
    shutil.copyfile("web/pokebyte/beta/background-sync.js", out/"beta"/"background-sync.js")
    shutil.copyfile("web/pokebyte/beta/approved-backgrounds.json", out/"beta"/"approved-backgrounds.json")
    # If the owner has uploaded the approved scene bundle to the repository,
    # publish it automatically. Never publish CI-generated scenery fixtures.
    approved_path=Path("web/pokebyte/approved-backgrounds.zip")
    if approved_path.exists():
        reference=json.loads(Path("web/pokebyte/beta/approved-backgrounds.json").read_text())
        with zipfile.ZipFile(approved_path) as approved:
            names=set(approved.namelist())
            expected={"assets/"+e["name"] for e in reference["files"]}
            actual={n for n in names if n.endswith(".bmp")}
            if actual!=expected:
                raise ValueError(f"Approved scenes ZIP must contain exactly 12 assets/: {actual ^ expected}")
            for item in reference["files"]:
                raw=approved.read("assets/"+item["name"])
                if (len(raw)!=item["bytes"] or sha(raw)!=item["sha256"] or
                    f"{zlib.crc32(raw):08x}"!=item["crc32"]):
                    raise ValueError("Approved artwork mismatch: "+item["name"])
        shutil.copyfile(approved_path,out/"beta"/"approved-backgrounds.zip")
        print("PASS: included 12 verified original approved BMPs for automatic HTTPS installer download")
    else:
        print("INFO: approved art ZIP not on GitHub yet; site permits owner to select exact approved Phase 3 Windows ZIP")
    sizes={}
    for name, _ in PARTS:
        raw=original[name]
        (out/"beta"/"firmware"/name).write_bytes(raw)
        sizes[name]={"size":len(raw),"sha256":sha(raw)}
    manifest={
        "name":"PokéByte experimental hardware test",
        "version":"PB4-dev-"+args.commit[:10],
        "new_install_prompt_erase":True,
        "new_install_improv_wait_time":0,
        "builds":[{
            "chipFamily":"ESP32-S3",
            "parts":[{"path":"firmware/"+name,"offset":offset} for name,offset in PARTS]
        }],
    }
    (out/"beta"/"manifest.json").write_text(json.dumps(manifest,indent=2)+"\n",encoding="utf-8")
    (out/"beta"/"SHA256SUMS.json").write_text(json.dumps({
        "dev_build":args.commit,"chip_family":"ESP32-S3",
        "important":"EARLY EXPERIMENTAL TEST. DO NOT USE ON HARDWARE WITH IRREPLACEABLE DATA. Original microSD left untouched.",
        "parts":sizes,
        "nvs_unwritten_range":"0x9000–0xDFFF",
        "secondary_app_untouched_range":"0x310000–0x60FFFF",
        "external_sd_targeted":False,
    },indent=2)+"\n",encoding="utf-8")
    assert "TamaPoke.ino.merged.bin" not in str(list(out.rglob("*")))
    print("PASS: staged beta site and exactly four verified region-scoped flash parts")
    print("  source:", args.commit[:10])
    for name, offset in PARTS:
        print(f"  {name}: at {offset:#08x}, {len(original[name])} bytes, sha256={sizes[name]['sha256']}")
    print("  No firmware part overlaps NVS or external SD card.")
    print("  This is only a web TEST PACKAGE. Physical device acceptance still pending.")

if __name__=="__main__":
    main()
