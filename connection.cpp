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
    qDebug() << "Pilotes Qt disponibles:" << drivers;
    
    // Vérifier si QODBC est disponible
    if (!drivers.contains("QODBC")) {
        qDebug() << "ERREUR: Le pilote QODBC n'est pas disponible!";
        qDebug() << "Installez les pilotes ODBC pour Qt.";
        return false;
    }
    
    // Méthode 1: Utiliser la source de données ODBC avec utilisateur/mot de passe
    // La source ODBC utilise XE comme TNS Service Name
    qDebug() << "Tentative 1a: Utilisation de la source de données ODBC 'databseqodbc' avec DSN";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    // Utiliser la source ODBC mais avec utilisateur et mot de passe
    db.setDatabaseName("DSN=databseqodbc;UID=hiba;PWD=0000;");
    
    if (db.open()) {
        qDebug() << "Connexion reussie avec la source de données ODBC (DSN)!";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        QSqlError error = db.lastError();
        qDebug() << "Echec avec databseqodbc (DSN):" << error.text();
    }
    
    // Méthode 1b: Utiliser directement le nom de la source de données
    qDebug() << "Tentative 1b: Utilisation directe de 'databseqodbc'";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("databseqodbc");
    db.setUserName("hiba");
    db.setPassword("0000");
    
    if (db.open()) {
        qDebug() << "Connexion reussie avec la source de données ODBC (direct)!";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        QSqlError error = db.lastError();
        qDebug() << "Echec avec databseqodbc (direct):" << error.text();
    }
    
    // Méthode 1c: Essayer aussi avec source_projet2A comme DSN
    qDebug() << "Tentative 1c: Utilisation de la source de données ODBC 'source_projet2A' avec DSN";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DSN=source_projet2A;UID=hiba;PWD=0000;");
    
    if (db.open()) {
        qDebug() << "Connexion reussie avec la source de données ODBC source_projet2A (DSN)!";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        QSqlError error = db.lastError();
        qDebug() << "Echec avec source_projet2A (DSN):" << error.text();
    }
    
    // Méthode 1d: Utilisation directe de source_projet2A
    qDebug() << "Tentative 1d: Utilisation directe de 'source_projet2A'";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source_projet2A");
    db.setUserName("hiba");
    db.setPassword("0000");
    
    if (db.open()) {
        qDebug() << "Connexion reussie avec la source de données ODBC source_projet2A (direct)!";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        QSqlError error = db.lastError();
        qDebug() << "Echec avec source_projet2A (direct):" << error.text();
    }
    
    // Méthode 2: Connexion directe avec XE (TNS Service Name configuré dans ODBC)
    qDebug() << "Tentative 2: Connexion directe avec XE (TNS Service Name)";
    
    // Liste des formats de connexion à essayer avec XE
    QStringList connectionStrings;
    
    // Format 1: Avec host:port/service (format recommandé) - XE est le service name
    connectionStrings << "Driver={Oracle in XE};Dbq=localhost:1521/XE;Uid=hiba;Pwd=0000;";
    connectionStrings << "Driver={Oracle};Dbq=localhost:1521/XE;Uid=hiba;Pwd=0000;";
    
    // Format 2: Format TNS avec //host:port/service
    connectionStrings << "Driver={Oracle in XE};Dbq=//localhost:1521/XE;Uid=hiba;Pwd=0000;";
    connectionStrings << "Driver={Oracle};Dbq=//localhost:1521/XE;Uid=hiba;Pwd=0000;";
    
    // Format 3: SID simple (XE comme SID)
    connectionStrings << "Driver={Oracle in XE};Dbq=XE;Uid=hiba;Pwd=0000;";
    connectionStrings << "Driver={Oracle};Dbq=XE;Uid=hiba;Pwd=0000;";
    
    // Format 4: Avec source_projet2A comme service name (au cas où)
    connectionStrings << "Driver={Oracle in XE};Dbq=localhost:1521/source_projet2A;Uid=hiba;Pwd=0000;";
    connectionStrings << "Driver={Oracle};Dbq=//localhost:1521/source_projet2A;Uid=hiba;Pwd=0000;";
    connectionStrings << "Driver={Oracle in XE};Dbq=source_projet2A;Uid=hiba;Pwd=0000;";
    
    // Essayer avec différents noms de drivers Oracle communs
    QStringList oracleDrivers;
    oracleDrivers << "Oracle in XE" << "Oracle" << "Oracle in OraDB21Home1" 
                  << "Oracle in OraDB19Home1" << "Oracle in OraClient12Home1"
                  << "Oracle in OraDB23Home1" << "Oracle in OraDB18Home1";
    
    qDebug() << "Tentative de connexion avec les drivers Oracle suivants:" << oracleDrivers;
    
    // Essayer toutes les combinaisons
    for (const QString& driver : oracleDrivers) {
        for (const QString& connStr : connectionStrings) {
            QString testConnection = connStr;
            // Remplacer le nom du driver (trouver Driver={...} et le remplacer)
            int startPos = testConnection.indexOf("Driver={");
            if (startPos != -1) {
                int endPos = testConnection.indexOf("}", startPos);
                if (endPos != -1) {
                    testConnection.replace(startPos, endPos - startPos + 1, QString("Driver={%1}").arg(driver));
                }
            }
            
            qDebug() << "Tentative de connexion avec:" << testConnection;
            
            // Fermer et supprimer la connexion précédente si elle existe
            if (db.isOpen()) {
                QString oldName = db.connectionName();
                db.close();
                QSqlDatabase::removeDatabase(oldName);
            }
            static int connCounter = 0;
            QString connectionName = QString("ORACLE_CONN_%1").arg(++connCounter);
            db = QSqlDatabase::addDatabase("QODBC", connectionName);
            db.setDatabaseName(testConnection);
            
            if (db.open()) {
                qDebug() << "Connexion etablie avec succes!";
                qDebug() << "Driver utilise:" << db.driverName();
                qDebug() << "Database name:" << db.databaseName();
                return true;
            } else {
                QSqlError error = db.lastError();
                qDebug() << "Echec:" << error.text();
                qDebug() << "Code erreur:" << error.nativeErrorCode();
                qDebug() << "Type erreur:" << error.type();
            }
        }
    }
    
    qDebug() << "Toutes les tentatives de connexion ont echoue!";
    qDebug() << "Derniere erreur:" << db.lastError().text();
    return false;
}

QSqlDatabase& Connection::getDatabase()
{
    return db;
}

