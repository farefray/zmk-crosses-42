# Crosses 42 keyboard UX improvement plan

This is the durable, multi-session roadmap for improving how Max learns,
uses, and evolves the Crosses 42. It complements `AGENT_HANDOFF.md`, which
remains the operational firmware handoff.

## Current checkpoint

- Started: 2026-08-28
- Current phase: Phase 3 — one workflow at a time
- Next action: checkpoint the hardware-verified Auto Mouse, reduced-HRM, and
  motion-gate sources, then choose the next single usability experiment.
- Firmware changes authorized now: none
- Flashing authorized now: none
- KeyPeek behavior changes authorized now: none
- Active principle: measure, teach, simplify

## Goal

Increase actual daily use of the keyboard's valuable capabilities while
reducing memorization burden, accidental activations, duplicated paths, and
features that exist only on paper.

Success means:

1. The current layout is accurately visible when and where it is needed.
2. High-value actions become discoverable without consulting historical notes.
3. We can distinguish an undiscovered feature from an awkward or unnecessary
   feature using privacy-preserving evidence.
4. Firmware changes are small, reversible, hardware-tested, and retained only
   when they improve the experience.

## Non-goals

- No wholesale keymap rewrite.
- No new alpha layout merely for novelty.
- No auto-shift revival unless Max explicitly reopens it.
- No storage of typed characters, words, key sequences, clipboard data, or
  foreground-window titles.
- No simultaneous firmware and KeyPeek behavior changes in one experiment.
- No flashing or live configuration writes without Max's explicit approval.

## Baseline snapshot — 2026-08-28

### Hardware and platform

- 42-key wireless split Crosses with two PMW3610 trackballs.
- Right half is the ZMK central half and the Studio/Raw HID endpoint.
- Primary host is Windows; both English and Ukrainian input workflows matter.
- Active firmware commit: `280f43efdfdaa338c8e00fe4eae2584061e3446e`.

### Current interaction surface

- Layers: Base, Sym, Nav, Mouse, UKR, Game.
- Custom behaviors: `hml`, `hmr`, `ht`, `sht`, `bspc_del`, `mscl`,
  `td_sym`, `swapper`.
- Combos: Caps Word, bootloader, Mouseless overlay, Mouseless free mode,
  Ctrl+V.
- Major advanced workflows: home-row mods, NumWord, Nav editing cluster,
  Alt-Tab swapper, media controls, trackball buttons and scrolling, Mouse Grid
  Jump, Ukrainian input, and a timing-free Game layer.
- KeyPeek provides live layer visualization and an app-aware hotkey coach, but
  currently has no persistent usage measurement.

### Known baseline problems

1. `README.md` and `keymap-drawer/crosses.svg` describe an old layout rather
   than the active six-layer keymap. This is the highest-confidence
   discoverability defect.
2. `AGENT_HANDOFF.md` does not yet summarize Stage 10 Grid Jump even though it
   is present in the active keymap.
3. Powerful actions are distributed across the keymap comments, nine stage
   documents, KeyPeek documentation, and muscle memory; there is no concise
   daily-use guide.
4. Ctrl and mouse workflows have several overlapping access paths. We do not
   yet know which are valuable, undiscovered, awkward, or redundant.
5. `zmk-raw-hid`, `zmk-keypeek-layer-notifier`, `zmk-auto-layer`, and
   `zmk-tri-state` track moving `main`/`master` revisions, reducing build
   reproducibility.
6. Space/Shift `quick-tap-ms = 0` is a deliberate timing decision with a
   repeat tradeoff. It must be evaluated from real use, not changed from a
   generic checklist.

### Existing workspace state to preserve

The repository already contains untracked backup files, UF2 artifacts,
keymap-drawer scratch files, and module checkouts. They predate this plan and
must not be deleted, staged, or treated as plan output without explicit review.

## Adopted expert reference

`keymap-ai` is installed as a personal Codex skill from the reviewed commit:

`7177613d800926fc07e4ba512a863cec49d3091c`

Installed path:

`C:\Users\maksy\.codex\skills\keymap-ai`

