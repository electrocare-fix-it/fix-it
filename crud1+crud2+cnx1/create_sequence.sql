-- Script SQL pour créer la séquence Oracle pour ID_OBJET
-- À exécuter dans Oracle SQL Developer ou SQL*Plus

-- Créer la séquence pour générer automatiquement les ID_OBJET
CREATE SEQUENCE SEQ_OBJETS_ID
START WITH 1
INCREMENT BY 1
NOCACHE
NOCYCLE;

-- Optionnel: Si vous voulez que la séquence commence après le dernier ID existant
-- SELECT NVL(MAX(ID_OBJET), 0) + 1 FROM OBJETS;
-- ALTER SEQUENCE SEQ_OBJETS_ID RESTART START WITH <valeur_trouvée>;

COMMIT;

