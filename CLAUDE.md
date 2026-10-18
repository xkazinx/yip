# CLAUDE.md (yip)

`D:\code\CLAUDE.md` applies here too ("upd", commit messages, builds). This project:

- **Build:** `powershell -ExecutionPolicy Bypass -File build.ps1`: `setup.ps1` (kazilib submodule, SFML 3.1), then
  kazilib's FASTBuild runner on `src\fastbuild.json`: **Release, x86 (Win32), the 64-bit-hosted tools**
  (`src\Directory.Build.props`), distributed to the workers that answer. Debug only when the prompt asks
  (`build.ps1 -Configuration Debug`). Settings live in `src\yip\yip.vcxproj`; the `.bff` is generated
  (`src\build\fbuild\`). Build after every prompt that changes code; it stops this checkout's running exe first.
- **Kazilib first:** yip uses kazilib's headers (`lib/shared/include`: `stdnames.h`, `vector_ops.h`, `under.h` /
  magic_enum, `scene/manager.h`, `initflags.h`, `exception_report.h`, `testerino/tcore.h`), not its library; use
  what kazilib has before writing code here, and code another project could use goes to kazilib.
- **Tests:** `node src\dep\kazilib\testerino\tt.js run src\tests\testerino\play.tt` (0 failed after a change it
  covers; extend it for new features). The bridge is `src\yip\Testerino.cpp`; keys the game polls go through
  `input::Held` (`Input.h`) so a test can hold them while the window is in the background. Run test programs
  outside the command sandbox.
- **Names:** members `_name`, functions and classes `PascalCase`, globals `g_` in `namespace global`, constants `k`
  (`kResX`), enums counted with `enum_count` and numbered with `Under` (`D:\code\hb\hbmillennium\CLAUDE.md` has the
  full rules every project follows).
