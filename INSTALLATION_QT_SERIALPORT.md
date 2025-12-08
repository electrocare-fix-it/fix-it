# Installation de Qt SerialPort

## Méthode 1 : Via Qt Maintenance Tool (Recommandé)

1. Ouvrez **Qt Maintenance Tool**
2. Sélectionnez votre installation Qt
3. Cliquez sur **Add or remove components**
4. Développez **Qt [version]**
5. Cochez **Qt SerialPort**
6. Cliquez sur **Next** et suivez les instructions

## Méthode 2 : Réinstaller Qt avec SerialPort

Si vous utilisez Qt Installer :
1. Réinstallez Qt en sélectionnant **Qt SerialPort** dans les composants

## Méthode 3 : Compiler Qt SerialPort depuis les sources

Si les méthodes précédentes ne fonctionnent pas, vous pouvez compiler Qt SerialPort depuis les sources.

## Vérification

Après installation, vérifiez que le module est disponible :
- Le fichier `.pro` devrait compiler sans erreur
- Le module `serialport` devrait être reconnu

