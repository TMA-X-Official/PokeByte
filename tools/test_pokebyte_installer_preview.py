#!/usr/bin/env python3
"""Safety gates for the UNRELEASED PokéByte static installer preview."""
from html.parser import HTMLParser
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
PAGE = ROOT / "web/pokebyte/index.html"
ORIGINAL_PAGE = ROOT / "web/index.html"
ORIGINAL_MANIFEST = ROOT / "web/manifest.json"

class InstallerParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.buttons = []
        self.scripts = []
        self.install_elements = []
        self.links = []
    def handle_starttag(self, tag, attrs):
        attributes = dict(attrs)
        if tag == "button":
            self.buttons.append(attributes)
        if tag == "script":
            self.scripts.append(attributes)
        if tag == "esp-web-install-button":
            self.install_elements.append(attributes)
        if tag == "a":
            self.links.append(attributes.get("href", ""))
    handle_startendtag = handle_starttag

assert PAGE.exists(), "Missing new PokéByte preview page"
site = PAGE.read_text(encoding="utf-8")
parser = InstallerParser()
parser.feed(site)
assert len(parser.buttons) == 1, "Only one explicitly disabled install control is expected"
assert "disabled" in parser.buttons[0], "The installer must remain DISABLED"
assert not parser.scripts, "Do not load JavaScript or WebSerial before hardware release"
assert not parser.install_elements, "No active ESP Web Tools component allowed yet"
assert "manifest.json" not in site, "Never accidentally link original TamaPoke manifest"
assert "navigator.serial" not in site and "navigator.usb" not in site
assert not re.search(r"firmware/[^ \"]+\.bin", site)
assert "Waveshare ESP32-S3-Touch-AMOLED-1.75" in site
assert "1.75C" in site
assert "no firmware" in site.lower() or "not ready" in site.lower()
assert "Adventure or Battle add-ons" in site, "Cancelled Phase 4 must not ship"
assert "303" in site and "12 approved backgrounds" in site
assert ORIGINAL_PAGE.exists() and ORIGINAL_MANIFEST.exists(), "Original installer must be preserved"
assert "TamaPoke" in ORIGINAL_MANIFEST.read_text(encoding="utf-8")
assert "tamapoke.bin" in ORIGINAL_MANIFEST.read_text(encoding="utf-8")
assert "Install TamaPoke" in ORIGINAL_PAGE.read_text(encoding="utf-8")
print("PASS: PokéByte installer is branded, static, accessible and safely locked; upstream original untouched")
