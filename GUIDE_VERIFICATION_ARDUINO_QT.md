# 🔍 Guide de Vérification - Arduino et Application Qt

## ✅ Étape 1 : Vérifier le Code Arduino

### 1.1 : Téléverser le Code sur l'Arduino

1. **Ouvrez Arduino IDE**
2. **Ouvrez le fichier** `rfid_access_control.ino`
3. **Installez la bibliothèque MFRC522** si nécessaire :
   - Outils → Gérer les bibliothèques
   - Recherchez "MFRC522"
   - Installez la bibliothèque de GithubCommunity
4. **Sélectionnez votre carte Arduino** :
   - Outils → Type de carte → Arduino Uno (ou votre modèle)
5. **Sélectionnez le port COM** :
   - Outils → Port → COM3, COM4, COM5, etc.
6. **Téléversez le code** (Ctrl+U ou bouton "Téléverser")

### 1.2 : Vérifier que le Code est Téléversé

1. **Ouvrez le Moniteur Série** (Ctrl+Shift+M)
2. **Configurez** :
   - Vitesse : **9600 baud**
   - Fin de ligne : **Nouvelle ligne**
3. **Vous devriez voir** :
   ```
   === SYSTEME DE CONTROLE D'ACCES RFID ===
   En attente de carte...
   ```
4. **Fermez le Moniteur Série** (important !)

---

## ✅ Étape 2 : Vérifier le Câblage

### 2.1 : Vérification des Connexions

| Composant | Pin Arduino | Vérification |
|-----------|-------------|--------------|
| LED Verte | Pin 2 | ✅ Connectée avec résistance 220Ω |
| LED Rouge | Pin 4 | ✅ Connectée avec résistance 220Ω |
| Buzzer | Pin 8 | ✅ Connecté (actif) |
| RFID MFRC522 | SPI (10, 11, 12, 13) | ✅ Connecté selon schéma SPI |

### 2.2 : Test Rapide des LEDs et Buzzer

1. **Ouvrez le Moniteur Série Arduino**
2. **Scannez la carte de Hiba Riahi** → LED verte doit s'allumer
3. **Scannez la carte d'Omar Askri** → LED rouge + buzzer doivent s'activer

---

## ✅ Étape 3 : Vérifier l'Intégration Qt

### 3.1 : Compiler l'Application Qt

1. **Ouvrez Qt Creator**
2. **Ouvrez le projet** `partie1Github.pro`
3. **Nettoyez le projet** : Build → Clean All
4. **Compilez** : Build → Rebuild All
5. **Vérifiez qu'il n'y a pas d'erreurs**

### 3.2 : Vérifier que les Fichiers sont Présents

Vérifiez que ces fichiers existent :
- ✅ `serialportmanager.h`
- ✅ `serialportmanager.cpp`
- ✅ `gestionpieces.h`
- ✅ `gestionpieces.cpp`
- ✅ `rfid_access_control.ino` (pour référence)

### 3.3 : Vérifier le Fichier .pro

Ouvrez `partie1Github.pro` et vérifiez que ces lignes sont présentes :

```pro
QT += core gui sql serialport
...
SOURCES += ... \
    serialportmanager.cpp
...
HEADERS += ... \
    serialportmanager.h
```

---

## ✅ Étape 4 : Tester la Connexion

### 4.1 : Préparer l'Environnement

1. **Fermez COMPLÈTEMENT Arduino IDE** (pas seulement le Moniteur Série)
2. **Fermez TOUS les programmes** qui pourraient utiliser le port série
3. **Vérifiez le port COM** dans le Gestionnaire de périphériques :
   - Clic droit sur "Ce PC" → Propriétés → Gestionnaire de périphériques
   - Ports (COM et LPT) → Notez le port de l'Arduino (ex: COM5)

### 4.2 : Lancer l'Application Qt

1. **Lancez l'application Qt** depuis Qt Creator
2. **Connectez-vous** avec vos identifiants
3. **Ouvrez "Gestion Pièces Détachées"**
4. **Regardez la console de sortie** dans Qt Creator

### 4.3 : Messages Attendus dans la Console

**Si la connexion réussit** :
```
=== TEST CONNEXION ARDUINO RFID ===
Ports série disponibles: ("COM5")
Tentative 1/5 de connexion au port COM5...
✓ Connecté à l'Arduino sur le port: COM5
En attente de scan de carte RFID...
```

**Si la connexion échoue** :
```
=== TEST CONNEXION ARDUINO RFID ===
Ports série disponibles: ("COM5")
Tentative 1/5 de connexion au port COM5...
Échec tentative 1: Accès refusé (code: 5)
...
```

### 4.4 : Résoudre l'Erreur "Accès Refusé"

Si vous voyez "Accès refusé", suivez ces étapes **dans l'ordre** :

1. **Fermez COMPLÈTEMENT Arduino IDE**
   - Vérifiez dans le Gestionnaire des tâches qu'aucun processus "Arduino" n'est actif

2. **Débranchez l'Arduino USB**
   - Attendez 3 secondes
   - Rebranchez l'Arduino USB
   - Attendez que Windows reconnaisse le port (notification)

3. **Fermez l'application Qt**
   - Fermez complètement l'application
   - Relancez l'application Qt

4. **Si le problème persiste** :
   - Redémarrez votre ordinateur
   - Rebranchez l'Arduino après le redémarrage
   - Relancez l'application Qt

---

## ✅ Étape 5 : Tester avec les Cartes RFID

### 5.1 : Test avec la Carte de Hiba Riahi (8B 8C ED 00)

