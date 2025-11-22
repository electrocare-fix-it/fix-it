#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include "client.h"
#include "objetelectronique.h"
#include "connection.h"

class DatabaseManager
{
public:
    static DatabaseManager& getInstance();
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    bool createTables();
    bool tableExists(const QString& tableName) const;
    bool createSequenceIfNotExists();
    
    bool insertClient(const Client& client);
    bool updateClient(const Client& client);
    bool deleteClient(const QString& cin);
    QList<Client> getAllClients();
    Client getClientByCin(const QString& cin);
    Client getClientById(int idClient) const;
    bool clientExists(const QString& cin) const;
    int getClientIdByCin(const QString& cin) const;
    
    bool insertObjet(const ObjetElectronique& objet, int idClient = -1);
    bool updateObjet(const ObjetElectronique& objet, int idClient = -1);
    bool deleteObjet(const QString& reference);
    QList<ObjetElectronique> getAllObjets();
    ObjetElectronique getObjetByReference(const QString& reference);
    bool objetExists(const QString& reference) const;
    int getClientIdByObjetReference(const QString& reference) const;
    
    QString getLastError() const;

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    QString detectTableName() const;
    QStringList getTableColumns(const QString& tableName) const;
    
    Connection& m_connection;
    QString m_lastError;
};

#endif // DATABASE_H

