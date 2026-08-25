# Stage 9 — A3 + B1 (drop mod-morphs, dedicated ;: key)

**Goal**: act on the BASE-punctuation UX issues surfaced in the 2026-05-12
layer audit (`Wireless Crosses\LAYER_UX_AUDIT.md`). Max picked audit option
A3 (apostrophe at pos 23, comma at pos 32, no mod-morphs) plus B1 (`;:`
mod-morph at pos 35, replacing `\|`). Bundled as a single stage at Max's
request, contrary to the usual one-stage-at-a-time cadence.

## Pain points addressed

1. **Comma muscle-memory mismatch.** Stage 8 swapped `,;` to outer-pinky
   home and `'` to middle-bottom, citing a previous-keyboard muscle memory.
   In practice Max kept reaching for `,` at pos 32 (the QWERTY position) and
   getting `'`. Stage 9 puts comma back at pos 32 and apostrophe at pos 23
   — which also aligns with Miryoku / urob convention (apostrophe is more
   frequent than semicolon, so it deserves the pinky-home slot).
2. **Over-allocated BASE pos 35.** `\|` was rare in daily typing. Replaced
   with a new `;:` mod-morph (`mscl`) — `;` is more frequent than `\` and
   benefits from a direct key. `\` still reachable on SYM (relocated to
   pos 29, the bracket cluster).
3. **mcs/mdc mod-morph cleanup.** With apostrophe and comma plain on BASE,
   the Shift+key path now produces standard QWERTY shifts (`"`, `<`, `>`)
   instead of custom `;` / `:`. mcs and mdc are deleted from the behaviors
   block — `;` and `:` are reached via the new mscl at pos 35.

## BASE layer — punctuation changes

| Pos | Stage 8         | Stage 9       | Tap → Shift |
|-----|-----------------|---------------|-------------|
| 23  | mcs (`,;`)      | SQT plain     | `'` → `"`   |
| 32  | SQT (`'"`)      | COMMA plain   | `,` → `<`   |
| 33  | mdc (`.:`)      | DOT plain     | `.` → `>`   |
| 34  | FSLH (`/?`)     | FSLH plain    | `/` → `?` (unchanged) |
| 35  | BSLH (`\|`)     | mscl (`;:`)    | `;` → `:`   |

`<` and `>` are now Shift-reachable on BASE (Shift+pos-32, Shift+pos-33).
This is *new* — pre-Stage-9 they were SYM-only. Net win for HTML/generics
typing without entering SYM.

## SYM layer — column-alignment maintained against new BASE

The locked column rule (shifted-on-BASE = same column on SYM at pos
23/32/33/34/35) is preserved:

| Pos | Stage 8 | Stage 9 | Mirrors BASE Shift |
|-----|---------|---------|--------------------|
| 23  | `;`     | `"`     | SQT (pos 23 BASE)  |
| 32  | `"`     | `<`     | COMMA (pos 32 BASE) |
| 33  | `:`     | `>`     | DOT (pos 33 BASE)  |
| 34  | `?`     | `?`     | FSLH (unchanged)   |
| 35  | `\`     | `:`     | mscl (pos 35 BASE)  |

Backslash relocates from SYM pos 35 → SYM pos 29 (was `<`, now the escape
char joining the bracket cluster on left hand).

### Stage 9 follow-up — `.` on SYM pos 33 (typing `1.5.0` without exiting)

Max requested direct `.` access on SYM for version numbers and decimals.
Applied:

- SYM pos 33: `>` → `.` (BASE-tap mirror, exception to the column rule)
- SYM pos 30: `&none` → `>` (relocated so `>` is still directly tappable)

Why this is OK: `>` is still reachable two ways on SYM (pos 30 direct, or
Shift+pos-33 since Shift+`.` = `>` via the OS). Number-typing in Numword
mode works because `.` is in `num_word`'s `continue-list` (defined in
`zmk-auto-layer/dts/behaviors/num_word.dtsi`) — the layer stays active
through `.`.

Column-alignment rule for pos 33 is now BASE-TAP mirror, not BASE-SHIFT.
This is a deliberate exception. Other locked columns (23/32/34/35) still
follow BASE-shift mirror.

## SYM layer — full layout (Stage 9, final)

```
Row 1 (0-11):   `   1   2   3   4   5     6   7   8   9   0   BSPC
Row 2 (12-23):  ~   !   @   #   $   %     ^   &   *   (   )   "
Row 3 (24-35):  _   [   ]   {   }   \     >   =   <   .   ?   :
Thumbs:        trans trans toBASE         trans trans trans
```

