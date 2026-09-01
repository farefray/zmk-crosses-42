"""Regenerate verified Crosses diagrams from the active ZMK keymap."""

from pathlib import Path
import subprocess
import sys

import yaml


ROOT = Path(__file__).resolve().parent
REPO = ROOT.parent
KEY_COUNT = 42
LAYERS = ("Base", "Sym", "Nav", "Mouse", "UKR", "Game")


def load_yaml(name: str):
    with (ROOT / name).open(encoding="utf-8") as stream:
        return yaml.safe_load(stream)


parsed_path = ROOT / "_verified_parse.yaml"
subprocess.run(
    [
        sys.executable,
        "-m",
        "keymap_drawer",
        "-c",
        str(REPO / "keymap_drawer.config.yaml"),
        "parse",
        "-z",
        str(REPO / "config" / "crosses.keymap"),
        "-o",
        str(parsed_path),
    ],
    check=True,
)

parsed = load_yaml(parsed_path.name)
labels = load_yaml("crosses_labels.yaml").get("layers", {})
trackballs = load_yaml("trackballs.yaml")

for layer_name, bindings in parsed["layers"].items():
    if len(bindings) != KEY_COUNT:
        raise ValueError(f"{layer_name}: expected {KEY_COUNT} keys, got {len(bindings)}")

    if layer_name in labels:
        overlay = labels[layer_name]
        if len(overlay) < KEY_COUNT:
            raise ValueError(f"{layer_name}: label overlay has only {len(overlay)} keys")
        bindings[:] = overlay[:KEY_COUNT]

    bindings.extend(trackballs.get(layer_name, trackballs["default"]))

with (ROOT / "crosses.yaml").open("w", encoding="utf-8", newline="\n") as stream:
    yaml.safe_dump(parsed, stream, allow_unicode=True, sort_keys=False)

draw_base = [
    sys.executable,
    "-m",
    "keymap_drawer",
    "-c",
    str(REPO / "keymap_drawer.config.yaml"),
    "draw",
    str(ROOT / "crosses.yaml"),
    "-j",
    str(ROOT / "layout.json"),
    "-l",
    "gggw_crosses_42_layout",
]

subprocess.run([*draw_base, "-o", str(ROOT / "crosses.svg")], check=True)
for layer_name in LAYERS:
    subprocess.run(
        [
            *draw_base,
            "-s",
            layer_name,
            "--keys-only",
            "-o",
            str(ROOT / f"crosses_{layer_name.lower()}.svg"),
        ],
        check=True,
    )

subprocess.run(
    [*draw_base, "--combos-only", "-o", str(ROOT / "crosses_combos.svg")],
    check=True,
)
parsed_path.unlink()
