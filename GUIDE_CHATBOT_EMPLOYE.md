# 🤖 Guide du Chatbot Intelligent pour la Gestion des Employés

## 📋 Vue d'ensemble

Le chatbot intelligent est un assistant conversationnel qui permet de rechercher et d'obtenir des informations sur les employés de manière naturelle et intuitive. Contrairement à une simple barre de recherche, le chatbot :

- **Comprend le langage naturel** : Vous pouvez poser des questions en français naturel
- **Gère l'ambiguïté** : Si plusieurs employés portent le même nom, le chatbot vous demande de préciser
- **Fournit des réponses contextuelles** : Répond directement à votre question sans afficher toutes les informations
- **Recherche intelligente** : Trouve les employés même avec des orthographes approximatives

## 🚀 Fonctionnalités Principales

### 1. **Recherche par Nom ou Prénom**

Le chatbot peut rechercher un employé en utilisant :
- Le prénom seul : "Marie"
- Le nom seul : "Dupont"
- Le nom complet : "Jean Dupont"
- Recherche partielle : "Mar" trouvera "Marie", "Marc", etc.

**Exemples de questions :**
- "Informations sur Marie"
- "Qui est Jean Dupont ?"
- "Trouve Paul"

### 2. **Requêtes Contextuelles Spécifiques**

Le chatbot comprend différents types de questions et répond directement :

#### 📍 **Adresse**
- "Quelle est l'adresse de Marie ?"
- "Où habite Jean Dupont ?"
- "Adresse de Paul Martin"

#### 📞 **Téléphone**
- "Quel est le numéro de téléphone de Marie ?"
- "Téléphone de Jean"
- "Numéro de Paul"

#### 📧 **Email**
- "Quel est l'email de Marie ?"
- "Adresse email de Jean Dupont"
- "Courriel de Paul"

#### 📊 **Statut**
- "Quel est le statut de Marie ?"
- "Statut de Jean Dupont"
- "Situation de Paul"

#### 🎂 **Âge**
- "Quel âge a Marie ?"
- "Date de naissance de Jean"
- "Né en quelle année pour Paul ?"

#### 📋 **Synthèse Complète**
- "Synthèse pour Marie Leclerc"
- "Résumé de Jean Dupont"
- "Dossier complet de Paul"
- "Toutes les informations sur Marie"

### 3. **Gestion de l'Ambiguïté**

Si plusieurs employés portent le même nom, le chatbot vous présente la liste et vous demande de choisir :

**Exemple :**
```
Vous : "Informations sur Marie"

🤖 Assistant : J'ai trouvé 2 employé(s) portant ce nom :

1. **Marie Dupont** (ID: 456, Statut: Actif)
2. **Marie Leclerc** (ID: 789, Statut: En Congé)

💡 Tapez le numéro (1, 2, etc.) pour sélectionner l'employé souhaité.
```

Vous pouvez alors taper "1" ou "2" pour sélectionner l'employé souhaité, ou cliquer sur le bouton correspondant.

### 4. **Recherche Floue (Fuzzy Search)**

Le chatbot peut trouver des employés même si l'orthographe n'est pas exacte :

**Exemple :**
```
Vous : "Qui est Dufour ?"

🤖 Assistant : Je n'ai pas trouvé Dufour. Voulez-vous parler de Marc Dufourt (ID: 900) ?
```

### 5. **Filtrage du Tableau**

Pour une recherche simple qui doit filtrer le tableau :

**Exemple :**
```
Vous : "Recherche Paul"
Vous : "Affiche Marie dans le tableau"
```

Le chatbot filtrera automatiquement le tableau pour afficher l'employé recherché.

## 💡 Exemples d'Utilisation

### Scénario 1 : Recherche Simple
```
Vous : "Quelle est l'adresse de Marie ?"

🤖 Assistant : L'adresse de **Marie Dupont** (ID: 456) est :

15, Rue des Ateliers, 75000 Paris
```

