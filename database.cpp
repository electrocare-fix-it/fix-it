#include "database.h"
#include <QDebug>
#include <QMetaType>

DatabaseManager::DatabaseManager()
    : m_connection(Connection::createInstance())
{
}

DatabaseManager::~DatabaseManager()
{
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connect()
{
    return m_connection.createConnection();
}

void DatabaseManager::disconnect()
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::isConnected() const
{
    QSqlDatabase& db = m_connection.getDatabase();
    return db.isOpen();
}

bool DatabaseManager::tableExists(const QString& tableName) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM user_tables WHERE UPPER(table_name) = UPPER(:tableName)");
    query.bindValue(":tableName", tableName);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    return query.value(0).toInt() > 0;
}

bool DatabaseManager::createTables()
{
    return true;
}

bool DatabaseManager::createSequenceIfNotExists()
{
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            return false;
        }
        
        QSqlQuery query(db);
        
        QStringList sequences = {"SEQ_CLIENT", "SEQ_CLIENT_ID", "CLIENT_SEQ"};
        bool sequenceFound = false;
        
        for (const QString& seqName : sequences) {
            if (!query.prepare("SELECT COUNT(*) FROM user_sequences WHERE sequence_name = :seqName")) {
                continue;
            }
            query.bindValue(":seqName", seqName);
            
            if (query.exec() && query.next() && query.value(0).toInt() > 0) {
                sequenceFound = true;
                break;
            }
        }
        
        if (!sequenceFound) {
            qDebug() << "Creation de la sequence SEQ_CLIENT...";
            QString createSeq = "CREATE SEQUENCE SEQ_CLIENT START WITH 1 INCREMENT BY 1 NOCACHE";
            if (!query.exec(createSeq)) {
                qDebug() << "Erreur lors de la creation de la sequence:" << query.lastError().text();
            }
        }
        
        QStringList objSequences = {"SEQ_OBJET", "SEQ_OBJETS_ID", "SEQ_OBJET_ELECTRONIQUE"};
        bool objSequenceFound = false;
        
        for (const QString& seqName : objSequences) {
            if (!query.prepare("SELECT COUNT(*) FROM user_sequences WHERE sequence_name = :seqName")) {
                continue;
            }
            query.bindValue(":seqName", seqName);
            
            if (query.exec() && query.next() && query.value(0).toInt() > 0) {
                objSequenceFound = true;
                break;
            }
        }
        
        if (!objSequenceFound) {
            qDebug() << "Creation de la sequence SEQ_OBJET...";
            QString createSeq = "CREATE SEQUENCE SEQ_OBJET START WITH 1 INCREMENT BY 1 NOCACHE";
            if (!query.exec(createSeq)) {
                qDebug() << "Erreur lors de la creation de la sequence SEQ_OBJET:" << query.lastError().text();
            }
        }
        
        return true;
    } catch (...) {
        qDebug() << "Exception dans createSequenceIfNotExists";
        return false;
    }
}

