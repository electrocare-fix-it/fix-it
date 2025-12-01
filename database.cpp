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
    
    QString insertQuery;
    if (client.getDateNaissance().isValid()) {
        insertQuery = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                      "VALUES (SEQ_CLIENT.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, TO_DATE(:date_naissance, 'YYYY-MM-DD'))";
    } else {
        insertQuery = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                      "VALUES (SEQ_CLIENT.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, NULL)";
    }
    
    query.prepare(insertQuery);
    
    query.bindValue(":cin", client.getCin());
    query.bindValue(":nom", client.getNom());
    query.bindValue(":prenom", client.getPrenom());
    query.bindValue(":telephone", client.getTelephone());
    query.bindValue(":email", client.getEmail());
    query.bindValue(":adresse", client.getAdresse());
    
    if (client.getDateNaissance().isValid()) {
        query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
    }
    
    if (!query.exec()) {
        QString seqError = query.lastError().text();
        qDebug() << "Erreur initiale:" << seqError;
        
        if (seqError.contains("SEQ_CLIENT", Qt::CaseInsensitive) || 
            seqError.contains("sequence", Qt::CaseInsensitive) ||
            seqError.contains("ORA-02289", Qt::CaseInsensitive)) {
            
            qDebug() << "Sequence SEQ_CLIENT non trouvee, essai avec SEQ_CLIENT_ID...";
            
            QString insertQuery2;
            if (client.getDateNaissance().isValid()) {
                insertQuery2 = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                              "VALUES (SEQ_CLIENT_ID.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, TO_DATE(:date_naissance, 'YYYY-MM-DD'))";
            } else {
                insertQuery2 = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                              "VALUES (SEQ_CLIENT_ID.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, NULL)";
            }
            
            query.prepare(insertQuery2);
            query.bindValue(":cin", client.getCin());
            query.bindValue(":nom", client.getNom());
            query.bindValue(":prenom", client.getPrenom());
            query.bindValue(":telephone", client.getTelephone());
            query.bindValue(":email", client.getEmail());
            query.bindValue(":adresse", client.getAdresse());
            if (client.getDateNaissance().isValid()) {
                query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
            }
            
            if (!query.exec()) {
                qDebug() << "Sequence SEQ_CLIENT_ID non trouvee, essai avec CLIENT_SEQ...";
                
                QString insertQuery3;
                if (client.getDateNaissance().isValid()) {
                    insertQuery3 = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                                  "VALUES (CLIENT_SEQ.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, TO_DATE(:date_naissance, 'YYYY-MM-DD'))";
                } else {
                    insertQuery3 = "INSERT INTO CLIENT (ID_CLIENT, CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, DATE_NAISSANCE) "
                                  "VALUES (CLIENT_SEQ.NEXTVAL, :cin, :nom, :prenom, :telephone, :email, :adresse, NULL)";
                }
                
                query.prepare(insertQuery3);
                query.bindValue(":cin", client.getCin());
                query.bindValue(":nom", client.getNom());
                query.bindValue(":prenom", client.getPrenom());
                query.bindValue(":telephone", client.getTelephone());
                query.bindValue(":email", client.getEmail());
                query.bindValue(":adresse", client.getAdresse());
                if (client.getDateNaissance().isValid()) {
                    query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
                }
                
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
    QString updateQuery;
    if (client.getDateNaissance().isValid()) {
        updateQuery = "UPDATE CLIENT SET "
                      "NOM = :nom, "
                      "PRENOM = :prenom, "
                      "TELEPHONE = :telephone, "
                      "EMAIL = :email, "
                      "ADRESSE = :adresse, "
                      "DATE_NAISSANCE = TO_DATE(:date_naissance, 'YYYY-MM-DD') "
                      "WHERE CIN = :cin";
    } else {
        updateQuery = "UPDATE CLIENT SET "
                      "NOM = :nom, "
                      "PRENOM = :prenom, "
                      "TELEPHONE = :telephone, "
                      "EMAIL = :email, "
                      "ADRESSE = :adresse, "
                      "DATE_NAISSANCE = NULL "
                      "WHERE CIN = :cin";
    }
    
    query.prepare(updateQuery);
    
    query.bindValue(":cin", client.getCin());
    query.bindValue(":nom", client.getNom());
    query.bindValue(":prenom", client.getPrenom());
    query.bindValue(":telephone", client.getTelephone());
    query.bindValue(":email", client.getEmail());
    query.bindValue(":adresse", client.getAdresse());
    
    if (client.getDateNaissance().isValid()) {
        query.bindValue(":date_naissance", client.getDateNaissance().toString("yyyy-MM-dd"));
    }
    
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
    query.prepare("SELECT NVL(CIN, ''), NVL(NOM, ''), NVL(PRENOM, ''), NVL(TELEPHONE, ''), NVL(EMAIL, ''), NVL(ADRESSE, ''), NVL(TO_CHAR(DATE_NAISSANCE, 'DD-MM-YY'), '') FROM CLIENT ORDER BY NOM, PRENOM");
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return clients;
    }
    
    while (query.next()) {
        QDate dateNaissance;
        QVariant dateValue = query.value(6);
        if (!dateValue.isNull() && dateValue.isValid()) {
            if (dateValue.typeId() == QMetaType::QDate) {
                dateNaissance = dateValue.toDate();
            } else {
                QString dateStr = dateValue.toString().trimmed();
                if (!dateStr.isEmpty()) {
                    dateNaissance = QDate::fromString(dateStr, "dd-MM-yyyy");
                    if (!dateNaissance.isValid()) {
                        dateNaissance = QDate::fromString(dateStr, "dd/MM/yyyy");
                    }
                    if (!dateNaissance.isValid()) {
                        dateNaissance = QDate::fromString(dateStr, "yyyy-MM-dd");
                    }
                    if (!dateNaissance.isValid()) {
                        QStringList parts = dateStr.split("-");
                        if (parts.size() == 3) {
                            bool ok;
                            int day = parts[0].toInt(&ok);
                            if (ok) {
                                int month = parts[1].toInt(&ok);
                                if (ok) {
                                    int year = parts[2].toInt(&ok);
                                    if (ok && year < 100) {
                                        year += 2000;
                                        dateNaissance = QDate(year, month, day);
                                    }
                                }
                            }
                        }
                    }
                }
            }
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
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, NVL(TO_CHAR(DATE_NAISSANCE, 'DD-MM-YY'), '') FROM CLIENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return client;
    }
    
    if (query.next()) {
        QDate dateNaissance;
        QVariant dateValue = query.value(6);
        if (!dateValue.isNull() && dateValue.isValid()) {
            if (dateValue.typeId() == QMetaType::QDate) {
                dateNaissance = dateValue.toDate();
            } else {
                QString dateStr = dateValue.toString().trimmed();
                if (!dateStr.isEmpty()) {
                    dateNaissance = QDate::fromString(dateStr, "dd-MM-yyyy");
                    if (!dateNaissance.isValid()) {
                        dateNaissance = QDate::fromString(dateStr, "dd/MM/yyyy");
                    }
                    if (!dateNaissance.isValid()) {
                        dateNaissance = QDate::fromString(dateStr, "yyyy-MM-dd");
                    }
                    if (!dateNaissance.isValid()) {
                        QStringList parts = dateStr.split("-");
                        if (parts.size() == 3) {
                            bool ok;
                            int day = parts[0].toInt(&ok);
                            if (ok) {
                                int month = parts[1].toInt(&ok);
                                if (ok) {
                                    int year = parts[2].toInt(&ok);
                                    if (ok && year < 100) {
                                        year += 2000;
                                        dateNaissance = QDate(year, month, day);
                                    }
                                }
                            }
                        }
                    }
                }
            }
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

Client DatabaseManager::getClientById(int idClient) const
{
    Client client;
    
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen() || idClient <= 0) {
        return client;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT CIN, NOM, PRENOM, TELEPHONE, EMAIL, ADRESSE, NVL(TO_CHAR(DATE_NAISSANCE, 'DD-MM-YY'), '') FROM CLIENT WHERE ID_CLIENT = :idClient");
    query.bindValue(":idClient", idClient);
    
    if (!query.exec()) {
        return client;
    }
    
    if (query.next()) {
        QDate dateNaissance;
        QVariant dateValue = query.value(6);
        if (!dateValue.isNull() && dateValue.isValid()) {
            if (dateValue.typeId() == QMetaType::QDate) {
                dateNaissance = dateValue.toDate();
            } else {
                QString dateStr = dateValue.toString().trimmed();
                if (!dateStr.isEmpty()) {
                    dateNaissance = QDate::fromString(dateStr, "dd-MM-yyyy");
                    if (!dateNaissance.isValid()) {
                        dateNaissance = QDate::fromString(dateStr, "dd/MM/yyyy");
                    }
                    if (!dateNaissance.isValid()) {
                        dateNaissance = QDate::fromString(dateStr, "yyyy-MM-dd");
                    }
                    if (!dateNaissance.isValid()) {
                        QStringList parts = dateStr.split("-");
                        if (parts.size() == 3) {
                            bool ok;
                            int day = parts[0].toInt(&ok);
                            if (ok) {
                                int month = parts[1].toInt(&ok);
                                if (ok) {
                                    int year = parts[2].toInt(&ok);
                                    if (ok && year < 100) {
                                        year += 2000;
                                        dateNaissance = QDate(year, month, day);
                                    }
                                }
                            }
                        }
                    }
                }
            }
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

int DatabaseManager::getClientIdByCin(const QString& cin) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return -1;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec() || !query.next()) {
        return -1;
    }
    
    return query.value(0).toInt();
}

int DatabaseManager::getClientIdByObjetReference(const QString& reference) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return -1;
    }
    
    QString tableName = detectTableName();
    QString upperName = tableName.toUpper();
    
    QSqlQuery query(db);
    QString selectQuery;
    
    if (upperName.contains("OBJET_ELECTRONIQUE")) {
        QStringList columns = getTableColumns(tableName);
        if (columns.isEmpty()) {
            return -1;
        }
        QString refColumn = columns[0];
        bool isNumeric = false;
        int refAsInt = reference.toInt(&isNumeric);
        
        if (isNumeric && refColumn.toUpper() == "ID_OBJET") {
            selectQuery = QString("SELECT ID_CLIENT_COURANT FROM %1 WHERE ID_OBJET = :reference").arg(tableName);
            query.prepare(selectQuery);
            query.bindValue(":reference", refAsInt);
        } else {
            selectQuery = QString("SELECT ID_CLIENT_COURANT FROM %1 WHERE %2 = :reference").arg(tableName).arg(refColumn);
            query.prepare(selectQuery);
            query.bindValue(":reference", reference);
        }
    } else {
        QStringList columns = getTableColumns(tableName);
        if (columns.isEmpty()) {
            return -1;
        }
        selectQuery = QString("SELECT ID_CLIENT_COURANT FROM %1 WHERE %2 = :reference").arg(tableName).arg(columns[0]);
        query.prepare(selectQuery);
        query.bindValue(":reference", reference);
    }
    
    if (!query.exec()) {
        qDebug() << "Erreur dans getClientIdByObjetReference:" << query.lastError().text();
        qDebug() << "Requête:" << selectQuery;
        qDebug() << "Référence:" << reference;
        return -1;
    }
    
    if (!query.next()) {
        qDebug() << "Aucun résultat trouvé pour la référence:" << reference;
        return -1;
    }
    
    QVariant value = query.value(0);
    if (value.isNull() || !value.isValid()) {
        qDebug() << "ID_CLIENT_COURANT est NULL pour la référence:" << reference;
        return -1;
    }
    
    int idClient = value.toInt();
    qDebug() << "ID_CLIENT_COURANT trouvé:" << idClient << "pour la référence:" << reference;
    return idClient;
}

int DatabaseManager::getIdPieceByObjetReference(const QString& reference) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return -1;
    }
    
    QString tableName = detectTableName();
    QString upperName = tableName.toUpper();
    
    if (!upperName.contains("OBJET_ELECTRONIQUE")) {
        return -1;
    }
    
    QSqlQuery checkCol(db);
    QString checkColQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND UPPER(column_name) = 'ID_PIECE'");
    checkCol.prepare(checkColQuery);
    checkCol.bindValue(":tableName", tableName);
    if (!checkCol.exec() || !checkCol.next() || checkCol.value(0).toInt() == 0) {
        return -1;
    }
    
    QStringList columns = getTableColumns(tableName);
    QSqlQuery query(db);
    QString selectQuery = QString("SELECT ID_PIECE FROM %1 WHERE %2 = :reference")
                          .arg(tableName).arg(columns[0]);
    
    query.prepare(selectQuery);
    query.bindValue(":reference", reference);
    
    if (!query.exec() || !query.next()) {
        return -1;
    }
    
    QVariant value = query.value(0);
    if (value.isNull() || !value.isValid()) {
        return -1;
    }
    
    return value.toInt();
}

QString DatabaseManager::getEtatObjetByReference(const QString& reference, int idClient) const
{
    QSqlDatabase& db = m_connection.getDatabase();
    if (!db.isOpen()) {
        return QString();
    }
    
    if (!tableExists("ETAT_OBJET")) {
        return QString();
    }
    
    QString tableName = detectTableName();
    QStringList columns = getTableColumns(tableName);
    QString upperName = tableName.toUpper();
    
    QSqlQuery getIdQuery(db);
    QString getIdQueryStr;
    bool isNumeric = false;
    int refAsInt = reference.toInt(&isNumeric);
    
    if (upperName.contains("OBJET_ELECTRONIQUE") && isNumeric) {
        getIdQueryStr = QString("SELECT ID_OBJET FROM %1 WHERE ID_OBJET = :reference")
                        .arg(tableName);
        getIdQuery.prepare(getIdQueryStr);
        getIdQuery.bindValue(":reference", refAsInt);
    } else {
        getIdQueryStr = QString("SELECT ID_OBJET FROM %1 WHERE %2 = :reference")
                        .arg(tableName).arg(columns[0]);
        getIdQuery.prepare(getIdQueryStr);
        getIdQuery.bindValue(":reference", reference);
    }
    
    if (!getIdQuery.exec() || !getIdQuery.next()) {
        return QString();
    }
    
    int idObjet = getIdQuery.value(0).toInt();
    if (idObjet <= 0) {
        return QString();
    }
    
    QSqlQuery query(db);
    QString selectQuery;
    
    if (idClient > 0) {
        selectQuery = "SELECT ETAT FROM ETAT_OBJET WHERE ID_OBJET = :id_objet AND ID_CLIENT = :id_client ORDER BY DATE_MODIFICATION DESC";
        query.prepare(selectQuery);
        query.bindValue(":id_objet", idObjet);
        query.bindValue(":id_client", idClient);
    } else {
        selectQuery = "SELECT ETAT FROM ETAT_OBJET WHERE ID_OBJET = :id_objet ORDER BY DATE_MODIFICATION DESC";
        query.prepare(selectQuery);
        query.bindValue(":id_objet", idObjet);
    }
    
    if (!query.exec() || !query.next()) {
        return QString();
    }
    
    return query.value(0).toString();
}

bool DatabaseManager::insertObjet(const ObjetElectronique& objet, int idClient, int idPiece)
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
        
        QString tableName = detectTableName();
        QString upperName = tableName.toUpper();
        
        qDebug() << "Table détectée:" << tableName;
        qDebug() << "Technicien à insérer:" << objet.getTechnicien();
        qDebug() << "Prix à insérer:" << objet.getPrix();
        
        QSqlQuery checkCols(db);
        QStringList columnsToCheck;
        if (upperName.contains("OBJET_ELECTRONIQUE")) {
            columnsToCheck = {"TECHNICIEN", "PRIX"};
        } else {
            columnsToCheck = {"technicien", "prix"};
        }
        
        for (const QString& col : columnsToCheck) {
            QString checkQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND UPPER(column_name) = UPPER(:colName)");
            checkCols.prepare(checkQuery);
            checkCols.bindValue(":tableName", tableName);
            checkCols.bindValue(":colName", col);
            if (checkCols.exec() && checkCols.next() && checkCols.value(0).toInt() == 0) {
                QString alterQuery;
                QString colLower = col.toLower();
                if (colLower == "technicien") {
                    if (upperName.contains("OBJET_ELECTRONIQUE")) {
                        alterQuery = QString("ALTER TABLE %1 ADD TECHNICIEN VARCHAR2(100)").arg(tableName);
                    } else {
                        alterQuery = QString("ALTER TABLE %1 ADD %2 VARCHAR2(100)").arg(tableName).arg(col);
                    }
                } else if (colLower == "prix") {
                    if (upperName.contains("OBJET_ELECTRONIQUE")) {
                        alterQuery = QString("ALTER TABLE %1 ADD PRIX NUMBER(10,2) DEFAULT 0").arg(tableName);
                    } else {
                        alterQuery = QString("ALTER TABLE %1 ADD %2 NUMBER(10,2) DEFAULT 0").arg(tableName).arg(col);
                    }
                }
                if (!alterQuery.isEmpty()) {
                    QSqlQuery alterQueryExec(db);
                    if (alterQueryExec.exec(alterQuery)) {
                        qDebug() << "Colonne" << col << "ajoutée à la table" << tableName;
                    } else {
                        qDebug() << "Erreur lors de l'ajout de la colonne" << col << ":" << alterQueryExec.lastError().text();
                    }
                }
            } else {
                qDebug() << "Colonne" << col << "existe déjà dans" << tableName;
            }
        }
        
        QSqlError lastError;
        QString lastQuery;
        
        bool hasTechnicienPrix = false;
        if (upperName.contains("OBJET_ELECTRONIQUE")) {
            QSqlQuery checkTechPrix(db);
            QString checkColsQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND (UPPER(column_name) = 'TECHNICIEN' OR UPPER(column_name) = 'PRIX')");
            checkTechPrix.prepare(checkColsQuery);
            checkTechPrix.bindValue(":tableName", tableName);
            if (checkTechPrix.exec() && checkTechPrix.next()) {
                int count = checkTechPrix.value(0).toInt();
                hasTechnicienPrix = (count >= 2);
            }
        }
        
        QStringList tableSchemas;
        tableSchemas << "objets|reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix|:reference, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat, :technicien, :prix";
        tableSchemas << "OBJETS|reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix|:reference, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat, :technicien, :prix";
        
        bool hasIdClient = false;
        bool hasIdPiece = false;
        if (upperName.contains("OBJET_ELECTRONIQUE")) {
            QSqlQuery checkIdClient(db);
            QString checkColsQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND UPPER(column_name) = 'ID_CLIENT_COURANT'");
            checkIdClient.prepare(checkColsQuery);
            checkIdClient.bindValue(":tableName", tableName);
            if (checkIdClient.exec() && checkIdClient.next()) {
                hasIdClient = (checkIdClient.value(0).toInt() > 0);
            }
            
            QSqlQuery checkIdPiece(db);
            QString checkPieceQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND UPPER(column_name) = 'ID_PIECE'");
            checkIdPiece.prepare(checkPieceQuery);
            checkIdPiece.bindValue(":tableName", tableName);
            if (checkIdPiece.exec() && checkIdPiece.next()) {
                hasIdPiece = (checkIdPiece.value(0).toInt() > 0);
            }
        }
        
        QString columnsList;
        QString valuesList;
        
        columnsList = "ID_OBJET, NOM_OBJET, MARQUE, MODELE, COULEUR, NUM_SERIE, TYPE_OBJET, ETAT";
        valuesList = "SEQ_OBJET.NEXTVAL, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat";
        
        if (hasTechnicienPrix) {
            columnsList += ", TECHNICIEN, PRIX";
            valuesList += ", :technicien, :prix";
        }
        
        if (hasIdClient) {
            columnsList += ", ID_CLIENT_COURANT";
            if (idClient > 0) {
                valuesList += ", :id_client";
            } else {
                valuesList += ", NULL";
            }
        }
        
        if (hasIdPiece) {
            columnsList += ", ID_PIECE";
            if (idPiece > 0) {
                valuesList += ", :id_piece";
            } else {
                valuesList += ", NULL";
            }
        }
        
        if (upperName.contains("OBJET_ELECTRONIQUE")) {
            tableSchemas << QString("OBJET_ELECTRONIQUE|%1|%2").arg(columnsList).arg(valuesList);
        }
        
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
            
            QString technicien = objet.getTechnicien();
            int prix = objet.getPrix();
            
            if (tableName.toUpper() == "OBJET_ELECTRONIQUE") {
                query.bindValue(":nom", objet.getNom());
                query.bindValue(":marque", objet.getMarque());
                query.bindValue(":modele", objet.getModele());
                query.bindValue(":couleur", objet.getCouleur());
                query.bindValue(":numero_serie", objet.getNumeroSerie());
                query.bindValue(":type", objet.getType());
                query.bindValue(":etat", objet.getEtat());
                if (hasTechnicienPrix) {
                    query.bindValue(":technicien", technicien.isEmpty() ? QString() : technicien);
                    query.bindValue(":prix", prix);
                }
                if (hasIdClient && idClient > 0) {
                    query.bindValue(":id_client", idClient);
                }
                if (hasIdPiece && idPiece > 0) {
                    query.bindValue(":id_piece", idPiece);
                }
            } else {
                query.bindValue(":reference", objet.getReference());
                query.bindValue(":nom", objet.getNom());
                query.bindValue(":marque", objet.getMarque());
                query.bindValue(":modele", objet.getModele());
                query.bindValue(":couleur", objet.getCouleur());
                query.bindValue(":numero_serie", objet.getNumeroSerie());
                query.bindValue(":type", objet.getType());
                query.bindValue(":etat", objet.getEtat());
                query.bindValue(":technicien", technicien.isEmpty() ? QString() : technicien);
                query.bindValue(":prix", prix);
                qDebug() << "Binding technicien:" << technicien << "prix:" << prix;
            }
            
            if (query.exec()) {
                qDebug() << "Insertion réussie dans" << tableName;
                qDebug() << "Technicien inséré:" << technicien << "Prix inséré:" << prix;
                return true;
            } else {
                lastError = query.lastError();
                lastQuery = insertQuery;
                qDebug() << "Tentative avec table" << tableName << "échouée:" << lastError.text();
                qDebug() << "Requête:" << insertQuery;
                qDebug() << "Valeurs bindées - technicien:" << technicien << "prix:" << prix;
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

bool DatabaseManager::updateObjet(const ObjetElectronique& objet, int idClient, int idPiece)
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
    
    bool hasTechnicienPrix = false;
    bool hasIdClient = false;
    bool hasIdPiece = false;
    if (upperName.contains("OBJET_ELECTRONIQUE")) {
        QSqlQuery checkTechPrix(db);
        QString checkColsQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND (UPPER(column_name) = 'TECHNICIEN' OR UPPER(column_name) = 'PRIX')");
        checkTechPrix.prepare(checkColsQuery);
        checkTechPrix.bindValue(":tableName", tableName);
        if (checkTechPrix.exec() && checkTechPrix.next()) {
            int count = checkTechPrix.value(0).toInt();
            hasTechnicienPrix = (count >= 2);
        }
        
        QSqlQuery checkIdClient(db);
        QString checkIdClientQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND UPPER(column_name) = 'ID_CLIENT_COURANT'");
        checkIdClient.prepare(checkIdClientQuery);
        checkIdClient.bindValue(":tableName", tableName);
        if (checkIdClient.exec() && checkIdClient.next()) {
            hasIdClient = (checkIdClient.value(0).toInt() > 0);
        }
        
        QSqlQuery checkIdPiece(db);
        QString checkIdPieceQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND UPPER(column_name) = 'ID_PIECE'");
        checkIdPiece.prepare(checkIdPieceQuery);
        checkIdPiece.bindValue(":tableName", tableName);
        if (checkIdPiece.exec() && checkIdPiece.next()) {
            hasIdPiece = (checkIdPiece.value(0).toInt() > 0);
        }
    }
    
    QString updateQuery;
    if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix && hasIdClient && hasIdPiece) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "TECHNICIEN = :technicien, "
                              "PRIX = :prix, "
                              "ID_CLIENT_COURANT = :id_client, "
                              "ID_PIECE = :id_piece "
                              "WHERE %9 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[0]);
    } else if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix && hasIdClient) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "TECHNICIEN = :technicien, "
                              "PRIX = :prix, "
                              "ID_CLIENT_COURANT = :id_client "
                              "WHERE %9 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[0]);
    } else if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix && hasIdPiece) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "TECHNICIEN = :technicien, "
                              "PRIX = :prix, "
                              "ID_PIECE = :id_piece "
                              "WHERE %9 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[0]);
    } else if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "TECHNICIEN = :technicien, "
                              "PRIX = :prix "
                              "WHERE %9 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[0]);
    } else if (upperName.contains("OBJET_ELECTRONIQUE") && hasIdClient) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat, "
                              "ID_CLIENT_COURANT = :id_client "
                              "WHERE %9 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[0]);
    } else if (upperName.contains("OBJET_ELECTRONIQUE")) {
        updateQuery = QString("UPDATE %1 SET "
                              "%2 = :nom, "
                              "%3 = :marque, "
                              "%4 = :modele, "
                              "%5 = :couleur, "
                              "%6 = :numero_serie, "
                              "%7 = :type, "
                              "%8 = :etat "
                              "WHERE %9 = :reference")
                      .arg(tableName)
                      .arg(columns[1]).arg(columns[2]).arg(columns[3])
                      .arg(columns[4]).arg(columns[5]).arg(columns[6])
                      .arg(columns[7]).arg(columns[0]);
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
    
    QString ref = objet.getReference();
    bool isNumeric = false;
    int refAsInt = ref.toInt(&isNumeric);
    
    if (upperName.contains("OBJET_ELECTRONIQUE") && isNumeric) {
        query.bindValue(":reference", refAsInt);
    } else {
        query.bindValue(":reference", ref);
    }
    
    query.bindValue(":nom", objet.getNom());
    query.bindValue(":marque", objet.getMarque());
    query.bindValue(":modele", objet.getModele());
    query.bindValue(":couleur", objet.getCouleur());
    query.bindValue(":numero_serie", objet.getNumeroSerie());
    query.bindValue(":type", objet.getType());
    query.bindValue(":etat", objet.getEtat());
    
    if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix) {
        query.bindValue(":technicien", objet.getTechnicien());
        int prix = objet.getPrix();
        query.bindValue(":prix", prix);
    } else if (!upperName.contains("OBJET_ELECTRONIQUE")) {
        query.bindValue(":technicien", objet.getTechnicien());
        int prix = objet.getPrix();
        query.bindValue(":prix", prix);
    }
    
    if (upperName.contains("OBJET_ELECTRONIQUE") && hasIdClient) {
        if (idClient > 0) {
            query.bindValue(":id_client", idClient);
        } else {
            query.bindValue(":id_client", QVariant());
        }
    }
    
    if (upperName.contains("OBJET_ELECTRONIQUE") && hasIdPiece) {
        if (idPiece > 0) {
            query.bindValue(":id_piece", idPiece);
        } else {
            query.bindValue(":id_piece", QVariant());
        }
    }
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Erreur SQL updateObjet:" << m_lastError;
        qDebug() << "Requête:" << updateQuery;
        qDebug() << "Valeurs - Reference:" << ref << "Prix:" << objet.getPrix() << "ID Client:" << idClient;
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
        
        bool hasTechnicienPrix = false;
        if (upperName.contains("OBJET_ELECTRONIQUE")) {
            QSqlQuery checkTechPrix(db);
            QString checkColsQuery = QString("SELECT COUNT(*) FROM user_tab_columns WHERE UPPER(table_name) = UPPER(:tableName) AND (UPPER(column_name) = 'TECHNICIEN' OR UPPER(column_name) = 'PRIX')");
            checkTechPrix.prepare(checkColsQuery);
            checkTechPrix.bindValue(":tableName", tableName);
            if (checkTechPrix.exec() && checkTechPrix.next()) {
                int count = checkTechPrix.value(0).toInt();
                hasTechnicienPrix = (count >= 2);
            }
        }
        
        QString selectQuery;
        if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix) {
            selectQuery = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8, "
                                  "NVL(TECHNICIEN, ''), NVL(TO_CHAR(PRIX), '0') "
                                  "FROM %9 ORDER BY %1")
                          .arg(columns[0]).arg(columns[1]).arg(columns[2]).arg(columns[3])
                          .arg(columns[4]).arg(columns[5]).arg(columns[6]).arg(columns[7])
                          .arg(tableName);
        } else if (upperName.contains("OBJET_ELECTRONIQUE")) {
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
            
            if (upperName.contains("OBJET_ELECTRONIQUE") && hasTechnicienPrix) {
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
            } else if (upperName.contains("OBJET_ELECTRONIQUE")) {
                ref = query.value(0).toString();
                nom = query.value(1).toString();
                marque = query.value(2).toString();
                modele = query.value(3).toString();
                couleur = query.value(4).toString();
                numSerie = query.value(5).toString();
                type = query.value(6).toString();
                etat = query.value(7).toString();
                QString techStr = query.value(8).toString();
                tech = techStr;
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

QList<DatabaseManager::PieceDetachee> DatabaseManager::getAllPiecesDetachees()
{
    QList<PieceDetachee> pieces;
    
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            qDebug() << "getAllPiecesDetachees: Base de données non ouverte, tentative de connexion...";
            if (!m_connection.createConnection()) {
                qDebug() << "getAllPiecesDetachees: Impossible de se connecter";
                return pieces;
            }
            db = m_connection.getDatabase();
        }
        
        QSqlQuery checkTable(db);
        QString checkQuery = "SELECT COUNT(*) FROM user_tables WHERE UPPER(table_name) = 'PIECE_DETACHEE'";
        if (checkTable.exec(checkQuery) && checkTable.next()) {
            int tableExists = checkTable.value(0).toInt();
            qDebug() << "getAllPiecesDetachees: Table PIECE_DETACHEE existe:" << (tableExists > 0);
            if (tableExists == 0) {
                qDebug() << "getAllPiecesDetachees: La table PIECE_DETACHEE n'existe pas!";
                m_lastError = "La table PIECE_DETACHEE n'existe pas dans la base de données";
                return pieces;
            }
        }
        
        QSqlQuery query(db);
        QString selectQuery = "SELECT ID_PIECE, NOM_PIECE, CATEGORIE, PRIX_UNITAIRE FROM PIECE_DETACHEE ORDER BY NOM_PIECE";
        
        qDebug() << "getAllPiecesDetachees: Exécution de la requête:" << selectQuery;
        
        if (!query.exec(selectQuery)) {
            m_lastError = query.lastError().text();
            qDebug() << "getAllPiecesDetachees: Erreur SQL:" << m_lastError;
            qDebug() << "getAllPiecesDetachees: Détails de l'erreur:" << query.lastError().databaseText();
            return pieces;
        }
        
        int count = 0;
        while (query.next()) {
            PieceDetachee piece;
            piece.id = query.value(0).toInt();
            piece.nom = query.value(1).toString();
            piece.categorie = query.value(2).toString();
            piece.prix = query.value(3).toDouble();
            
            if (piece.id > 0 && !piece.nom.isEmpty()) {
                pieces.append(piece);
                count++;
                qDebug() << "getAllPiecesDetachees: Pièce trouvée - ID:" << piece.id << "Nom:" << piece.nom << "Prix:" << piece.prix;
            } else {
                qDebug() << "getAllPiecesDetachees: Pièce invalide ignorée - ID:" << piece.id << "Nom:" << piece.nom;
            }
        }
        
        qDebug() << "getAllPiecesDetachees: Total de" << count << "pièces détachées chargées";
        
        if (count == 0) {
            qDebug() << "getAllPiecesDetachees: ATTENTION - Aucune pièce détachée trouvée dans la table!";
        }
    } catch (const std::exception& e) {
        m_lastError = QString("Exception lors de la récupération des pièces détachées: %1").arg(e.what());
        qDebug() << "getAllPiecesDetachees: Exception capturée:" << e.what();
    } catch (...) {
        m_lastError = "Exception inconnue lors de la récupération des pièces détachées";
        qDebug() << "getAllPiecesDetachees: Exception inconnue capturée";
    }
    
    return pieces;
}