Use it as an audit and debugging reference, not as authority over Max's
hardware-tested preferences. Version-sensitive recommendations still require
current ZMK documentation or compiled-output verification.

## Phases

### Phase 0 — current-state audit

Deliverables:

- `KEYBOARD_UX_AUDIT.md` containing the 42-position grid, per-layer tables,
  combo map, behavior inventory, firmware/module topology, and findings ranked
  by expected payoff.
- Each finding includes a real failure or friction scenario, proposed remedy,
  affected repository, required build/flash scope, and verification method.
- Accurate current per-layer diagrams, initially generated locally rather than
  automatically committed by CI.

Exit criteria:

- The audit reflects the actual current sources, not the old README image.
- Max confirms which findings match lived experience.
- No firmware behavior has changed.

### Phase 1 — discoverability

Candidate work, ordered from least intrusive:

1. Replace stale diagrams and add a compact daily-use layer guide.
2. Show each layer's purpose and its few highest-value actions in KeyPeek.
3. Improve contextual coaching only where it reduces lookup burden; avoid a
   permanently noisy overlay.
4. Visually verify Windows scaling, opacity, light/dark content, held layers,
   and app-specific coach interaction.

Exit criteria:

- The documentation and overlay agree with active firmware.
- A user can discover the primary action of every non-base layer within a few
  seconds.
- No firmware reflash is required unless separately approved.

### Phase 2 — privacy-preserving usage evidence

Design before implementation. Candidate local aggregates:

- Layer activation count.
- Layer dwell-time buckets.
- Physical key-position counts per layer.
- Layer-to-layer transition counts.
- Explicit exit versus timeout/implicit exit where observable.

Privacy boundary:

- Never persist produced characters, words, ordered key streams, clipboard
  content, window titles, or document content.
- Prefer aggregate counters written locally by KeyPeek.
- Keep collection off by default until Max explicitly approves the design.
- Provide a visible reset/export action and document the storage location.

Exit criteria:

- The privacy design is approved before implementation.
- Metrics survive restart, can be reset, and cannot reconstruct typed text.
- At least seven representative days are observed before pruning features.

### Phase 3 — simplify and improve one workflow at a time

Initial evaluation candidates, not predetermined changes:

1. NumWord entry and exit.
2. Nav clipboard/editing cluster.
3. Mouse/Grid Jump entry, toggle, and click paths.
4. Ctrl access duplication.
5. Space/Shift timing and repeat tradeoff.
6. Low-value or hard-to-discover toggled layers.

For each experiment:

1. State one hypothesis and the evidence behind it.
2. Capture the before state and expected failure scenario.
3. Make one reversible change in one repository.
4. Build and perform static/visual QA.
5. State exactly which half or application must be updated.
6. Wait for explicit flash/run approval.
7. Record Max's hardware result before starting the next experiment.

### Phase 4 — maintenance hardening

- Pin compatible module revisions after verifying them against the active ZMK
  commit and compiled devicetree.
- Evaluate a one-handed System layer for Bluetooth profiles, output switching,
  reset, bootloader, and travel soft-off; do not add it automatically.
- Add diagram automation only after local output is trusted and the bot-commit
  workflow is explicitly accepted.
- Reconcile `AGENT_HANDOFF.md`, stage history, README, and active sources.

### Phase 5 — continuous iteration

- Review aggregate usage and subjective friction periodically, not constantly.
- Prefer coaching for valuable-but-hidden actions.
- Prefer redesign for frequently attempted-but-awkward actions.
- Prefer removal for low-value actions that remain unused after being taught.

## Work register