bool DatabaseManager::insertClient(const Client& client)
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        if (!m_connection.createConnection()) {
            m_lastError = "Impossible de se connecter à la base de données: " + db.lastError().text();
            return false;
        }
    }
    
    qDebug() << "Driver utilise pour INSERT:" << db.driverName();
    qDebug() << "Database name:" << db.databaseName();
    
    createSequenceIfNotExists();
    
    QSqlQuery query(db);
    
    QString insertQuery = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                          "VALUES (SEQ_CLIENT.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, TO_DATE(:date_naissance, 'YYYY-MM-DD'))";
    
    query.prepare(insertQuery);
    
    query.bindValue(":cin", client.getCin());
    query.bindValue(":nom", client.getNom());
    query.bindValue(":prenom", client.getPrenom());
    query.bindValue(":telephone", client.getTelephone());
    query.bindValue(":email", client.getEmail());
    query.bindValue(":adresse", client.getAdresse());
    query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
    
    if (!query.exec()) {
        QString seqError = query.lastError().text();
        qDebug() << "Erreur initiale:" << seqError;
        
        if (seqError.contains("SEQ_CLIENT", Qt::CaseInsensitive) || 
            seqError.contains("sequence", Qt::CaseInsensitive) ||
            seqError.contains("ORA-02289", Qt::CaseInsensitive)) {
            
            qDebug() << "Sequence SEQ_CLIENT non trouvee, essai avec SEQ_CLIENT_ID...";
            
            QString insertQuery2 = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                                  "VALUES (SEQ_CLIENT_ID.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, TO_DATE(:date_naissance, 'YYYY-MM-DD'))";
            
            query.prepare(insertQuery2);
            query.bindValue(":cin", client.getCin());
            query.bindValue(":nom", client.getNom());
            query.bindValue(":prenom", client.getPrenom());
            query.bindValue(":telephone", client.getTelephone());
            query.bindValue(":email", client.getEmail());
            query.bindValue(":adresse", client.getAdresse());
            query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
            
            if (!query.exec()) {
                qDebug() << "Sequence SEQ_CLIENT_ID non trouvee, essai avec CLIENT_SEQ...";
                
                QString insertQuery3 = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                                      "VALUES (CLIENT_SEQ.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, TO_DATE(:date_naissance, 'YYYY-MM-DD'))";
                
                query.prepare(insertQuery3);
                query.bindValue(":cin", client.getCin());
                query.bindValue(":nom", client.getNom());
                query.bindValue(":prenom", client.getPrenom());
                query.bindValue(":telephone", client.getTelephone());
                query.bindValue(":email", client.getEmail());
                query.bindValue(":adresse", client.getAdresse());
                query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
                
                if (!query.exec()) {
                    m_lastError = query.lastError().text();
                    qDebug() << "Erreur SQL:" << query.lastQuery();
                    qDebug() << "Erreur Oracle:" << m_lastError;
                    return false;
                }
            }
        } else {
            m_lastError = seqError;
            qDebug() << "Erreur SQL:" << query.lastQuery();
            qDebug() << "Erreur Oracle:" << m_lastError;
            return false;
        }
    }
    
    return true;
}

bool DatabaseManager::updateClient(const Client& client)
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("UPDATE CLIENT SET "
                  "NOM = :nom, "
                  "PRENOM = :prenom, "
                  "TELEPHONE = :telephone, "
                  "EMAIL = :email, "
                  "ADRESSE = :adresse, "
                  "DATE_NAISSANCE = TO_DATE(:date_naissance, 'YYYY-MM-DD') "
                  "WHERE CIN = :cin");
    
    query.bindValue(":cin", client.getCin());
    query.bindValue(":nom", client.getNom());
    query.bindValue(":prenom", client.getPrenom());
    query.bindValue(":telephone", client.getTelephone());
    query.bindValue(":email", client.getEmail());
    query.bindValue(":adresse", client.getAdresse());
    query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::deleteClient(const QString& cin)
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("DELETE FROM CLIENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

QList<Client> DatabaseManager::getAllClients()
{
    QList<Client> clients;
    
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return clients;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE FROM CLIENT ORDER BY NOM, PRENOM");
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return clients;
    }
    
    while (query.next()) {
        QDate dateNaissance;
        if (query.value(6).typeId() == QMetaType::QDate) {
            dateNaissance = query.value(6).toDate();
        } else {
            dateNaissance = QDate::fromString(query.value(6).toString(), "yyyy-MM-dd");
        }
        
        Client client(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            dateNaissance
        );
        clients.append(client);
    }
    
    return clients;
}

Client DatabaseManager::getClientByCin(const QString& cin)
{
    Client client;
    
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return client;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE FROM CLIENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return client;
    }
    
    if (query.next()) {
        QDate dateNaissance;
        if (query.value(6).typeId() == QMetaType::QDate) {
            dateNaissance = query.value(6).toDate();
        } else {
            dateNaissance = QDate::fromString(query.value(6).toString(), "yyyy-MM-dd");
        }
        
        client = Client(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            dateNaissance
        );
    }
    
    return client;
}

bool DatabaseManager::clientExists(const QString& cin) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM CLIENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    return query.value(0).toInt() > 0;
}

