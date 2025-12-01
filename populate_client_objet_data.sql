-- ============================
-- Populate id_objet and etat columns in CLIENT table
-- This script populates data from existing relationships in other tables
-- ============================

-- Step 1: Populate id_objet from OBJET_ELECTRONIQUE where id_client_courant matches
MERGE INTO CLIENT c
USING (
    SELECT id_client_courant as id_client, MAX(id_objet) as id_objet
    FROM OBJET_ELECTRONIQUE
    WHERE id_client_courant IS NOT NULL
    GROUP BY id_client_courant
) o ON (c.id_client = o.id_client AND c.id_objet IS NULL)
WHEN MATCHED THEN
    UPDATE SET c.id_objet = o.id_objet;

-- Step 2: For clients still without id_objet, get it from ETAT_OBJET table
MERGE INTO CLIENT c
USING (
    SELECT id_client, id_objet
    FROM (
        SELECT id_client, id_objet,
               ROW_NUMBER() OVER (PARTITION BY id_client ORDER BY date_modification DESC) as rn
        FROM ETAT_OBJET
    )
    WHERE rn = 1
) e ON (c.id_client = e.id_client AND c.id_objet IS NULL)
WHEN MATCHED THEN
    UPDATE SET c.id_objet = e.id_objet;

-- Step 3: Populate etat from ETAT_OBJET table (most recent state)
MERGE INTO CLIENT c
USING (
    SELECT id_client, id_objet, etat
    FROM (
        SELECT id_client, id_objet, etat,
               ROW_NUMBER() OVER (PARTITION BY id_client, id_objet ORDER BY date_modification DESC) as rn
        FROM ETAT_OBJET
    )
    WHERE rn = 1
) e ON (c.id_client = e.id_client AND c.id_objet = e.id_objet AND c.etat IS NULL)
WHEN MATCHED THEN
    UPDATE SET c.etat = e.etat;

-- Step 4: For remaining NULL etat values, get from OBJET_ELECTRONIQUE table
MERGE INTO CLIENT c
USING (
    SELECT id_objet, NVL(etat, 'en cours de réparation') as etat
    FROM OBJET_ELECTRONIQUE
) o ON (c.id_objet = o.id_objet AND c.etat IS NULL)
WHEN MATCHED THEN
    UPDATE SET c.etat = o.etat;

-- Step 5: Set default etat for any remaining NULL values
UPDATE CLIENT
SET etat = 'en cours de réparation'
WHERE id_objet IS NOT NULL 
  AND etat IS NULL;

-- Diagnostic: Check if relationships exist
SELECT 'OBJET_ELECTRONIQUE with id_client_courant' as check_type, COUNT(*) as count
FROM OBJET_ELECTRONIQUE
WHERE id_client_courant IS NOT NULL
UNION ALL
SELECT 'ETAT_OBJET records' as check_type, COUNT(*) as count
FROM ETAT_OBJET
UNION ALL
SELECT 'Clients in CLIENT table' as check_type, COUNT(*) as count
FROM CLIENT;

-- Verify the results
SELECT 
    COUNT(*) as total_clients,
    COUNT(id_objet) as clients_with_objet,
    COUNT(etat) as clients_with_etat,
    COUNT(CASE WHEN id_objet IS NULL THEN 1 END) as clients_without_objet,
    COUNT(CASE WHEN etat IS NULL THEN 1 END) as clients_without_etat
FROM CLIENT;

-- Commit the changes
COMMIT;

