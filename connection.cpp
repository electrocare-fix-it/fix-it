#include "connection.h"

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection::~Connection()
{
    if (db.isOpen())
        db.close();
}

Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::createConnection()
{
    if (db.isOpen()) {
        return true;
    }
    
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << "Pilotes disponibles:" << drivers;
    
    QString directConnection = "Driver={Oracle in XE};Dbq=XE;Uid=yesmin;Pwd=esprit18;";
    
    db.setDatabaseName(directConnection);
    
    if (db.open()) {
        qDebug() << "Connexion etablie avec pilote Oracle";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        qDebug() << "Echec de la connexion directe !" << db.lastError().text();
        
        QString directConnection2 = "Driver={Oracle};Dbq=XE;Uid=yesmin;Pwd=esprit18;";
        db.setDatabaseName(directConnection2);
        
        if (db.open()) {
            qDebug() << "Connexion etablie avec pilote Oracle (nom court)";
            return true;
        } else {
            qDebug() << "Echec avec pilote Oracle (nom court) !" << db.lastError().text();
            return false;
        }
    }
}

QSqlDatabase& Connection::getDatabase()
{
    return db;
}

