# Crosses 42 KeyPeek layer coach specification

Status: Paused pending the pointing firmware redesign
Work item: UX-005
Scope: KeyPeek design only; no implementation or live configuration change is
authorized by this document.

Max's subsequent lived feedback established that the current hold-to-click
Mouse workflow is physically awkward and Grid Jump is unused. Mouse coaching
must be rewritten after `POINTING_UX_AUDIT.md` is resolved; do not implement
this draft as-is.

## What layer coaching means

The existing KeyPeek keyboard overlay answers **where is each key?** Layer
coaching should answer **what is this layer for, and what should I try first?**

It is a small, click-through teaching card that appears only when a configured
non-base layer remains active long enough to look intentional. It contains one
purpose statement and three or four high-value actions. It is not a second
keyboard diagram, a guided tour, a notification history, or usage telemetry.

Example:

```text
┌─ Mouse · jump, refine, click ───────────┐
│ 1–18        Jump to a screen zone       │
│ Right ball  Refine the pointer           │
│ Thumb M1    Click                        │
│ EXIT        Leave toggled Mouse          │
└─────────────────────────────────────────┘
```

## Recommended v1 experience

1. Base never shows a coaching card.
2. When Sym, Nav, Mouse, UKR, or Game becomes active, start a quiet 350 ms
   threshold.
3. If the same layer remains active, show its card at the top-center of the
   screen. Quick practiced layer taps therefore remain quiet.
4. Hide the card immediately when the layer exits or a different coached layer
   becomes the active candidate.
5. Auto-hide after 4.5 seconds even when a toggled layer remains active. Do not
   show it again until that layer has exited and been entered again.
6. Do not animate the card in v1. The threshold supplies calmness without
   introducing motion or relying on animation-completion events.
7. If the existing application-specific hotkey coach also matches, show only
   that more specific coach. Never stack two teaching cards.
8. Provide a plain **Enable layer coaching** checkbox in KeyPeek settings. It
   should default off during development and be enabled deliberately for the
   learning trial.

The 350 ms threshold is slightly longer than the current 200–260 ms keyboard
tap/hold decisions. It should avoid flashing during quick layer chords while
remaining easy to invoke by deliberately holding a layer for help. The 4.5
second lifetime follows the usual 3–5 second range for non-critical transient
guidance.

## Proposed content

Keep each card to a title, one short purpose line, three or four rows, and an
optional footer. Content must use user-facing names rather than firmware
behavior identifiers.

### Sym

- Title: `Sym · numbers & punctuation`
- Purpose: `Hold for a symbol · double-tap the thumb for NumWord`
- `1–0` — Numbers
- `[ ] { } \` — Brackets and escape character
- `< > = .` — Comparison and version punctuation
- `BASE` — Leave NumWord

### Nav

- Title: `Nav · edit & move`
- Purpose: `One hand for editing, navigation, and system actions`
- `Undo Cut Copy Paste` — Clipboard row
- `← ↓ ↑ →` — Move the caret
- `Alt-Tab` — Switch Windows tasks
- `UKR GAME MOUSE` — Toggle a persistent layer

### Mouse

- Title: `Mouse · jump, refine, click`
- Purpose: `Choose a zone first; use the right ball for precision`
- `1–18` — Jump to a screen zone
- `Right ball` — Refine the pointer
- `Thumb M1/M2/M3` — Click
- `EXIT` — Leave toggled Mouse
- Footer: `Grid Jump requires the Windows helper`

### UKR

- Title: `UKR · Ukrainian input`
- Purpose: `The firmware layer and Windows language must agree`
- `Win+Space` — Switch Windows to Ukrainian first
- `Top-left` — Exit UKR
- `Ctrl` — Dedicated Base Ctrl still falls through
- Footer: `Switch both states back when finished`

The rare `Ґ` shortcut is intentionally excluded from coaching until it is
hardware-verified. Static layer resolution indicates that active UKR layer 4
can mask the lower Sym layer 1 binding at the documented position.

### Game

- Title: `Game · timing-free QWERTY`
- Purpose: `Plain keys with no home-row or thumb timing decisions`
- `QWERTY` — Conventional movement layout
- `Ctrl Space Alt` — Dedicated thumbs
- `EXIT` — Three safe exit positions

## Visual and interaction rules

- Reuse the established hotkey-coach card language: dark translucent surface,
  rounded corners, warm title, green key labels, and plain action text.
- Default width: 360 px. Allow natural height; do not clip or horizontally
  scroll.
- Default position: top-center. The current keyboard overlay defaults to
  bottom-right and the app-specific coach defaults to top-right.
- Maintain at least 4.5:1 text contrast against the card surface.
- Remain click-through and never take keyboard focus.
- Do not use close, hover, or pointer-only controls on the card. The layer exit,
  auto-hide timer, and settings checkbox provide dismissal and user control.
- A rapid layer change cancels the old pending/visible state and directly
  starts the new candidate. No stale card may appear later.
- Hide while KeyPeek settings are open, when the keyboard disconnects, and for
  active layers with no configured coaching entry.

## State model

KeyPeek already receives case-insensitive ZMK display names through
`Keyboard::active_layer_names()`. The layer coach should consume those names;
it does not need a firmware or protocol change.

When several layers are active, choose the highest configured active layer by
firmware layer order. State transitions are:

```text
inactive → candidate delay → visible → timed out
    ↑            │             │          │
    └ layer exit ┴ layer exit ─┴──────────┘

