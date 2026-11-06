# continue.md (yip, master)

## Repos

| Repo | Commit | State |
|---|---|---|
| yip | master: the showcase retake (this file's) on `2e49f42` (the web / deploy commit) | pushed |
| kazilib (`src/dep/kazilib`) | master `ad10d04` (points drawn in a browser: the star field; on `a216c7d` browser.js held keys, `2a274a9` web builds) | pushed |

The kazilib pointer moved from `50df5d0` to master: `Testerino.cpp` reads
`tcore::Endpoint::_dir` (was `Dir()`), `SceneMgr::Current` reads `Manager::_current` (was
`Current()`).

## Last work (2026-11-06)

- **Web:** yip in a browser (Emscripten, kazilib's SFML web build: SDL3 window, GL ES 1
  on WebGL2). `src\yip\CMakeLists.txt`, `build_web.ps1` → `web\`. Browser-only: one tick
  and one frame per pass (`App::Tick`), keys from SDL's state without the focus check
  (`input::Held`), no depth / stencil request; Windows-only behind `_WIN32` (Testerino
  bridge stubs, `TextMgr`'s `ExeDir`, `targetver.h` / `tchar.h`).
- **Console lines** for the browser test: `Scene: <name>` (SceneMgr) and
  `Enemy beaten: <n>` (SGame).
- **Deployed:** https://kazin.dev/projects/yip/ (`deploy_ubuntu.ps1`): `/opt/yip/web` on
  `kazin-music`, Caddy snippet `kazin.dev.d/yip.caddy`; static, no service.
- **Star field in the browser:** WebGL draws `GL_POINTS` only at the shader's
  `gl_PointSize`, which kazilib's GL ES 1 emulation never set, so the stars
  (`sf::PrimitiveType::Points`) were 0 px; kazilib `ad10d04` sets it. Rebuilt and redeployed
  (the coordinator), showcase retaken with the stars.
- **Showcase** from the live page: `docs\screenshot.png` (README top), `docs\showcase\`
  `1_menu` .. `5_laser.png`, `docs\showcase\kazin\yip.jpg`, `yip-1` .. `yip-4.jpg`
  (1024x768, for the kazin.dev card; the portfolio repo is the parent session's).
- **kazilib:** `web/tools/browser.js` `{"down": k}` / `{"up": k}` held keys and held
  Control / Shift / Alt.
- **struct, public:** 19 headers (`2e46a03`).

## Tested

| Test | Result |
|---|---|
| Testerino `play.tt` (native, Windows) | 69 passed, 0 failed |
| `src\tests\web\run_web_test.ps1` (local, `web\`, headless Edge) | passed (enemies beaten, Escape / Enter, no page error) |
| same, `-Url https://kazin.dev/projects/yip/` (on kazilib `ad10d04`, stars drawn) | passed (4 enemies beaten) |

Known gaps: only Chromium (Edge, SwiftShader) tested; no Firefox / Safari, no phone, no
real GPU (`to-resolve.md`).

## Next steps

- `to-resolve.md`: the manual checks and open questions (enemies only by Ctrl+E, the
  collider outlines on screen, Arial on a public page).

## The user's prompts (this work)

1. "now I want you standarize the work and do the same with the githubs of yip, yips, tileship, tiled, nft, use various agents if well scoped"
2. "all the process we've done with tictactoe"
3. "I don't see the yip background"