| ID | Work item | Status | Evidence / dependency |
|---|---|---|---|
| UX-001 | Install pinned `keymap-ai` skill | Done | Commit `7177613...` installed 2026-08-28 |
| UX-002 | Produce current-state UX audit | Done | `KEYBOARD_UX_AUDIT.md`; source-derived tables and ranked findings |
| UX-003 | Generate accurate per-layer diagrams | Done | Six layer SVGs plus combined and combo views; local visual QA |
| UX-004 | Replace stale README and add daily-use guide | Done | Approved 2026-08-28; documentation only |
| UX-005 | Design compact KeyPeek layer coaching | Paused | Draft exists; Mouse content must follow the pointing redesign |
| UX-006 | Write local aggregate-metrics privacy spec | Done | Approved and documented in KeyPeek 2026-09-01 |
| UX-007 | Implement approved KeyPeek metrics | Done | Small opt-in local counter; no firmware change |
| UX-008 | Collect representative usage baseline | In progress | Collection confirmed working 2026-09-01; review after at least seven days |
| UX-009 | Run first single-workflow experiment | Done | Auto Mouse retained after daily-use feedback |
| UX-010 | Pin firmware module revisions | Done | All fetched firmware modules pinned; customized KeyPeek notifier maintained locally; clean left/right builds passed 2026-09-01 |
| UX-011 | Focused pointing UX audit | Done | `POINTING_UX_AUDIT.md`; lived feedback overrides discoverability assumption |
| UX-012 | Select first no-hold click experiment | Approved | 1000 ms Auto Mouse; no Grid Jump; center-thumb Mouse 1 |
| UX-013 | Implement and hardware-test pointing experiment | Done | Flashed by Max; Auto Mouse works well |
| UX-014 | Restrict BASE HRMs to Win/Alt | Done | A/R/I/O retain HRMs; S/T/N/E are plain; hardware-confirmed 2026-09-01 |
| UX-015 | Gate incidental Auto Mouse activation | Done | 6 counts within 80 ms plus 500 ms prior-idle guard; hardware-confirmed 2026-09-01 |

## Decision log

### 2026-08-28

- Adopted `keymap-ai` as a pinned advisory skill, not a runtime dependency or
  source of automatic firmware changes.
- Chose discoverability and evidence as the first problems to solve; adding
  more features is not the default response.
- Chose local aggregate metrics as the possible evidence mechanism, subject
  to a separate privacy-design approval.
- Preserved the existing stage-by-stage and explicit-flash-approval workflow.
- Confirmed that source truth includes Stage 10 Grid Jump while the keymap
  header, README, and handoff narration had drifted.
- Chose user-facing Ukrainian glyphs and numbered Grid Jump zones as drawing
  overlays; literal HID bindings remain preserved in the parsed source YAML.

## Multi-session protocol

At the start of every session:

1. Read this file and `AGENT_HANDOFF.md`.
2. Check both repository working trees before editing.
3. Confirm the current phase, next work-register item, and authorization scope.
4. Read only the relevant `keymap-ai` references and verify version-sensitive
   claims against current sources or official documentation.

At the end of every session:

1. Update the current checkpoint and work-register status.
2. Append decisions that constrain future work.
3. Record verification performed and anything still pending hardware testing.
4. Leave one unambiguous next action.
5. Keep firmware and KeyPeek commits separate and stage only intended files.

## Session log

### 2026-08-28 — roadmap bootstrap

- Evaluated `keymap-ai` and reviewed its skill, ZMK audit, behavior, pitfall,
  and diagram guidance.
- Installed the skill at the pinned reviewed commit.
- Captured the initial hardware, firmware, layer, behavior, combo, tooling, and
  documentation baseline.
- Created this roadmap.
- Next: UX-002, read-only current-state audit.

### 2026-08-28 — UX-002 and UX-003

- Produced `KEYBOARD_UX_AUDIT.md` from the active firmware, shield, module,
  and KeyPeek sources.
- Recorded the 42-position grid, all six layers, all five combos, all eight
  custom behaviors, both pointing paths, and ten ranked findings.
- Regenerated combined, per-layer, and combo-only diagrams with both
  trackballs represented.
- Added drawing-only Ukrainian output labels and Grid Jump zone numbers.
- Visually checked Base, Nav, Mouse, UKR, and combo diagrams and corrected
  label and overlap issues found during rendering.
- Updated `AGENT_HANDOFF.md` to acknowledge Stage 10 without claiming an
  unrecorded hardware result.
- No firmware or KeyPeek behavior changed; no build, flash, or live config
  write was performed.
- Next: Max reviews the audit, then UX-004.

### 2026-08-28 — UX-004

