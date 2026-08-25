# Crosses 42 — Stage 6 Notes (KeyPeek migration)

**Status: drafted, NOT applied. Apply only after Stage 4+5 is hardware-verified.**

Stage 6 was originally scoped for two mod-morphs and a right-outer-thumb
repurpose, all atop the OverKeys F13-F17 macro shim. We've now folded
the **KeyPeek migration** into the same stage — replacing the host-side
overlay tool entirely. Net effect: same three keymap UX changes, plus
all the F-key macro scaffolding gets deleted.

This file documents the consolidated stage. The `.archive/` folder holds
the old OverKeys setup if we need to fall back.

## What's new vs Stage 4+5

| Feature | Where | Behavior |
|---|---|---|
| Mod-morph comma/semicolon | BASE pos 32 | tap=`,`, Shift+tap=`;` |
| Mod-morph dot/colon | BASE pos 33 | tap=`.`, Shift+tap=`:` |
| Right outer thumb → momentary Mouse | BASE pos 41 | press=enter Mouse, release=exit |
| **KeyPeek migration** | host + firmware | layer overlay reads layer state directly via Raw HID; F13-F17 macro shim deleted |

Slash mod-morph (`/`↔`\`) considered and **rejected** — Shift+`/` already
produces `?` via the OS, which Max actively uses. Don't disturb it.

NAV-layer modifier cleanup (LCTRL/LALT/LGUI/LSHFT at pos 13–16) considered
and **deferred** — Max isn't sure yet whether he uses Shift+Arrow /
Ctrl+Arrow chords during NAV navigation. Re-evaluate after a few weeks of
real use; if those mods stay untouched, repurpose them in a later stage.

## Why migrate to KeyPeek

OverKeys gave us a working overlay, but the integration was awkward:

- Every momentary or toggle layer macro had to fire an F-key on press AND
  on release so OverKeys could flip the overlay back. That was nine
  bespoke macros (`m_sym_mo`, `m_nav_mo`, `m_mouse_mo`, `m_*_enter`,
  `m_*_exit`, `m_sym_to`, `m_sym_unlock`).
- A Caps Word `continue-list` had to enumerate F13-F17 so the trailing
  F14 fired on NAV release wouldn't break an active Caps Word session.
- `m_*_enter` macros needed a 200ms wait so the overlay F-key landed
  *after* the NAV-release F-key — race condition in OverKeys' "type:
  toggle" semantics, papered over with a delay.
- `overkeys_config.json` had to mirror the keymap by hand. Drift is
  inevitable on a layout that's still settling.

KeyPeek's ZMK integration sidesteps all of it. It uses two upstream
modules — `zzeneg/zmk-raw-hid` (transport) and
`srwi/zmk-keypeek-layer-notifier` (forwards layer-state events into
Raw HID) — plus ZMK Studio (already enabled) for keymap reading. The
host overlay then reflects the actual layer state with no keymap-side
scaffolding.

## Exact changes

### `config/west.yml`

Two new remotes and two new projects appended. Both track upstream main
(zmk-raw-hid uses `main`, zmk-keypeek-layer-notifier uses `master`).

```yaml
remotes:
  - name: zzeneg
    url-base: https://github.com/zzeneg
  - name: srwi
    url-base: https://github.com/srwi
projects:
  - name: zmk-raw-hid
    remote: zzeneg
    revision: main
  - name: zmk-keypeek-layer-notifier
    remote: srwi
    revision: master
```

### `build.yaml`

`raw_hid_adapter` shield appended to both halves. Without it, KeyPeek
can't see the device.

```yaml
- shield: crosses_left raw_hid_adapter
- shield: crosses_right raw_hid_adapter
```

### `config/crosses.conf`

`CONFIG_RAW_HID=y` added (defensive — the shield should auto-enable it,
but explicit is better).

### `config/crosses.keymap`

Three positive UX changes plus the macro purge:

1. **Mod-morphs:** new `mcs` and `mdc` behaviors. Same shape as the
   existing `bspc_del` block.
2. **Right outer thumb on BASE (pos 41):** `&kp LGUI` → `&mo MOUSE`. LGUI
   stays reachable via `hml LGUI A` (left pinky HRM).
3. **All m_* macros deleted.** The whole `macros { ... }` node is gone.
   - `td_sym` rebound: `<&m_sym_mo>, <&m_sym_to>` → `<&mo SYM>, <&to SYM>`.
   - SYM pos 38: `&m_sym_unlock` → `&to BASE`.
   - NAV pos 24/25/26: `&m_ukr_enter`/`&m_game_enter`/`&m_mouse_enter` →
     `&tog UKR`/`&tog GAME`/`&tog MOUSE`.
   - MOUSE pos 0, UKR pos 0, GAME pos 11/35/40: `&m_*_exit` → `&tog *`.
   - BASE pos 36 (NAV thumb): `&m_nav_mo` → `&mo NAV`.
4. **Caps Word `continue-list` reverted to default.** F13-F17 no longer
   need to keep the session alive (no F-keys fire on layer transitions
   anymore). The whole trailing `&caps_word { continue-list = ... };`
   block at the bottom of the file is removed.

## Build & flash

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

The HID descriptor set changes (new Raw HID interface), so:

- USB hosts: no action needed, hot-plug picks up the new descriptors.
- BT hosts that already paired the keyboard: re-pair after flashing if
  keystrokes don't make it through. Worst case, run `settings_reset`
  on both halves and re-do the split-pair recovery sequence in
  `AGENT_HANDOFF.md`.

After flashing, install KeyPeek (https://github.com/srwi/keypeek/releases)
and point it at the device. See `KEYPEEK_SETUP.md` for first-run details.

## Hardware test checklist

### 1. Mod-morph — comma/semicolon

- Tap `,` → outputs `,`
- Hold any Shift source (HRM `T`/`N` or SPC/⇧) and tap `,` → outputs `;`
- Verify in a code editor: `for i := 0; i < n; i++` should be typeable
  with no SYM-thumb trips
- Verify Shift release works: type `,` after Shift release → `,` not `;`

### 2. Mod-morph — dot/colon

- Tap `.` → outputs `.`
- Hold Shift + tap `.` → outputs `:`
- Verify: `package main:` (Go) or `key: value` (YAML) typeable directly

### 3. Slash unchanged

- Tap `/` → `/`
- Shift + `/` → `?` (OS-native, not via morph)

### 4. Right outer thumb → Mouse momentary

- Press and hold right outer thumb → KeyPeek shows MOUSE layer
- While held: L = left click, U = right click, Y = middle, J = back, `=` = forward
- Release thumb → back to BASE; right home row types letters again
- Trackball cursor works during the hold

### 5. KeyPeek layer sync (replaces F-key sync)

- BASE → SYM (hold thumb): KeyPeek shows SYM as long as the thumb is held
- BASE → NAV (hold thumb): same
- BASE → MOUSE (hold right outer thumb): same
- Double-tap SYM thumb to lock: overlay stays on SYM after release
- Tap any SYM thumb again to unlock: overlay returns to BASE
- NAV → tap UKR/Game/Mouse: overlay shows the new layer; release NAV does
  not flicker the overlay (no more trailing F14)
- All exits return overlay to BASE

### 6. Cross-layer fall-through behaviour

- On SYM (hold inner thumb): press right outer thumb → MOUSE briefly,
  release returns to wherever you were. Confirm no broken state.
- On NAV: press right outer thumb → C_MUTE fires (NAV overrides pos 41).
- On UKR (toggled on): press right outer thumb → MOUSE briefly, exit works.

### 7. Regression — Stage 4+5 still works

- BASE/SYM/NAV/MOUSE/UKR/GAME entry and exit
- HRM on all 8 home-row positions
- Tap-dance brackets, BSPC/DEL morph, ESC/~, SPC/⇧, ENT/⌃
- Trackballs (cursor + scroll)
- Caps Word: trigger on NAV, type a word, verify auto-shifts work and
  digits/underscore continue the session as expected

### 8. KeyPeek install + connect (one-time)

- KeyPeek launches, sees the device in the dropdown
- Selecting it renders the overlay with correct legends pulled from
  Studio
- Live layer changes reflect in real time
- See `KEYPEEK_SETUP.md` for troubleshooting

## Decision log (assumptions worth re-evaluating)

### NAV-layer modifiers (pos 13–16)

**Decision:** keep as-is.
**Why:** Max isn't sure if he uses Shift+Arrow / Ctrl+Arrow chords during
nav-key usage. The HRM-via-fall-through path is unreliable for those
chords because of `require-prior-idle-ms = 200`, so dedicated mods on NAV
are the canonical solution if you need them.
**Re-evaluate:** in 2–3 weeks. If pos 13–16 on NAV stay untouched in real
use, repurpose them (candidates: media + brightness, window snap shortcuts).

### SYM and UKR pos 41 — let cascade or pin LGUI?

**Decision:** let cascade (no explicit binding on SYM/UKR pos 41; they
remain `&trans` and fall through to BASE's new `&mo MOUSE`).
**Why:** simplest. Max accesses LGUI via `hml LGUI A` left pinky, not
through the right outer thumb on those layers. The cascade means pressing
right outer thumb while on SYM or UKR enters Mouse momentarily, which is
unintuitive but harmless — exit via top-left or release.
**Re-evaluate:** if it ever becomes annoying. Fix is one line per layer:
`&trans` → `&kp LGUI` at pos 41.

### Tracking module main vs pinning

**Decision:** track main/master for both new modules.
**Why:** they're both small, single-purpose modules with low churn. If
upstream breaks, west.yml accepts a SHA pin (see how `zmk` is pinned to
`ac7f75b`). Costs of unpinning a module that breaks: one west update +
one rebuild. Costs of pinning: occasional manual revision bumps to keep
KeyPeek desktop and firmware in lockstep.

## After this stage

Open items / candidate Stage 7+:

- **NAV-layer modifier cleanup** — decide after 2–3 weeks of real use
- **Auto-mouse-layer on trackball motion** — would replace or complement
  the thumb-momentary path. Requires pulling in an additional ZMK module
  that provides a layer-toggle input processor; verify compatibility with
  the pinned ZMK SHA in `west.yml` before committing.
- **Combos for Ctrl+C / Ctrl+V / Ctrl+S** — if HRM-ctrl latency proves
  annoying in practice. Would need `require-prior-idle-ms` tuning to
  not conflict with HRM.
- **HRM timing tighten** — 260→200ms tapping-term, 200→150ms idle, when
  fingers know the layout.
- **HRM/mod-morph state in KeyPeek** — currently not surfaced (KeyPeek's
  layer notifier only forwards layer changes). If we want the overlay to
  show held mods or pending tap-dance state, that's a custom event
  listener in a separate module — feature request upstream first.
