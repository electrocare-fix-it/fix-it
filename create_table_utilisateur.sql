-- Script SQL pour créer la table UTILISATEUR dans Oracle
-- À exécuter dans SQL Developer

-- Supprimer la table si elle existe déjà
DROP TABLE UTILISATEUR CASCADE CONSTRAINTS;

-- Créer la table UTILISATEUR
CREATE TABLE UTILISATEUR (
    id_utilisateur INT PRIMARY KEY,
    login VARCHAR2(50) UNIQUE NOT NULL,
    password VARCHAR2(100) NOT NULL,
    role VARCHAR2(50) NOT NULL
);

-- Créer une séquence pour l'ID
DROP SEQUENCE seq_utilisateur;
CREATE SEQUENCE seq_utilisateur START WITH 1 INCREMENT BY 1;

-- Insérer les utilisateurs par défaut
-- Mot de passe: "admin" (hashé en MD5: 21232f297a57a5a743894a0e4a801fc3)
INSERT INTO UTILISATEUR (id_utilisateur, login, password, role) 
VALUES (seq_utilisateur.NEXTVAL, 'gerant', '21232f297a57a5a743894a0e4a801fc3', 'Gérant de l''atelier');

INSERT INTO UTILISATEUR (id_utilisateur, login, password, role) 
VALUES (seq_utilisateur.NEXTVAL, 'technicien', '21232f297a57a5a743894a0e4a801fc3', 'Technicien');

INSERT INTO UTILISATEUR (id_utilisateur, login, password, role) 
VALUES (seq_utilisateur.NEXTVAL, 'receptionniste', '21232f297a57a5a743894a0e4a801fc3', 'Réceptionniste');

-- Vérifier les données insérées
SELECT * FROM UTILISATEUR;

-- Commit des changements
COMMIT;

