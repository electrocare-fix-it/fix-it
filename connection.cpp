#include "connection.h"
#include <QProcessEnvironment>

namespace {
const char* kQociConnectionName = "ORACLE_QOCI_MAIN";

QString envOrDefault(const char* name, const char* fallback)
{
    const QByteArray val = qgetenv(name);
    return val.isEmpty() ? QString::fromUtf8(fallback) : QString::fromUtf8(val);
}
}

Connection::Connection()
{
    // Ne crée pas encore la connexion ici, on la créera dans createConnection()
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

    const QStringList drivers = QSqlDatabase::drivers();
    qDebug() << "Pilotes Qt disponibles:" << drivers;

    // Paramètres par défaut de ta base (peuvent être surchargés avec des variables d'environnement)
    //   FIXIT_DB_HOST, FIXIT_DB_PORT, FIXIT_DB_SERVICE, FIXIT_DB_USER, FIXIT_DB_PASSWORD
    const QString host     = envOrDefault("FIXIT_DB_HOST", "localhost");
    const int     port     = qEnvironmentVariableIntValue("FIXIT_DB_PORT", nullptr) == 0 ? 1521 : qEnvironmentVariableIntValue("FIXIT_DB_PORT", nullptr);
    const QString service  = envOrDefault("FIXIT_DB_SERVICE", "XE");
    const QString user     = envOrDefault("FIXIT_DB_USER", "jacem");
    const QString password = envOrDefault("FIXIT_DB_PASSWORD", "esprit18");

    qDebug() << "Paramètres Oracle:" << "host" << host << "service" << service << "user" << user;

    // 1) Tentative avec le driver natif Oracle QOCI si disponible
    if (drivers.contains("QOCI")) {
        qDebug() << "Tentative de connexion Oracle via QOCI";

        if (QSqlDatabase::contains(kQociConnectionName)) {
            db = QSqlDatabase::database(kQociConnectionName);
        } else {
            db = QSqlDatabase::addDatabase("QOCI", kQociConnectionName);
        }

        db.setHostName(host);
        db.setPort(port);
        db.setDatabaseName(service); // Service name / SID
        db.setUserName(user);
        db.setPassword(password);

        if (db.open()) {
            qDebug() << "Connexion Oracle réussie via QOCI";
            return true;
        } else {
            QSqlError err = db.lastError();
            qDebug() << "Echec connexion QOCI:" << err.text() << "code:" << err.nativeErrorCode();
        }
    } else {
        qDebug() << "Driver QOCI non disponible, passage en ODBC.";
    }

    // 2) Fallback via ODBC / DSN
    if (!drivers.contains("QODBC")) {
        qDebug() << "ERREUR: Le pilote QODBC n'est pas disponible non plus.";
        return false;
    }

    const QString dsn = envOrDefault("FIXIT_DSN", "databseqodbc");

    qDebug() << "Paramètres de connexion ODBC utilisés:"
             << "DSN=" << dsn << "USER=" << user;

    // Méthode 1: Utiliser la source de données ODBC avec utilisateur/mot de passe
    qDebug() << "Tentative 1a: Utilisation de la source de données ODBC via DSN";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    // Utiliser la source ODBC mais avec utilisateur et mot de passe
    db.setDatabaseName(QString("DSN=%1;UID=%2;PWD=%3;").arg(dsn, user, password));
    
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
    qDebug() << "Tentative 1b: Utilisation directe du DSN";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(dsn);
    db.setUserName(user);
    db.setPassword(password);
    
    if (db.open()) {
        qDebug() << "Connexion reussie avec la source de données ODBC (direct)!";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        QSqlError error = db.lastError();
        qDebug() << "Echec avec databseqodbc (direct):" << error.text();
    }
    
    // Méthode 1c: Essayer aussi avec source_projet2A comme DSN (fallback)
    qDebug() << "Tentative 1c: Utilisation de la source de données ODBC 'source_projet2A' avec DSN (fallback)";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DSN=%1;UID=%2;PWD=%3;").arg("source_projet2A", user, password));
    
    if (db.open()) {
        qDebug() << "Connexion reussie avec la source de données ODBC source_projet2A (DSN)!";
        qDebug() << "Driver utilise:" << db.driverName();
        qDebug() << "Database name:" << db.databaseName();
        return true;
    } else {
        QSqlError error = db.lastError();
        qDebug() << "Echec avec source_projet2A (DSN):" << error.text();
    }
    
    // Méthode 1d: Utilisation directe de source_projet2A (fallback)
    qDebug() << "Tentative 1d: Utilisation directe de 'source_projet2A' (fallback)";
    
    if (db.isOpen()) {
        QString oldName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(oldName);
    }
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source_projet2A");
    db.setUserName(user);
    db.setPassword(password);
    
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
    connectionStrings << QString("Driver={Oracle in XE};Dbq=localhost:1521/XE;Uid=%1;Pwd=%2;").arg(user, password);
    connectionStrings << QString("Driver={Oracle};Dbq=localhost:1521/XE;Uid=%1;Pwd=%2;").arg(user, password);
    
    // Format 2: Format TNS avec //host:port/service
    connectionStrings << QString("Driver={Oracle in XE};Dbq=//localhost:1521/XE;Uid=%1;Pwd=%2;").arg(user, password);
    connectionStrings << QString("Driver={Oracle};Dbq=//localhost:1521/XE;Uid=%1;Pwd=%2;").arg(user, password);
    
    // Format 3: SID simple (XE comme SID)
    connectionStrings << QString("Driver={Oracle in XE};Dbq=XE;Uid=%1;Pwd=%2;").arg(user, password);
    connectionStrings << QString("Driver={Oracle};Dbq=XE;Uid=%1;Pwd=%2;").arg(user, password);
    
    // Format 4: Avec source_projet2A comme service name (au cas où)
    connectionStrings << QString("Driver={Oracle in XE};Dbq=localhost:1521/source_projet2A;Uid=%1;Pwd=%2;").arg(user, password);
    connectionStrings << QString("Driver={Oracle};Dbq=//localhost:1521/source_projet2A;Uid=%1;Pwd=%2;").arg(user, password);
    connectionStrings << QString("Driver={Oracle in XE};Dbq=source_projet2A;Uid=%1;Pwd=%2;").arg(user, password);
    
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

