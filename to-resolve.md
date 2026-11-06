# to-resolve.md (yip)

## Web version (2026-11-06, the web / deploy commit; continue.md)

Manual checks:

- **Firefox and Safari**: open https://kazin.dev/projects/yip/, Start Game, Ctrl+E a few
  times, fly with the arrows holding A, beat an enemy, Escape and Enter. Why: only Edge
  (Chromium, software WebGL) was tested; Safari's WebGL2 and key events differ most, and
  a browser may keep Ctrl+E for itself (Edge / Chrome: the address bar's search) where
  the page doesn't take it first.
- **A phone**: the same page. Why: yip is keyboard only (arrows, A, Ctrl+E); a phone has
  no way to play yet. Decide whether it needs touch controls.
- **A real GPU and a high refresh rate (120 / 144 Hz)**: the game's speed and the fire
  rate look the same as at 60 Hz. Why: in a browser the logic runs once per animation
  frame with the real time passed, in whole milliseconds (`DeltaVelocity`).
- **Audio**: nothing to check today (yip plays no sound); kazilib's SFML web build has no
  audio module, so a sound added later stays out of the web build (`__EMSCRIPTEN__`)
  until it has one.

Open questions:

- **Enemies only by Ctrl+E**: nothing spawns them on its own, so a visitor sees an empty
  sky until reading the README / the line under the play link. Spawn them on a timer?
- **Collider outlines**: `DEBUG_MODE` (`global.h`) is always on, so the live page shows
  the red origin dots and collider boxes. Turn it off for the web build (or for all)?
- **Arial on a public page**: `deploy\arial.ttf` (Microsoft's) is packed in the web
  build's `index.data`, served by kazin.dev. Is that allowed, or switch to a free font?
