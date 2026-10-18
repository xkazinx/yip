# Yip

A side-scrolling space shooter in SFML 3.1 (C++23, x86), built on kazilib.

The world scrolls right; the ship (the white triangle) moves inside the left 80 % of
the screen and fires at the yellow enemies, which fire red shots back at it.

## Keys

| Key | What |
|---|---|
| Enter / click Start Game | from the menu into a new game |
| Arrows | move (Left / Right against the scroll, Up / Down) |
| A (held) | fire, every 100 ms |
| 1 / 2 / 3 | weapon: Normal / Laser / Triple (Laser fires the normal shot for now) |
| Ctrl+E | an enemy, off the right of the screen |
| Escape | back to the menu |

The top left shows enemies beaten (of those that came), shots that hit (of those
fired) and the enemy shots taken. `DEBUG_MODE` (`global.h`) draws each object's
origin and collider in red.

## Build

```
powershell -ExecutionPolicy Bypass -File build.ps1
```

`setup.ps1` (run by `build.ps1`) checks out the kazilib submodule (`src\dep\kazilib`)
and SFML 3.1.0 x86 (`src\dep\SFML-3.1.0`: a junction to `D:\code\kazilib\SFML-3.1.0`
when it is there, else the official 32-bit release, downloaded). The build is kazilib's
FASTBuild tool on `src\fastbuild.json`: Release, x86, the 64-bit-hosted tools, v145,
distributed to the workers that answer. Output: `deploy\yip.exe` with SFML's DLLs and
the fonts beside it. `-Configuration Debug` builds `yip_d.exe`.

## Launch flags (kazilib `initflags.h`)

`title=<text>` (the window's title), `fps` (prints the logic / render counts each
second), `catchexceptions` (an exception writes `deploy\exceptions\exception_<pid>.txt`
and ends the run, no JIT dialog), `testerino=<slot>` (the test bridge).

## Tests

`node src\dep\kazilib\testerino\tt.js run src\tests\testerino\play.tt` plays it end to
end: the menu, Start Game, the weapons, an enemy beaten, moving to the edges, missed
shots, Escape and a fresh game (report in `testerino\reports\play.txt`, screenshots in
`testerino\client_1\shots\`). The bridge's verbs (`help` lists them): `state`, `key`,
`hold` / `release` (keys the game reads every tick), `spawn`, `click`, `shot`, `quit`;
conditions `scene=`, `killed=` / `killed~` (at least), `enemies=`, `hits_taken=`.
