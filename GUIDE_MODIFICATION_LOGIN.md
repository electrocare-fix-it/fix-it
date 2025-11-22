# Guide de Modification de la Page d'Authentification

## 📋 Comment modifier `login.ui` dans Qt Designer

### 1. Ouvrir le fichier dans Qt Designer
- Dans Qt Creator, faites un clic droit sur `login.ui`
- Sélectionnez "Ouvrir avec" → "Qt Designer"
- Ou ouvrez Qt Designer séparément et ouvrez le fichier `login.ui`

### 2. Modifier les tailles des Labels

#### Pour changer la taille de police d'un label :
1. Sélectionnez le label (ex: `lblLogin`, `lblPassword`, `lblRole`)
2. Dans le panneau "Property Editor" à droite :
   - Trouvez la propriété **"font"**
   - Cliquez sur le bouton "..." à côté
   - Modifiez la **"Point Size"** (ex: 12, 14, 16, 18, etc.)
   - Vous pouvez aussi changer le style (Bold, Italic, etc.)

#### Pour changer la taille du widget label :
1. Sélectionnez le label
2. Dans "Property Editor" :
   - **minimumSize** : Taille minimale (width, height)
   - **maximumSize** : Taille maximale (width, height)
   - Vous pouvez aussi redimensionner directement en glissant les bords

### 3. Modifier les tailles des LineEdit

#### Pour changer la taille de police :
1. Sélectionnez le LineEdit (ex: `txtLogin`, `txtPassword`)
2. Dans "Property Editor" → **"font"** → **"Point Size"**

#### Pour changer la hauteur :
1. Sélectionnez le LineEdit
2. Modifiez **minimumSize** et **maximumSize** (propriété height)
   - Exemple : minimumSize height = 35, maximumSize height = 50

### 4. Ajouter une Image

#### Méthode 1 : Utiliser une image depuis les ressources (Recommandé)
1. **Ajoutez votre image au fichier `img.qrc`** :
   - Ouvrez `img.qrc` dans Qt Creator
   - Cliquez sur "Add" → "Add Files"
   - Sélectionnez votre image (PNG, JPG, etc.)
   - Sauvegardez

2. **Dans Qt Designer** :
   - Sélectionnez le label `lblImageLogo` (en haut de la page)
   - Dans "Property Editor", trouvez **"pixmap"**
   - Cliquez sur le bouton "..." à côté
   - Cliquez sur "Resource Browser" (icône dossier)
   - Sélectionnez votre image depuis `:/res/...`
   - Cliquez "OK"

#### Méthode 2 : Utiliser une image depuis un fichier
1. Sélectionnez `lblImageLogo`
2. Dans "Property Editor" → **"pixmap"** → "..." → "File System"
3. Naviguez vers votre fichier image
4. Sélectionnez-le et cliquez "OK"

#### Ajuster la taille de l'image :
- **minimumSize** : Taille minimale (ex: 150x150)
- **maximumSize** : Taille maximale (ex: 300x300)
- **scaledContents** : Mettre à `true` pour que l'image s'adapte automatiquement

### 5. Modifier le ComboBox (Rôle)

1. Sélectionnez `comboRole`
2. **Taille de police** : "Property Editor" → "font" → "Point Size"
3. **Hauteur** : Modifiez `minimumSize` et `maximumSize`

### 6. Modifier les Boutons

1. Sélectionnez un bouton (`btnLogin` ou `btnCancel`)
2. **Taille de police** : "Property Editor" → "font" → "Point Size"
3. **Taille du bouton** : Modifiez `minimumSize` et `maximumSize`
4. **Texte** : Modifiez la propriété **"text"**

### 7. Modifier le Titre

1. Sélectionnez `lblTitle`
2. **Taille de police** : "font" → "Point Size" (actuellement 24)
3. **Texte** : Modifiez la propriété **"text"**
4. **Style** : "font" → Cochez "Bold" pour le gras

### 8. Modifier les Couleurs et Styles

1. Sélectionnez le widget (ou le widget parent pour appliquer à tous)
2. Dans "Property Editor", trouvez **"styleSheet"**
3. Cliquez sur "..." pour éditer le CSS
4. Modifiez les couleurs, bordures, etc.

Exemple pour changer la couleur de fond :
```css
QWidget {
    background-color: #VOTRE_COULEUR;
}
```

## 📝 Widgets disponibles dans login.ui

| Nom du Widget | Type | Description |
|--------------|------|-------------|
| `lblImageLogo` | QLabel | Label pour afficher une image/logo |
| `lblTitle` | QLabel | Titre "AUTHENTIFICATION" |
| `lblLogin` | QLabel | Label "Nom d'utilisateur:" |
| `txtLogin` | QLineEdit | Champ de saisie pour le login |
| `lblPassword` | QLabel | Label "Mot de passe:" |
| `txtPassword` | QLineEdit | Champ de saisie pour le mot de passe |
| `lblRole` | QLabel | Label "Rôle:" |
| `comboRole` | QComboBox | Liste déroulante pour le rôle |
| `btnLogin` | QPushButton | Bouton "Se connecter" |
| `btnCancel` | QPushButton | Bouton "Annuler" |

## 🎨 Exemples de modifications courantes

### Augmenter la taille de tous les labels à 16px :
1. Sélectionnez chaque label (`lblLogin`, `lblPassword`, `lblRole`)
2. "font" → "Point Size" → 16

### Augmenter la hauteur des LineEdit à 45px :
1. Sélectionnez `txtLogin` et `txtPassword`
2. `minimumSize` → height = 45
3. `maximumSize` → height = 60

### Ajouter un logo en haut :
1. Sélectionnez `lblImageLogo`
2. "pixmap" → Sélectionnez votre image
3. Ajustez `minimumSize` et `maximumSize` selon vos besoins

## ⚠️ Important

- **Sauvegardez toujours** après vos modifications (Ctrl+S)
- **Recompilez** le projet après avoir modifié `login.ui`
- Les images doivent être dans le dossier du projet ou dans `img.qrc` pour être incluses dans l'application
- Les noms des widgets (comme `txtLogin`, `btnLogin`) ne doivent **PAS** être modifiés, car ils sont utilisés dans le code C++

## 🔄 Après modification

1. Sauvegardez `login.ui`
2. Dans Qt Creator : **Build** → **Rebuild Project**
3. Exécutez l'application pour voir les changements

