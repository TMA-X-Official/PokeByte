#!/usr/bin/env python3
"""Source-structure and protected-art checks for the Phases 1–3 ESP32 port.

This does NOT replace an Arduino compile, physical board QA or backup test.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
sketch = (ROOT / "TamaPoke.ino").read_text(encoding="utf-8")
render = (ROOT / "pokebyte/embedded_scene.h").read_text(encoding="utf-8")
sd = (ROOT / "sdmon.cpp").read_text(encoding="utf-8")
personality = (ROOT / "pokebyte/personality_pack01.cpp").read_text(encoding="utf-8")
ci = (ROOT / ".github/workflows/ci.yml").read_text(encoding="utf-8")
original_art = (ROOT / "APPROVED-ASSETS.md").read_text(encoding="utf-8")

assert 'SD_MMC.begin("/sdcard", true /* 1-bit SDMMC */, false' in sd, (
    "Do not allow SD auto-format on mount failure"
)
assert 'pokebyte_embedded::draw(gfx,false,sceneHour())' in sketch
assert 'pokebyte_embedded::draw(gfx,true,sceneHour())' in sketch
assert "pokebyte_embedded::setAmbient" in sketch
assert "pokebyte_embedded::setWeather" in sketch
assert "pbSurpriseStage" in sketch and "pokebyteAdvanceSurprise" in sketch
assert "pokebyteSetSocialSnapshot" in sketch
assert "pokebyteSocialGreeting" in sketch
assert "PB_SURPRISE_VICTORY" in sketch and "PB_SURPRISE_FRIENDSHIP" in sketch
assert "PB_SURPRISE_WAKE" in sketch and "PB_SURPRISE_WANDER" in sketch
assert "pokebyte/personality_pack01.h" in sketch
assert "pokebyte/personality_pack02.h" in sketch
assert "pokebyte/personality_pack03.h" in sketch
assert "defined(POKEBYTE_PC) || defined(ARDUINO)" in sketch
assert "cp pokebyte/personality_pack0*.cpp" in ci
assert "cp -R pokebyte" in ci
assert "Preferences" in personality
assert 'p.begin("pbvisual",true)' in render
assert 'f.seek(offset)' in render and 'f.read(row,ROW)' in render
assert "MALLOC_CAP_SPIRAM" in render and "MALLOC_CAP_8BIT" in render
assert "draw16bitRGBBitmap" in render
assert '"/pokebyte/%s_%s.bmp"' in render
assert "CRC mismatch" in render and "requiredCrc" in render
assert "0xae38373bU" in render and "0xd527026bU" in render
assert len(re.findall(r"0x[0-9a-f]{8}U", render)) >= 11
assert "303" in original_art and "meadow_morning.bmp" in original_art
assert "test-adventure" not in sketch.lower()
assert "battle_pack01" not in sketch.lower()
print("PASS: native Waveshare source routes to approved art, SD safeguard, and all three personality packs")
print("Note: a green host audit is NOT proof of on-device support or safe flashing")
