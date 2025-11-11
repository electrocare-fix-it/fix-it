#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QString>

class Connection
{
public:
    Connection();
    bool createconnect();
    bool openOracleConnection(const QString &host, const QString &database, int port, 
                              const QString &username, const QString &password, const QString &sid);
    
    // Méthode statique pour obtenir une connexion Oracle partagée
    static QSqlDatabase getOracleConnection();
    static bool initializeOracleConnection(const QString &host = "localhost", 
                                           const QString &sid = "XE",
                                           const QString &username = "jacem", 
                                           const QString &password = "esprit18",
                                           int port = 1521);
};

#endif // CONNECTION_H

