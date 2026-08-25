# Stage 8 — Layer UX overhaul (consistency pass)

**Goal**: address the five UX pain points Max surfaced after living with Stage
7 for a while. This is a *consistency* stage: same character lives on the
same finger across layers, base layer matches Max's prior muscle memory, and
the most-misfiring shortcut (Ctrl+V) gets a dedicated combo.

## Pain points addressed

1. **Symbol-finger inconsistency.** `;`, `:`, `?`, `"` jumped one column
   outward when moving BASE → SYM. Now column-aligned with BASE.
2. **`'` ↔ `,;` swap on BASE.** Apostrophe moves to pos 32 (middle-bottom)
   to match Max's prior keyboard. `,;` (mcs) moves to pos 23 (outer-pinky
   home row).
3. **Ctrl+V → "sv" misfire.** Adding a `combo_ctrl_v` on positions 14+29
   (S+V). Combos take precedence over hold-tap timing, so 40ms-window
   simultaneous press fires Ctrl+V reliably regardless of HRM positional
   rule. HRM `hml LCTRL S` is *not* removed — it still works for opposite-
   hand Ctrl+letter shortcuts.
4. **NAV pos 27/28/29 idle.** Filled with `Ctrl+F` / `Ctrl+S` / `Ctrl+Y`
   (Find / Save / Redo). Pairs with the clipboard cluster on the row above.
5. **SYM number-row mismatch + KeyPeek dual-glyph clutter.**
   - Number-row Shift partners now match US convention: pos 20/21/22 →
     `*`/`(`/`)` (was `(`/`)`/`*`).
   - KeyPeek fork patched to render single-glyph labels for
     LBKT/RBKT/BSLH/MINUS/EQUAL/COMMA/PERIOD/SLASH/SEMI/SQT/GRAVE — so
     SYM positions show just `[`, `]`, `\`, `;`, `'`, ... without the QMK
     keycap-style shifted twin (`{\n[`, `}\n]`, etc).

## SYM layer — full layout (Getreuer-inspired, Sub-B)

```
Row 1 (0-11):   `   1   2   3   4   5     6   7   8   9   0   BSPC
Row 2 (12-23):  ~   !   @   #   $   %     ^   &   *   (   )   ;
Row 3 (24-35):  _   [   ]   {   }   <     >   =   "   :   ?   \
Thumbs:        trans trans toBASE         trans trans trans
```

### Why this arrangement

- **Brackets clustered on left hand row 3** (`[ ] { }` at pos 25-28).
  - `[]` is an inward roll (ring → middle).
  - `{}` is an inward roll (index-outer → index-inner).
  - `(` and `)` stay on right (pos 21/22) where Shift+9 and Shift+0 are on
    a standard layout — familiar muscle memory, also matches what you'd
    type via SYM-Shift on the digit row.
- **`<>` as a symmetric cross-hand pair** (pos 29 left, pos 30 right).
  Index-extension columns mirror each other across the split, so typing
  `<>` is a fast hand-to-hand roll.
- **`=` at right-index home position** (pos 31). Getreuer's signature
  placement: makes `==`, `!=`, `<=`, `=>` all comfortable.
- **`;`, `"`, `:`, `?`, `\` columns match BASE** (pos 23, 32, 33, 34, 35).
  This is the *core* consistency fix — every shifted-on-BASE character
  occupies the same column on SYM.
- **`+` and `-` are not duplicated on SYM.** `+` is Shift+`=` (pos 10 on
  BASE), `-` is at pos 11 on BASE directly. No need to also place them on
  SYM — saves slots for brackets.
- **No standalone `/` on SYM.** `/` is at pos 34 on BASE; pos 34 on SYM
  is `?` (the shift partner). Saves another slot.

## BASE layer — changes

```
Row 2 R (pos 18-23):  M    N    E    I    O    ,;(mcs)
Row 3 R (pos 30-35):  K    H    '    .:(mdc) /   \
```

Only changes from Stage 7:
- pos 23: was `&kp SQT` → now `&mcs`
- pos 32: was `&mcs` → now `&kp SQT`

Left hand unchanged.