- Max approved the documentation-only discoverability step.
- Replaced the template README with a compact daily-use guide grounded in the
  active keymap and the verified per-layer diagrams.
- Documented one recommended workflow for Sym/NumWord, Nav editing, Mouse Grid
  Jump, UKR state pairing, and Game entry/exit.
- Added the combo map, hardware roles, KeyPeek boundary, safe maintenance
  pointers, source-of-truth rule, and upstream module credits.
- Explicitly marked Grid Jump as source-present without claiming an unrecorded
  latest hardware result.
- No firmware or KeyPeek behavior changed; no build, flash, or live config
  write was performed.
- Next: UX-005, compact KeyPeek layer coaching design.

### 2026-08-28 — UX-005 design draft

- Defined layer coaching as a compact purpose-and-actions card that complements
  the full keyboard overlay instead of duplicating it.
- Proposed a 350 ms intentional-hold threshold and 4.5 second maximum display
  so practiced quick chords remain quiet.
- Proposed top-center placement, a global settings toggle, no v1 animation,
  and app-specific hotkey-coach precedence to prevent competing cards.
- Drafted user-facing Sym, Nav, Mouse, UKR, and Game content plus a bounded
  KeyPeek-only implementation and QA sequence.
- Preserved the privacy boundary: active layer names are read live, but no key,
  character, history, process, title, or usage data is stored.
- Static layer-priority review found that the documented UKR `Ґ` path may be
  masked by the higher UKR layer. Excluded it from coaching pending hardware
  verification.
- No KeyPeek code, live configuration, firmware, build, or flash was changed.
- Next: Max reviews the five proposed defaults in the layer-coach spec.

### 2026-08-28 — pointing priority correction

- Max reported that Grid Jump has never been used in practice and that the
  trackballs are largely unused because clicking requires an additional held
  layer key.
- Reclassified the mouse issue from discoverability to an ergonomic firmware
  failure. Paused UX-005 instead of coaching the current workflow.
- Produced `POINTING_UX_AUDIT.md` from the active keymap, Crosses shield
  overlays, and the pinned ZMK temporary-layer implementation.
- Rejected automatic activation of the current Mouse layer because its 18 Grid
  Jump bindings could hijack ordinary left-hand keys after pointer movement.
- Recommended first evaluating a simplified, right-ball-triggered temporary
  click layer with transparent alphas and two opposite-hand click positions.
- Preserved a smaller fallback: replace the unused Base Mouse-hold thumb with a
  direct primary-click key if the physical posture supports simultaneous ball
  movement and key hold.
- No firmware, build, flash, KeyPeek behavior, or live helper changed.
- Next: confirm the physically comfortable click/drag fingers and positions.

### 2026-08-28 — Auto Mouse candidate

- Clarified that the right ball is operated by normal hand movement; no
  finger-specific mapping is required because the opposite left half becomes
  the control surface.
- Selected a candidate where right-ball motion temporarily activates Mouse,
  the 18 left alpha keys remain Grid Jump zones, and the left thumb cluster is
  Mouse 1/2/3.
- Confirmed from source that the existing KeyPeek notifier forwards every ZMK
  layer-state change. The current KeyPeek non-base visibility setting should
  show the temporary Mouse layer without a new desktop feature.
- Preserved the left ball's existing scrolling path and the current manual
  Mouse thumb as a first-test fallback.
- Documented the unresolved post-pointer typing risk and two policies: direct
  zones or a guarded Grid sublayer.
- Generated a drawing-only candidate; no firmware, KeyPeek, live helper, build,
  or flash changed.
- Next: choose the zone policy and confirm Mouse 1/2/3 thumb order.

### 2026-08-28 — Auto Mouse direction approved

- Max accepted an intentional return-to-typing delay of up to about one second
  after right-ball movement.
- Removed Grid Jump from the candidate entirely rather than guarding it.
- Reworked the left side around conventional mouse actions: five buttons on
  the home row, discrete wheel directions on the bottom row, and a quiet top
  row reserved for needs demonstrated by real use.
- Selected Mouse 2 / Mouse 1 / Mouse 3 on the left thumbs so primary click and
  click-drag use the center Space position.
