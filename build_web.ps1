# yip for a web browser (WebAssembly): src\yip\CMakeLists.txt with Emscripten,
# against kazilib's SFML web build. Output src\build\web\yip-build\index.html / .js /
# .wasm / .data, copied to web\ (the folder deploy_ubuntu.ps1 sends and
# src\tests\web\run_web_test.ps1 serves).
#   powershell -ExecutionPolicy Bypass -File build_web.ps1 [-Deps <folder>] [-Debug]
# -Deps   where kazilib's SFML / SFGUI web builds are (default D:\code\tools\kazilib-web,
#         the shared one); built there by src\dep\kazilib\web\build_deps.ps1 when missing
# -Debug  function names in stack traces and Emscripten's memory checks
param([string]$Deps = 'D:\code\tools\kazilib-web', [switch]$Debug)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$emsdk = 'D:\code\tools\emsdk'
$sfmlDir = Join-Path $Deps 'sfml-web\lib\cmake\SFML'

if (-not (Test-Path (Join-Path $sfmlDir 'SFMLConfig.cmake'))) {
	& (Join-Path $root 'src\dep\kazilib\web\build_deps.ps1') -Out $Deps -Emsdk $emsdk
	if ($LASTEXITCODE) { throw 'web dependencies failed' }
}

$env:EMSDK_QUIET = 1
. (Join-Path $emsdk 'emsdk_env.ps1') | Out-Null
$ninja = (Get-Command ninja -ErrorAction SilentlyContinue)
$ninja = if ($ninja) { $ninja.Source } else {
	Get-ChildItem 'C:\Program Files\Microsoft Visual Studio' -Recurse -Filter ninja.exe -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
}
if (-not $ninja) { throw 'ninja.exe not found (PATH or Visual Studio)' }

$build = Join-Path $root 'src\build\web\yip-build'
$linkFlags = if ($Debug) { '--profiling-funcs -sASSERTIONS=2 -sSAFE_HEAP=1' } else { '' }
emcmake cmake -S (Join-Path $root 'src\yip') -B $build -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" -DCMAKE_BUILD_TYPE=Release `
	-DSFML_STATIC_LIBRARIES=ON "-DSFML_DIR=$($sfmlDir -replace '\\', '/')" "-DCMAKE_EXE_LINKER_FLAGS=$linkFlags" | Out-Null
if ($LASTEXITCODE) { throw 'configure failed' }
# All logical CPUs but one, at most 8 (other builds share the machine)
$jobs = [Math]::Max(1, [Math]::Min(8, [Environment]::ProcessorCount - 1))
cmake --build $build -j $jobs
if ($LASTEXITCODE) { throw 'web build failed' }

$web = Join-Path $root 'web'
New-Item -ItemType Directory -Force $web | Out-Null
Copy-Item (Join-Path $build 'index.*') $web -Force
Write-Host "Web build: $build (copied to $web)"