bool DatabaseManager::insertObjet(const ObjetElectronique& objet)
{
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            if (!m_connection.createConnection()) {
                m_lastError = "Impossible de se connecter à la base de données: " + db.lastError().text();
                return false;
            }
            db = m_connection.getDatabase();
        }
        
        if (!db.isOpen()) {
            m_lastError = "La connexion à la base de données n'est pas ouverte";
            return false;
        }
        
        createSequenceIfNotExists();
        
        QSqlError lastError;
        QString lastQuery;
        
        QStringList tableSchemas;
        tableSchemas << "objets|reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix|:reference, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat, :technicien, :prix";
        tableSchemas << "OBJETS|reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix|:reference, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat, :technicien, :prix";
        tableSchemas << "OBJET_ELECTRONIQUE|ID_OBJET, NOM_OBJET, MARQUE, MODELE, COULEUR, NUM_SERIE, TYPE_OBJET, ETAT, ID_EMPLOYE_COURANT|SEQ_OBJET.NEXTVAL, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat, NULL";
        
        for (const QString& schema : tableSchemas) {
            QStringList parts = schema.split("|");
            if (parts.size() != 3) continue;
            
            QString tableName = parts[0];
            QString columns = parts[1];
            QString values = parts[2];
            
            QSqlQuery query(db);
            QString insertQuery = QString("INSERT INTO %1 (%2) VALUES (%3)")
                                  .arg(tableName)
                                  .arg(columns)
                                  .arg(values);
            
            if (!query.prepare(insertQuery)) {
                lastError = query.lastError();
                lastQuery = insertQuery;
                qDebug() << "Erreur de préparation avec table" << tableName << ":" << lastError.text();
                continue;
            }
            
            if (tableName.toUpper() == "OBJET_ELECTRONIQUE") {
                query.bindValue(":nom", objet.getNom());
                query.bindValue(":marque", objet.getMarque());
                query.bindValue(":modele", objet.getModele());
                query.bindValue(":couleur", objet.getCouleur());
                query.bindValue(":numero_serie", objet.getNumeroSerie());
                query.bindValue(":type", objet.getType());
                query.bindValue(":etat", objet.getEtat());
            } else {
                query.bindValue(":reference", objet.getReference());
                query.bindValue(":nom", objet.getNom());
                query.bindValue(":marque", objet.getMarque());
                query.bindValue(":modele", objet.getModele());
                query.bindValue(":couleur", objet.getCouleur());
                query.bindValue(":numero_serie", objet.getNumeroSerie());
                query.bindValue(":type", objet.getType());
                query.bindValue(":etat", objet.getEtat());
                query.bindValue(":technicien", objet.getTechnicien());
                query.bindValue(":prix", objet.getPrix());
            }
            
            if (query.exec()) {
                return true;
            } else {
                lastError = query.lastError();
                lastQuery = insertQuery;
                qDebug() << "Tentative avec table" << tableName << "échouée:" << lastError.text();
            }
        }
        
        m_lastError = lastError.text();
        qDebug() << "Erreur SQL insertObjet:" << m_lastError;
        qDebug() << "Dernière requête:" << lastQuery;
        return false;
    } catch (...) {
        m_lastError = "Exception non gérée lors de l'insertion";
        qDebug() << "Exception dans insertObjet";
        return false;
    }
}

