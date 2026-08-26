#!/usr/bin/env python3

from pathlib import Path
import xml.etree.ElementTree as ET
from PIL import Image
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

PROJECT_ROOT = Path(__file__).resolve().parent
QRC_FILE = PROJECT_ROOT /  "icons.qrc"
RESOURCE_DIR = QRC_FILE.parent
OUTPUT_DIR = RESOURCE_DIR / "generated-icons"

SIZES = (32, 48, 64, 128)
SUPPORTED_EXTENSIONS = {".png", ".jpg", ".jpeg", ".gif", ".ico"}


def safe_output_name(relative_icon_path: Path, size: int) -> Path:
    ratio = size // 16
    stem = relative_icon_path.with_stem(f"{relative_icon_path.stem}@{ratio}")
    return OUTPUT_DIR / stem.with_suffix(".png")


def generate_icon(source_file: Path, output_file: Path, size: int) -> None:
    output_file.parent.mkdir(parents=True, exist_ok=True)

    with Image.open(source_file) as image:
        image = image.convert("RGBA")

        # Image.thumbnail() only ever shrinks. Since the sources are 16x16 (or
        # barely larger), it was a no-op for every target size and the icon
        # ended up centred on a transparent canvas instead of filling it --
        # icons then rendered at 1/N of their intended size on HiDPI displays.
        # Scale explicitly, preserving the aspect ratio of non-square sources.
        factor = min(size / image.width, size / image.height)
        width = max(1, round(image.width * factor))
        height = max(1, round(image.height * factor))
        if (width, height) != image.size:
            image = image.resize((width, height), Image.LANCZOS)

        canvas = Image.new("RGBA", (size, size), (0, 0, 0, 0))
        # No mask: replace the region outright so semi-transparent edges from
        # the LANCZOS resampling survive instead of being composited away.
        canvas.paste(image, ((size - width) // 2, (size - height) // 2))

        canvas.save(output_file, "PNG")


def main() -> None:
    tree = ET.parse(QRC_FILE)
    root = tree.getroot()

    generated = 0
    skipped = 0

    for file_node in root.findall(".//file"):
        if not file_node.text:
            continue

        relative_path = Path(file_node.text.strip())
        icon_path = os.path.join(SCRIPT_DIR, file_node.text)

        if relative_path.suffix.lower() not in SUPPORTED_EXTENSIONS:
            skipped += 1
            continue

        source_file = RESOURCE_DIR / relative_path

        if not source_file.exists():
            print(f"Fehlt: {source_file}")
            skipped += 1
            continue

        for size in SIZES:
            output_file = safe_output_name(relative_path, size)
            generate_icon(source_file, output_file, size)
            print(f"Erzeugt: {output_file}")

        generated += 1

    print()
    print(f"Fertig. Verarbeitete Icons: {generated}")
    print(f"Übersprungen: {skipped}")


if __name__ == "__main__":
    main()
