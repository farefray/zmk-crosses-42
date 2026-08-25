# Crosses 42 — Stage 4+5 Notes

## What's new vs Stage 3

| Feature | Where | Behavior |
|---|---|---|
| ESC moved to top-left | BASE pos 0 | tap=ESC, hold=TILDE (same hold-tap as before, new position) |
| TAB moved to home outer left | BASE pos 12 | plain `&kp TAB` |
| Caps Word | NAV pos 12 (home outer left) | hold NAV → tap home outer left → release; type constant → auto-off on space/punct |
| UKR toggle (F16) | NAV pos 24 (bottom outer left) | hold NAV → tap → UKR layer ON; EXIT returns to BASE |
| GAME toggle (F17) | NAV pos 25 | hold NAV → tap → GAME layer ON; EXIT on three right-side keys |
| MOUSE toggle (F15) | NAV pos 26 | hold NAV → tap → MOUSE layer ON; EXIT on top-left of MOUSE |
| OverKeys: Mouse overlay | F15 | shows button positions + scroll wheel |
| OverKeys: UKR overlay | F16 | shows Ukrainian Cyrillic letters per key |
| OverKeys: Game overlay | F17 | shows QWERTY game layout |

Auto-shift removed from scope — too much interaction with existing hold behaviors.

## ESC/TAB swap

TAB is now at the old ESC position (home row outer left, second row). ESC is at top-left.
The `&ht TILDE ESC` hold-tap is preserved — hold ESC key for TILDE. TILDE also still on SYM row 2 left outer.

## Caps Word

ZMK native, no module needed. Activation: hold NAV → tap home outer left → release NAV. Then type freely.

Auto-deactivates on: Space, Enter, Esc, Backspace/Delete (except UNDER — underscores keep Caps Word active). This means `HTTP_STATUS_OK` works: type `http_status_ok` with Caps Word active → outputs `HTTP_STATUS_OK`.

## Layer toggle pattern (MOUSE / UKR / GAME)

All three use the same `&tog` + F-key pattern:
- On NAV layer: tap the toggle key → F-key fires (OverKeys overlay ON) + layer toggles ON
- On the destination layer EXIT key: same macro → F-key fires (overlay OFF) + layer toggles OFF
- BASE stays the default layer throughout; `&trans` on the new layers falls through to BASE

## UKR layer specifics

- Thumbs are `&trans` — SYM and NAV thumbs still work (needed for Ґ via Shift+BSLH on SYM)
- Bottom-left outer (pos 24) is `&none` — blocks accidental {[< fallthrough
- **Ґ** (rare, 33rd letter): hold SYM thumb → on SYM layer, press `\` while holding a Shift source
- **Windows IME** must be switched manually to Ukrainian before entering this layer (Win+Space)
- Period and comma: `/` = period, Shift+`/` = comma — this is correct Ukrainian IME behavior

## GAME layer specifics

- All alpha keys are plain `&kp` — zero hold-tap timing, no HRM
- Right-side unused positions are `&none` (blocks BASE HRM fallthrough, not `&trans`)
- EXIT on three keys: top-right outer (pos 11), bottom-right outer (pos 35), right inner thumb (pos 40)

## Build & flash

Same fish loop as always. No settings_reset needed (no NVS structure changes):

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

After building, update OverKeys with the new `overkeys_config.json` (paste contents into OverKeys config editor) to get the Mouse/UKR/Game overlays.

## Hardware test checklist

### 1. ESC at top-left

- Tap top-left key → ESC (check in a text editor: should dismiss dialogs, exit Vim insert, etc.)
- Hold top-left key → TILDE (`~`)
- Confirm old ESC position (pos 12, now TAB) types TAB

### 2. Caps Word

- Hold NAV thumb → tap home outer left → release NAV
- Type `http_status_ok` → should output `HTTP_STATUS_OK`
- Hit Space → Caps Word deactivates, next character is lowercase
- Repeat test: type `http` + underscore → stays in caps (UNDER continues Caps Word)
- Shift+BSPC while Caps Word active → should produce DEL (mod-morph still works)

### 3. NAV layer toggle keys

Hold NAV → confirm three bottom-left positions produce:
- Outer left: enters UKR layer (OverKeys UKR overlay appears)
- Middle: enters GAME layer (OverKeys Game overlay appears)
- Inner: enters MOUSE layer (OverKeys Mouse overlay appears)

### 4. MOUSE layer

- Enter via NAV → MOUSE toggle
- Right home row: MB4 MB1 MB2 MB3 MB5 (test in a browser — right-click, middle-click, etc.)
- Right bottom row: WH↑ WH↓ WH← WH→ (scroll a page)
- Left inner thumbs: MB1, MB2
- Right inner thumbs: MB1, MB3
- Trackballs still move cursor and scroll (they're always active, unrelated to layer)
- EXIT (top-left of MOUSE layer) returns to BASE; OverKeys overlay disappears

### 5. UKR layer

- Win+Space to switch Windows IME to Ukrainian
- Enter UKR via NAV toggle
- Type all rows and verify Cyrillic output:
  - Top row: Й Ц У К Е / Н Г Ш Щ З
  - Home row: Ф І В А П / Р О Л Д Ж
  - Bottom row: Я Ч С М И / Т Ь Б Ю
- Х (left bracket key, pos 12): outputs Х
- Ї (right bracket key, pos 35): outputs Ї
- Period test: `/` key → `.` (period), Shift+`/` → `,` (comma) — critical: auto-shift is NOT active
- Ґ access: hold SYM thumb → on SYM layer, Shift + `\` position → Ґ
- Thumbs transparent: BSPC, Space, Enter still work normally
- EXIT returns to BASE; OverKeys overlay disappears
- Win+Space to switch IME back to English

### 6. GAME layer

- Enter via NAV toggle
- Left half: ESC 1-5 / TAB Q W E R T / Shft A S D F G / Ctrl Spc Alt
- Right half: 6-0 / Y U I O P / H J K L ;
- Confirm no HRM: hold A/S/D/F briefly while typing — no modifier fires
- EXIT from all three right-side exit keys: top-right outer, bottom-right outer, right inner thumb

### 7. OverKeys overlay sync

For each layer: enter → overlay appears with correct layout → exit → overlay disappears.
No stale overlay after exiting a layer.

### 8. Regression: everything from Stage 3 still works

- Tap-dance brackets {[< and }]>
- SYM momentary and lock/unlock
- HRM on all 8 home-row positions
- ESC/~, SPC/⇧, ENT/⌃, BSPC→DEL
- Trackballs (cursor and scroll)

## After this stage

When Stage 4+5 feels solid, the only remaining known deferred items are:
- HRM timing tighten (260→200ms tapping-term, 200→150ms idle) — whenever fingers know the layout
- Combos for Ctrl+C/V/S if HRM-ctrl latency proves annoying in practice
- OverKeys overlays for UKR/Game are basic — can add detail (show all Cyrillic labels, etc.) later