1. **Scannez la carte blanche** sur le lecteur RFID
2. **Vérifiez dans l'application Qt** :
   - ✅ **Message popup** : "Bienvenue Hiba Riahi ! Tu as l'accès pour la gestion des pièces détachées."
   - ✅ **Interface activée** : Tous les boutons et champs sont **activés** (non grisés)
3. **Vérifiez sur l'Arduino** :
   - ✅ **LED verte** s'allume pendant 3 secondes
   - ✅ **Pas de buzzer**
4. **Vérifiez dans la console Qt** :
   ```
   Message reçu de l'Arduino: ACCESS_GRANTED|HIBA_RIAHI|ACTIF
   ```

### 5.2 : Test avec la Carte d'Omar Askri (03 D1 10 2D)

1. **Scannez la carte bleue** sur le lecteur RFID
2. **Vérifiez dans l'application Qt** :
   - ✅ **Message popup** : "Accès refusé ! Omar Askri est en congé. La gestion des pièces détachées est désactivée."
   - ✅ **Interface désactivée** : Tous les boutons et champs sont **grisés** (désactivés)
3. **Vérifiez sur l'Arduino** :
   - ✅ **LED rouge** s'allume pendant 3 secondes
   - ✅ **Buzzer sonne 3 fois** (bip-bip-bip)
4. **Vérifiez dans la console Qt** :
   ```
   Message reçu de l'Arduino: ACCESS_DENIED|OMAR_ASKRI|EN_CONGE
   ```

### 5.3 : Test avec une Carte Inconnue

1. **Scannez une carte qui n'est ni Hiba ni Omar**
2. **Vérifiez dans l'application Qt** :
   - ✅ **Message popup** : "Accès refusé ! La gestion des pièces détachées est désactivée."
   - ✅ **Interface désactivée** : Tous les boutons et champs sont **grisés**
3. **Vérifiez sur l'Arduino** :
   - ✅ **LED rouge** s'allume pendant 3 secondes
   - ✅ **Buzzer sonne 3 fois** (bip-bip-bip)
4. **Vérifiez dans la console Qt** :
   ```
   Message reçu de l'Arduino: ACCESS_DENIED|INCONNU|CARTE_INCONNUE
   ```

---

## ✅ Étape 6 : Vérification Finale

### Checklist Complète

#### Arduino (Hardware)
- [ ] Arduino branché via USB
- [ ] LED verte connectée au Pin 2
- [ ] LED rouge connectée au Pin 4
- [ ] Buzzer connecté au Pin 8
- [ ] Module RFID MFRC522 correctement branché
- [ ] Port COM visible dans le Gestionnaire de périphériques

#### Code Arduino
- [ ] Code `rfid_access_control.ino` téléversé sur l'Arduino
- [ ] Moniteur Série affiche "En attente de carte..." (puis fermé)
- [ ] LED verte s'allume pour Hiba Riahi
- [ ] LED rouge s'allume pour Omar Askri et cartes fausses
- [ ] Buzzer sonne pour Omar Askri et cartes fausses

#### Application Qt
- [ ] Application compile sans erreurs
- [ ] `serialportmanager.cpp` et `serialportmanager.h` présents
- [ ] `gestionpieces.cpp` contient `setupRFIDAccess()`
- [ ] Console Qt affiche "Connecté à l'Arduino"
- [ ] Messages "ACCESS_GRANTED" et "ACCESS_DENIED" apparaissent dans la console
- [ ] Interface s'active/désactive selon la carte scannée
- [ ] Messages popup s'affichent correctement

---

## 🆘 Dépannage

### Problème : "Aucun port série disponible"

**Solutions** :
1. Vérifiez que l'Arduino est branché via USB
2. Vérifiez dans le Gestionnaire de périphériques que le port COM est visible
3. Installez les pilotes USB de l'Arduino si nécessaire
4. Essayez un autre câble USB

### Problème : "Accès refusé" ou "Délai de temporisation de sémaphore"

**Solutions** :
1. Fermez COMPLÈTEMENT Arduino IDE
2. Fermez TOUS les programmes utilisant le port série
3. Débranchez et rebranchez l'Arduino
4. Redémarrez l'application Qt
5. Si le problème persiste, redémarrez l'ordinateur

### Problème : Aucun message n'apparaît dans la console Qt

**Solutions** :
1. Vérifiez que le Moniteur Série Arduino est fermé
2. Vérifiez que l'Arduino envoie bien les messages (testez avec le Moniteur Série)
3. Vérifiez la vitesse de transmission (9600 baud)
4. Vérifiez que le code Arduino est bien téléversé

### Problème : L'interface ne s'active/désactive pas

**Solutions** :
1. Vérifiez que les messages "ACCESS_GRANTED" ou "ACCESS_DENIED" apparaissent dans la console
2. Vérifiez que les signaux sont bien connectés dans `setupRFIDAccess()`
3. Vérifiez que `enableAccess()` et `disableAccess()` sont bien appelés

---

## 📝 Notes Importantes

1. **NE JAMAIS** ouvrir le Moniteur Série Arduino en même temps que l'application Qt
2. **TOUJOURS** fermer le Moniteur Série avant de lancer l'application Qt
3. **VÉRIFIER** le port COM dans le Gestionnaire de périphériques si vous avez plusieurs ports série
4. **REDÉMARRER** l'application Qt après avoir changé le port COM de l'Arduino

---

## ✅ Si Tout Fonctionne

Si tous les tests passent, votre système RFID est **parfaitement intégré** avec l'application Qt ! 🎉

Vous pouvez maintenant :
- Scanner la carte de Hiba Riahi pour activer l'interface
- Scanner la carte d'Omar Askri ou une carte inconnue pour désactiver l'interface
- Utiliser normalement la gestion des pièces détachées après autorisation

