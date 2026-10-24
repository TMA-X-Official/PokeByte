# PokéByte — Accepted Windows Artwork & Sprite Packaging Manifest

**Approved:** 24 October 2026, Phase 3 finished Windows playtest. **Purpose:** prevent loss of the user's approved backgrounds and avoid the earlier *grey rectangle / duplicated sunset scene* regression.

## Canonical Windows release and source backup

- **Accepted release:** `PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip`
- **SHA-256 of full release ZIP:** `2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0`
- **User-uploaded historical background reference:** `PokeByte-Phase3-Personality-Pack-03-LITTLE-SURPRISES-WINDOWS(1).zip`
- **SHA-256 of backup ZIP:** `afb21867c8375543acdc800c8ed8c6eb5285aac6874e9a27e2d1b79441d4577c`
- All 12 accepted Windows background assets are precisely **466×466 BMP**. Hashes below were calculated by reading the *uncompressed* file bytes from the user-approved ZIP. Each also matched the supplied historical backup file **byte-for-byte**.
- All **303** `tools/sdcard/mons/*.bin` files matched the historical backup byte-for-byte.

## Canonical SHA-256 of each BMP (raw uncompressed data)

```text
48099a165d99ac8ba8c438ea8a91f67d35cab433a664d1c4b22dd6ec0cad2cbe  assets/meadow_afternoon.bmp
bb1572806daae0aa81bf1740fd02757e93c69ae5ebbe4907c7f8081a2e286a86  assets/meadow_day.bmp
6ca3bed83cae6e0e70f6c3a3a1c19adf856072bf81e44c6b7152aa4dcad8af38  assets/meadow_evening.bmp
bc25a03460134402323a0f5c6c7a192e7b2e3877a41a21acad4b5595345e0284  assets/meadow_morning.bmp
68b1252dc00385ab1b73d1d8f0561b68b7124fbd09ee622477815c010452bf87  assets/meadow_night.bmp
5da099398e59896354d32b08cade4933d4fdee2e4d2cc63535fc736e30c95225  assets/minigame.bmp
3a0abf7e23c6e3352c7e920b7cf0ba5debeef85e6b693680fde1aa5cd026ed01  assets/minigame_afternoon.bmp
2e56113de8c75606328a0e55c72015e0a71d057464092f6ad3bb6314489810cd  assets/minigame_day.bmp
5da099398e59896354d32b08cade4933d4fdee2e4d2cc63535fc736e30c95225  assets/minigame_evening.bmp
e88f74a4b0cddb9b4cda9485ae052b8a725c66caf42febb203c56fcbef230e59  assets/minigame_morning.bmp
6e9c8bb6769363f07b62fb7c2d706d63ac9f75cb39f0e9a13808f2252301020d  assets/minigame_night.bmp
6ca3bed83cae6e0e70f6c3a3a1c19adf856072bf81e44c6b7152aa4dcad8af38  assets/sunset-forest.bmp
```

The fact that two legacy fallback bitmaps match a particular approved scene is expected; **it does not mean the five home day-part scenes can be replaced with the same file**. The five distinct home meadow images above MUST retain distinct individual hashes.

## Original Pokémon sprite-set fingerprint

- **Count:** `303` unique files inside `tools/sdcard/mons/` ending in `.bin`.
- **Combined verification digest:** `45624145dab29c46c045c44fea7625884e25507af9d6cac352c317345286294c`.
- This combined digest is computed by sorting **file paths**, then sequentially feeding SHA-256: `UTF8(filepath) + b"\\0" + SHA256(file_bytes)` for each entry. Reproduce it with the script below. Every sprite also matched the user's backup individually when it was examined.

## Mandatory packaging verification (Python 3 standard library)

```python
from hashlib import sha256
from pathlib import Path
from zipfile import ZipFile
import struct

zip_path = Path("PokeByte-Phase3-Pack03-Distinct-Surprises-Approved-Art-Windows.zip")
expected = {
    "assets/meadow_afternoon.bmp": "48099a165d99ac8ba8c438ea8a91f67d35cab433a664d1c4b22dd6ec0cad2cbe",
    "assets/meadow_day.bmp": "bb1572806daae0aa81bf1740fd02757e93c69ae5ebbe4907c7f8081a2e286a86",
    "assets/meadow_evening.bmp": "6ca3bed83cae6e0e70f6c3a3a1c19adf856072bf81e44c6b7152aa4dcad8af38",
    "assets/meadow_morning.bmp": "bc25a03460134402323a0f5c6c7a192e7b2e3877a41a21acad4b5595345e0284",
    "assets/meadow_night.bmp": "68b1252dc00385ab1b73d1d8f0561b68b7124fbd09ee622477815c010452bf87",
    "assets/minigame.bmp": "5da099398e59896354d32b08cade4933d4fdee2e4d2cc63535fc736e30c95225",
    "assets/minigame_afternoon.bmp": "3a0abf7e23c6e3352c7e920b7cf0ba5debeef85e6b693680fde1aa5cd026ed01",
    "assets/minigame_day.bmp": "2e56113de8c75606328a0e55c72015e0a71d057464092f6ad3bb6314489810cd",
    "assets/minigame_evening.bmp": "5da099398e59896354d32b08cade4933d4fdee2e4d2cc63535fc736e30c95225",
    "assets/minigame_morning.bmp": "e88f74a4b0cddb9b4cda9485ae052b8a725c66caf42febb203c56fcbef230e59",
    "assets/minigame_night.bmp": "6e9c8bb6769363f07b62fb7c2d706d63ac9f75cb39f0e9a13808f2252301020d",
    "assets/sunset-forest.bmp": "6ca3bed83cae6e0e70f6c3a3a1c19adf856072bf81e44c6b7152aa4dcad8af38",
}
with ZipFile(zip_path) as z:
    assert z.testzip() is None, "Corrupt archive entry"
    found = {n for n in z.namelist() if n.startswith("assets/") and n.endswith(".bmp")}
    assert found == set(expected), ("missing or unexpected background BMP", found ^ set(expected))
    for filename, digest in expected.items():
        b = z.read(filename)
        assert sha256(b).hexdigest() == digest, filename
        assert b[:2] == b"BM" and struct.unpack_from("<ii", b, 18) == (466, 466), filename
    sprites = sorted(n for n in z.namelist()
                     if n.startswith("tools/sdcard/mons/") and n.endswith(".bin"))
    assert len(sprites) == 303
    h = sha256()
    for name in sprites:
        h.update(name.encode() + b"\0" + sha256(z.read(name)).digest())
    assert h.hexdigest() == "45624145dab29c46c045c44fea7625884e25507af9d6cac352c317345286294c"
print("PASS: exact approved backgrounds and original PMD sprite set")
```

**Warning:** Windows GitHub Actions tests generate **synthetic BMP fixtures** with `pokebyte/test_modes_ci.py`. These are for automated checks only, and are **not approved scenery**. CI-generated Windows artifacts must have the correct approved user backup's background bytes restored and verified before playtest distribution. Do not overwrite all day-parts with `sunset-forest.bmp`! Preserve original stable working ZIP separately.

These are integrity references, **not a replacement for the user's private local archive of original approved binary files**. GitHub does not currently contain the complete original approved BMPs as verified canonical repository binary assets.
