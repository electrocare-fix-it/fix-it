#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include "client.h"

class DatabaseManager
{
public:
    static DatabaseManager& getInstance();
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    bool createTables();
    bool tableExists(const QString& tableName) const;
    
    bool insertClient(const Client& client);
    bool updateClient(const Client& client);
    bool deleteClient(const QString& cin);
    QList<Client> getAllClients();
    Client getClientByCin(const QString& cin);
    bool clientExists(const QString& cin) const;
    
    QString getLastError() const;

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    QSqlDatabase m_db;
    QString m_lastError;
    
    QString getDatabasePath() const;
};

#endif // DATABASE_H

