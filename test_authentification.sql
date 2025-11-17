-- Script SQL pour tester et vérifier la table UTILISATEUR
-- À exécuter dans SQL Developer

-- 1. Vérifier que la table existe
SELECT COUNT(*) AS nombre_tables 
FROM user_tables 
WHERE UPPER(table_name) = 'UTILISATEUR';

-- 2. Voir tous les utilisateurs
SELECT * FROM UTILISATEUR;

-- 3. Vérifier un utilisateur spécifique (exemple: gerant)
SELECT id_utilisateur, login, password, role 
FROM UTILISATEUR 
WHERE login = 'gerant';

-- 4. Vérifier le hash MD5 du mot de passe "admin"
-- Le hash MD5 de "admin" devrait être: 21232f297a57a5a743894a0e4a801fc3
SELECT login, password, role 
FROM UTILISATEUR 
WHERE password = '21232f297a57a5a743894a0e4a801fc3';

-- 5. Tester l'authentification (simulation de la requête du code)
SELECT role 
FROM UTILISATEUR 
WHERE login = 'gerant' 
  AND password = '21232f297a57a5a743894a0e4a801fc3' 
  AND role = 'Gérant de l''atelier';

-- 6. Vérifier la séquence
SELECT sequence_name, last_number 
FROM user_sequences 
WHERE sequence_name = 'SEQ_UTILISATEUR';

