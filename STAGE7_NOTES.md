# Stage 7 — Layer UX Overhaul

**Date:** 2026-05-10
**Status:** Code landed, hardware test pending

## What changed

### SPC/⇧ rolling fix
- **Problem:** Fast typing rolled Space into the next letter, triggering Shift instead of Space. Root cause: `&mt LSHFT SPACE` uses `hold-preferred` flavor — any key press during the tapping term activates the hold.
- **Fix:** New `sht` (space_hold_tap) behavior with `balanced` flavor, `tapping-term-ms=200`, `require-prior-idle-ms=150`, `quick-tap-ms=175`. With balanced, the next key must be pressed *and released* while Space is held — rolling naturally produces Space, deliberate Shift still works.

### NAV layer overhaul
- **Left home row (pos 13-16):** Was `LCTRL / LALT / LGUI / LSHFT` (duplicated HRM on BASE, nearly useless). Now: **Undo (Ctrl+Z) / Cut (Ctrl+X) / Copy (Ctrl+C) / Paste (Ctrl+V)**. One-hand clipboard while holding NAV.
- **Pos 17 (left inner column home):** Was `&trans`. Now: **Alt-Tab swapper** (zmk-tri-state module). Tap once to open task switcher, tap again to cycle, release NAV to confirm.
- **Left thumbs (pos 36-38):** Was `PSCRN / &trans / C_VOL_DN`. Now: **MB1 / MB2 / MB3** mouse buttons for quick trackball clicks while holding NAV.
- **Pos 22 (right home outer):** Was `&trans`. Now: **PrintScreen** (moved from old left thumb).
- **Pos 35 (right bottom outer):** Was `&bootloader` (dangerously easy to hit accidentally). Now: **Play/Pause**. Bootloader moved to a combo (see below).
- **Pos 41 (right outer thumb):** Was `C_MUTE` (duplicate of pos 34). Now: **Vol Down** to pair with Vol Up at pos 40.

### Combos (new section in keymap)
- **Caps Word combo (pos 36+37, BASE layer):** Press BSPC + SPC thumbs simultaneously → Caps Word. Natural two-thumb gesture, no need to enter NAV first.
- **Bootloader combo (pos 0+11, NAV layer):** Press both outer top corners (F12+F11) while holding NAV → bootloader. Intentionally hard to trigger — requires holding NAV plus two far-apart keys.

### Numword (zmk-auto-layer module)
- `td_sym` double-tap changed from `&to SYM` (manual lock) to `&num_word SYM` (auto-layer). Stays active while typing numbers/BSPC/DEL/DOT/COMMA/math operators. Auto-exits on any alpha key. Eliminates manual SYM lock/unlock cycle for number entry.

### Alt-Tab swapper (zmk-tri-state module)
- New `swapper` behavior at NAV pos 17 (G position on BASE). First tap holds Alt + sends Tab, subsequent taps send Tab. Arrow keys (pos 20, 31, 32, 33) are in `ignored-key-positions` so you can navigate the Windows task switcher. 5-second safety timeout. Alt releases when NAV layer deactivates.

## New module dependencies (west.yml)
- `urob/zmk-auto-layer` (main) — provides `&num_word` behavior
- `dhruvinsh/zmk-tri-state` (main) — provides `zmk,behavior-tri-state`

## Test checklist
- [ ] Build compiles with both new modules (west update + build)
- [ ] Space key types Space during fast typing (no false Shift)
- [ ] Hold Space + type letter = Shift works
- [ ] Double-tap Space = two spaces (quick-tap-ms)
- [ ] Hold NAV + press left thumb = MB1/MB2/MB3 clicks
- [ ] Hold NAV + A/R/S/T = Undo/Cut/Copy/Paste
- [ ] Hold NAV + G = Alt-Tab swapper opens task switcher
- [ ] Tap swapper again = cycles windows
- [ ] Release NAV = confirms window selection (Alt releases)
- [ ] Hold NAV + arrow keys during swapper = navigate task switcher
- [ ] Hold NAV + right bottom outer = Play/Pause
- [ ] Hold NAV + right outer thumb = Vol Down
- [ ] PrintScreen on NAV (pos 22, right of Ins) works
- [ ] Double-tap ▽SYM = Numword activates (SYM layer)
- [ ] Type numbers in Numword = stays in SYM
- [ ] Type a letter in Numword = exits back to BASE
- [ ] Press BSPC + SPC simultaneously = Caps Word activates
- [ ] Type HELLO_WORLD in Caps Word = correct (auto-exits on space)
- [ ] Bootloader combo: hold NAV + press F12+F11 simultaneously = bootloader
- [ ] Normal F12 and F11 presses on NAV still work individually
- [ ] UKR layer still works (thumbs &trans → BASE thumbs pass through)
- [ ] UKR pos 24 (&trans) falls through to LCTRL — Ctrl+C/V works in Ukrainian
- [ ] Game layer still works (pure QWERTY, no interference from combos)
- [ ] KeyPeek overlay shows correct layer state for all transitions

### Stage 7b — BASE outer columns + Mouseless (same build)
- [ ] pos 24 (left bottom outer) = LCTRL works as Ctrl modifier
- [ ] Hold LCTRL (pos 24) + Z = Undo (same-hand)
- [ ] Hold LCTRL (pos 24) + S = Save (same-hand, fixes HRM gap)
- [ ] Hold LCTRL (pos 24) + C/X/V = Copy/Cut/Paste (same-hand)
- [ ] pos 35 (right bottom outer) = backslash types \
- [ ] Shift + pos 35 = pipe | (standard shifted backslash)
- [ ] D+V combo (pos 28+29) = sends ScrollLock (verify with key tester)
- [ ] K+H combo (pos 30+31) = sends Pause/Break (verify with key tester)
- [ ] Mouseless: configure "show overlay" → ScrollLock in settings
- [ ] Mouseless: configure "toggle free mode" → Pause/Break in settings
- [ ] Mouseless: D+V combo triggers overlay
- [ ] Mouseless: K+H combo triggers free mode
- [ ] KeyPeek: mod-morph labels show ",;" and ".:" after ID re-read
- [ ] KeyPeek: BSPC shows "⌫⌦" after ID re-read
- [ ] KeyPeek: swapper shows "Alt⇥" after ID re-read
- [ ] KeyPeek: re-read ALL behavior IDs from overlay (td_lbrc/td_rbrc removed, sht/swapper added — all IDs shifted)
