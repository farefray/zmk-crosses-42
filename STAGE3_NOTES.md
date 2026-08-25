# Crosses 42 — Stage 3 Notes

## What's new vs Stage 2

| Feature | Where | Behavior |
|---|---|---|
| TD `{[<` | Left bottom outer (pos 24) | 1× = `{`, 2× = `[`, 3× = `<` |
| TD `}]>` | Right bottom outer (pos 35) | 1× = `}`, 2× = `]`, 3× = `>` |
| ▽SYM tap-dance | Left inner thumb (pos 38) | 1× hold = momentary SYM, 2× tap = lock SYM |
| SYM-unlock | Same position on SYM layer | 1× tap from locked SYM = back to BASE |
| HRM timing — looser for learning | `hml`/`hmr` | `tapping-term-ms` 200→260, `require-prior-idle-ms` 150→200 |

The HRM shift means you have to hold the home-row mod a hair longer (260ms vs 200ms) before it counts as a hold. In practice, this trades a tiny bit of "intent recognition" sharpness for a lot fewer accidental shifts during the layout-learning phase. Tighten back to 200/150 once your fingers know where everything is — usually 2-3 weeks.

## Lost vs Stage 2

The bottom-outer columns are no longer `LSHFT`/`RSHFT`. To capitalize while learning:

- Hold the SPC/⇧ thumb + tap a letter (still works, unchanged)
- Hold home-row T (left, LSHFT) or N (right, RSHFT) past 260ms, then tap the letter
- Stage 4's auto-shift will give you a *third* option: just hold any letter ~175ms and you get its capital

If this proves painful in the first day or two of typing, ping me — we can put a `LSHFT` somewhere convenient as a temporary measure, or accelerate Stage 4.

## Build & flash

Same as Stage 2 — your usual fish loop:

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

Drag-drop both UF2s in bootloader mode. No settings_reset needed.

## Hardware test checklist

### 1. Tap-dance brackets, left

- Tap left bottom outer once → `{`
- Tap twice (within ~200ms) → `[`
- Tap three times → `<`
- Tap, wait a beat, tap again → `{{` (each tap resolves separately)

### 2. Tap-dance brackets, right

Same as above on right bottom outer: `}`, `]`, `>`.

### 3. ▽SYM momentary (the common case)

- Hold left inner thumb → SYM active
- Tap `q` → produces `1`
- Release thumb → back to BASE

This should feel exactly like Stage 2's `&mo SYM`.

### 4. ▽SYM lock (the new case)

- Double-tap the left inner thumb (two quick taps within ~200ms, no hold) → you're now locked on SYM
- Type as long as you want — `qwerty` produces `12345…` style output
- Tap the same thumb once → back to BASE

If after a double-tap you find yourself stuck on SYM and the thumb tap doesn't unlock you, the binding might have raced. Try double-tapping again.

### 5. HRM timing still feels right

Type a normal sentence. With `tapping-term-ms=260` and `require-prior-idle-ms=200`, accidental HRM activations during typing should be very rare. If they're still happening (you see random capital letters or shortcuts firing), tell me and we'll bump further.

### 6. Everything else still works

Trackballs, NAV layer, ESC/~, SPC/⇧, ENT/⌃, BSPC→DEL — all unchanged from Stage 2.

## After this stage

When Stage 3 feels solid, ping me and we move to Stage 4: auto-shift + Caps Word. Auto-shift in particular will help your typing onboarding a lot — capital letters become "hold the letter for 175ms" instead of needing to remember HRM-shift positions.
