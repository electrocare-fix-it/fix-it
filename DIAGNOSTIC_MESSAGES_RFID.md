# 🔍 Diagnostic - Messages RFID Ne S'Affichent Pas

## ✅ Étape 1 : Vérifier la Connexion Arduino

### 1.1 : Dans la Console Qt

Quand vous ouvrez "Gestion Pièces Détachées", vous devriez voir :

```
=== TEST CONNEXION ARDUINO RFID ===
Ports série disponibles: ("COM5")
Tentative de connexion (essaiera tous les ports disponibles)...
✓✓✓ SUCCÈS: Connecté à l'Arduino sur le port: COM5
=== CONNEXION RFID ACTIVE ===
```

**✅ Si vous voyez "SUCCÈS: Connecté"** → La connexion fonctionne
**❌ Si vous voyez "ERREUR: Impossible de se connecter"** → Problème de connexion

### 1.2 : Vérifier que le Port est Bien Ouvert

Dans la console, cherchez :
```
Port ouvert en mode ReadWrite
Bytes disponibles: 0
Timer de verification demarre (toutes les 100ms)
```

**✅ Si vous voyez ces messages** → Le port est ouvert
**❌ Si vous ne voyez pas ces messages** → Le port n'est pas ouvert

---

## ✅ Étape 2 : Vérifier la Réception des Données

### 2.1 : Scannez une Carte et Regardez la Console

Vous devriez voir UN de ces messages :

**Option A : Signal readyRead fonctionne**
```
========================================
*** readData() APPELE ***
Port ouvert: true
Bytes disponibles: XX
*** DONNEES RECUES DE L'ARDUINO ***
```

**Option B : Timer détecte les données**
```
Timer detecte des donnees disponibles: XX bytes
*** DONNEES RECUES DE L'ARDUINO ***
```

**❌ Si vous ne voyez AUCUN de ces messages** → Les données ne sont pas reçues

### 2.2 : Vérifier le Contenu des Données

Si vous voyez "*** DONNEES RECUES DE L'ARDUINO ***", regardez :

```
Donnees brutes (ascii): ACCESS_GRANTED|HIBA_RIAHI|ACTIF
ou
Donnees brutes (ascii): ACCESS_DENIED|OMAR_ASKRI|EN_CONGE
```

**✅ Si vous voyez "ACCESS_GRANTED|" ou "ACCESS_DENIED|"** → Les données arrivent correctement
**❌ Si vous ne voyez pas ces messages** → L'Arduino n'envoie pas les bonnes données

---

## ✅ Étape 3 : Vérifier le Parsing

### 3.1 : Vérifier que le Message est Parsé

Vous devriez voir :

```
========================================
*** PARSING MESSAGE ***
>>> ACCESS_GRANTED trouve <<<
*** EMISSION SIGNAL accessGranted ***
Signal accessGranted EMIS avec succes
```

ou

```
>>> ACCESS_DENIED trouve <<<
*** EMISSION SIGNAL accessDenied ***
Signal accessDenied EMIS avec succes
```

**✅ Si vous voyez "EMISSION SIGNAL"** → Le signal est émis
**❌ Si vous ne voyez pas ce message** → Le parsing ne fonctionne pas

---

## ✅ Étape 4 : Vérifier la Réception des Signaux

### 4.1 : Vérifier que le Signal est Reçu

Vous devriez voir :

```
========================================
*** SIGNAL accessGranted RECU ***
Employee: HIBA_RIAHI
Status: ACTIF
========================================
```

ou

```
========================================
*** SIGNAL accessDenied RECU ***
Employee: OMAR_ASKRI
Status: EN_CONGE
========================================
```

**✅ Si vous voyez "SIGNAL ... RECU"** → Le signal est bien reçu
**❌ Si vous ne voyez pas ce message** → Le signal n'est pas reçu (problème de connexion des signaux)

---

## ✅ Étape 5 : Vérifier l'Affichage du Message

### 5.1 : Vérifier que le MessageBox est Créé

Vous devriez voir :

```
Preparation du message: Bienvenue Hiba Riahi...
Ouverture du QMessageBox...
Message affiche et ferme
```

**✅ Si vous voyez "Ouverture du QMessageBox"** → Le message devrait s'afficher
**❌ Si vous ne voyez pas ce message** → Le code n'arrive pas jusqu'à l'affichage

---

## 🆘 Solutions selon le Problème

### Problème 1 : Aucune donnée reçue ("readData() APPELE" n'apparaît jamais)

**Causes possibles :**
- Le signal `readyRead` n'est pas connecté
- Le timer ne fonctionne pas
- L'Arduino n'envoie pas de données

**Solutions :**
1. Vérifiez que l'Arduino est bien branché
2. Vérifiez que le code Arduino est téléversé
3. Ouvrez le Moniteur Série Arduino et scannez une carte pour vérifier que l'Arduino envoie bien les messages
4. Fermez le Moniteur Série avant de lancer Qt

### Problème 2 : Les données arrivent mais ne sont pas parsées

**Causes possibles :**
- Le format du message n'est pas correct
- Le message contient des caractères invisibles

**Solutions :**
1. Vérifiez dans la console le contenu exact de "Donnees brutes (ascii)"
2. Vérifiez que le message commence bien par "ACCESS_GRANTED|" ou "ACCESS_DENIED|"

### Problème 3 : Le signal est émis mais pas reçu

**Causes possibles :**
- Les signaux ne sont pas connectés dans `setupRFIDAccess()`
- Le widget `gestionpieces` n'est pas le bon parent

**Solutions :**
1. Vérifiez que `setupRFIDAccess()` est bien appelé dans le constructeur
2. Vérifiez que les `connect()` sont bien exécutés

### Problème 4 : Le signal est reçu mais le message ne s'affiche pas

**Causes possibles :**
- Le QMessageBox est créé mais pas affiché
- La fenêtre est derrière d'autres fenêtres

**Solutions :**
1. Vérifiez que vous voyez "Ouverture du QMessageBox" dans la console
2. Essayez de minimiser toutes les autres fenêtres
3. Vérifiez que l'application Qt est au premier plan

---

## 📝 Checklist de Diagnostic

Quand vous scannez une carte, notez ce que vous voyez dans la console :

- [ ] "readData() APPELE" ou "Timer detecte des donnees"
- [ ] "DONNEES RECUES DE L'ARDUINO"
- [ ] "ACCESS_GRANTED|" ou "ACCESS_DENIED|" dans les données
- [ ] "PARSING MESSAGE"
- [ ] "EMISSION SIGNAL accessGranted/accessDenied"
- [ ] "SIGNAL accessGranted/accessDenied RECU"
- [ ] "Ouverture du QMessageBox"
- [ ] "Message affiche et ferme"

**Indiquez quelles cases sont cochées** pour que je puisse identifier exactement où se situe le problème.