candidate change → cancel previous state → new candidate delay
```

After `timed out`, the same layer stays silent until it exits. This prevents a
toggled UKR, Game, or Mouse layer from repeatedly resurfacing the card.

## Configuration boundary

Recommended implementation:

- Add a separate strict `layer-coach.json` v1 contract for layer names,
  content, timing, position, width, and opacity.
- Keep it separate from `hotkey-coach.json`; the existing hotkey schema
  deliberately requires a foreground-process match, while layer coaching is
  keyboard-global.
- Reuse a presentation-only coach panel renderer so the two features look
  related without merging their matching/state logic.
- Preserve first-run template versus live-file behavior, one-second hot reload,
  strict unknown-field rejection, and last-known-good fallback.
- Store only the ordinary enable/disable preference in KeyPeek settings.
- Do not edit the live `%APPDATA%` file during implementation without Max's
  explicit approval.

This feature reads only the active layer names already available to KeyPeek.
It must not store typed characters, physical key events, layer history,
foreground processes, window titles, or any usage counters.

## Implementation slices

Each slice should remain reviewable, with no firmware work:

1. Add a pure layer-coach state machine and deterministic timing tests.
2. Add the strict configuration model, bundled template, reload/fallback tests,
   and documentation.
3. Extract a presentation-only shared coach card renderer without changing the
   existing hotkey coach's matching or visible content.
4. Add the settings checkbox and integrate layer selection, timer scheduling,
   and hotkey-coach precedence.
5. Build the release application and perform Windows visual QA before touching
   the live installation.

## Acceptance checks

- A layer activation shorter than 350 ms produces no teaching card.
- Holding each coached layer past the threshold displays the correct card.
- Layer exit or layer replacement hides/replaces the card immediately.
- A toggled layer auto-hides after 4.5 seconds and stays silent until re-entry.
- Base and unknown layers never show a card.
- The app-specific hotkey coach suppresses the generic layer card when both
  match.
- Invalid JSON retains the last valid in-memory configuration.
- The checkbox disables all layer coaching without disabling the keyboard
  overlay or application-specific hotkey coach.
- The card is legible and non-overlapping at Windows 100%, 125%, 150%, and
  200% scaling, against both light and dark foreground content.
- The overlay remains click-through and never steals focus.
- Rust formatting, tests, Clippy with warnings denied, release build, and diff
  checks pass.
- Manual UI verification is reported separately from compilation.

## Decisions requested from Max

The recommended defaults to approve or adjust before implementation are:

1. 350 ms intentional-hold threshold.
2. 4.5 second maximum display time.
3. Top-center placement.
4. App-specific hotkey coach takes priority instead of showing two cards.
5. Global enable/disable checkbox for v1; per-layer muting can wait until real
   use shows it is needed.
