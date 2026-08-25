# Crosses 42 firmware — agent handoff

**Read this first if you're picking up the project in a new chat.**
Authoritative spec is `Wireless Crosses\PROJECT_HANDOFF.md` (in the
project's mounted folder). This file is operational state.

---

## Where we are

Stage-by-stage progress. Hardware-verified means Max actually flashed it
and confirmed it works on the keyboard.

| Stage | Scope | Code in keymap? | Hardware verified? |
|---|---|---|---|
| 1 | Bare Colemak-DH base, plain Sym/Nav, 42-key chosen-layout fix | yes | **yes** |
| 2 | HRM (GACS, mirrored, positional), ESC/~, SPC/⇧, ENT/⌃, BSPC→DEL | yes | **yes** |
| 3 | TD `{[<` / `}]>` brackets, ▽SYM tap-dance lock + OverKeys macros | yes | **yes** (then macros stripped in Stage 6) |
| 4+5 | ESC→top-left, Caps Word, Mouse/UKR/Game layers | yes | **pending** |
| 6 | Mod-morph `,;`/`.:`, Mouse on right outer thumb, **KeyPeek migration** (F13-F17 macros stripped, Raw HID + KeyPeek modules wired in) | yes | **pending** |
| 7 | **Layer UX overhaul**: SPC/⇧ rolling fix, NAV clipboard+swapper+mouse+media, Numword, Caps Word combo, bootloader combo, BASE outer columns (LCTRL/BSLH replacing TD brackets), Mouseless combos (D+V/K+H), zmk-auto-layer + zmk-tri-state modules | yes | **pending** |
| 8 | **Consistency pass**: BASE `'`↔`,;` swap (pos 23↔32), SYM realign (Getreuer-inspired brackets-on-left, column-aligned `;:?"\` with BASE), NAV pos 27/28/29 = Ctrl+F/S/Y, S+V combo for Ctrl+V, KeyPeek fork patched to single-glyph labels | yes | **pending** |
| 9 | **Punctuation rework (audit A3+B1)**: drop mcs/mdc mod-morphs, BASE pos 23=`'`, pos 32=`,`, pos 33=`.`, pos 35=new mscl (`;:` mod-morph); SYM realigned (pos 23=`"`, pos 32=`<`, pos 33=`>`, pos 35=`:`); `\` relocated to SYM pos 29; UKR Ґ path now Shift+SYM+pos-29 | yes | **pending** |

Auto-shift (`urob/zmk-auto-shift`) dropped from scope — too much interaction
with existing hold behaviors. Not starting it.

Current loosened-for-learning HRM tuning:
`tapping-term-ms = 260`, `quick-tap-ms = 175`, `require-prior-idle-ms = 200`,
`flavor = balanced`, `hold-trigger-on-release`. Tighten to 200/175/150 when
Max says fingers know the layout.

Layer-overlay tooling: **KeyPeek** (Rust, srwi/keypeek). Reads layer state
from the device over Raw HID and keymap via ZMK Studio. See `KEYPEEK_SETUP.md`.
The OverKeys F13-F17 macro shim that used to live in the keymap is gone —
old config archived under `.archive/`.

---

## File map

In **this repo** (`F:\zmk-crosses-42\`):

| Path | What it is |
|---|---|
| `config/crosses.keymap` | Active keymap (Stage 9 — punctuation rework, audit A3+B1) |
| `config/crosses.conf` | Active conf (BLE tweaks + Studio + pointing + sleep) |
| `config/crosses.keymap.bak` / `crosses.conf.bak` | Pre-Stage-1 user attempt (do not use, kept for diff) |
| `config/west.yml` | Manifest. Pulls upstream zmk, `gggw-zmk-keebs` (zephyr-4.1), `zzeneg/zmk-raw-hid` (main), `srwi/zmk-keypeek-layer-notifier` (master), `urob/zmk-auto-layer` (main, Numword), `dhruvinsh/zmk-tri-state` (main, Alt-Tab swapper) |
| `build.yaml` | CI build matrix (left, right, settings_reset) — both halves now include `raw_hid_adapter` shield |
| `STAGE1_NOTES.md` / `STAGE2_NOTES.md` / `STAGE3_NOTES.md` / `STAGE4_5_NOTES.md` / `STAGE6_NOTES.md` / `STAGE7_NOTES.md` / `STAGE8_NOTES.md` / `STAGE9_NOTES.md` | Historical notes per stage — what changed, test checklist |
| `KEYPEEK_SETUP.md` | KeyPeek desktop install + ZMK module integration guide |
| `.archive/OVERKEYS_SETUP.md` / `.archive/overkeys_config.json` | Old OverKeys setup, kept for fallback |
| `gggw-zmk-keebs/boards/shields/crosses/` | Upstream shield (read-only reference) |

Outside the repo:

| Path | What it is |
|---|---|
| `Wireless Crosses\PROJECT_HANDOFF.md` | The authoritative layout spec — what Max wants the keyboard to do |
| `Wireless Crosses\crosses_42_v5.html` | Custom interactive layer visualizer |
| `Wireless Crosses\crosses_42_keycap_spec.xlsx` | Per-key legend for keycap vendor (already ordered) |

---

## Build commands (Max's setup)

WSL2 Ubuntu 24, fish shell, Zephyr SDK 0.16.9 at `/home/fare/zephyr-sdk-0.16.9`,
ZMK source under `zmk/` in this repo (vendored via west).

**Standard rebuild — both halves with Studio enabled and Raw HID for KeyPeek:**

```fish
for shield in crosses_left crosses_right
    west build --pristine -s zmk/app -b nice_nano \
        -- -DZMK_CONFIG=/mnt/f/zmk-crosses-42/config \
           -DSHIELD="$shield raw_hid_adapter" \
           -DSNIPPET=studio-rpc-usb-uart \
           -DCONFIG_ZMK_STUDIO=y \
           -DCONFIG_ZMK_STUDIO_LOCKING=n
    cp build/zephyr/zmk.uf2 /mnt/f/zmk-crosses-42/$shield.uf2
end
```

Output: `crosses_left.uf2`, `crosses_right.uf2` at the repo root.

Note the `raw_hid_adapter` shield is now appended on both halves — this is
what `zmk-keypeek-layer-notifier` writes layer events into. Without it,
KeyPeek can't see the device.

**Settings_reset UF2 (for NVS wipes):**

```fish
west build --pristine -s zmk/app -b nice_nano -- -DSHIELD=settings_reset
cp build/zephyr/zmk.uf2 /mnt/f/zmk-crosses-42/settings_reset.uf2
```

**USB-logging diagnostic build (for the LEFT/peripheral half):**

```fish
west build --pristine -s zmk/app -b nice_nano \
    -- -DZMK_CONFIG=/mnt/f/zmk-crosses-42/config \
       -DSHIELD="crosses_left raw_hid_adapter" \
       -DSNIPPET=zmk-usb-logging
cp build/zephyr/zmk.uf2 /mnt/f/zmk-crosses-42/crosses_left.uf2
```

Then plug the left half into USB on Windows, find its COM port in Device
Manager, open PuTTY at default baud (USB CDC virtual baud). To re-add the
log macros to `crosses.conf`, see commented hint at bottom of that file.
Right half already has the studio snippet which provides the same console.

---

## Flash sequence

Standard (just landing new firmware): bootloader-mode each half (double-tap
reset), drag the matching `.uf2` onto the drive that appears. Done.

After a code change that DOESN'T affect layer count or BLE config: just
the standard sequence. NVS persists.

After settings_reset OR any change that should clear NVS:
1. Bootloader-mode the half.
2. Drag `settings_reset.uf2` — it boots, wipes NVS, halts.
3. Bootloader-mode again.
4. Drag the real firmware.
5. Repeat for the other half if needed.
6. **Re-pair sequence** (see below) if you reset both halves.

---

## Operational gotchas

### 1. The chosen-block — non-negotiable for 42-key

`config/crosses.keymap` MUST contain:

```dts
/ {
    chosen { zmk,physical-layout = &gggw_crosses_42_layout; };
    ...
};
```

Without it, the upstream shield's default 54-key transform is used, and a
42-binding keymap gets silently scrambled across the wrong matrix
positions. This was the original "scrambled flash" symptom.

### 2. Split BLE pairing after settings_reset

Settings_reset wipes pairing on both halves. They auto-pair on first
simultaneous boot, but sometimes need a nudge. Procedure:

1. Plug right half into USB.
2. Power-cycle both halves within ~5 seconds of each other (reset buttons
   or unplug/replug).
3. Wait 60-90 seconds. Both NVS get fresh, central scans, peripheral
   advertises, they pair.

If still no pair after several attempts:
- Verify `crosses_left.uf2` and `crosses_right.uf2` are different files (`md5sum`).
- Verify each half got its matching firmware (left got `crosses_left.uf2`).
- If still broken, do another full settings_reset → real firmware on both
  halves and try again. Took Max several attempts last time.

The BLE conf items in `crosses.conf` (`CONFIG_ZMK_BLE_EXPERIMENTAL_FEATURES`,
`CONFIG_BT_GATT_ENFORCE_SUBSCRIPTION=n`, `CONFIG_BT_CTLR_PHY_2M=y`, etc.)
are load-bearing. Don't trim them again.

### 3. ZMK Studio NVS overrides

`CONFIG_ZMK_STUDIO_LOCKING=n` means Studio can write per-position keymap
overrides to NVS that survive firmware reflashes. Symptom: positions on
the keyboard typing the wrong character even after a clean rebuild.
Remedy: settings_reset, see above. This bit Max once already.

### 4. ssd1306 init failure in logs

The upstream `crosses.dtsi` defines an OLED at `&pro_micro_i2c`. Max
doesn't have an OLED. The firmware logs `<err> ssd1306: Failed to
initialize device!` at boot. **Ignore.** Cosmetic only.

---

## Tuning currently in effect

In `crosses.keymap`:

```
hml / hmr (Home Row Mods):
  flavor                  balanced
  tapping-term-ms         260      (loosened from 200, learning phase)
  quick-tap-ms            175
  require-prior-idle-ms   200      (loosened from 150, learning phase)
  hold-trigger-on-release yes
  hold-trigger-key-positions = opposite-hand only

sht (Space/Shift — Stage 7):
  flavor                  balanced  (was hold-preferred via &mt)
  tapping-term-ms         200
  quick-tap-ms            175
  require-prior-idle-ms   150
  NOTE: fixes rolling-through-Space misfire. If Space still triggers
  Shift during fast typing, lower tapping-term-ms to 180.

ht (ESC/~ hold-tap):
  flavor                  tap-preferred
  tapping-term-ms         200
  quick-tap-ms            175

td_lbrc / td_rbrc / td_sym:
  tapping-term-ms         200

td_sym behavior change (Stage 7):
  1× hold = &mo SYM (momentary, unchanged)
  2× tap  = &num_word SYM (was &to SYM lock)
  Numword auto-exits on non-number key — no manual unlock needed.

swapper (Alt-Tab, Stage 7):
  tri-state: holds Alt, taps Tab repeatedly, releases Alt on layer exit.
  ignored-key-positions = arrows (20,31,32,33) for task-switcher nav.
  timeout-ms = 5000 (safety auto-release).
```

If Max reports HRM still firing during typing, bump `tapping-term-ms`
and/or `require-prior-idle-ms` by 25-40ms. If it's not firing when wanted,
go the other direction. Update both `hml` and `hmr` together.

---

## What's deferred

Auto-shift dropped from scope entirely.

Remaining deferred items:

1. **HRM timing tighten** — `tapping-term-ms` 260→200, `require-prior-idle-ms`
   200→150. Do this when Max confirms muscle memory is solid on Colemak-DH
   AND the Stage 9 punctuation rework has settled.
   Update both `hml` and `hmr` together.

2. **Auto-mouse-layer** — activate Mouse layer automatically on trackball motion
   (ZMK `CONFIG_ZMK_POINTING_BEHAVIOR_AUTO_LAYER`). Revisit after daily-driving
   the current manual mouse setup for a while.

3. **More combos** — clipboard is now on NAV (Ctrl+Z/X/C/V), so the original
   combo proposal for those is resolved. Stage 8/9 deferred items:
   - Extend the S+V Ctrl+V combo pattern to S+X / S+C / S+D / S+A.
   - `=>` combo on SYM pos 30+31 (audit Issue 6 — same-finger bigram on
     right index columns). Pos 30 SYM is currently `&none` (freed in
     Stage 9), so a combo there is unobstructed.

4. **Repeat key (`&key_repeat`)** — audit option B2. Candidate slots: SYM
   pos 30 (free post Stage 9), or NAV pos 22 (replacing the dubious INS).

### Stage 9 follow-up — KeyPeek IDs stale

The Stage 9 behaviors-block edit (mcs/mdc removed, mscl added) means every
custom-behavior ID has shifted. After flashing Stage 9 firmware, open
KeyPeek, watch the overlay render `0x..` hex strings for custom behaviors,
and update the const values in
`F:\keypeek-crosses42\src\zmk_keycode_labels\crosses42.rs`. Order in the
behaviors block (post Stage 9): hml, hmr, ht, sht, bspc_del, mscl, td_sym,
swapper.

### KeyPeek gaps to revisit

KeyPeek reads layer state directly via Raw HID, so the entire OverKeys
F-key shim is gone. Some sub-layer states are still not surfaced:

- HRM held-state is not visualized (no event for "L-shift held via T").
- Tap-dance state mid-sequence (e.g. one tap of `{[<` waiting to see if
  another tap comes) is not visualized — KeyPeek shows the layer, not
  pending behavior state.
- Mod-morph state (BSPC→DEL with Shift held) shows as plain BSPC.

These are protocol limits in zmk-keypeek-layer-notifier — it forwards
layer-state changes, not behavior-engine internals. If we ever want HRM
or mod-morph indication, that's a feature request upstream (or we'd add
a custom event listener in a separate module).

---

## Communication norms with Max

- Stop after each stage; let Max report hardware results before starting
  the next.
- Don't bundle multiple stages even if it seems faster.
- Loose HRM timing (260/200) is the active learning-phase preset; tighten
  to 200/150 only when Max says he's ready.
- Small visual checks (grid printer, SVG render) before sending him to
  flash, not after. Saves a flash cycle when something's obviously off.
- When something's mysteriously broken, prefer empirical diagnosis (USB
  logging, nRF Connect phone scan) over speculation.

---

## Quick reference

**Key positions (forty_two_layout, row-major):**

```
0  1  2  3  4  5         6  7  8  9 10 11    ← row 1 (top alphas)
12 13 14 15 16 17       18 19 20 21 22 23    ← row 2 (home + HRM)
24 25 26 27 28 29       30 31 32 33 34 35    ← row 3 (bottom + brackets)
            36 37 38    39 40 41             ← thumbs
```

Hand groupings (used by HRM positional triggers):

```
LEFT_HAND_KEYS  = 0..5, 12..17, 24..29, 36..38
RIGHT_HAND_KEYS = 6..11, 18..23, 30..35, 39..41
```
