-- ============================
-- TABLE : ETAT_OBJET
-- Table pour stocker l'état des objets par client
-- ============================
CREATE TABLE ETAT_OBJET (
    id_objet          INT NOT NULL,
    id_client         INT NOT NULL,
    etat              VARCHAR2(50) NOT NULL,
    date_modification DATE DEFAULT SYSDATE,
    PRIMARY KEY (id_objet, id_client),
    CONSTRAINT fk_etat_objet_objet FOREIGN KEY (id_objet)
        REFERENCES OBJET_ELECTRONIQUE(id_objet)
        ON DELETE CASCADE,
    CONSTRAINT fk_etat_objet_client FOREIGN KEY (id_client)
        REFERENCES CLIENT(id_client)
        ON DELETE CASCADE,
    CONSTRAINT chk_etat_valide CHECK (etat IN ('en cours de réparation', 'réparé', 'il n''a pas été réparé'))
);

-- Index pour améliorer les performances
CREATE INDEX idx_etat_objet_client ON ETAT_OBJET(id_client);
CREATE INDEX idx_etat_objet_objet ON ETAT_OBJET(id_objet);


