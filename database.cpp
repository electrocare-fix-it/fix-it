#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

QString DatabaseManager::getDatabasePath() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataPath)) {
        dir.mkpath(dataPath);
    }
    return dataPath + "/clients.db";
}

bool DatabaseManager::connect()
{
    if (m_db.isOpen()) {
        return true;
    }
    
    QString dbPath = getDatabasePath();
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }
    
    return createTables();
}

void DatabaseManager::disconnect()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::isConnected() const
{
    return m_db.isOpen();
}

bool DatabaseManager::tableExists(const QString& tableName) const
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=:tableName");
    query.bindValue(":tableName", tableName);
    
    if (!query.exec()) {
        return false;
    }
    
    return query.next();
}

bool DatabaseManager::createTables()
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS clients ("
                                "cin TEXT PRIMARY KEY NOT NULL,"
                                "nom TEXT NOT NULL,"
                                "prenom TEXT NOT NULL,"
                                "telephone TEXT NOT NULL,"
                                "email TEXT NOT NULL,"
                                "adresse TEXT NOT NULL,"
                                "date_naissance DATE NOT NULL"
                                ")";
    
    if (!query.exec(createTableQuery)) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    QString createObjetsTableQuery = "CREATE TABLE IF NOT EXISTS objets ("
                                     "reference TEXT PRIMARY KEY NOT NULL,"
                                     "nom TEXT NOT NULL,"
                                     "marque TEXT NOT NULL,"
                                     "modele TEXT,"
                                     "couleur TEXT,"
                                     "numero_serie TEXT,"
                                     "type TEXT NOT NULL,"
                                     "etat TEXT NOT NULL,"
                                     "technicien TEXT,"
                                     "prix INTEGER NOT NULL DEFAULT 0"
                                     ")";
    
    if (!query.exec(createObjetsTableQuery)) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::insertClient(const Client& client)
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO clients (cin, nom, prenom, telephone, email, adresse, date_naissance) "
                  "VALUES (:cin, :nom, :prenom, :telephone, :email, :adresse, :date_naissance)");
    
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

bool DatabaseManager::updateClient(const Client& client)
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("UPDATE clients SET "
                  "nom = :nom, "
                  "prenom = :prenom, "
                  "telephone = :telephone, "
                  "email = :email, "
                  "adresse = :adresse, "
                  "date_naissance = :date_naissance "
                  "WHERE cin = :cin");
    
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
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM clients WHERE cin = :cin");
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
    
    if (!m_db.isOpen()) {
        return clients;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT cin, nom, prenom, telephone, email, adresse, date_naissance FROM clients ORDER BY nom, prenom");
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return clients;
    }
    
    while (query.next()) {
        Client client(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            QDate::fromString(query.value(6).toString(), "yyyy-MM-dd")
        );
        clients.append(client);
    }
    
    return clients;
}

Client DatabaseManager::getClientByCin(const QString& cin)
{
    Client client;
    
    if (!m_db.isOpen()) {
        return client;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT cin, nom, prenom, telephone, email, adresse, date_naissance FROM clients WHERE cin = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return client;
    }
    
    if (query.next()) {
        client = Client(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            QDate::fromString(query.value(6).toString(), "yyyy-MM-dd")
        );
    }
    
    return client;
}

bool DatabaseManager::clientExists(const QString& cin) const
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM clients WHERE cin = :cin");
    query.bindValue(":cin", cin);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    
    return query.value(0).toInt() > 0;
}

bool DatabaseManager::insertObjet(const ObjetElectronique& objet)
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO objets (reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix) "
                  "VALUES (:reference, :nom, :marque, :modele, :couleur, :numero_serie, :type, :etat, :technicien, :prix)");
    
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
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::updateObjet(const ObjetElectronique& objet)
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("UPDATE objets SET "
                  "nom = :nom, "
                  "marque = :marque, "
                  "modele = :modele, "
                  "couleur = :couleur, "
                  "numero_serie = :numero_serie, "
                  "type = :type, "
                  "etat = :etat, "
                  "technicien = :technicien, "
                  "prix = :prix "
                  "WHERE reference = :reference");
    
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
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

bool DatabaseManager::deleteObjet(const QString& reference)
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM objets WHERE reference = :reference");
    query.bindValue(":reference", reference);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    
    return true;
}

QList<ObjetElectronique> DatabaseManager::getAllObjets()
{
    QList<ObjetElectronique> objets;
    
    if (!m_db.isOpen()) {
        return objets;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix FROM objets ORDER BY reference");
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return objets;
    }
    
    while (query.next()) {
        ObjetElectronique objet(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(6).toString(),
            query.value(7).toString(),
            query.value(8).toString(),
            query.value(9).toInt()
        );
        objets.append(objet);
    }
    
    return objets;
}

ObjetElectronique DatabaseManager::getObjetByReference(const QString& reference)
{
    ObjetElectronique objet;
    
    if (!m_db.isOpen()) {
        return objet;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix FROM objets WHERE reference = :reference");
    query.bindValue(":reference", reference);
    
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return objet;
    }
    
    if (query.next()) {
        objet = ObjetElectronique(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(6).toString(),
            query.value(7).toString(),
            query.value(8).toString(),
            query.value(9).toInt()
        );
    }
    
    return objet;
}

bool DatabaseManager::objetExists(const QString& reference) const
{
    if (!m_db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM objets WHERE reference = :reference");
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

