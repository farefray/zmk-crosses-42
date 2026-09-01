# Crosses 42 current-state UX audit

> Historical snapshot from 2026-08-28. The live firmware now uses simplified
> Auto Mouse without Grid Jump, Win/Alt-only BASE HRMs, and a hardware-verified
> motion gate. Use `config/crosses.keymap`, `README.md`, and
> `KEYBOARD_UX_PLAN.md` for current behavior.

Audit date: 2026-08-28
Scope: read-only review of the active ZMK source, imported module checkouts,
Crosses shield files, generated layer diagrams, and the KeyPeek fork.
Firmware behavior changed: no
KeyPeek behavior changed: no
Flashing performed: no

## Executive conclusion

The keyboard does not need more features first. It already has a strong,
thoughtful six-layer system. The largest present-day UX problem is that the
system is difficult to see and learn: the README image was stale, the handoff
stopped before Grid Jump, the Ukrainian layer was described with raw US HID
labels, and several high-value actions have multiple competing access paths.

The first improvement should therefore be a truthful daily-use guide plus
small, contextual KeyPeek coaching. Usage measurement should follow only after
the existing features have been taught. Firmware simplification should wait
until subjective feedback and local aggregate evidence agree.

## Source-of-truth checkpoint

| Item | Confirmed state |
|---|---|
| Firmware repository | `F:\zmk-crosses-42` |
| Firmware HEAD | `280f43efdfdaa338c8e00fe4eae2584061e3446e` |
| Active keymap | `config/crosses.keymap` |
| Active layers | Base, Sym, Nav, Mouse, UKR, Game |
| ZMK source | `ac7f75b8591d39aaf3b66b9d26f26c9ed921a009` (`v0.3-83-gac7f75b8`) |
| KeyPeek repository | `F:\keypeek-crosses42` |
| KeyPeek HEAD | `9b7d0518de6ca98f165f90f7b1c9140beb8ac69a` |
| KeyPeek version lineage | `0.9.0-8-g9b7d051` |
| Primary platform | Windows, with English and Ukrainian host layouts |
| Runtime KeyPeek config | Not inspected in this audit; the repository template is not assumed to be the live file |

The current firmware source contains Stage 10 Grid Jump even though the file
header still says Stage 9. The source bindings below are authoritative.

## Hardware and data paths

- 42-key wireless split using nice!nano-class controllers.
- Right half is central. It owns USB, ZMK Studio, Raw HID host communication,
  and the right PMW3610 trackball.
- Both build targets include the `raw_hid_adapter` shield; the right build also
  includes the Studio USB/UART snippet.
- Both trackballs are `force-awake` while ZMK is active. System sleep begins
  after 15 minutes of idle time.
- Bluetooth uses the existing higher-power and split-reliability settings;
  these are treated as deliberate hardware tradeoffs.

### Pointing paths

| Device | Source processing | Transport / final processing | Daily role |
|---|---|---|---|
| Left PMW3610 | 800 CPI; X/Y converted to scroll; scroll scaled to 1/4; rate limited on the left peripheral | Processed wheel events sent over split BLE to the right central | Vertical and horizontal scrolling on every layer |
| Right PMW3610 | 700 CPI; axes swapped and inverted as mounted | Rate limited on the right central, then sent as pointer motion | Cursor movement on every layer |

This architecture is valid and simple. Its constraint is that the left ball is
converted to scrolling before the central knows the active layer. Any future
layer-aware left-ball mode would require raw motion from the left half and new
central processing, so both halves would need coordinated firmware.

## Firmware and module chain

