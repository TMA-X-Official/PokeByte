"""Make FIVE synthetic test scenes from the existing fallback BMP for CI only.
These are NOT user artwork and are replaced byte-for-byte with the five
approved environment images in the downloadable Windows playtest ZIP.
No Pillow or external packages required.
"""
from pathlib import Path
import struct

assets = Path("pc/build/windows/PokeByte-Phase1-PC/assets")
home = (assets / "sunset-forest.bmp").read_bytes()
game = (assets / "minigame.bmp").read_bytes()
assert home[:2] == game[:2] == b"BM"
modes = ("morning", "day", "afternoon", "evening", "night")
for i, mode in enumerate(modes):
    for label, data in (("meadow", home), ("minigame", game)):
        buf = bytearray(data)
        _, w, h = struct.unpack_from("<Iii", buf, 10)
        # offset at byte 10, width at byte 18, height at byte 22
        pixel_offset = struct.unpack_from("<I", buf, 10)[0]
        w, h = struct.unpack_from("<ii", buf, 18)
        bpp = struct.unpack_from("<H", buf, 28)[0]
        assert (w, h, bpp) == (466, 466, 24)
        stride = ((w * 3 + 3) // 4) * 4
        # Distinct visible color bar in scenery area (CI only).
        for y in range(120, 155):
            for x in range(378, 435):
                p = pixel_offset + (h - 1 - y) * stride + 3 * x
                buf[p:p+3] = bytes([35+i*29, 64+i*17, 100+i*13])
        (assets / f"{label}_{mode}.bmp").write_bytes(buf)
print("Created five distinct CI-only test images for home and minigame")