## NAV layer — changes

```
Row 3 L (pos 24-29):  tog UKR  tog GAME  tog MOUSE  LC(F)  LC(S)  LC(Y)
```

- pos 27 (Find): was `&trans`, now `&kp LC(F)`
- pos 28 (Save): was `&trans`, now `&kp LC(S)`
- pos 29 (Redo): was `&trans`, now `&kp LC(Y)` — Windows convention

Combined with existing NAV home-row clipboard (`LC(Z)` / `LC(X)` / `LC(C)` /
`LC(V)` at pos 13-16), the left hand on NAV now covers all the everyday
editing shortcuts in a single hold.

## Ctrl+V combo

```dts
combo_ctrl_v {
    timeout-ms = <40>;
    key-positions = <14 29>;   // S + V
    bindings = <&kp LC(V)>;
    layers = <BASE>;
    require-prior-idle-ms = <150>;
};
```

Press S and V within 40ms of each other on BASE → Ctrl+V fires. The
`require-prior-idle-ms = 150` gate prevents the combo from triggering during
fast typing bursts (no English bigram "sv" in normal prose anyway).

This sits alongside, not replacing:
- `hml LCTRL S` (pos 14) — still works for opposite-hand Ctrl+letter.
- LCTRL at pos 24 — still the long-tail Ctrl key for everything else.

If Ctrl+V via S+V combo works well, we can extend the pattern in Stage 9 to
S+X / S+C / S+D / S+A for the rest of the clipboard cluster.

## KeyPeek fork patch

Edit `keypeek-crosses42/src/zmk_keycode_labels/keycode_label.rs` to flatten
these dual-glyph labels:

| Keycode        | Stage 7 label | Stage 8 label |
|----------------|---------------|---------------|
| MINUS          | `_\n-`        | `-`           |
| EQUAL          | `+\n=`        | `=`           |
| LEFT_BRACKET   | `{\n[`        | `[`           |
| RIGHT_BRACKET  | `}\n]`        | `]`           |
| BACKSLASH      | `\|\n\\`      | `\`           |
| SEMICOLON      | `:\n;`        | `;`           |
| SINGLE_QUOTE   | `"\n'`        | `'`           |
| GRAVE          | `~\n``        | `` ` ``       |
| COMMA          | `<\n,`        | `,`           |
| PERIOD         | `>\n.`        | `.`           |
| SLASH          | `?\n/`        | `/`           |

After rebuilding the firmware, KeyPeek behavior IDs will also need a refresh
(td_sym, mcs, mdc, etc.) — see the STALE comment block in `crosses42.rs`.
Read the fresh `0x..` codes off the overlay and update the consts.

## Test checklist

- [ ] BASE typing — natural English prose, watch for `,` vs `;` (mcs at
      pos 23 now).
- [ ] BASE `'` at pos 32 — type `don't`, `it's`, etc.; should feel right
      if it matches your prior keyboard's muscle memory.
- [ ] SYM `;` `:` `?` `"` finger consistency — type `:`, `?`, `"` while
      holding SYM and confirm fingers don't slide.
- [ ] SYM bracket cluster — type `[]`, `{}`, `()`, `<>` and verify the
      inward-roll feel on left hand for `[]` and `{}`.
- [ ] SYM `=` at right-index home — type `==`, `!=`, `=>`.
- [ ] NAV Find/Save/Redo — hold NAV, press C/D/V; expect Ctrl+F, Ctrl+S,
      Ctrl+Y.
- [ ] Ctrl+V combo — press S+V simultaneously on BASE; should paste.
      Then type a sentence with "sv" never appearing in it and confirm
      no false fires (it shouldn't, English bigram is too rare).
- [ ] KeyPeek labels — SYM positions should show single-glyph (`]` not
      `}]`, `[` not `{[`, `;` not `:;`).

## Deferred to Stage 9+

- Extend Ctrl+V combo pattern to S+X/S+C/S+D/S+A if Stage 8 combo proves
  reliable.
- Move Mouse layer combos if the new BASE layout makes them inconvenient.
- HRM timing tighten (260/200 → 200/150) once Stage 8 settles.