### Scénario 2 : Ambiguïté Résolue
```
Vous : "Informations sur Marie"

🤖 Assistant : J'ai trouvé 2 employé(s) portant ce nom :

1. **Marie Dupont** (ID: 456, Statut: Actif)
2. **Marie Leclerc** (ID: 789, Statut: En Congé)

💡 Tapez le numéro (1, 2, etc.) pour sélectionner l'employé souhaité.

Vous : "1"

🤖 Assistant : 👤 **Marie Dupont** (ID: 456)

📊 **Statut** : Actif
📧 **Email** : marie.dupont@example.com
📞 **Téléphone** : 06 12 34 56 78
📍 **Adresse** : 15, Rue des Ateliers, 75000 Paris
🎂 **Date de naissance** : 15/03/1990 (34 ans)
```

### Scénario 3 : Synthèse Complète
```
Vous : "Synthèse pour Jean Dupont"

🤖 Assistant : 📋 **SYNTHÈSE - Jean Dupont** (ID: 321)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✅ **Statut** : Actif

📧 **Email** : jean.dupont@example.com
📞 **Téléphone** : 06 98 76 54 32
📍 **Adresse** : 42, Avenue de la République, 69000 Lyon

🎂 **Date de naissance** : 22/07/1985 (39 ans)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## 🎨 Interface Utilisateur

Le chatbot est intégré dans l'interface de gestion des employés. Il apparaît dans une page séparée du `QStackedWidget` :

- **Page 0** : Interface principale avec formulaire et tableau
- **Page 1** : Chatbot intelligent

### Navigation

Pour accéder au chatbot, vous pouvez :
1. Ajouter un bouton dans l'interface pour basculer vers la page chatbot
2. Le chatbot peut être affiché en permanence dans un panneau latéral (à implémenter selon vos préférences)

## 🔧 Intégration Technique

### Signaux Émis

Le chatbot émet deux signaux principaux :

1. **`employeeSelected(int employeeId)`** : Émis quand un employé est sélectionné
   - Permet de charger les informations dans le formulaire
   - Permet de sélectionner la ligne dans le tableau

2. **`filterTableRequested(const QString& filter)`** : Émis pour filtrer le tableau
   - Permet de filtrer le tableau avec le nom de l'employé

### Connexion à la Base de Données

Le chatbot utilise la classe `Connection` existante pour accéder à la table `EMPLOYE` :
- `id_employe` (clé primaire)
- `nom`
- `prenom`
- `date_naissance`
- `adresse`
- `num_tel`
- `email`
- `statut`

## 🚀 Améliorations Futures Possibles

1. **Historique des conversations** : Sauvegarder les conversations précédentes
2. **Suggestions automatiques** : Proposer des questions fréquentes
3. **Recherche avancée** : Recherche par critères multiples (statut + nom, etc.)
4. **Actions directes** : Modifier le statut, envoyer un email, etc. depuis le chatbot
5. **Statistiques** : Afficher des statistiques sur les employés
6. **Notifications** : Alertes pour les contrats qui arrivent à expiration

## 📝 Notes Techniques

- Le chatbot utilise un traitement de langage naturel simple basé sur des expressions régulières
- La recherche dans la base de données utilise des requêtes SQL avec `LIKE` pour la recherche partielle
- Les réponses sont formatées en HTML pour un affichage riche
- L'interface est responsive et s'adapte à la taille de la fenêtre

## 🎯 Avantages par Rapport à une Barre de Recherche Simple

| Fonctionnalité | Barre de Recherche | Chatbot Intelligent |
|----------------|-------------------|---------------------|
| Recherche par nom | ✅ | ✅ |
| Gestion de l'ambiguïté | ❌ | ✅ |
| Réponses contextuelles | ❌ | ✅ |
| Recherche floue | ❌ | ✅ |
| Synthèse d'information | ❌ | ✅ |
| Langage naturel | ❌ | ✅ |
| Suggestions interactives | ❌ | ✅ |

---

**Développé avec ❤️ pour améliorer l'expérience utilisateur de la gestion des employés**

