# Setări și configurări de compilare
$SrcDir = "../src"
$BuildDir = "../build"
$OutputFile = "ShadowToken.exe"
$Compiler = "g++"
$CxxFlags = "-O2 -std=c++17 -Wall"
$IncludeDirs = "-I../src -I../src/core -I../src/utils -I../src/evasion -I../src/privilege_escalation -I../src/fileless_injection -I../src/c2_communication -I../src/persistence"

# Verificare dacă directorul de build există
if (-Not (Test-Path -Path $BuildDir)) {
    Write-Host "[Info] Crearea directorului de build." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Comanda de compilare
Write-Host "[Info] Compilarea fișierelor sursă..." -ForegroundColor Cyan
$CppFiles = Get-ChildItem -Path $SrcDir -Recurse -Filter "*.cpp" | ForEach-Object { $_.FullName } | ForEach-Object { "`"$_`"" }
$Command = "$Compiler $CxxFlags $IncludeDirs -o `"$BuildDir/$OutputFile`" $($CppFiles -join ' ')"

# Executarea comenzii
Invoke-Expression $Command

# Verificare de erori
if ($LASTEXITCODE -eq 0) {
    Write-Host "[Success] Compilare completă. Executabilul se află în $BuildDir/$OutputFile" -ForegroundColor Green
} else {
    Write-Host "[Error] Compilare eșuată." -ForegroundColor Red
    exit 1
}