bool DatabaseManager::updateObjet(const ObjetElectronique& objet)
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        if (!m_connection.createConnection()) {
            m_lastError = "Impossible de se connecter à la base de données: " + db.lastError().text();
            return false;
        }
        db = m_connection.getDatabase();
    }
    
    QString tableName = detectTableName();
    QStringList columns = getTableColumns(tableName);
    QString upperName = tableName.toUpper();
    
    QString updateQuery;
    if (upperName.contains("OBJET_ELECTRONIQUE")) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "%9 = :technicien "
                              "WHERE %10 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[8]).arg(columns[0]);
    } else {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "%9 = :technicien, "
                              "%10 = :prix "
                              "WHERE %11 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[8]).arg(columns[9])
                      .arg(columns[0]);
    }
    
    QSqlQuery query(db);
    if (!query.prepare(updateQuery)) {
        m_lastError = query.lastError().text();
        qDebug() << "Erreur de préparation updateObjet:" << m_lastError;
        return false;
    }
    
    query.bindValue(":reference", objet.getReference());
    query.bindValue(":nom", objet.getNom());
    query.bindValue(":marque", objet.getMarque());
    query.bindValue(":modele", objet.getModele());
    query.bindValue(":couleur", objet.getCouleur());
    query.bindValue(":numero_serie", objet.getNumeroSerie());
    query.bindValue(":type", objet.getType());
    query.bindValue(":etat", objet.getEtat());
    query.bindValue(":technicien", objet.getTechnicien());
    
    if (!upperName.contains("OBJET_ELECTRONIQUE")) {
        query.bindValue(":prix", objet.getPrix());
    }
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Erreur SQL updateObjet:" << m_lastError;
        qDebug() << "Requête:" << updateQuery;
        return false;
    }
    
    return true;
}

bool DatabaseManager::deleteObjet(const QString& reference)
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        if (!m_connection.createConnection()) {
            m_lastError = "Impossible de se connecter à la base de données: " + db.lastError().text();
            return false;
        }
        db = m_connection.getDatabase();
    }
    
    QString tableName = detectTableName();
    QStringList columns = getTableColumns(tableName);
    
    QString deleteQuery = QString("DELETE FROM %1 WHERE %2 = :reference")
                          .arg(tableName).arg(columns[0]);
    
    QSqlQuery query(db);
    if (!query.prepare(deleteQuery)) {
        m_lastError = query.lastError().text();
        qDebug() << "Erreur de préparation deleteObjet:" << m_lastError;
        return false;
    }
    
    query.bindValue(":reference", reference);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Erreur SQL deleteObjet:" << m_lastError;
        qDebug() << "Requête:" << deleteQuery;
        return false;
    }
    
    return true;
}

QString DatabaseManager::detectTableName() const
{
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            return "objets";
        }
        
        QStringList tableNames = {"objets", "OBJETS", "OBJET_ELECTRONIQUE", "\"objets\"", "\"OBJET_ELECTRONIQUE\""};
        
        for (const QString& name : tableNames) {
            QSqlQuery query(db);
            QString testSql = QString("SELECT COUNT(*) FROM %1").arg(name);
            if (query.exec(testSql)) {
                qDebug() << "Table detectee:" << name;
                return name;
            }
        }
        
        return "objets";
    } catch (...) {
        qDebug() << "Exception dans detectTableName";
        return "objets";
    }
}

QStringList DatabaseManager::getTableColumns(const QString& tableName) const
{
    QStringList columns;
    QString upperName = tableName.toUpper();
    
    if (upperName.contains("OBJET_ELECTRONIQUE")) {
        columns << "ID_OBJET" << "NOM_OBJET" << "MARQUE" << "MODELE" << "COULEUR" 
                << "NUM_SERIE" << "TYPE_OBJET" << "ETAT" << "ID_EMPLOYE_COURANT" << "ID_CLIENT_COURANT";
    } else {
        columns << "reference" << "nom" << "marque" << "modele" << "couleur" 
                << "numero_serie" << "type" << "etat" << "technicien" << "prix";
    }
    
    return columns;
}

