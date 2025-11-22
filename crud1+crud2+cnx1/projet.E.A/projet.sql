-- Supprimer les anciennes tables si elles existent (ordre inverse des dépendances)
DROP TABLE REPARER CASCADE CONSTRAINTS;
DROP TABLE POSSEDER CASCADE CONSTRAINTS;
DROP TABLE UTILISER CASCADE CONSTRAINTS;
DROP TABLE PIECE_DETACHEE CASCADE CONSTRAINTS;
DROP TABLE OBJET_ELECTRONIQUE CASCADE CONSTRAINTS;
DROP TABLE CLIENT CASCADE CONSTRAINTS;
DROP TABLE EMPLOYE CASCADE CONSTRAINTS;

-- ============================
-- TABLE : EMPLOYE
-- ============================
CREATE TABLE EMPLOYE (
    id_employe        INT PRIMARY KEY,
    nom               VARCHAR2(50),
    prenom            VARCHAR2(50),
    date_naissance    DATE,
    adresse           VARCHAR2(100),
    num_tel           VARCHAR2(20),
    email             VARCHAR2(100),
    statut            VARCHAR2(30)
);

-- ============================
-- TABLE : CLIENT
-- ============================
CREATE TABLE CLIENT (
    id_client         INT PRIMARY KEY,
    nom               VARCHAR2(50),
    prenom            VARCHAR2(50),
    cin               VARCHAR2(20) UNIQUE,
    telephone         VARCHAR2(20),
    email             VARCHAR2(100),
    adresse           VARCHAR2(100),
    date_naissance    DATE
);

-- ============================
-- TABLE : OBJET_ELECTRONIQUE
-- (contient FK vers client et employe courants, NULL si inconnu)
-- ============================
CREATE TABLE OBJET_ELECTRONIQUE (
    id_objet          INT PRIMARY KEY,
    nom_objet         VARCHAR2(50),
    marque            VARCHAR2(50),
    modele            VARCHAR2(50),
    couleur           VARCHAR2(30),
    num_serie         VARCHAR2(50),
    type_objet        VARCHAR2(40),
    etat              VARCHAR2(30),
    -- propriétaire / technicien courant (peuvent être NULL)
    id_client_courant INT,
    id_employe_courant INT,
    CONSTRAINT fk_objet_clientcourant FOREIGN KEY (id_client_courant)
        REFERENCES CLIENT(id_client)
        ON DELETE SET NULL,
    CONSTRAINT fk_objet_employecourant FOREIGN KEY (id_employe_courant)
        REFERENCES EMPLOYE(id_employe)
        ON DELETE SET NULL
);

-- ============================
-- TABLE : PIECE_DETACHEE
-- ============================
CREATE TABLE PIECE_DETACHEE (
    id_piece          INT PRIMARY KEY,
    nom_piece         VARCHAR2(80),
    categorie         VARCHAR2(40),
    prix_unitaire     NUMBER(10,2),
    date_entree       DATE,
    quantite_stock    INT,
    date_sortie       DATE,
    fournisseur       VARCHAR2(100)
);

-- ============================
-- TABLE : UTILISER (n-n Objet <-> Pièce)
-- conserve la date d'utilisation / quantité utilisée
-- ============================
CREATE TABLE UTILISER (
    id_objet          INT,
    id_piece          INT,
    date_utilisation  DATE,
    quantite_utilisee INT DEFAULT 1,
    PRIMARY KEY (id_objet, id_piece, date_utilisation),
    CONSTRAINT fk_utiliser_objet FOREIGN KEY (id_objet)
        REFERENCES OBJET_ELECTRONIQUE(id_objet)
        ON DELETE CASCADE,
    CONSTRAINT fk_utiliser_piece FOREIGN KEY (id_piece)
        REFERENCES PIECE_DETACHEE(id_piece)
        ON DELETE CASCADE
);

-- ============================
-- TABLE : POSSEDER
-- Représente la relation Client ? Objet (historique de possession)
-- Une ligne = période pendant laquelle un client possède un objet
-- ============================
CREATE TABLE POSSEDER (
    id_possession     INT PRIMARY KEY,
    id_client         INT NOT NULL,
    id_objet          INT NOT NULL,
    date_debut        DATE DEFAULT SYSDATE,
    date_fin          DATE NULL,
    commentaire       VARCHAR2(200),
    CONSTRAINT fk_poss_client FOREIGN KEY (id_client)
        REFERENCES CLIENT(id_client)
        ON DELETE CASCADE,
    CONSTRAINT fk_poss_objet FOREIGN KEY (id_objet)
        REFERENCES OBJET_ELECTRONIQUE(id_objet)
        ON DELETE CASCADE
);

-- ============================
-- TABLE : REPARER
-- Représente les interventions (historique des réparations)
-- Une ligne = une réparation par un employé sur un objet
-- ============================
CREATE TABLE REPARER (
    id_reparation     INT PRIMARY KEY,
    id_objet          INT NOT NULL,
    id_employe        INT NOT NULL,
    date_debut        DATE DEFAULT SYSDATE,
    date_fin          DATE NULL,
    description       VARCHAR2(4000),
    statut_reparation VARCHAR2(30), -- ex: en_cours, terminee, annulée
    CONSTRAINT fk_repar_objet FOREIGN KEY (id_objet)
        REFERENCES OBJET_ELECTRONIQUE(id_objet)
        ON DELETE CASCADE,
    CONSTRAINT fk_repar_employe FOREIGN KEY (id_employe)
        REFERENCES EMPLOYE(id_employe)
        ON DELETE SET NULL
);

-- ============================
-- Exemples d'indexs (optionnel)
-- ============================
CREATE INDEX idx_objet_numserie ON OBJET_ELECTRONIQUE(num_serie);
CREATE INDEX idx_piece_categorie ON PIECE_DETACHEE(categorie);
CREATE INDEX idx_poss_client ON POSSEDER(id_client);
CREATE INDEX idx_repar_employe ON REPARER(id_employe);
