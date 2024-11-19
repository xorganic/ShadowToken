# Setări și configurări
$BuildDir = "../build"
$OutputFile = "ShadowToken.exe"
$ConfigDir = "../config"
$DeployDir = "../deploy"
$DeployArchive = "ShadowToken_v2025.zip" # Arhivele .tar.gz nu sunt native în Windows, folosim .zip

# Verificare dacă build-ul a fost creat
if (-Not (Test-Path -Path (Join-Path $BuildDir $OutputFile))) {
    Write-Host "[Error] Binarul $OutputFile nu există. Rulați build.ps1 înainte de deploy." -ForegroundColor Red
    exit 1
}

# Crearea directorului de deploy dacă nu există
if (-Not (Test-Path -Path $DeployDir)) {
    Write-Host "[Info] Crearea directorului de deploy." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $DeployDir | Out-Null
}

# Crearea arhivei de deploy
Write-Host "[Info] Crearea arhivei de deploy..." -ForegroundColor Cyan
$DeployArchivePath = Join-Path $DeployDir $DeployArchive

# Adaugă fișierul executabil și fișierele de configurare în arhivă
try {
    Compress-Archive -Path (Join-Path $BuildDir $OutputFile), $ConfigDir -DestinationPath $DeployArchivePath -Force
    Write-Host "[Success] Arhiva de deploy creată în $DeployArchivePath" -ForegroundColor Green
} catch {
    Write-Host "[Error] Crearea arhivei de deploy a eșuat." -ForegroundColor Red
    exit 1
}
