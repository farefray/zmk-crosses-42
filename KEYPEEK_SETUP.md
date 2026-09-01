# KeyPeek setup — Crosses 42

KeyPeek is a Rust-based on-screen keyboard overlay that mirrors the
keyboard's active layer in real time. It replaces OverKeys (see the
`.archive/` folder for the old setup if anything goes sideways).

The pieces:

1. **Firmware integration** (already wired into this repo)
   - `zzeneg/zmk-raw-hid` — adds a Raw HID interface to the device.
   - `config/src/keypeek_notifier.c` — the reproducible Crosses variant of
     `srwi/zmk-keypeek-layer-notifier`; forwards layer, key-position, and
     explicit-modifier events into Raw HID.
   - The `raw_hid_adapter` shield is appended to both halves in
     `build.yaml`. `CONFIG_RAW_HID=y` is set in `crosses.conf`.
2. **ZMK Studio** is already enabled (right half, via the
   `studio-rpc-usb-uart` snippet + `CONFIG_ZMK_STUDIO=y` cmake arg).
   KeyPeek reads the live keymap and physical layout via Studio's RPC.
3. **KeyPeek desktop app** — install separately, see below.

The Crosses 42 fork also includes optional local aggregate usage metrics. They
are disabled by default and configured from **KeyPeek Settings → Local Usage
Metrics**. See the fork's `docs/LOCAL_USAGE_METRICS.md` for the exact stored
fields and privacy boundary. This desktop-only feature does not require another
firmware flash. The installed release was confirmed connected and collecting by
Max on 2026-09-01.

## Install KeyPeek

Grab the latest release for your platform from
https://github.com/srwi/keypeek/releases. As of writing, 0.3.1 is the
current build (March 2026).

Windows: extract the zip somewhere and run `keypeek.exe`. No installer
required.

## First-run sequence

1. Flash the new firmware to **both** halves. Because the HID descriptor
   set has changed (a new Raw HID interface), Bluetooth pairing may need
   to be redone — see "BT re-pair dance" below.
2. Plug the **right** half into USB. (Right has the studio snippet, which
   KeyPeek uses to fetch keymap + layout.)
3. Launch KeyPeek. The connected device should appear in the dropdown.
4. Select it. The overlay should render automatically with the current
   keymap labels and the active layer highlighted.

When you switch layers (SYM, NAV, MOUSE, UKR, GAME), the overlay should
follow within a frame or two — Raw HID is sub-millisecond, so any visible
delay is the GUI redraw budget, not transport latency.

## What changed in the keymap

The old OverKeys integration toggled F13-F17 keystrokes whenever a layer
was entered or exited, then OverKeys watched for those keystrokes on the
host side. That bought us five purpose-built macros (`m_sym_mo`,
`m_nav_mo`, `m_mouse_enter`, `m_mouse_exit`, `m_mouse_mo`,
`m_ukr_enter`, `m_ukr_exit`, `m_game_enter`, `m_game_exit`) plus a
hand-tuned `caps_word.continue-list` to keep an active Caps Word session
alive while the trailing F-key fired on NAV release.

All of that is gone in Stage 6. The keymap now uses bare `&mo` and
`&tog`. KeyPeek reads layer state directly from the device, so there is
nothing to sync.

`overkeys_config.json` is preserved in `.archive/` if you ever need to
fall back; the old setup guide is at `.archive/OVERKEYS_SETUP.md`.

## BT re-pair dance

The `raw_hid_adapter` shield adds a new HID interface. Hosts that have
already paired via Bluetooth before the change will see a different
descriptor and may refuse to bond, or pair-but-fail-to-route reports.

If keystrokes don't make it to the host after flashing:

1. Forget the keyboard on the host (Settings → Bluetooth → ...).
2. Flash `settings_reset.uf2` to **both** halves.
3. Re-flash `crosses_left.uf2` and `crosses_right.uf2`.
4. Run the split-pair recovery sequence from `AGENT_HANDOFF.md`
   (power-cycle both halves within ~5s of each other, wait 60-90s).
5. Re-pair from the host.

For USB-only operation (which is how you'd typically use KeyPeek
anyway, since the right half is plugged in), no re-pair is needed —
this only affects BT bonds.

## Troubleshooting

**KeyPeek doesn't see the device**
- Confirm the right half is the one plugged in (the side with the
  `studio-rpc-usb-uart` snippet).
- Check that the `raw_hid_adapter` shield made it into the build —
  inspect `build.yaml` and confirm both shield lines list it.
- In Windows Device Manager → HIDs, the keyboard should show
  multiple HID-compliant device children. One of them is the Raw
  HID endpoint (usage page 0xFF60 by default, per zmk-raw-hid's
  Kconfig).

**Layer changes show up but keymap labels are wrong**
- KeyPeek reads the keymap via Studio. If `CONFIG_ZMK_STUDIO=y` got
  dropped from the right-half cmake-args, the labels will fall back to
  defaults or the device will refuse the Studio handshake.
- Verify with: in Studio (web UI), connect to the keyboard. If Studio
  works, KeyPeek should too.

**Studio NVS overrides ghost in**
- Already documented in `AGENT_HANDOFF.md` §3 — `CONFIG_ZMK_STUDIO_LOCKING=n`
  lets Studio write per-position overrides that survive reflashes. If
  the live keymap diverges from `crosses.keymap`, run a `settings_reset`
  cycle on the affected half.