QList<ObjetElectronique> DatabaseManager::getAllObjets()
{
    QList<ObjetElectronique> objets;
    
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            return objets;
        }
        
        QString tableName = detectTableName();
        QStringList columns = getTableColumns(tableName);
        if (columns.size() < 10) {
            qDebug() << "Erreur: colonnes insuffisantes" << columns.size();
            return objets;
        }
        
        QString upperName = tableName.toUpper();
        
        QString selectQuery;
        if (upperName.contains("OBJET_ELECTRONIQUE")) {
            selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8, "
                                  "NVL(TO_CHAR(%9), ''), NVL(TO_CHAR(%10), '0') "
                                  "FROM %11 ORDER BY %1")
                          .arg(columns[0]).arg(columns[1]).arg(columns[2]).arg(columns[3])
                          .arg(columns[4]).arg(columns[5]).arg(columns[6]).arg(columns[7])
                          .arg(columns[8]).arg(columns[9]).arg(tableName);
        } else {
            selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10 FROM %11 ORDER BY %1")
                          .arg(columns[0]).arg(columns[1]).arg(columns[2]).arg(columns[3])
                          .arg(columns[4]).arg(columns[5]).arg(columns[6]).arg(columns[7])
                          .arg(columns[8]).arg(columns[9]).arg(tableName);
        }
        
        QSqlQuery query(db);
        if (!query.exec(selectQuery)) {
            m_lastError = query.lastError().text();
            qDebug() << "Erreur getAllObjets:" << m_lastError;
            qDebug() << "Requete:" << selectQuery;
            return objets;
        }
        
        while (query.next()) {
            QString ref, nom, marque, modele, couleur, numSerie, type, etat, tech;
            int prix = 0;
            
            if (upperName.contains("OBJET_ELECTRONIQUE")) {
                ref = query.value(0).toString();
                nom = query.value(1).toString();
                marque = query.value(2).toString();
                modele = query.value(3).toString();
                couleur = query.value(4).toString();
                numSerie = query.value(5).toString();
                type = query.value(6).toString();
                etat = query.value(7).toString();
                tech = query.value(8).toString();
                QString prixStr = query.value(9).toString();
                prix = prixStr.toInt();
            } else {
                ref = query.value(0).toString();
                nom = query.value(1).toString();
                marque = query.value(2).toString();
                modele = query.value(3).toString();
                couleur = query.value(4).toString();
                numSerie = query.value(5).toString();
                type = query.value(6).toString();
                etat = query.value(7).toString();
                tech = query.value(8).toString();
                prix = query.value(9).toInt();
            }
            
            ObjetElectronique objet(ref, nom, marque, modele, couleur, numSerie, type, etat, tech, prix);
            objets.append(objet);
        }
        
        return objets;
    } catch (...) {
        qDebug() << "Exception dans getAllObjets";
        return objets;
    }
}

ObjetElectronique DatabaseManager::getObjetByReference(const QString& reference)
{
    ObjetElectronique objet;
    
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return objet;
    }
    
    QString tableName = detectTableName();
    QStringList columns = getTableColumns(tableName);
    QString upperName = tableName.toUpper();
    
    QString selectQuery;
    if (upperName.contains("OBJET_ELECTRONIQUE")) {
        selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8, "
                              "NVL(TO_CHAR(%9), ''), NVL(TO_CHAR(%10), '0') "
                              "FROM %11 WHERE %1 = :reference")
                      .arg(columns[0]).arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6]).arg(columns[7])
                      .arg(columns[8]).arg(columns[9]).arg(tableName);
    } else {
        selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10 FROM %11 WHERE %1 = :reference")
                      .arg(columns[0]).arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6]).arg(columns[7])
                      .arg(columns[8]).arg(columns[9]).arg(tableName);
    }
    
    QSqlQuery query(db);
    query.prepare(selectQuery);
    query.bindValue(":reference", reference);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return objet;
    }
    
    if (query.next()) {
        QString ref = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString marque = query.value(2).toString();
        QString modele = query.value(3).toString();
        QString couleur = query.value(4).toString();
        QString numSerie = query.value(5).toString();
        QString type = query.value(6).toString();
        QString etat = query.value(7).toString();
        QString tech = query.value(8).toString();
        int prix = upperName.contains("OBJET_ELECTRONIQUE") ? query.value(9).toString().toInt() : query.value(9).toInt();
        
        objet = ObjetElectronique(ref, nom, marque, modele, couleur, numSerie, type, etat, tech, prix);
    }
    
    return objet;
}

bool DatabaseManager::objetExists(const QString& reference) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QString tableName = detectTableName();
    QStringList columns = getTableColumns(tableName);
    
    QString selectQuery = QString("SELECT COUNT(*) FROM %1 WHERE %2 = :reference")
                          .arg(tableName).arg(columns[0]);
    
    QSqlQuery query(db);
    query.prepare(selectQuery);
    query.bindValue(":reference", reference);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    return query.value(0).toInt() > 0;
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}

