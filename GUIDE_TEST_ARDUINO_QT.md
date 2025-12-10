# Guide de Test - Vérifier que l'Arduino fonctionne avec l'Application Qt

## 📋 Prérequis

1. **Arduino branché** via USB à votre ordinateur
2. **Code Arduino téléversé** (`rfid_access_control.ino`)
3. **Application Qt compilée** et prête à être lancée
4. **Câblage vérifié** :
   - LED verte → Pin 2
   - LED rouge → Pin 4
   - Buzzer → Pin 8
   - Module RFID MFRC522 → Pins SPI

---

## 🔍 Étape 1 : Vérifier le Port Série de l'Arduino

### Sur Windows :
1. Ouvrez le **Gestionnaire de périphériques**
   - Clic droit sur "Ce PC" → **Propriétés** → **Gestionnaire de périphériques**
2. Cherchez **"Ports (COM et LPT)"**
3. Notez le port COM de votre Arduino (ex: **COM3**, **COM4**, **COM5**, etc.)

### Alternative - Arduino IDE :
1. Ouvrez **Arduino IDE**
2. Allez dans **Outils → Port**
3. Notez le port sélectionné (ex: **COM3**)

---

## 🔍 Étape 2 : Test avec le Moniteur Série Arduino (Test de Base)

### 2.1 : Ouvrir le Moniteur Série
1. Ouvrez **Arduino IDE**
2. Allez dans **Outils → Moniteur série** (ou `Ctrl+Shift+M`)
3. **Configurez** :
   - Vitesse : **9600 baud**
   - Fin de ligne : **Nouvelle ligne**

### 2.2 : Vérifier les Messages de Démarrage
Vous devriez voir :
```
=== SYSTEME DE CONTROLE D'ACCES RFID ===
En attente de carte...
```

**✅ Si vous voyez ce message** : L'Arduino fonctionne et attend les cartes RFID.

**❌ Si vous ne voyez rien** :
- Vérifiez que le code est bien téléversé
- Vérifiez le port COM sélectionné
- Vérifiez la vitesse (9600 baud)

### 2.3 : Tester avec les Cartes RFID

#### Test 1 : Carte de Hiba Riahi (8B 8C ED 00)
1. **Scannez** la carte blanche
2. **Vérifiez dans le Moniteur Série** :
   ```
   UID detecte: 8B 8C ED 00
   ACCESS_GRANTED|HIBA_RIAHI|ACTIF
   >>> ACCES AUTORISE <<<
   Employe: HIBA_RIAHI
   Statut: ACTIF
   Bienvenue! Tu as l'acces pour la gestion pieces detachees
   ```
3. **Vérifiez visuellement** :
   - ✅ LED verte s'allume (3 secondes)
   - ✅ Pas de buzzer
   - ✅ Pas de LED rouge

#### Test 2 : Carte d'Omar Askri (03 D1 10 2D)
1. **Scannez** la carte bleue
2. **Vérifiez dans le Moniteur Série** :
   ```
   UID detecte: 03 D1 10 2D
   ACCESS_DENIED|OMAR_ASKRI|EN_CONGE
   >>> ACCES REFUSE <<<
   Employe: OMAR_ASKRI
   Statut: EN_CONGE
   ALERTE: Carte non autorisee detectee!
   ```
3. **Vérifiez visuellement** :
   - ✅ LED rouge s'allume (3 secondes)
   - ✅ **Buzzer sonne 3 fois**
   - ✅ Pas de LED verte

#### Test 3 : Carte Fausse/Inconnue
1. **Scannez** une carte qui n'est ni Hiba ni Omar
2. **Vérifiez dans le Moniteur Série** :
   ```
   UID detecte: XX XX XX XX (autre UID)
   >>> CARTE INCONNUE DETECTEE - ACTIVATION BUZZER <<<
   ACCESS_DENIED|INCONNU|CARTE_NON_AUTORISEE
   >>> ACCES REFUSE <<<
   Employe: INCONNU
   Statut: CARTE_NON_AUTORISEE
   ALERTE: Carte non autorisee detectee!
   ```
3. **Vérifiez visuellement** :
   - ✅ LED rouge s'allume (3 secondes)
   - ✅ **Buzzer sonne 3 fois**
   - ✅ Pas de LED verte

---

## 🔍 Étape 3 : Test avec l'Application Qt

### 3.1 : Préparer l'Application Qt

1. **Fermez le Moniteur Série Arduino** (important !)
   - Un seul programme peut utiliser le port série à la fois
   - Si le Moniteur Série est ouvert, l'application Qt ne pourra pas se connecter

2. **Compilez** votre application Qt (si pas déjà fait)

3. **Lancez** l'application Qt

### 3.2 : Vérifier la Connexion Automatique

L'application Qt devrait **automatiquement** :
- Détecter le port série de l'Arduino
- Se connecter au port série (9600 baud)
- Afficher des messages dans la console/debug

**Comment vérifier la connexion** :

