# 🔍 Vérification du Port COM5 - Guide de Dépannage

## ✅ Étape 1 : Vérifier que le Port est Libre

### 1.1 : Fermer TOUS les Programmes Utilisant le Port

**IMPORTANT** : Le port COM5 ne peut être utilisé que par UN SEUL programme à la fois.

1. **Fermez COMPLÈTEMENT Arduino IDE** :
   - Fermez toutes les fenêtres Arduino IDE
   - Ouvrez le **Gestionnaire des tâches** (Ctrl+Shift+Esc)
   - Cherchez "Arduino" dans les processus
   - Si vous trouvez "Arduino.exe" ou "java.exe" (Arduino IDE), faites **Clic droit → Terminer la tâche**

2. **Fermez TOUS les Moniteurs Série** :
   - Vérifiez qu'aucun Moniteur Série n'est ouvert
   - Fermez tous les programmes de terminal série (PuTTY, Tera Term, etc.)

3. **Fermez l'Application Qt** (si elle est ouverte)

### 1.2 : Vérifier le Port dans le Gestionnaire de Périphériques

1. Ouvrez le **Gestionnaire de périphériques**
2. Allez dans **Ports (COM et LPT)**
3. Vérifiez que **Arduino Uno (COM5)** est présent
4. **Clic droit** sur **Arduino Uno (COM5)** → **Propriétés**
5. Vérifiez que l'état est **"Ce périphérique fonctionne correctement"**

### 1.3 : Tester le Port avec un Programme Simple

Si vous avez encore des problèmes, testez avec ce code simple dans Arduino IDE :

```cpp
void setup() {
  Serial.begin(9600);
  Serial.println("Test COM5");
}

void loop() {
  Serial.println("OK");
  delay(1000);
}
```

1. Téléversez ce code
2. Ouvrez le Moniteur Série
3. Si vous voyez "Test COM5" et "OK" toutes les secondes → Le port fonctionne
4. **FERMEZ le Moniteur Série** avant de lancer l'application Qt

---

## ✅ Étape 2 : Lancer l'Application Qt

### 2.1 : Ordre d'Exécution CORRECT

1. **Fermez Arduino IDE complètement**
2. **Vérifiez le Gestionnaire des tâches** qu'aucun processus Arduino n'est actif
3. **Lancez l'application Qt**
4. **Connectez-vous**
5. **Ouvrez "Gestion Pièces Détachées"**

### 2.2 : Vérifier les Messages dans la Console

Dans la console Qt, vous devriez voir :

```
=== TEST CONNEXION ARDUINO RFID ===
Ports série disponibles: ("COM5")
=== Tentative de connexion au port: COM5
Vérification de la disponibilité du port COM5...
Port COM5 est disponible!
  Tentative 1/15...
  Tentative 2/15...
✓✓✓ SUCCÈS: Connecté à l'Arduino sur le port: COM5
```

---

## 🆘 Si l'Erreur "Accès Refusé" Persiste

### Solution 1 : Redémarrer le Port COM

1. Ouvrez le **Gestionnaire de périphériques**
2. Allez dans **Ports (COM et LPT)**
3. **Clic droit** sur **Arduino Uno (COM5)**
4. Sélectionnez **"Désactiver le périphérique"**
5. Attendez 3 secondes
6. **Clic droit** à nouveau → **"Activer le périphérique"**
7. Attendez que Windows reconnaisse le port
8. Relancez l'application Qt

### Solution 2 : Débrancher et Rebrancher l'Arduino

1. **Fermez l'application Qt**
2. **Débranchez l'Arduino USB**
3. **Attendez 10 secondes**
4. **Rebranchez l'Arduino USB**
5. **Attendez que Windows reconnaisse le port** (notification)
6. **Vérifiez dans le Gestionnaire de périphériques** que COM5 est toujours là
7. **Lancez l'application Qt**

### Solution 3 : Redémarrer l'Ordinateur

Si rien ne fonctionne :

1. **Fermez tous les programmes**
2. **Débranchez l'Arduino**
3. **Redémarrez l'ordinateur**
4. **Rebranchez l'Arduino** après le redémarrage
5. **Attendez que Windows reconnaisse le port**
6. **Lancez l'application Qt**

---

## 📝 Notes Importantes

1. **NE JAMAIS** ouvrir le Moniteur Série Arduino en même temps que l'application Qt
2. **TOUJOURS** fermer Arduino IDE complètement avant de lancer Qt
3. **VÉRIFIER** le Gestionnaire des tâches qu'aucun processus Arduino n'est actif
4. **UN SEUL PROGRAMME** peut utiliser le port COM5 à la fois

---

## ✅ Vérification Finale

Si après toutes ces étapes, vous voyez toujours "Accès refusé", il est possible que :

1. Un autre programme utilise le port en arrière-plan
2. Les permissions Windows ne sont pas correctes
3. Le port est verrouillé par un processus système

Dans ce cas, contactez-moi avec :
- Les messages exacts de la console Qt
- Une capture d'écran du Gestionnaire des tâches (processus)
- Une capture d'écran du Gestionnaire de périphériques (propriétés de COM5)

