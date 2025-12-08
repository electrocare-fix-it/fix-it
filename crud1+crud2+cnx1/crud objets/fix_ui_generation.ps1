# Script pour forcer la régénération des fichiers UI
Write-Host "Nettoyage des fichiers UI générés..." -ForegroundColor Yellow

# Chemins à nettoyer
$paths = @(
    "C:\Users\jacem\OneDrive\Desktop\crud objets",
    "C:\Users\jacem\OneDrive\Desktop\partie1Github",
    "C:\Users\jacem\OneDrive\Desktop\crud objets\crud objets"
)

foreach ($path in $paths) {
    if (Test-Path $path) {
        Write-Host "Recherche dans: $path" -ForegroundColor Cyan
        
        # Supprimer les fichiers UI générés
        Get-ChildItem -Path $path -Recurse -Filter "ui_*.h" -ErrorAction SilentlyContinue | ForEach-Object {
            Write-Host "Suppression: $($_.FullName)" -ForegroundColor Red
            Remove-Item $_.FullName -Force
        }
        
        # Supprimer les fichiers MOC générés
        Get-ChildItem -Path $path -Recurse -Filter "moc_*.cpp" -ErrorAction SilentlyContinue | ForEach-Object {
            Write-Host "Suppression: $($_.FullName)" -ForegroundColor Red
            Remove-Item $_.FullName -Force
        }
        
        # Supprimer les Makefiles
        Get-ChildItem -Path $path -Recurse -Filter "Makefile*" -ErrorAction SilentlyContinue | ForEach-Object {
            Write-Host "Suppression: $($_.FullName)" -ForegroundColor Red
            Remove-Item $_.FullName -Force
        }
        
        # Supprimer les dossiers de build
        $buildDirs = @("build-Debug", "build", "debug", "release")
        foreach ($buildDir in $buildDirs) {
            $buildPath = Join-Path $path $buildDir
            if (Test-Path $buildPath) {
                Write-Host "Suppression du dossier de build: $buildPath" -ForegroundColor Red
                Remove-Item $buildPath -Recurse -Force -ErrorAction SilentlyContinue
            }
        }
    }
}

Write-Host ""
Write-Host "Nettoyage terminé!" -ForegroundColor Green
Write-Host ""
Write-Host "PROCHAINES ÉTAPES DANS QT CREATOR:" -ForegroundColor Yellow
Write-Host "1. Menu Build -> Clean Project" -ForegroundColor White
Write-Host "2. Menu Build -> Run qmake (IMPORTANT!)" -ForegroundColor White
Write-Host "3. Menu Build -> Rebuild Project" -ForegroundColor White
Write-Host ""
Write-Host "Le fichier ui_gestionobjetelectronique.h sera régénéré automatiquement" -ForegroundColor Cyan
Write-Host "à partir de gestionobjetelectronique.ui" -ForegroundColor Cyan

