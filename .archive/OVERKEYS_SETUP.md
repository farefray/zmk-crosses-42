# OverKeys integration for Crosses 42

OverKeys is the Windows on-screen overlay that follows your active keyboard
layer. It doesn't talk to ZMK directly — it watches the host for trigger
keys (`F13`, `F14`) and updates its overlay when it sees them. Your firmware
sends those triggers via macros wrapped around the layer changes.

## How the sync works

| Action on the keyboard | What ZMK sends | What OverKeys does |
|---|---|---|
| Hold left inner thumb (momentary SYM) | Tap F13, hold `&mo SYM`, release `&mo SYM`, tap F13 | Toggles overlay → SYM, then back to Base on release |
| Double-tap left inner thumb (lock SYM) | Tap F13, `&to SYM` | Toggles overlay → SYM and stays |
| Tap left inner thumb on locked SYM (unlock) | Tap F13, `&to BASE` | Toggles overlay → Base |
| Hold right inner thumb (momentary NAV) | Tap F14, hold `&mo NAV`, release `&mo NAV`, tap F14 | Toggles overlay → NAV, back to Base on release |

The "tap-on, tap-off" pattern works because `type: toggle` in OverKeys flips
on each press. Two presses = back where you started.

## Install OverKeys

```powershell
winget install AngeloConvento.OverKeys
```

Or grab the installer from
<https://github.com/conventoangelo/OverKeys/releases/latest>. It runs as a
small tray app — right-click the tray icon for Preferences.

## OverKeys settings to flip on

Right-click tray → Preferences. You need:

- **General → Use user layouts** = ON
- **Advanced → Turn on advanced settings** = ON
- **Advanced → Use 6 column layout** = ON
- **Advanced → Show top row** = OFF (the row 0 of the layout is a placeholder for OverKeys' "top row" requirement; you don't want to see it because the Crosses 42 has no number row)

## Drop in the config

1. Right-click tray → Preferences → **Advanced** → **Open config**
2. The OverKeys config file opens in your default JSON editor.
3. Replace its contents with the contents of `overkeys_config.json` from this repo (or merge the `userLayouts` and `defaultUserLayout` keys into your existing config if you want to keep other OverKeys settings).
4. Save the file.
5. Right-click tray → **Reload config**.

You should now see the Crosses 42 base layer in the overlay.

## Test the layer sync

After flashing the updated firmware (Stage 3 + macros), test:

1. Hold the left inner thumb (▽SYM). The overlay should swap to the Sym
   layer (showing `1 2 3 4 5 6 7 8 9 0` etc). Release → back to Base.
2. Double-tap the same thumb. The overlay should *stay* on Sym. The center
   thumb on the overlay's bottom row will read "BASE" — that's the unlock
   action. Tap that physical key once → overlay returns to Base.
3. Hold the right inner thumb (▽NAV). Overlay swaps to Nav (F-keys, arrows).
   Release → back to Base.

If a layer change doesn't update the overlay, the F13/F14 trigger probably
isn't reaching the host. Quickest check: open a text editor, momentarily
press the SYM thumb — the editor's "find" dialog often pops up on F13 or
related extended F-keys, telling you the key was sent. Or use a key
inspector like *KeyboardSpyTool* to see exactly what's being sent.

## Caveats

- **F13/F14 conflicts.** A few apps bind extended F-keys for their own
  shortcuts. If your overlay starts toggling unexpectedly on its own, an app
  is grabbing F13 or F14. Pick different triggers in `overkeys_config.json`
  AND in `crosses.keymap` (any `Fxx` from F13–F24 works) and rebuild both.
- **Lock-state desync after sleep/disconnect.** If the keyboard goes to
  sleep while a layer is locked, ZMK and OverKeys can fall out of sync.
  Tapping the SYM thumb twice usually re-syncs them. We can add a "sync"
  combo if this gets annoying.
- **OverKeys auto-hide.** OverKeys hides itself after a short delay of no
  typing. If you want it to always stay visible, turn that off in
  Preferences.

## What's NOT visualized

- Held HRM mods (e.g. holding `T` for shift). The overlay doesn't know about
  those. ZMK could send a trigger for HRM activation but it'd flicker
  enough to be useless. Skip.
- Tap-dance state. The bracket tap-dances render as `{[<` / `}]>` regardless
  of how many taps you've done.
- Mod-morph state (BSPC vs DEL with shift held). Just renders as BSPC.

These would need a richer protocol than F-key triggers; OverKeys doesn't
have one for ZMK. The Kanata integration does, but kanata is the wrong
choice here as we discussed earlier.

## Tuning the look

OverKeys is heavily themable. Try **Aurora**, **Catppuccin**, or **Matrix**
themes from the README's screenshots — Preferences → Theme. Set opacity
moderate (0.6-0.8) so it doesn't dominate the screen. Position to a corner
of your screen and turn off auto-hide if you want it as a permanent
reference for the first couple weeks.
