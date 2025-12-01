-- ============================
-- Add NOT NULL constraints and foreign key to CLIENT table
-- Run this AFTER populate_client_objet_data.sql has populated the data
-- ============================

-- Step 1: Make id_objet column NOT NULL
ALTER TABLE CLIENT
MODIFY id_objet INT NOT NULL;

-- Step 2: Make etat column NOT NULL
ALTER TABLE CLIENT
MODIFY etat VARCHAR2(50) NOT NULL;

-- Step 3: Add check constraint for etat values (if it doesn't exist)
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

-- Step 4: Add foreign key constraint (if it doesn't exist)
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

-- Step 5: Create indexes for better query performance (if they don't exist)
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

