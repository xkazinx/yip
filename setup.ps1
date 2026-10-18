# The libraries yip builds against, x86 (Win32):
#   kazilib  src\dep\kazilib  (submodule: std names, enums, scenes, startup flags,
#            exception reports, Testerino, the FASTBuild tool)
#   SFML     src\dep\SFML-3.1.0  (a junction to D:\code\kazilib\SFML-3.1.0 when that
#            shared SDK is there, as the other projects use; else the official
#            vc17 32-bit release, downloaded)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$dep = Join-Path $root 'src\dep'

# Only when it is empty: an update would reset a kazilib commit not yet recorded here
if (-not (Test-Path (Join-Path $dep 'kazilib\fastbuild\build.ps1'))) {
    git -C $root submodule update --init src/dep/kazilib
    if ($LASTEXITCODE) { throw 'submodule update failed' }
}

$sfml = Join-Path $dep 'SFML-3.1.0'
if (-not (Test-Path (Join-Path $sfml 'include\SFML\Graphics.hpp'))) {
    $shared = 'D:\code\kazilib\SFML-3.1.0'
    if (Test-Path (Join-Path $shared 'include\SFML\Graphics.hpp')) {
        New-Item -ItemType Junction -Path $sfml -Target $shared | Out-Null
        Write-Host "SFML: linked to $shared"
    } else {
        $zip = Join-Path $env:TEMP 'SFML-3.1.0-windows-vc17-32-bit.zip'
        Write-Host 'SFML: downloading 3.1.0 (vc17 32-bit)'
        Invoke-WebRequest 'https://github.com/SFML/SFML/releases/download/3.1.0/SFML-3.1.0-windows-vc17-32-bit.zip' -OutFile $zip
        Expand-Archive $zip -DestinationPath $dep -Force
        Remove-Item $zip
    }
}
