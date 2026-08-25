# Crosses 42 — Stage 2 Notes

## What's new vs Stage 1

| Feature | Spec ref | Implementation |
|---|---|---|
| Home Row Mods (GACS, mirrored) | §3, §4 | `hml` / `hmr` hold-taps with `hold-trigger-key-positions` set to opposite hand's keys |
| ESC / ~ | §4 ESC/~ | Custom hold-tap `ht` (because hold sends a non-mod keycode) |
| SPC / ⇧ | §4 SPC/⇧ | Plain `&mt LSHFT SPACE` |
| ENT / ⌃ | §4 ENT/CTRL | Plain `&mt LCTRL RET` |
| BSPC → DEL | §4 BSPC | `bspc_del` mod-morph, Shift consumed |

Tuning starting points (matches `PROJECT_HANDOFF.md` §7, will iterate):

- HRM `tapping-term-ms` = 200ms — hold threshold
- HRM `quick-tap-ms` = 175ms — rapid re-press skips hold
- HRM `require-prior-idle-ms` = 150ms — keys must be idle this long before HRM activates (so HRM doesn't fire mid-word)
- HRM flavor = `balanced` — fast nested press = hold; otherwise tap heuristic decides
- HRM `hold-trigger-on-release` — mod is committed only when the *next* key releases, allowing roll detection
- ESC/~ uses a separate hold-tap `ht` because the hold target (TILDE) is a keycode, not a modifier

## What's still NOT in this stage (deferred)

- Tap-dance brackets (`{[<` / `}]>`) — bottom outer columns are still LSHFT/RSHFT
- SYM-lock thumb (double-tap to lock) — SYM thumb is still plain momentary `&mo SYM`
- Auto-shift on alphas/symbols
- Caps Word
- Mouse / UKR / Game layers

## Build & flash

Use your usual fish loop, but **left no longer needs the `zmk-usb-logging` snippet** since pairing works:

```fish
for shield in crosses_left crosses_right
    west build --pristine -s zmk/app -b nice_nano \
        -- -DZMK_CONFIG=/mnt/f/zmk-crosses-42/config \
           -DSHIELD=$shield \
           -DSNIPPET=studio-rpc-usb-uart \
           -DCONFIG_ZMK_STUDIO=y \
           -DCONFIG_ZMK_STUDIO_LOCKING=n
    cp build/zephyr/zmk.uf2 /mnt/f/zmk-crosses-42/$shield.uf2
end
```

(Studio on both halves is fine — it's what was working before. We only deviated to `zmk-usb-logging` for diagnostic. Your pre-Stage-1 loop is back to being correct.)

Flash sequence: just drag each UF2 onto its half via bootloader. **No settings_reset needed** — we're not changing layer count or fundamental structure, so existing NVS/pairing keeps working.

## Hardware test checklist

Tap each test in order and stop on the first thing that's wrong.

### 1. Home row tap = letters (HRM doesn't fire on quick taps)

- Type `arstg mneio` quickly — should produce exactly that. If anything types as a modifier (e.g. caps, weird shortcut firing), HRM `tapping-term-ms` is too short — bump to 220-240.

### 2. Home row hold = mod activates on opposite hand

- Hold `T` (left pinky-side, T = LSHFT), tap `n` → should produce **N** capitalized.
- Hold `N` (right index-side, N = RSHFT), tap `t` → should produce **T** capitalized.
- Hold `S` (left, CTRL), tap `f` → Ctrl+F (find dialog in most apps).
- Hold `E` (right, CTRL), tap `s` → Ctrl+S (save).
- Hold `A` (left, GUI), tap `l` → Win+L (lock screen — be ready!).

### 3. Same-hand HRM should NOT fire

- Hold `T` (left, LSHFT), tap `g` (also left) → should produce **tg** (lowercase), NOT shifted G. If it shifts, the positional trigger isn't working.

### 4. ESC vs ~ on the home outer-left

- Tap quickly → ESC (no visible character; check Vim or a REPL).
- Hold ~200ms → `~` appears.

### 5. SPC vs ⇧ on the left inner thumb

- Tap → space.
- Hold + tap a letter → that letter capitalized.
- Hold alone (long press) → nothing visible (just shift held), no spaces should leak.

### 6. ENT vs ⌃ on the right middle thumb

- Tap → newline.
- Hold + tap a letter → Ctrl+letter (e.g. hold + `c` = Ctrl+C copy, hold + `s` = Ctrl+S save).

### 7. BSPC → DEL morph

- Tap thumb → backspace as expected.
- Hold either Shift (could be the bottom-outer LSHFT/RSHFT, or HRM T/N) + tap thumb → forward delete (deletes the character TO THE RIGHT of cursor).
- Important: it should NOT produce Shift+BSPC (some apps interpret that as a different action) — Shift is consumed.

### 8. Layers still work

- Hold left inner thumb → SYM layer; type `qwfpb` → produces `12345`.
- Hold right inner thumb → NAV layer; tap home row → arrows up/down/etc.

### 9. Trackballs still work

Move the right trackball → cursor moves. Move the left trackball → page scrolls. (Unchanged from Stage 1, just confirming nothing regressed.)

## If something feels off

- HRM firing during normal typing → `tapping-term-ms` too low or `require-prior-idle-ms` too low. Bump both by 25-50ms.
- HRM not firing when you actually want it → `tapping-term-ms` too high. Lower it.
- ESC/~ feels sticky → `ht` `tapping-term-ms` too high; lower to 175.
- SPC/⇧ producing extra spaces → `&mt` defaults are sometimes too aggressive on tap; we may need to convert it to the same `ht` custom hold-tap with `flavor = "balanced"`.

Tell me which test fails (if any) and we'll tune. If everything passes we move to Stage 3 (tap-dance brackets + SYM lock).
