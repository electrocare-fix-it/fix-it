#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Driver={Oracle in XE};Dbq=Source_Projet2A;Uid=hiba;Pwd=0000;");


    if (!db.open()) {
        qDebug() << "Erreur de connexion :" << db.lastError().text();
        return false;
    }

    qDebug() << "Connexion réussie !";
    return true;
}

bool Connection::openOracleConnection(const QString &host, const QString &database, int port, 
                                      const QString &username, const QString &password, const QString &sid)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    
    // Construction de la chaîne de connexion Oracle
    QString connectionString = QString("Driver={Oracle in XE};Dbq=%1:%2/%3;Uid=%4;Pwd=%5;")
                                  .arg(host)
                                  .arg(port)
                                  .arg(sid.isEmpty() ? database : sid)
                                  .arg(username)
                                  .arg(password);
    
    db.setDatabaseName(connectionString);
    
    if (!db.open()) {
        qDebug() << "Erreur de connexion Oracle :" << db.lastError().text();
        return false;
    }
    
    qDebug() << "Connexion Oracle réussie !";
    return true;
}

// Initialisation de la connexion Oracle partagée
bool Connection::initializeOracleConnection(const QString &host, const QString &sid,
                                           const QString &username, const QString &password, int port)
{
    // Supprimer la connexion existante si elle existe
    if (QSqlDatabase::contains("ORACLE_CONNECTION")) {
        QSqlDatabase::removeDatabase("ORACLE_CONNECTION");
    }
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "ORACLE_CONNECTION");
    
    // Construction de la chaîne de connexion Oracle
    QString connectionString = QString("Driver={Oracle in XE};Dbq=%1:%2/%3;Uid=%4;Pwd=%5;")
                                  .arg(host)
                                  .arg(port)
                                  .arg(sid)
                                  .arg(username)
                                  .arg(password);
    
    db.setDatabaseName(connectionString);
    
    if (!db.open()) {
        qDebug() << "Erreur de connexion Oracle :" << db.lastError().text();
        return false;
    }
    
    // Activer le mode auto-commit pour Oracle (ou gérer manuellement avec commit())
    // Par défaut, Qt n'active pas l'auto-commit, donc on doit faire des commits manuels
    // Mais pour Oracle, on peut aussi activer l'auto-commit si nécessaire
    
    qDebug() << "Connexion Oracle initialisée avec succès !";
    return true;
}

// Obtenir la connexion Oracle partagée
QSqlDatabase Connection::getOracleConnection()
{
    if (QSqlDatabase::contains("ORACLE_CONNECTION")) {
        return QSqlDatabase::database("ORACLE_CONNECTION");
    }
    
    // Si la connexion n'existe pas, l'initialiser avec les paramètres par défaut
    initializeOracleConnection();
    return QSqlDatabase::database("ORACLE_CONNECTION");
}