#### Option A : Console Qt (Recommandé)
1. Lancez l'application Qt depuis **Qt Creator**
2. Regardez la **console de sortie** (Output)
3. Vous devriez voir :
   ```
   === TEST CONNEXION ARDUINO RFID ===
   Ports série disponibles: ("COM3")
   Tentative de connexion sur le port: COM3
   ✓ SUCCÈS: Connecté à l'Arduino sur le port: COM3
   En attente de scan de carte RFID...
   ```

**✅ Si vous voyez "SUCCÈS: Connecté à l'Arduino"** : La connexion fonctionne !

**❌ Si vous voyez une erreur** :
- Vérifiez que le Moniteur Série Arduino est fermé
- Vérifiez que le port COM est correct
- Vérifiez que l'Arduino est bien branché

#### Option B : Messages d'Erreur dans l'Application
Si la connexion échoue, l'application affichera un message d'erreur :
- "Aucun port série disponible" → Arduino non détecté
- "Impossible d'ouvrir le port" → Port déjà utilisé ou incorrect

### 3.3 : Tester avec les Cartes RFID dans l'Application Qt

#### Test 1 : Carte de Hiba Riahi (Accès Autorisé)
1. **Ouvrez** l'interface **"Gestion Pièces Détachées"** dans l'application Qt
2. **Scannez** la carte blanche (Hiba Riahi)
3. **Vérifiez dans l'application Qt** :
   - ✅ **Message popup** : "Bienvenue Hiba Riahi ! Tu as l'accès pour la gestion des pièces détachées."
   - ✅ **Interface activée** : Tous les boutons et champs sont **activés** (non grisés)
   - ✅ **LED verte** s'allume sur l'Arduino
   - ✅ **Pas de buzzer**

#### Test 2 : Carte d'Omar Askri (Accès Refusé)
1. **Scannez** la carte bleue (Omar Askri)
2. **Vérifiez dans l'application Qt** :
   - ✅ **Message popup** : "Accès refusé ! Omar Askri est en congé. La gestion des pièces détachées est désactivée."
   - ✅ **Interface désactivée** : Tous les boutons et champs sont **grisés** (désactivés)
   - ✅ **LED rouge** s'allume sur l'Arduino
   - ✅ **Buzzer sonne 3 fois** sur l'Arduino

#### Test 3 : Carte Fausse/Inconnue (Accès Refusé)
1. **Scannez** une carte qui n'est ni Hiba ni Omar
2. **Vérifiez dans l'application Qt** :
   - ✅ **Message popup** : "Accès refusé ! La gestion des pièces détachées est désactivée."
   - ✅ **Interface désactivée** : Tous les boutons et champs sont **grisés** (désactivés)
   - ✅ **LED rouge** s'allume sur l'Arduino
   - ✅ **Buzzer sonne 3 fois** sur l'Arduino

---

## 🔍 Étape 4 : Vérifier les Messages dans la Console Qt

### 4.1 : Activer les Messages de Debug

1. **Lancez l'application depuis Qt Creator**
2. **Ouvrez la console de sortie** (Output)
3. **Filtrez** les messages contenant "Arduino" ou "RFID"

### 4.2 : Messages Attendus

#### Au Démarrage :
```
=== TEST CONNEXION ARDUINO RFID ===
Ports série disponibles: ("COM3")
Tentative de connexion sur le port: COM3
✓ SUCCÈS: Connecté à l'Arduino sur le port: COM3
En attente de scan de carte RFID...
```

#### Quand une Carte est Scannée :
```
Message reçu de l'Arduino: ACCESS_GRANTED|HIBA_RIAHI|ACTIF
```
ou
```
Message reçu de l'Arduino: ACCESS_DENIED|OMAR_ASKRI|EN_CONGE
```

**✅ Si vous voyez ces messages** : La communication Arduino ↔ Qt fonctionne !

**❌ Si vous ne voyez pas ces messages** :
- Vérifiez que le Moniteur Série Arduino est fermé
- Vérifiez que l'Arduino envoie bien les messages (testez avec le Moniteur Série)
- Vérifiez la vitesse de transmission (9600 baud)

---

## ✅ Checklist Complète de Vérification

### Arduino (Hardware)
- [ ] Arduino branché via USB
- [ ] LED verte connectée au Pin 2
- [ ] LED rouge connectée au Pin 4
- [ ] Buzzer connecté au Pin 8
- [ ] Module RFID MFRC522 correctement branché
- [ ] Port COM visible dans le Gestionnaire de périphériques

### Code Arduino
- [ ] Code `rfid_access_control.ino` téléversé sur l'Arduino
- [ ] Moniteur Série affiche "En attente de carte..."
- [ ] LED verte s'allume pour Hiba Riahi
- [ ] LED rouge s'allume pour Omar Askri et cartes fausses
- [ ] Buzzer sonne pour Omar Askri et cartes fausses
- [ ] Buzzer ne sonne PAS au démarrage
- [ ] Buzzer ne sonne PAS pour Hiba Riahi