Compare to Stage 8:
- Row 2 pos 23: `;` → `"`
- Row 3 pos 29: `<` → `\` (escape char joins bracket cluster)
- Row 3 pos 30: `>` → `>` (kept after follow-up; was briefly `&none`)
- Row 3 pos 32: `"` → `<`
- Row 3 pos 33: `:` → `.` (after follow-up; was briefly `>`)
- Row 3 pos 35: `\` → `:`

## UKR layer impact — Ґ path shifts

Ukrainian Windows IME maps Shift+BSLH → Ґ. Pre-Stage-9, BSLH was on
SYM pos 35, so the Ґ path was:

> Hold SYM (left inner thumb) + hold Shift (via SPC⇧ thumb fall-through)
> + tap pos 35.

Post-Stage-9, BSLH is on SYM pos 29. New Ґ path:

> Hold SYM + hold Shift + tap pos 29.

Mechanically identical, just a different finger position. UKR layer
bindings themselves are unchanged (pos 35 UKR = RBKT = Ї, unaffected by
BASE pos 35 changing to mscl).

Worth a hardware-test note: confirm Ґ still produces correctly. Worth
remembering: home-row T/N on UKR are plain `&kp Y` / `&kp J` (no HRM), so
the Shift source on UKR is *only* the SPC⇧ thumb fall-through.

## Behaviors block — diff

Removed:
```dts
mcs: comma_semi {
    compatible = "zmk,behavior-mod-morph";
    #binding-cells = <0>;
    bindings = <&kp COMMA>, <&kp SEMI>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};

mdc: dot_colon {
    compatible = "zmk,behavior-mod-morph";
    #binding-cells = <0>;
    bindings = <&kp DOT>, <&kp COLON>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};
```

Added:
```dts
mscl: semi_colon {
    compatible = "zmk,behavior-mod-morph";
    #binding-cells = <0>;
    bindings = <&kp SEMI>, <&kp COLON>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};
