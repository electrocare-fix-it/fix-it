# Script pour nettoyer les fichiers UI générés et forcer la régénération
Write-Host "=== NETTOYAGE DES FICHIERS UI GÉNÉRÉS ===" -ForegroundColor Yellow
Write-Host ""

$projectPath = Get-Location

# Supprimer les fichiers UI générés
Write-Host "Suppression des fichiers ui_*.h..." -ForegroundColor Cyan
Get-ChildItem -Path $projectPath -Recurse -Filter "ui_*.h" -ErrorAction SilentlyContinue | ForEach-Object {
    Write-Host "  Suppression: $($_.Name)" -ForegroundColor Red
    Remove-Item $_.FullName -Force
}

# Supprimer les fichiers MOC générés
Write-Host "Suppression des fichiers moc_*.cpp..." -ForegroundColor Cyan
Get-ChildItem -Path $projectPath -Recurse -Filter "moc_*.cpp" -ErrorAction SilentlyContinue | ForEach-Object {
    Write-Host "  Suppression: $($_.Name)" -ForegroundColor Red
    Remove-Item $_.FullName -Force
}

# Supprimer les Makefiles
Write-Host "Suppression des Makefiles..." -ForegroundColor Cyan
Get-ChildItem -Path $projectPath -Recurse -Filter "Makefile*" -ErrorAction SilentlyContinue | ForEach-Object {
    Write-Host "  Suppression: $($_.Name)" -ForegroundColor Red
    Remove-Item $_.FullName -Force
}

# Supprimer les dossiers de build
Write-Host "Suppression des dossiers de build..." -ForegroundColor Cyan
$buildDirs = @("build-Debug", "build-Release", "build", "debug", "release", ".qmake.stash")
foreach ($buildDir in $buildDirs) {
    $buildPath = Join-Path $projectPath $buildDir
    if (Test-Path $buildPath) {
        Write-Host "  Suppression du dossier: $buildDir" -ForegroundColor Red
        Remove-Item $buildPath -Recurse -Force -ErrorAction SilentlyContinue
    }
}

Write-Host ""
Write-Host "=== NETTOYAGE TERMINÉ ===" -ForegroundColor Green
Write-Host ""
Write-Host "PROCHAINES ÉTAPES DANS QT CREATOR:" -ForegroundColor Yellow
Write-Host "1. Menu Build -> Clean Project" -ForegroundColor White
Write-Host "2. Menu Build -> Run qmake (TRÈS IMPORTANT!)" -ForegroundColor White
Write-Host "3. Menu Build -> Rebuild Project" -ForegroundColor White
Write-Host ""
Write-Host "Les fichiers ui_*.h seront régénérés automatiquement" -ForegroundColor Cyan
Write-Host "à partir des fichiers *.ui" -ForegroundColor Cyan
Write-Host ""