| Component | Revision in the current checkout | Purpose | Reproducibility state |
|---|---|---|---|
| ZMK | `ac7f75b8591d39aaf3b66b9d26f26c9ed921a009` | Core firmware | Pinned |
| `gggw-zmk-keebs` | manifest branch `zephyr-4.1`; checkout `33d2dca...` | Crosses shield, 42-key physical layout, PMW3610 setup | Branch name in manifest |
| `zmk-raw-hid` | manifest `main`; checkout `e25e325...` | Host Raw HID transport | Moving revision |
| `zmk-keypeek-layer-notifier` | manifest `master`; local authored checkout `3c1df661...` | Layer/modifier packets for KeyPeek | Moving upstream plus local branch |
| `zmk-auto-layer` | manifest `main`; checkout `3a885d90...` | NumWord | Moving revision |
| `zmk-tri-state` | manifest `main`; checkout `ebbc1f0c...` | Alt-Tab swapper | Moving revision |

The imported Crosses manifest also brings pointing-driver and processor modules;
several of those follow branches. Pinning is maintenance work, not a UX change,
and should happen only after a clean two-half build at known-good revisions.

## Physical position grid

All positional behaviors and combos use this 42-position grid:

```text
 0  1  2  3  4  5          6  7  8  9 10 11
12 13 14 15 16 17         18 19 20 21 22 23
24 25 26 27 28 29         30 31 32 33 34 35
             36 37 38     39 40 41
```

## Layer inventory

Legend: `tap / hold` for hold-taps; `tap / shifted` for mod-morphs; `▽`
means transparent fall-through.

### Base — everyday typing

| Row | Left positions and bindings | Right positions and bindings |
|---|---|---|
| Top | `0 Esc/~` · `1 Q` · `2 W` · `3 F` · `4 P` · `5 B` | `6 J` · `7 L` · `8 U` · `9 Y` · `10 =` · `11 -` |
| Home | `12 Tab` · `13 A/Win` · `14 R/Alt` · `15 S/Ctrl` · `16 T/Shift` · `17 G` | `18 M` · `19 N/Shift` · `20 E/Ctrl` · `21 I/AltGr` · `22 O/Win` · `23 '` |
| Bottom | `24 Ctrl` · `25 Z` · `26 X` · `27 C` · `28 D` · `29 V` | `30 K` · `31 H` · `32 ,` · `33 .` · `34 /` · `35 ;/:` |
| Thumbs | `36 Backspace/Delete` · `37 Space/Shift` · `38 Sym hold / 2× NumWord` | `39 Nav` · `40 Enter/Ctrl` · `41 Mouse` |

Primary purpose: Colemak-DH typing with positional home-row mods and direct
thumb access to the three daily function layers.

### Sym — numbers and punctuation