- KeyPeek remains the visible Mouse-mode indicator through the existing layer
  notifier; no desktop change is planned for this firmware experiment.
- Firmware implementation is now the next action. Build and static inspection
  come before a separate flash approval.

### 2026-08-28 — Auto Mouse implementation and build

- Added the core ZMK temporary-layer processor to the right/central trackball
  listener before report-rate limiting.
- Compiled values: Mouse layer 3, 1000 ms timeout, 150 ms prior-idle guard,
  and exclusions for all left-side mouse buttons, wheel actions, and thumbs.
- Replaced all 18 Grid Jump bindings with conventional mouse buttons, wheel
  actions, and quiet positions. Center left thumb is now Mouse 1.
- Built right and left firmware successfully in isolated candidate folders.
  Existing root UF2 files were not overwritten.
- Compiled left devicetree keeps the central listener disabled and preserves
  the peripheral left-ball scroll chain.
- No firmware was flashed and no KeyPeek code or live configuration changed.
- Next: explicit approval to flash the right/central candidate and live-test
  Auto Mouse plus KeyPeek visibility.

### 2026-09-01 — reduced HRMs and motion-gated Auto Mouse verified

- Max confirmed that the right/central motion-threshold firmware had already
  been flashed and currently works great.
- BASE HRMs are now deliberately limited to A/Win, R/Alt, I/AltGr, and O/Win;
  S/T/N/E remain plain keys with dedicated Ctrl and Shift alternatives.
- Auto Mouse keeps its 1000 ms layer timeout and conventional left-side mouse
  controls. A central-only gate now requires six cumulative X/Y counts within
  80 ms before activating Mouse, while preserving all pointer motion.
- The 500 ms prior-idle guard and motion gate prevent ordinary typing/key
  vibration from raising Mouse without making intentional pointing sluggish.
- The right/central artifact is hardware-verified. The left compatibility
  build compiled successfully but did not need flashing for this listener-only
  change.
- Next: checkpoint only the intended firmware sources, then choose one new
  experiment; do not bundle further firmware changes into this proven state.

### 2026-09-01 — UX-010 reproducible firmware dependencies

- Pinned the Crosses shield, both PMW3610 drivers, report-rate limiter,
  input-processor helpers, Raw HID, Auto Layer, and Tri-State to the exact
  revisions already present in the verified workspace. ZMK was already pinned.
- Replaced the unpublished local KeyPeek module checkout with an attributed
  local firmware extension that preserves the existing layer, key-position,
  and explicit-modifier Raw HID packet protocol.
- Validated the resolved West manifest and completed pristine builds for the
  right/central and left/peripheral targets using the authoritative build
  matrix. Right compiled with Studio; left deliberately did not.
- Compiled right devicetree retains Mouse layer 3, 1000 ms timeout, 500 ms
  prior-idle guard, six-count threshold, and 80 ms activation window. The left
  build retains peripheral scrolling and keeps the central listener disabled.
- No keymap behavior changed and no firmware flash is required for this
  maintenance checkpoint.
- Next: gather lived UX feedback; UX-006 privacy-spec design remains available
  without enabling collection.

### 2026-09-01 — UX-006/007 local aggregate metrics

- Kept the implementation in the KeyPeek desktop fork; the existing Raw HID
  position and layer packets required no firmware change or flash.
- Added one opt-in aggregate store with physical-position counts by layer,
  layer activations/transitions, and four hold-duration buckets. It stores no
  characters, ordered event history, applications, window titles, or clipboard
  data and has no network path.
- Added a compact Settings section with current totals, export, reset, exact
  data path, and a scrollable settings surface. Collection remains off by
  default and covers only periods when KeyPeek is connected.
- Validation: focused unit tests, full test suite, strict Clippy, release build,
  and isolated visual QA of the settings surface.
- Runtime confirmation: Max launched the installed release and confirmed that
  local counters are actively collecting on 2026-09-01.
- Next: gather at least seven representative days before drawing layout
  conclusions (UX-008); no new firmware experiment is scheduled meanwhile.