### Application Qt
- [ ] Application se lance sans erreur
- [ ] Console Qt affiche "SUCCÈS: Connecté à l'Arduino"
- [ ] Message "Bienvenue" apparaît pour Hiba Riahi
- [ ] Message "Accès refusé" apparaît pour Omar Askri
- [ ] Message "Accès refusé" apparaît pour cartes fausses
- [ ] Interface "Gestion Pièces Détachées" s'active pour Hiba Riahi
- [ ] Interface "Gestion Pièces Détachées" se désactive pour Omar Askri
- [ ] Interface "Gestion Pièces Détachées" se désactive pour cartes fausses

### Communication Arduino ↔ Qt
- [ ] Messages "ACCESS_GRANTED" ou "ACCESS_DENIED" apparaissent dans la console Qt
- [ ] Les messages correspondent aux cartes scannées
- [ ] Pas d'erreur de communication dans la console Qt

---

## 🐛 Dépannage

### Problème : L'application Qt ne se connecte pas à l'Arduino

**Symptômes** :
- Message "Aucun port série disponible"
- Message "Impossible d'ouvrir le port"

**Solutions** :
1. ✅ **Fermez le Moniteur Série Arduino** (très important !)
2. ✅ Vérifiez que le port COM est correct
3. ✅ Débranchez et rebranchez l'Arduino
4. ✅ Redémarrez l'application Qt
5. ✅ Vérifiez les messages d'erreur dans la console Qt

### Problème : Les messages n'apparaissent pas dans l'application Qt

**Symptômes** :
- L'application se connecte mais aucun message n'apparaît
- Les cartes sont scannées mais rien ne se passe dans Qt

**Solutions** :
1. ✅ Vérifiez que le Moniteur Série Arduino reçoit bien les messages
2. ✅ Vérifiez que le format des messages est correct : `ACCESS_GRANTED|...` ou `ACCESS_DENIED|...`
3. ✅ Vérifiez la vitesse de transmission (9600 baud)
4. ✅ Regardez les messages de debug dans la console Qt
5. ✅ Vérifiez que l'interface "Gestion Pièces Détachées" est bien ouverte

### Problème : Le buzzer ne sonne pas

**Symptômes** :
- LED rouge s'allume mais pas de buzzer
- Buzzer sonne au démarrage (ne devrait pas)

**Solutions** :
1. ✅ Vérifiez le câblage (buzzer sur Pin 8)
2. ✅ Testez avec `test_buzzer_simple.ino` pour isoler le problème
3. ✅ Vérifiez que c'est un buzzer actif (pas passif)
4. ✅ Vérifiez dans le Moniteur Série si les messages "ALERTE: Buzzer active" apparaissent

### Problème : L'interface ne s'active/désactive pas

**Symptômes** :
- Les cartes sont scannées mais l'interface reste dans le même état

**Solutions** :
1. ✅ Vérifiez que les signaux `accessGranted` et `accessDenied` sont bien connectés
2. ✅ Vérifiez les messages dans la console Qt
3. ✅ Vérifiez que l'interface "Gestion Pièces Détachées" est bien ouverte
4. ✅ Vérifiez que les messages Arduino sont bien reçus

---

## 📝 Notes Importantes

- **Ne laissez jamais le Moniteur Série Arduino ouvert** pendant que l'application Qt tourne (conflit de port)
- **Testez chaque carte plusieurs fois** pour vérifier la stabilité
- **Vérifiez toujours les messages dans la console Qt** pour le débogage
- **Le buzzer doit sonner 3 fois** pour chaque accès refusé (Omar + cartes fausses)

---

## ✨ Résultat Attendu Final

Quand tout fonctionne correctement :

| Carte | LED | Buzzer | Message Qt | Interface |
|-------|-----|--------|------------|-----------|
| **Hiba Riahi** (8B 8C ED 00) | 🟢 Verte | ❌ Non | "Bienvenue Hiba Riahi !" | ✅ Activée |
| **Omar Askri** (03 D1 10 2D) | 🔴 Rouge | ✅ 3 bips | "Accès refusé ! Omar Askri est en congé." | ❌ Désactivée |
| **Carte fausse** | 🔴 Rouge | ✅ 3 bips | "Accès refusé !" | ❌ Désactivée |

---

## 🎯 Test Rapide (2 minutes)

1. **Fermez** le Moniteur Série Arduino
2. **Lancez** l'application Qt
3. **Ouvrez** "Gestion Pièces Détachées"
4. **Vérifiez** la console Qt : doit afficher "SUCCÈS: Connecté à l'Arduino"
5. **Scannez** la carte de Hiba Riahi
6. **Vérifiez** :
   - ✅ Message "Bienvenue" apparaît
   - ✅ Interface activée
   - ✅ LED verte sur Arduino

**Si tout cela fonctionne** : ✅ **L'Arduino fonctionne parfaitement avec votre application Qt !**