| Row | Left positions and bindings | Right positions and bindings |
|---|---|---|
| Top | ``0 ` `` · `1 1` · `2 2` · `3 3` · `4 4` · `5 5` | `6 6` · `7 7` · `8 8` · `9 9` · `10 0` · `11 Backspace` |
| Home | `12 ~` · `13 !` · `14 @` · `15 #` · `16 $` · `17 %` | `18 ^` · `19 &` · `20 *` · `21 (` · `22 )` · `23 "` |
| Bottom | `24 _` · `25 [` · `26 ]` · `27 {` · `28 }` · `29 \` | `30 >` · `31 =` · `32 <` · `33 .` · `34 ?` · `35 :` |
| Thumbs | `36 ▽` · `37 ▽` · `38 Base` | `39 ▽` · `40 ▽` · `41 ▽` |

Primary purpose: momentary symbols or a NumWord run. Double-tapping Base
position 38 starts NumWord; a non-number/non-continuation key ends it.

### Nav — editing, functions, task switching, media

| Row | Left positions and bindings | Right positions and bindings |
|---|---|---|
| Top | `0 F12` · `1 F1` · `2 F2` · `3 F3` · `4 F4` · `5 F5` | `6 F6` · `7 F7` · `8 F8` · `9 F9` · `10 F10` · `11 F11` |
| Home | `12 Caps Word` · `13 Undo` · `14 Cut` · `15 Copy` · `16 Paste` · `17 Alt-Tab` | `18 Page Up` · `19 Home` · `20 Up` · `21 End` · `22 Insert` · `23 Print Screen` |
| Bottom | `24 UKR toggle` · `25 Game toggle` · `26 Mouse toggle` · `27 Find` · `28 Save` · `29 Redo` | `30 Page Down` · `31 Left` · `32 Down` · `33 Right` · `34 Mute` · `35 Play/Pause` |
| Thumbs | `36 Mouse 1` · `37 Mouse 2` · `38 Mouse 3` | `39 held Nav` · `40 Volume +` · `41 Volume -` |

Primary purpose: one-handed editor navigation and clipboard work, Windows
task switching, function keys, media, and access to persistent layers.

### Mouse — Grid Jump, clicks, and wheel keys

| Row | Left positions and bindings | Right positions and bindings |
|---|---|---|
| Top | `0–5 Zones 1–6` (`F13–F18`) | `6–10 ▽` · `11 Mouse exit` |
| Home | `12–17 Zones 7–12` (`F19–F24`) | `18 M4` · `19 M1` · `20 M2` · `21 M3` · `22 M5` · `23 ▽` |
| Bottom | `24–29 Zones 13–18` (`Ctrl+F13–F18`) | `30 ▽` · `31 Wheel ↑` · `32 Wheel ↓` · `33 Wheel ←` · `34 Wheel →` · `35 ▽` |
| Thumbs | `36 ▽` · `37 M1` · `38 M2` | `39 M1` · `40 M3` · `41 held Mouse` |

Primary purpose: hold Base position 41, tap a left-side screen zone, then use
the right ball for precision. The same layer can be toggled from Nav position
26 and exited at Mouse position 11. Grid Jump also depends on the external
Windows AHK helper being active.

### UKR — Windows Ukrainian IME positions

| Row | Left output | Right output |
|---|---|---|
| Top | `0 exit` · `1 Й` · `2 Ц` · `3 У` · `4 К` · `5 Е` | `6 Н` · `7 Г` · `8 Ш` · `9 Щ` · `10 З` · `11 Backspace` |
| Home | `12 Х` · `13 Ф` · `14 І` · `15 В` · `16 А` · `17 П` | `18 Р` · `19 О` · `20 Л` · `21 Д` · `22 Ж` · `23 Є` |
| Bottom | `24 ▽` · `25 Я` · `26 Ч` · `27 С` · `28 М` · `29 И` | `30 Т` · `31 Ь` · `32 Б` · `33 Ю` · `34 .` · `35 Ї` |
| Thumbs | `36 ▽` · `37 ▽` · `38 ▽` | `39 ▽` · `40 ▽` · `41 ▽` |

Primary purpose: remove hold-tap timing while the Windows Ukrainian layout is
active. Firmware layer and Windows input language are separate states. The
documented `Ґ` path through lower Sym position 29 requires hardware
verification because the active higher UKR layer may mask that binding.

### Game — timing-free QWERTY

| Row | Left positions and bindings | Right positions and bindings |
|---|---|---|
| Top | `0 Esc` · `1 1` · `2 2` · `3 3` · `4 4` · `5 5` | `6 6` · `7 7` · `8 8` · `9 9` · `10 0` · `11 exit` |
| Home | `12 Tab` · `13 Q` · `14 W` · `15 E` · `16 R` · `17 T` | `18 Y` · `19 U` · `20 I` · `21 O` · `22 P` · `23 blocked` |
| Bottom | `24 Shift` · `25 A` · `26 S` · `27 D` · `28 F` · `29 G` | `30 H` · `31 J` · `32 K` · `33 L` · `34 ;` · `35 exit` |
| Thumbs | `36 Ctrl` · `37 Space` · `38 Alt` | `39 blocked` · `40 exit` · `41 blocked` |

Primary purpose: conventional QWERTY game controls with no home-row-mod or
thumb timing decisions.

## Combo map

| Combo | Positions | Layer | Timing guard | Result / purpose |
|---|---:|---|---|---|
| Caps Word | `36 + 37` | Base | 50 ms | Backspace + Space thumbs start Caps Word |
| Bootloader | `0 + 11` | Nav | 50 ms | Deliberately difficult two-corner bootloader entry |
| Mouseless overlay | `28 + 29` | Base | 40 ms; 200 ms prior idle | D + V sends Scroll Lock |
| Mouseless free mode | `30 + 31` | Base | 40 ms; 200 ms prior idle | K + H sends Pause/Break |
| Ctrl+V | `14 + 29` | Base | 40 ms; 150 ms prior idle | R + V positions bypass same-hand HRM limits |

## Custom behavior inventory

| Behavior | Type and parameters | Used for | UX consequence |
|---|---|---|---|
| `hml` | Balanced hold-tap; 260/175/200 ms; opposite-hand trigger; trigger on release | Left home-row Win/Alt/Ctrl/Shift | Same-hand shortcuts need a different Ctrl path |
| `hmr` | Mirror of `hml` | Right home-row Shift/Ctrl/AltGr/Win | Designed for Windows modifier order |
| `ht` | Tap-preferred; 200/175 ms | Esc held for `~` | Low-risk dual-role corner key |
| `sht` | Balanced; 200/0/75 ms | Space held for Shift | Fast reuse is favored; hold-to-repeat is sacrificed |
| `bspc_del` | Shift-sensitive mod-morph | Backspace; Shift produces Delete | Compact editing, but KeyPeek cannot show the live morph state |
| `mscl` | Shift-sensitive mod-morph | `;` and `:` | Dedicated punctuation position |
| `td_sym` | 200 ms tap dance | Hold Sym; double-tap NumWord | High value but needs teaching; lone activation waits for tap-dance resolution |
| `swapper` | Tri-state; arrows ignored; 5 s safety timeout | Windows Alt-Tab on Nav | Efficient task switching while Nav remains active |

## KeyPeek interaction surface

Confirmed from the clean KeyPeek source checkout:

- It receives ZMK layer names and state over Raw HID.
- It has hardcoded Crosses 42 labels for all eight current custom behavior IDs.
- Its bundled coach profile targets `zed.exe` and appears when either Ctrl is
  held or the Nav layer is active.
- The bundled profile teaches six Herdr/Zed actions: F6/F7, F8, F9/F10, F11,
  and two Ctrl+B prefix sequences.
- The coach is process-aware, not terminal/tab/content-aware.
- The bundled JSON is only a first-run template. This audit did not read the
  live `%APPDATA%` configuration, so runtime equality is not claimed.
- There is no persistent layer/key-position usage measurement today.

## Ranked findings and proposed remedies

### 1. Restore one truthful map of the system

**Why it bites:** A user or future maintainer can learn from the old README
image, miss Grid Jump, or treat the Stage 9 narration as the latest behavior.
The handoff also contains conflicting hardware-verification claims.

**Remedy:** Adopt the new per-layer diagrams, update the README into a compact
daily-use guide, reconcile Stage 10 in the handoff, and explicitly label each
stage as source-present versus hardware-confirmed.

**Repository / update scope:** Firmware repository documentation only. No
build, flash, or desktop update.

**Verify:** Diff every diagram against the 42-position tables; ask Max to
confirm the physical legends and the current hardware-tested boundary.

### 2. Teach layer purpose, not every available key

**Why it bites:** KeyPeek can display the whole layer, but it does not explain
the small number of actions worth learning first. A dense overlay can still
leave NumWord, Alt-Tab, Grid Jump, and layer exits unused.

**Remedy:** Add compact, dismissible layer coaching: one purpose sentence and
three to five highest-value actions for Sym, Nav, Mouse, UKR, and Game. Reuse
the current coach architecture without making a permanently noisy panel.

**Repository / update scope:** KeyPeek only; rebuild/relaunch the desktop app.
No firmware flash.

**Verify:** Visual check at Windows scaling in light/dark content; each layer's
primary action should be discoverable within seconds and the panel must hide
when the layer exits.

### 3. Make the mouse story feel like one workflow

**Lived-experience update — 2026-08-28:** Max reports never using Grid Jump
and largely avoiding both trackballs because clicking requires an additional
held layer key. This supersedes the documentation-first remedy below. Treat the
problem as an ergonomic firmware failure and use `POINTING_UX_AUDIT.md` before
any Mouse coaching or automatic-layer work.

**Why it bites:** Mouse access currently includes a momentary thumb layer, a
Nav toggle, Nav thumb clicks, Mouse-layer click duplicates, two Mouseless
combos, two physical balls, wheel keys, and an external AHK Grid Jump helper.
If the helper is not running, 18 keys silently do nothing useful.

**Revised remedy:** Simplify the Mouse layer before automating it. Evaluate a
right-ball-triggered temporary click layer with transparent alpha positions and
two physically comfortable click keys. Grid Jump is a removal candidate, not a
workflow to teach first.

**Repository / update scope:** Documentation/KeyPeek first. A future keymap-only
change normally requires the right/central half; compare the left UF2 before
deciding to flash both. Left-ball processing changes require both halves.

**Verify:** Move, primary click, secondary click, drag, return immediately to
typing, and test incidental ball contact. Confirm Grid Jump cannot fire from
the experimental layer and left-ball scrolling is unchanged.

### 4. Pair the UKR firmware layer with the Windows input-language state

**Why it bites:** Entering UKR does not switch Windows to Ukrainian, and
Win+Space does not toggle the firmware layer. Either can be active alone, so
the visible layer name and produced characters may disagree.

**Remedy:** Immediately teach a single entry/exit ritual and display Ukrainian
output legends. Later evaluate one reversible paired-switch mechanism or a
KeyPeek mismatch indicator if Windows exposes a reliable active-layout signal.

**Repository / update scope:** Documentation/KeyPeek first, no flash. A future
firmware pairing change is central-keymap behavior and should be tested on the
right half first.

**Verify:** Switch English → Ukrainian → English across two applications and
confirm both the overlay state and actual characters every time.

### 5. Add privacy-preserving evidence only after teaching

**Why it bites:** Today we cannot distinguish “never discovered” from “tried
and awkward.” Removing an unused-looking feature now could erase something
valuable before it was taught.

**Remedy:** After Phase 1, add opt-in local aggregates: layer activation count,
dwell buckets, physical position counts by layer, and layer transitions. Never
store characters, ordered key streams, words, clipboard data, or window titles.

**Repository / update scope:** KeyPeek only; no firmware flash.

**Verify:** Privacy review, local reset/export, restart persistence, and a test
showing that the stored data cannot reconstruct typed text.

### 6. Consolidate Ctrl only when evidence identifies a loser

**Why it bites:** Ctrl is available through Base S-hold, Base E-hold, dedicated
Base position 24, Enter-hold, Nav's direct shortcuts, and the R+V combo. This is
robust, but it creates several competing mental models for the same action.

**Remedy:** Teach Nav as the preferred editing cluster and keep one direct Ctrl
escape hatch. Measure or ask which other paths are actually relied on before
removing anything. Do not automatically expand the single Ctrl+V combo family.

**Repository / update scope:** Coaching first. Any later binding change is
central keymap behavior; right-half-first hardware test.

**Verify:** Real editing in Zed/browser/terminal, including same-hand Ctrl+A/S/W
and rapid copy/paste/undo sequences.

### 7. Pin moving module revisions

**Why it bites:** A clean rebuild at a later date can fetch changed module code
even when `config/crosses.keymap` is unchanged, making a firmware regression
difficult to reproduce.

**Remedy:** Record the known-good module commits and pin them one at a time,
including imported pointing dependencies, then build both halves and inspect
the compiled devicetree.

**Repository / update scope:** Firmware manifest; rebuild both halves. Flashing
is unnecessary if the produced images match the known-good images; otherwise
use the normal staged two-half test.

**Verify:** Pristine reproducible builds, module revision report, UF2 hashes,
and compiled listener/hold-tap values.

### 8. Treat pointing power and layer-awareness as measured tradeoffs

**Why it bites:** Both balls stay awake while ZMK is active, and the left ball's
pre-BLE scroll conversion prevents layer-aware modes. Changing either without
evidence could trade battery life, latency, and reliability unpredictably.

**Remedy:** Record practical battery life and scroll satisfaction first. Only
move left-ball conversion to the central if a concrete per-layer pointer or
gesture use case justifies the added BLE traffic and two-half complexity.

**Repository / update scope:** Sensor power changes affect the owning half;
left split-processing changes require both halves.

**Verify:** Multi-day battery comparison plus cursor/scroll latency and wake
testing, not compilation alone.

### 9. A System layer is a maintenance option, not a current blocker

**Why it bites:** Bootloader entry exists, but Bluetooth profiles, output
selection, reset, and travel soft-off are not presented as a one-handed recovery
surface. Recovery therefore depends more on physical reset actions and notes.

**Remedy:** After the daily layers settle, evaluate a small System layer with
both-half bootloader/reset, BT profiles, output selection, and a guarded travel
soft-off. Add only the controls Max will use.

**Repository / update scope:** Firmware keymap/config; build and flash both
halves because shutdown/recovery behavior is device-wide.

**Verify:** One-handed access, both-half bootloader/reset, BT profile recovery,
USB/BLE output switching, guarded power-off, and deliberate reset-button wake.

### 10. Keep Space/Shift unchanged until real use says otherwise

**Why it bites:** `quick-tap-ms = 0` supports rapid Space → Shift reuse but
removes hold-to-repeat. Generic advice would change it; Max's prior typing
failure was specifically about fast thumb reuse and roll resolution.

**Remedy:** Preserve 200/0/75 balanced behavior now. Ask only whether Space
repeat is missed and whether intended rapid capitals still produce lowercase.
Run one timing experiment only if either symptom is real.

**Repository / update scope:** Firmware keymap. Flash right/central first; the
left image can be compared and flashed only if required.

**Verify:** Fast Space→Shift typing, Space rolls, deliberate selection with
Shift, and long Space hold in applications where repeat matters.

## Independently testable order of work

1. **Documentation only:** review this audit and adopt the verified diagrams.
   No build or flash.
2. **KeyPeek only:** add compact layer-purpose coaching. Desktop rebuild and
   visual QA; no firmware change.
3. **KeyPeek only:** approve and implement local aggregate metrics. Observe at
   least seven representative days.
4. **One workflow experiment:** choose Mouse, UKR, Ctrl, or Space/Shift from
   evidence and subjective friction. Change one repository only.
5. **Firmware maintenance:** pin one module family at a time and reproduce both
   builds before any flash decision.
6. **Optional System layer:** evaluate separately after daily UX stabilizes.

## Generated visual references

- `keymap-drawer/crosses_base.svg`
- `keymap-drawer/crosses_sym.svg`
- `keymap-drawer/crosses_nav.svg`
- `keymap-drawer/crosses_mouse.svg`
- `keymap-drawer/crosses_ukr.svg`
- `keymap-drawer/crosses_game.svg`
- `keymap-drawer/crosses_combos.svg`
- `keymap-drawer/crosses.svg` (combined)

The source-derived `keymap-drawer/crosses.yaml` preserves literal bindings.
`keymap-drawer/crosses_labels.yaml` overlays user-facing Ukrainian output and
Grid Jump zone numbers for drawing only; it does not affect firmware.

## Verification completed

- Read the active 458-line keymap in full.
- Read the active config, manifest, build matrix, workflow, Crosses shield,
  42-key layout, and both pointing overlays.
- Confirmed right-central topology and both pointing-processing paths.
- Confirmed current local module commits and moving manifest revisions.
- Parsed all six layers and five combos with keymap-drawer 0.23.0.
- Regenerated combined, per-layer, and combo-only SVGs.
- Visually checked Base, Nav, Mouse, UKR, and combo renders; corrected stale
  firmware identifiers, combo overlap, Ukrainian legends, and Grid Jump labels.
- Confirmed the KeyPeek source tree is clean and its current custom behavior
  IDs match the eight firmware behaviors.
- Did not compile firmware, flash hardware, launch KeyPeek, or change live
  application configuration.
