# Crosses 42 focused pointing UX audit

Audit date: 2026-08-28
Trigger: Max reports no practical Grid Jump use, little or no trackball use,
acceptable pointer movement, and an unusable click workflow because clicking
requires an additional held layer key.
Scope: Read-only firmware and hardware-path review. No firmware, build, flash,
KeyPeek, or live-helper change was performed.

## Revised conclusion

The pointing problem is no longer primarily discoverability. The right ball is
already an always-available pointer and its movement quality is acceptable.
The blocking interaction is click access.

The current design asks Max to hold Mouse with one thumb, move the ball, then
press another Mouse-layer button. Grid Jump adds another workflow on top of
that, but it has never become part of daily use. Coaching this design would
teach friction rather than remove it.

The selected direction is a small firmware experiment that makes the whole
left split a Mouse control surface after right-ball motion, without holding a
layer key. KeyPeek should visualize the automatically active Mouse layer through
its existing layer-state path. Generic layer coaching remains paused until the
pointing workflow is settled.

## Confirmed current behavior

### Hardware paths

| Device | Current behavior | Layer activation |
|---|---|---|
| Right PMW3610, central half | 700 CPI pointer; rate-limited on the central listener | None; pointer movement is always active |
| Left PMW3610, peripheral half | 800 CPI; converted to horizontal/vertical scroll and scaled to 1/4 before BLE | None; scroll is always active |

Both sensors are currently `force-awake` while ZMK is active.

### Click access

- Base position 41 holds the Mouse layer; it is not a click.
- Nav exposes Mouse 1/2/3 on left thumb positions 36/37/38, but Nav must be
  held.
- Mouse exposes duplicated buttons on its right home row and thumb keys, but
  Mouse must be held or toggled first.
- Nav position 26 toggles Mouse; Mouse position 11 exits it.
- No primary or secondary mouse button is directly available on Base.

### Grid Jump

- Eighteen left alpha positions on Mouse send F13–F24 and Ctrl+F13–F18.
- An external Windows AHK helper turns those events into a 6×3 screen jump.
- Without the helper, the events have no useful pointing effect.
- Max reports never using this workflow in practice.

## Failure scenario

To click a target with the right ball today, Max must first remember and hold a
layer thumb, operate the ball, and press a different click key while the hold is
still active. That changes an ordinary point-and-click into a multi-finger
chord. The extra coordination cost is high enough that the otherwise acceptable
trackball movement is abandoned.

This is a firmware interaction problem, not something better labels can solve.

## Options considered

### 1. Right-ball temporary Mouse layer — selected candidate

Right-ball motion would activate a simplified Mouse layer for roughly 1000 ms.
No layer key would be held.

Proposed experimental shape:

- Remove Grid Jump entirely from the experimental layer.
- Put Mouse 4/1/2/3/5 on the left home row and discrete wheel directions on
  the left bottom row; keep the top row quiet until a real need appears.
- Put secondary, primary, and middle click on the three left thumb positions,
  with primary in the center Space position for click-drag comfort.
- Make the right half mostly transparent and keep one explicit Mouse exit.
- Right-ball motion activates Mouse through ZMK's core temporary-layer input
  processor.
- Use about 150 ms prior-idle protection so an incidental ball brush during
  typing does not raise Mouse.
- Ball motion refreshes the roughly 1000 ms timeout.
- Click and wheel positions remain usable throughout the temporary layer.
- Max explicitly accepts the short left-side return-to-typing delay after
  pointer movement; KeyPeek provides the visible mode boundary.
- Keep the left ball's current scroll path unchanged for this first experiment.

Expected workflow:

```text
move right ball → KeyPeek shows Mouse → left click/drag or mouse action
```

Benefits:

- Removes the held-layer requirement.
- Supports dragging with one hand on the ball and the other on the click key.
- Uses ZMK functionality already present in the pinned source checkout, and
  the existing KeyPeek layer notifier already forwards its state changes.
- Can be implemented on the central/right pointing listener and keymap without
  changing the left peripheral processing path.

Risks to test:

- The one-second tail may still feel too long or too short in real typing.
- The frequency-optimized Mouse 2/1/3 thumb order must be tested for comfort.
- Press-to-dismiss ordering must be confirmed in the compiled firmware and on
  hardware rather than assumed from source alone.

Likely update scope: keymap plus right-central listener override. Build both
artifacts for comparison; first hardware test should flash the right/central
half only unless the compiled left image changes for a relevant reason. No
settings reset should be required.

### 2. Direct Base primary-click key — simplest fallback

Replace the current Base Mouse-hold thumb with a direct Mouse 1 key. The right
ball is already always active, so this would make pointing and primary click
available without any automatic layer.

Benefits:

- Very small and predictable firmware change.
- Direct Mouse 1 can be held for dragging.
- Removes an unused Mouse-hold gesture.

Tradeoffs:

- It consumes the current position 41 function.
- Secondary click still needs a separately chosen path.
- It works only if that key can be held comfortably while the ball is moving;
  this depends on which digit operates the physical ball.

Likely update scope: keymap only; right/central-first flash test.

### 3. Add automatic activation to the current Mouse layer — rejected

This would make ordinary left-hand typing positions emit Grid Jump commands
after every pointer movement. Even with a short timeout, accidental F13–F24
events and AHK jumps would be unacceptable. Automatic activation is viable only
after the Mouse layer is simplified.

### 4. More coaching for the current workflow — paused

Coaching can help a good hidden workflow. It cannot make a physically awkward
hold-move-click chord comfortable. Mouse coaching should be rewritten only
after the firmware interaction is decided and hardware-tested.

## Recommended order of work

1. Review `POINTING_UX_CANDIDATE.md` and its simplified no-Grid drawing.
2. Implement one right-ball-only, 1000 ms temporary-layer experiment with
   Grid Jump absent and center-thumb Mouse 1.
4. Build both halves, inspect the compiled listener and keymap, and compare the
   left artifact before deciding flash scope.
5. With explicit approval, flash the right half first and test pointer, primary
   click, secondary click, drag, timeout, accidental activation, and immediate
   return to typing.
6. Keep, tune, or revert from that hardware result before touching the left
   ball or resurrecting any advanced pointing feature.

## Success criteria for the first experiment

- Move and primary-click without holding a layer key.
- Secondary click is reachable without a multi-key hold during pointer motion.
- Click-drag works naturally.
- Quick return to typing does not produce accidental clicks.
- Incidental ball contact while typing does not activate Mouse.
- Grid Jump never fires accidentally.
- Left-ball scrolling behaves exactly as before.
