# Crosses 42 — Stage 1 Notes

## What changed in this stage

1. **Added the missing `chosen` block** to `config/crosses.keymap`. This is the
   one-line fix that explains the scrambled flash you saw before:

   ```dts
   chosen {
       zmk,physical-layout = &gggw_crosses_42_layout;
   };
   ```

   Without it, the upstream shield's `crosses.dtsi` defaults to the **54-key**
   layout, which expects 54 bindings/layer. Your 42 bindings were being mapped
   into the upper rows of a 54-position matrix, so every key landed on the
   wrong row and the thumbs never bound at all.

2. **Stripped the keymap down to plain bindings.** No HRM, no auto-shift, no
   tap-dance, no mod-morph, no mod-tap (beyond a couple of bare fallbacks).
   Goal: prove the foundation works before re-introducing timing-sensitive
   behaviors that themselves can produce confusing symptoms.

3. **Slimmed `config/crosses.conf`** to the minimum required for trackballs +
   BLE + battery reporting + sleep + USB.

4. **Backups** of your previous keymap and conf were saved next to the new
   files as `crosses.keymap.bak` / `crosses.conf.bak`.

## Stage-1 layout (what's actually flashed)

Reflects the literal binding order in `config/crosses.keymap`. Verified against
the spec from `PROJECT_HANDOFF.md` §3-§5. Positions follow
`gggw_crosses_42_layout`'s row-major order.

```
══ Base ══
┌─────┬─────┬─────┬─────┬─────┬─────┐    ┌─────┬─────┬─────┬─────┬─────┬─────┐
│ TAB │  Q  │  W  │  F  │  P  │  B  │    │  J  │  L  │  U  │  Y  │  =  │  -  │
│ ESC │  A  │  R  │  S  │  T  │  G  │    │  M  │  N  │  E  │  I  │  O  │  '  │
│LSHFT│  Z  │  X  │  C  │  D  │  V  │    │  K  │  H  │  ,  │  .  │  /  │RSHFT│
└─────┴─────┴─────┴─────┴─────┴─────┘    └─────┴─────┴─────┴─────┴─────┴─────┘
                  │BSPC │SPACE│mo SY│    │mo NV│ RET │ LGUI│

══ Sym (hold right-most thumb of left half) ══
┌─────┬─────┬─────┬─────┬─────┬─────┐    ┌─────┬─────┬─────┬─────┬─────┬─────┐
│  `  │  1  │  2  │  3  │  4  │  5  │    │  6  │  7  │  8  │  9  │  0  │BSPC │
│  ~  │  !  │  @  │  #  │  $  │  %  │    │  ^  │  &  │  (  │  )  │  *  │  +  │
│  _  │  \  │  [  │  {  │  <  │  |  │    │  >  │  }  │  ]  │  ;  │  :  │  ?  │
└─────┴─────┴─────┴─────┴─────┴─────┘    └─────┴─────┴─────┴─────┴─────┴─────┘

══ Nav (hold left-most thumb of right half) ══
┌─────┬─────┬─────┬─────┬─────┬─────┐    ┌─────┬─────┬─────┬─────┬─────┬─────┐
│ F12 │ F1  │ F2  │ F3  │ F4  │ F5  │    │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │
│  ·  │CTRL │ ALT │ GUI │SHFT │  ·  │    │PG↑  │HOME │  ↑  │ END │ INS │  ·  │
│  ·  │  ·  │  ·  │  ·  │  ·  │  ·  │    │PG↓  │  ←  │  ↓  │  →  │MUTE │BOOT │
└─────┴─────┴─────┴─────┴─────┴─────┘    └─────┴─────┴─────┴─────┴─────┴─────┘
                  │PSCRN│  ·  │ V-  │    │  ·  │ V+  │MUTE │
```

## Hardware test checklist

Build + flash both halves. Then verify, in order:

1. **Every alpha key types its label.** Walk the home row left to right
   (`A R S T G M N E I O`) and the top row (`Q W F P B J L U Y`). If anything
   is shifted by a row or shows up on a different physical key, stop — the
   matrix is still wrong.

2. **Bottom-row outer columns are LSHFT and RSHFT** (placeholder during this
   stage). Hold either, type `a` → uppercase A. Tap-dance brackets come back
   in Stage 4.

3. **Both thumb cluster halves work.** Inner-of-left thumb = momentary SYM;
   inner-of-right thumb = momentary NAV.

4. **SYM layer numbers and symbols.** Hold left inner thumb, tap `q w f p b`
   → produces `1 2 3 4 5`.

5. **NAV layer arrows.** Hold right inner thumb, tap `n e i o → up/down/etc.

6. **Both trackballs still work** (right=cursor, left=scroll). The
   trackball overlay wasn't touched, so this should be unchanged from last
   build.

7. **Bootloader access** (`NAV` + bottom-right-rightmost) — useful if a future
   stage misbehaves and you need to recover.

## What's coming next (Stage 2)

Once Stage 1 is verified on hardware, we'll re-introduce timing-sensitive
behaviors:

- Home row mods (GACS), with `hold-trigger-key-positions` set to opposite-hand
  keys only — so accidental same-hand activation is suppressed.
- Mod-taps: ESC/~, SPC/⇧, ENT/CTRL.
- Mod-morph BSPC → Del when Shift held.

After that: Stage 3 = tap-dance brackets + SYM lock, Stage 4 = auto-shift +
Caps Word, Stage 5 = Mouse/UKR/Game layers.
