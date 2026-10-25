#!/usr/bin/env python3
"""Fail-closed release checks for user-requested Waveshare visual/audio/SD fixes.

Hardware checks are still mandatory: this script only verifies repository source.
"""
from pathlib import Path
import json
ROOT=Path(__file__).resolve().parents[1]
sketch=(ROOT/"TamaPoke.ino").read_text()
audio=(ROOT/"audio.cpp").read_text()
sd=(ROOT/"sdmon.cpp").read_text()
installer=(ROOT/"web/pokebyte/beta/index.html").read_text()
sync=(ROOT/"web/pokebyte/beta/background-sync.js").read_text()
manifest=json.loads((ROOT/"web/pokebyte/beta/approved-backgrounds.json").read_text())
builder=(ROOT/"tools/build_pokebyte_beta_site.py").read_text()

bright=sketch.split("void updateBrightness(uint32_t now) {",1)[1].split("\n}\n",1)[0]
assert "dimStage = 0;" in bright and "screenOff ? 0" in bright
assert "idle > " not in bright and "pet.sleeping ? 25" not in bright
assert "lastPendingSaveFlush" in sketch and "pet.flushSave();" in sketch
assert "const int16_t amp = 2500;" in audio
assert "const int px = 177, py = 13, pw = 112, ph = 34;" in sketch
assert "gfx->fillRoundRect(px, py, pw, ph" in sketch
assert 'setSize(2);\n  setCur(px + 46' in sketch

assert len(manifest["files"])==12
assert manifest["approvedSourceZipSHA256"]=="2b2e185737e182c84dbbd39594f823fa36ac74d589b4d4b5a826e674f37e61c0"
assert all(e["bytes"]==652454 and len(e["sha256"])==64 and len(e["crc32"])==8 for e in manifest["files"])
assert len(set(e["name"] for e in manifest["files"]))==12
assert manifest["files"][0]["name"]=="meadow_morning.bmp"
assert 'path.startsWith("/pokebyte/")' in sd
assert 'line.startsWith("PBPUT ")' in sd
safe=sd.split('if(line.startsWith("PBPUT "))',1)[1].split('if (line.startsWith("PUT "))',1)[0]
assert 'if(SD_MMC.exists(path))' in safe and "ERR ART" in safe
assert 'SD_MMC.remove(path)' not in safe
assert 'SD_MMC.rename(temp,path)' in safe
assert 'SD_MMC.begin("/sdcard", true /* 1-bit SDMMC */, false' in sd

assert 'src="./background-sync.js"' in installer
assert 'id="install"' in installer and 'id="resumeSetup"' in installer
assert 'id="setupStatus"' in installer and 'id="setupLog"' in installer
assert 'id="installScenes"' not in installer, "Standalone background installer must be removed"
assert 'id="approvedZip"' not in installer, "Public install should not ask for a ZIP"
assert 'approvedArtworkReady&&consent.checked' in installer, "Block firmware if exact art absent"
for marker in ("approvedBundleZipSHA256","pbSha256(bytes)","pbCrc32(bytes)",
               "pbReadCrc","PBCRC ","PBPUT ","PBCAPS","getPorts",
               "requestPort","pbAutomaticFinish","pokebyte-scenes-ready"):
    assert marker in sync,marker
assert "f.write(chunk,got)" in sd and "got!=want" in sd, "USB frame ack must be exact"
assert 'original-sprites.json' in builder
assert '"count":303' in builder
assert 'approved_fingerprint="45624145dab29c46c045c44fea7625884e25507af9d6cac352c317345286294c"' in builder
assert 'if group.hexdigest()!=approved_fingerprint:' in builder
assert 'original-sprites.json' in sync
assert 'spritesInstalled' in sync and 'spritesSkipped' in sync
assert 'await pbReadCrc(dev,art.path)' in sync
assert 'Different existing Pokémon sprite ' in sync
assert '"./mons/"+art.name' in sync
assert 'await pbSha256(bytes)!==art.sha256' in sync
assert '303 original' in installer
assert "shutil.copyfile(\"web/pokebyte/beta/background-sync.js\"" in builder
assert "approved-backgrounds.zip" in builder and "zipfile.ZipFile" in builder
assert "sha(raw)" in builder and "zlib.crc32(raw)" in builder
print("PASS: awake screen never auto-dims, 50%-amplitude SFX, high-contrast 2x battery, periodic saves")
print("PASS: unified firmware-to-original-scene installation, strict art preflight, no separate background installer")
