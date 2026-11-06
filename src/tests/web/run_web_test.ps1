# yip in a web browser end to end: kazilib's headless browser driver
# (web\tools\browser.js: Edge, WebGL2 on SwiftShader) serves web\ (build_web.ps1) or
# opens the live page, waits for the menu, clicks Start Game, spawns enemies (Ctrl+E),
# picks the triple shot, flies right and up and down firing (held keys) until an
# enemy is beaten, then Escape to the menu and Enter into a fresh game. Steps in
# play.json; canvas screenshots in testerino\web\. Exit code = failed steps.
#   powershell -ExecutionPolicy Bypass -File src\tests\web\run_web_test.ps1 [-Url https://kazin.dev/projects/yip/]
# -Url  a deployed page instead of the local web\ folder
param([string]$Url = '', [int]$Port = 8094)
$ErrorActionPreference = 'Stop'
$root = Resolve-Path (Join-Path $PSScriptRoot '..\..\..')
$shots = Join-Path $root 'testerino\web'
New-Item -ItemType Directory -Force $shots | Out-Null

# Screenshot names in the steps land in testerino\web\
$steps = Join-Path $shots 'play.json'
(Get-Content (Join-Path $PSScriptRoot 'play.json') -Raw) -replace '"shot": "', ('"shot": "' + ($shots -replace '\\', '/') + '/') |
	Set-Content -Encoding utf8 $steps

$browser = Join-Path $root 'src\dep\kazilib\web\tools\browser.js'
if ($Url) {
	node $browser --url $Url --steps $steps
} else {
	$web = Join-Path $root 'web'
	if (-not (Test-Path (Join-Path $web 'index.wasm'))) { throw 'no web\index.wasm: run build_web.ps1' }
	node $browser --serve $web --port $Port --steps $steps
}
exit $LASTEXITCODE
