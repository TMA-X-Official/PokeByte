#!/usr/bin/env python3
"""Verify the user-approved Phase 3 ZIP and stage NONDESTRUCTIVE WaveShare SD files.

Does not open USB devices, format media, touch firmware/NVS, or alter the source ZIP.
The /pokebyte BMP path is a Phase 4 proposed asset layout, NOT hardware support yet.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import struct
from zipfile import ZipFile

APPROVED_ZIP_SHA256 = "2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0"
PMD_SET_SHA256 = "45624145dab29c46c045c44fea7625884e25507af9d6cac352c317345286294c"
BMP_HASHES = {
    "meadow_afternoon.bmp": "48099a165d99ac8ba8c438ea8a91f67d35cab433a664d1c4b22dd6ec0cad2cbe",
    "meadow_day.bmp": "bb1572806daae0aa81bf1740fd02757e93c69ae5ebbe4907c7f8081a2e286a86",
    "meadow_evening.bmp": "6ca3bed83cae6e0e70f6c3a3a1c19adf856072bf81e44c6b7152aa4dcad8af38",
    "meadow_morning.bmp": "bc25a03460134402323a0f5c6c7a192e7b2e3877a41a21acad4b5595345e0284",
    "meadow_night.bmp": "68b1252dc00385ab1b73d1d8f0561b68b7124fbd09ee622477815c010452bf87",
    "minigame.bmp": "5da099398e59896354d32b08cade4933d4fdee2e4d2cc63535fc736e30c95225",
    "minigame_afternoon.bmp": "3a0abf7e23c6e3352c7e920b7cf0ba5debeef85e6b693680fde1aa5cd026ed01",
    "minigame_day.bmp": "2e56113de8c75606328a0e55c72015e0a71d057464092f6ad3bb6314489810cd",
    "minigame_evening.bmp": "5da099398e59896354d32b08cade4933d4fdee2e4d2cc63535fc736e30c95225",
    "minigame_morning.bmp": "e88f74a4b0cddb9b4cda9485ae052b8a725c66caf42febb203c56fcbef230e59",
    "minigame_night.bmp": "6e9c8bb6769363f07b62fb7c2d706d63ac9f75cb39f0e9a13808f2252301020d",
    "sunset-forest.bmp": "6ca3bed83cae6e0e70f6c3a3a1c19adf856072bf81e44c6b7152aa4dcad8af38",
}

def sha(b: bytes) -> str:
    return hashlib.sha256(b).hexdigest()

def verify_archive(source: Path) -> tuple[dict[str, bytes], dict[str, bytes]]:
    if not source.is_file():
        raise ValueError(f"Source ZIP missing: {source}")
    if sha(source.read_bytes()) != APPROVED_ZIP_SHA256:
        raise ValueError("ZIP SHA-256 mismatch: this is NOT the accepted Phase 3 build")
    with ZipFile(source) as z:
        assert z.testzip() is None, "Corrupted ZIP"
        bmp_paths = {
            name for name in z.namelist()
            if name.startswith("assets/") and name.endswith(".bmp")
        }
        expected = {"assets/" + name for name in BMP_HASHES}
        assert bmp_paths == expected, ("Wrong background set", bmp_paths ^ expected)
        bmps: dict[str, bytes] = {}
        for name, digest in BMP_HASHES.items():
            data = z.read("assets/" + name)
            assert sha(data) == digest, name
            assert (data[:2] == b"BM" and
                    struct.unpack_from("<ii", data, 18) == (466, 466) and
                    struct.unpack_from("<H", data, 28)[0] == 24 and
                    struct.unpack_from("<I", data, 30)[0] == 0), name
            bmps[name] = data
        paths = sorted(
            name for name in z.namelist()
            if name.startswith("tools/sdcard/mons/") and name.endswith(".bin")
        )
        assert len(paths) == 303, ("Expected 303 PMD binaries", len(paths))
        checksum = hashlib.sha256()
        mons: dict[str, bytes] = {}
        for path in paths:
            data = z.read(path)
            checksum.update(path.encode("utf-8") + b"\x00" +
                            hashlib.sha256(data).digest())
            mons[path.removeprefix("tools/sdcard/mons/")] = data
        assert checksum.hexdigest() == PMD_SET_SHA256, "Original PMD sprites differ"
        assert "thumbs.bin" in mons, "Original Pokédex thumbnail file missing"
    return bmps, mons

def stage(source: Path, target: Path | None) -> None:
    bmps, mons = verify_archive(source)
    print(f"PASS approved archive: {len(bmps)} unique named BMPs; {len(mons)} PMD BINs")
    if target is None:
        print("Validation only. Use --output NEW_DIRECTORY to stage SD files.")
        return
    if target.exists():
        raise ValueError(f"Refusing to overwrite existing directory: {target}")
    (target / "pokebyte").mkdir(parents=True)
    (target / "mons").mkdir()
    for name, data in bmps.items():
        (target / "pokebyte" / name).write_bytes(data)
    for name, data in mons.items():
        (target / "mons" / name).write_bytes(data)
    manifest = {
        "source_zip_sha256": APPROVED_ZIP_SHA256,
        "note": "Approved Phase 3 artwork, not a bootable firmware. Manual SD copy after backup only.",
        "backgrounds": {"pokebyte/" + n: sha(b) for n, b in sorted(bmps.items())},
        "pmdbins": len(mons),
        "pmd_set_sha256": PMD_SET_SHA256,
    }
    (target / "POKEBYTE-APPROVED-ASSETS.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8"
    )
    print(f"PASS staged to {target.resolve()} — NO flashing or SD formatting performed")

if __name__ == "__main__":
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("--source", required=True, type=Path, help="Exact approved Phase 3 Windows ZIP")
    cli.add_argument("--output", type=Path, help="New empty/nonexistent staging directory")
    args = cli.parse_args()
    stage(args.source, args.output)
