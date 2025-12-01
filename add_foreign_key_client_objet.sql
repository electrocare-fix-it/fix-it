-- ============================
-- Add Foreign Key from CLIENT to OBJET_ELECTRONIQUE
-- Adds id_objet column to CLIENT table referencing OBJET_ELECTRONIQUE(id_objet)
-- Adds etat column to store the object's state
-- Both columns are NOT NULL, so all clients must have an associated object with a state
-- ============================

-- Step 1: Add foreign key column to CLIENT table (nullable initially)
ALTER TABLE CLIENT 
ADD id_objet INT;

-- Step 2: Add etat column to CLIENT table (nullable initially)
ALTER TABLE CLIENT
ADD etat VARCHAR2(50);

-- Step 3: Populate id_objet from OBJET_ELECTRONIQUE where id_client_courant matches
MERGE INTO CLIENT c
USING (
    SELECT id_client_courant as id_client, MAX(id_objet) as id_objet
    FROM OBJET_ELECTRONIQUE
    WHERE id_client_courant IS NOT NULL
    GROUP BY id_client_courant
) o ON (c.id_client = o.id_client AND c.id_objet IS NULL)
WHEN MATCHED THEN
    UPDATE SET c.id_objet = o.id_objet;

-- Step 3b: For clients still without id_objet, try to get it from ETAT_OBJET table
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

-- Step 4: Populate etat from ETAT_OBJET table (most recent state for the client-object pair)
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

-- Step 4b: For remaining NULL etat values, get from OBJET_ELECTRONIQUE table
MERGE INTO CLIENT c
USING (
    SELECT id_objet, NVL(etat, 'en cours de réparation') as etat
    FROM OBJET_ELECTRONIQUE
) o ON (c.id_objet = o.id_objet AND c.etat IS NULL)
WHEN MATCHED THEN
    UPDATE SET c.etat = o.etat;

-- Step 4c: Set default etat for any remaining NULL values
UPDATE CLIENT
SET etat = 'en cours de réparation'
WHERE id_objet IS NOT NULL 
  AND etat IS NULL;

-- Step 4d: For clients still without id_objet, assign a default object (if needed)
-- Uncomment the following if you want to assign a default object to clients without one:
-- UPDATE CLIENT
-- SET id_objet = (SELECT MIN(id_objet) FROM OBJET_ELECTRONIQUE),
--     etat = 'en cours de réparation'
-- WHERE id_objet IS NULL
--   AND EXISTS (SELECT 1 FROM OBJET_ELECTRONIQUE);

-- Step 5: Make id_objet column NOT NULL
-- NOTE: This will fail if there are still NULL values. 
-- Either uncomment Step 4d above, or delete clients without objects first.
ALTER TABLE CLIENT
MODIFY id_objet INT NOT NULL;

-- Step 6: Make etat column NOT NULL
ALTER TABLE CLIENT
MODIFY etat VARCHAR2(50) NOT NULL;

-- Step 7: Add check constraint for etat values (if it doesn't exist)
BEGIN
    EXECUTE IMMEDIATE 'ALTER TABLE CLIENT ADD CONSTRAINT chk_client_etat_valide CHECK (
        etat IN (''en cours de réparation'', ''réparé'', ''il n''''a pas été réparé'')
    )';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE = -2264 THEN
            NULL;
        ELSE
            RAISE;
        END IF;
END;
/

-- Step 8: Add foreign key constraint (if it doesn't exist)
BEGIN
    EXECUTE IMMEDIATE 'ALTER TABLE CLIENT ADD CONSTRAINT fk_client_objet FOREIGN KEY (id_objet)
        REFERENCES OBJET_ELECTRONIQUE(id_objet)
        ON DELETE CASCADE';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE = -2275 THEN
            NULL;
        ELSE
            RAISE;
        END IF;
END;
/

-- Create indexes for better query performance (if they don't exist)
BEGIN
    EXECUTE IMMEDIATE 'CREATE INDEX idx_client_objet ON CLIENT(id_objet)';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE = -955 THEN
            NULL;
        ELSE
            RAISE;
        END IF;
END;
/

BEGIN
    EXECUTE IMMEDIATE 'CREATE INDEX idx_client_etat ON CLIENT(etat)';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE = -955 THEN
            NULL;
        ELSE
            RAISE;
        END IF;
END;
/

-- Commit the changes
COMMIT;

