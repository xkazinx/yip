# CLAUDE.md (yip)

- **Build:** `powershell -ExecutionPolicy Bypass -File build.ps1` (`setup.ps1`: kazilib, SFML 3.1; then kazilib FASTBuild on `src\fastbuild.json`); settings in `src\yip\yip.vcxproj`; `-Configuration Debug` only if asked. Stops this checkout's running exe first.
- **Kazilib first:** headers only (`lib/shared/include`: `stdnames.h`, `vector_ops.h`, `under.h`, `scene/manager.h`, `initflags.h`, `exception_report.h`, `testerino/tcore.h`); reusable code goes to kazilib.
- **Tests:** `src\tests\testerino\play.tt`, 0 failed, extended for new features, outside the command sandbox. Bridge `src\yip\Testerino.cpp`; polled keys through `input::Held` (`Input.h`).
- **Names:** members `_name`, functions / types `PascalCase`, globals `g_` in `namespace global`, constants `k` (`kResX`), enums with `enum_count` / `Under` (full rules: `D:\code\hb\hbmillennium\CLAUDE.md`).
