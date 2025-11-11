# Script pour nettoyer et régénérer le projet Qt
Write-Host "Nettoyage du projet..." -ForegroundColor Yellow

# Supprimer les fichiers de build
$buildDirs = @(
    "build-Debug",
    "build",
    "debug",
    "release"
)

foreach ($dir in $buildDirs) {
    if (Test-Path "..\$dir") {
        Write-Host "Suppression de ..\$dir" -ForegroundColor Cyan
        Remove-Item "..\$dir" -Recurse -Force -ErrorAction SilentlyContinue
    }
}

# Supprimer les Makefiles
Get-ChildItem -Path ".." -Recurse -Filter "Makefile*" -ErrorAction SilentlyContinue | Remove-Item -Force
Get-ChildItem -Path ".." -Recurse -Filter "*.o" -ErrorAction SilentlyContinue | Remove-Item -Force
Get-ChildItem -Path ".." -Recurse -Filter "ui_*.h" -ErrorAction SilentlyContinue | Remove-Item -Force
Get-ChildItem -Path ".." -Recurse -Filter "moc_*.cpp" -ErrorAction SilentlyContinue | Remove-Item -Force
Get-ChildItem -Path ".." -Recurse -Filter "qrc_*.cpp" -ErrorAction SilentlyContinue | Remove-Item -Force

Write-Host "Nettoyage terminé!" -ForegroundColor Green
Write-Host ""
Write-Host "Prochaines étapes dans Qt Creator:" -ForegroundColor Yellow
Write-Host "1. Menu Build -> Clean Project" -ForegroundColor White
Write-Host "2. Menu Build -> Run qmake" -ForegroundColor White
Write-Host "3. Menu Build -> Rebuild Project" -ForegroundColor White