DatabaseManager::PieceDetachee DatabaseManager::getPieceDetacheeById(int id)
{
    PieceDetachee piece;
    piece.id = -1;
    
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            return piece;
        }
        
        QSqlQuery query(db);
        query.prepare("SELECT ID_PIECE, NOM_PIECE, CATEGORIE, PRIX_UNITAIRE FROM PIECE_DETACHEE WHERE ID_PIECE = :id");
        query.bindValue(":id", id);
        
        if (query.exec() && query.next()) {
            piece.id = query.value(0).toInt();
            piece.nom = query.value(1).toString();
            piece.categorie = query.value(2).toString();
            piece.prix = query.value(3).toDouble();
        }
    } catch (...) {
        m_lastError = "Exception lors de la récupération de la pièce détachée";
    }
    
    return piece;
}

double DatabaseManager::getPrixPieceDetachee(int idPiece)
{
    try {
        QSqlDatabase& db = m_connection.getDatabase();
        if (!db.isOpen()) {
            return 0.0;
        }
        
        QSqlQuery query(db);
        query.prepare("SELECT PRIX_UNITAIRE FROM PIECE_DETACHEE WHERE ID_PIECE = :id");
        query.bindValue(":id", idPiece);
        
        if (query.exec() && query.next()) {
            return query.value(0).toDouble();
        }
    } catch (...) {
        m_lastError = "Exception lors de la récupération du prix";
    }
    
    return 0.0;
}

QString DatabaseManager::getLastError() const
{
    return m_lastError;
}

