#!/usr/bin/env python3
"""Convert PokéByte's versioned meadow RGB PNG to a Windows-compatible 24-bit BMP.

Pure Python standard library: the GitHub Windows build does not need Pillow,
ImageMagick or a second download. Supports ordinary non-interlaced RGB8 PNG.
"""
from pathlib import Path
import struct
import sys
import zlib

SIGNATURE = b"\x89PNG\r\n\x1a\n"


def decode_rgb_png(source: bytes) -> tuple[int, int, bytes]:
    if not source.startswith(SIGNATURE):
        raise ValueError("Not a PNG file")
    pos = 8
    idat = bytearray()
    width = height = None
    while pos + 12 <= len(source):
        length = struct.unpack_from(">I", source, pos)[0]
        tag = source[pos + 4:pos + 8]
        end = pos + 12 + length
        if end > len(source):
            raise ValueError("Truncated PNG chunk")
        payload = source[pos + 8:pos + 8 + length]
        checksum = struct.unpack_from(">I", source, pos + 8 + length)[0]
        if zlib.crc32(tag + payload) != checksum:
            raise ValueError("Corrupt PNG chunk")
        pos = end
        if tag == b"IHDR":
            width, height, depth, color, compression, filtering, interlace = struct.unpack(">IIBBBBB", payload)
            if (width, height, depth, color, compression, filtering, interlace) != (466, 466, 8, 2, 0, 0, 0):
                raise ValueError("Expected a non-interlaced 466x466 RGB8 meadow PNG")
        elif tag == b"IDAT":
            idat.extend(payload)
        elif tag == b"IEND":
            break
    if width is None or not idat:
        raise ValueError("PNG missing image metadata or pixel data")
    unfiltered = zlib.decompress(idat)
    stride = width * 3
    if len(unfiltered) != height * (stride + 1):
        raise ValueError("Unexpected PNG decompressed size")
    out = bytearray(height * stride)
    previous = bytearray(stride)
    for y in range(height):
        start = y * (stride + 1)
        filter_type = unfiltered[start]
        scanline = bytearray(unfiltered[start + 1:start + 1 + stride])
        for i in range(stride):
            left = scanline[i - 3] if i >= 3 else 0
            above = previous[i]
            upper_left = previous[i - 3] if i >= 3 else 0
            if filter_type == 0:
                predictor = 0
            elif filter_type == 1:
                predictor = left
            elif filter_type == 2:
                predictor = above
            elif filter_type == 3:
                predictor = (left + above) // 2
            elif filter_type == 4:
                prediction = left + above - upper_left
                a, b, c = abs(prediction - left), abs(prediction - above), abs(prediction - upper_left)
                predictor = left if a <= b and a <= c else above if b <= c else upper_left
            else:
                raise ValueError(f"Invalid PNG filter: {filter_type}")
            scanline[i] = (scanline[i] + predictor) & 0xFF
        out[y * stride:(y + 1) * stride] = scanline
        previous = scanline
    return width, height, bytes(out)


def encode_bmp(width: int, height: int, rgb: bytes) -> bytes:
    stride = width * 3
    padded = (stride + 3) & ~3
    size = 54 + padded * height
    header = struct.pack("<2sIHHI", b"BM", size, 0, 0, 54)
    header += struct.pack("<IiiHHIIiiII", 40, width, height, 1, 24, 0, padded * height, 0, 0, 0, 0)
    pixel_data = bytearray(padded * height)
    for y in range(height):
        row = rgb[(height - 1 - y) * stride:(height - y) * stride]
        dest = y * padded
        for x in range(width):
            p = x * 3
            pixel_data[dest + p:dest + p + 3] = bytes((row[p + 2], row[p + 1], row[p]))
    return header + bytes(pixel_data)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("Usage: python pokebyte/build_meadow_asset.py INPUT.png OUTPUT.bmp")
    width, height, rgb = decode_rgb_png(Path(sys.argv[1]).read_bytes())
    out = Path(sys.argv[2])
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(encode_bmp(width, height, rgb))
    print(f"PokéByte meadow artwork: {width}x{height}, {out.stat().st_size:,} BMP bytes")


if __name__ == "__main__":
    main()