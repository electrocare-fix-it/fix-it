-- Script pour vérifier complètement la table UTILISATEUR
-- À exécuter dans SQL Developer

-- 1. Voir la structure complète de la table (toutes les colonnes)
SELECT * FROM UTILISATEUR;

-- 2. Voir seulement les colonnes importantes pour l'authentification
SELECT id_utilisateur, login, password, role FROM UTILISATEUR;

-- 3. Tester l'authentification pour chaque utilisateur
-- Test pour Gérant
SELECT 'Gérant test' AS test, role 
FROM UTILISATEUR 
WHERE login = 'gerant' 
  AND password = '21232f297a57a5a743894a0e4a801fc3' 
  AND role = 'Gérant de l''atelier';

-- Test pour Technicien
SELECT 'Technicien test' AS test, role 
FROM UTILISATEUR 
WHERE login = 'technicien' 
  AND password = '21232f297a57a5a743894a0e4a801fc3' 
  AND role = 'Technicien';

-- Test pour Réceptionniste
SELECT 'Réceptionniste test' AS test, role 
FROM UTILISATEUR 
WHERE login = 'receptionniste' 
  AND password = '21232f297a57a5a743894a0e4a801fc3' 
  AND role = 'Réceptionniste';

-- 4. Vérifier la structure de la table
SELECT column_name, data_type, data_length, nullable
FROM user_tab_columns
WHERE table_name = 'UTILISATEUR'
ORDER BY column_id;