```

**Label-naming gotcha:** The natural short name `msc` (matching the `mcs`/`mdc`
pattern) collides with upstream ZMK's `mouse_scroll` behavior, which is
labeled `msc` and used on the MOUSE layer (`&msc SCRL_UP` etc.). The build
fails with `devicetree error: Label 'msc' appears on /behaviors/mouse_scroll
and on /behaviors/semi_colon`. Resolved by renaming the custom label from
`msc` to `mscl`. Future custom mod-morphs should grep against upstream
behavior labels before picking a name.

Net: two behaviors removed, one added. Behavior IDs assigned by ZMK Studio
will shift for every other custom behavior in the block — KeyPeek fork
needs all IDs re-read after rebuild (see KeyPeek section below).

## KeyPeek fork updates

In `F:\keypeek-crosses42\src\zmk_keycode_labels\crosses42.rs`:

- `MCS` and `MDC` consts and their match arms removed.
- New `MSCL` const + match arm rendering label `;:`.
- All other custom-behavior IDs (HML / HMR / HT / SHT / BSPC_DEL / TD_SYM /
  SWAPPER) are now stale — the behaviors-block shape changed, so ZMK Studio
  will reassign IDs at build time. Placeholder IDs left in place with TODO
  comments; Max needs to read fresh IDs from the KeyPeek overlay after the
  Stage 9 firmware rebuild and patch the consts.

Single-glyph label patches from Stage 8 (`keycode_label.rs`) are unaffected
— they're keycode-level, not behavior-level. COMMA/DOT/SEMI/SQT etc. will
continue to render as single glyphs.

## Test checklist (hardware verification)

After flash:

1. **Apostrophe.** Tap pos 23 → `'`. Shift+pos-23 → `"`.
2. **Comma.** Tap pos 32 → `,`. Shift+pos-32 → `<`.
3. **Dot.** Tap pos 33 → `.`. Shift+pos-33 → `>`.
4. **Slash.** Tap pos 34 → `/`. Shift+pos-34 → `?`. (Unchanged from Stage 8.)
5. **Semi/colon.** Tap pos 35 → `;`. Shift+pos-35 → `:`.
6. **Backslash on SYM.** Hold SYM thumb, tap pos 29 → `\`. Shift → `|`.
7. **SYM pos 30 = `>`.** Hold SYM, tap pos 30 → `>`.
8. **SYM pos 33 = `.` (follow-up).** Hold SYM, tap pos 33 → `.`.
   Shift+SYM+pos-33 → `>` (Shift+. via OS).
9. **Version-number typing.** Double-tap td_sym (Numword), type `1.5.0`
   without exiting — all five characters emit correctly (Numword stays
   active because `.` is in num_word's continue-list).
10. **Column alignment spot-check.** Hold SYM, type pos 23/32/35 → see
    `"` / `<` / `:` respectively. (Pos 33 is now an intentional exception:
    SYM pos 33 mirrors BASE-tap `.`, not BASE-shift `>`.)
11. **`<` and `>` on BASE Shift.** Shift+pos-32 → `<`, Shift+pos-33 → `>`.
12. **UKR Ґ.** Toggle UKR (NAV → tog UKR), switch Windows IME to Ukrainian,
    hold SYM + hold SPC⇧ thumb + tap pos 29 → should emit Ґ.
13. **All Stage 8 mechanics still work.** Numword, Caps Word combo, S+V
    combo, swapper, bootloader combo, Mouseless combos (D+V, K+H).
14. **HRM untouched.** Home-row mods on both sides still produce GACS as
    expected. Same-hand positional rule still in force.
15. **KeyPeek overlay.** After updating behavior IDs, mscl should render
    as `;:` label. mcs/mdc labels should no longer appear anywhere.

## Risk surface

- **Bundled stage breaks usual cadence.** If anything misfires, bisection
  between the four BASE-position changes is harder. Max accepted this
  trade-off explicitly.
- **Lost punctuation density on Shift.** `;` and `:` no longer reachable
  via Shift+pos-23 and Shift+pos-33. Both now live solely at pos 35 (mscl).
  If muscle memory keeps reaching Shift+pos-23 for `;`, expect a brief
  adjustment period — the new `<` will appear instead.
- **Ґ requires re-learning a finger position.** Was pos 35, now pos 29.
  Low-frequency character so impact minimal.
- **KeyPeek ID staleness.** Custom-behavior labels will appear as raw hex
  strings until IDs are re-read and patched. Cosmetic, doesn't affect
  typing.

## Rollback

Stage 8 keymap is preserved at `config/crosses.keymap.bak` (Stage 6
attempt) — *not* a Stage 8 backup. To roll back to Stage 8 specifically,
recover from git history. The behaviors-block edit is the largest piece;
restoring mcs/mdc + reverting BASE pos 23/32/33/35 + reverting SYM column
alignment reverses the change.

## Deferred (Stage 10+)

- Extend S+V combo pattern to S+X / S+C / S+D / S+A (Stage 8 deferred).
- `=>` combo on SYM pos 30+31 (audit Issue 6 — same-finger bigram).
  Probably worth it since `=>` is frequent in JS/Rust/Go.
- Repeat key (`&key_repeat`). Audit option B2; could go on SYM pos 30
  (now free) or replace NAV INS at pos 22.
- HRM timing tighten (260/175/200 → 200/175/150) when muscle memory
  settles on the new layout.
