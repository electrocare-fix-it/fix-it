#include "login.h"
#include "ui_login.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    qDebug() << "Page de login créée";
    
    // Créer la table des utilisateurs si elle n'existe pas
    createUsersTableIfNotExists();
    
    // Configurer le ComboBox pour les rôles
    ui->comboRole->addItem("Technicien");
    ui->comboRole->addItem("Réceptionniste");
    ui->comboRole->addItem("Gérant de l'atelier");
    
    // Masquer le mot de passe
    ui->txtPassword->setEchoMode(QLineEdit::Password);
    
    // Connecter manuellement les boutons (au cas où la connexion automatique ne fonctionne pas)
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(on_btnLogin_clicked()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_clicked()));
    
    // Style pour forcer le texte en noir
    QString style = "QLabel { color: black !important; }"
                   "QLineEdit { color: black !important; }"
                   "QComboBox { color: black !important; }"
                   "QPushButton { color: black !important; }";
    this->setStyleSheet(style);
    
    qDebug() << "Page de login initialisée. Bouton login connecté:" << (ui->btnLogin != nullptr);
}

Login::~Login()
{
    delete ui;
}

void Login::createUsersTableIfNotExists()
{
    Connection& c = Connection::createInstance();
    QSqlDatabase& db = c.getDatabase();
    
    // Tenter de se connecter si la base n'est pas ouverte
    if (!db.isOpen()) {
        qDebug() << "Base de données non connectée. Tentative de connexion...";
        if (!c.createConnection()) {
            qDebug() << "ERREUR: Impossible de se connecter à la base de données";
            QMessageBox::warning(this, "Attention", 
                "La base de données n'est pas connectée. La table UTILISATEUR ne peut pas être créée.\n\n"
                "Vérifiez que:\n"
                "- Oracle est installé et démarré\n"
                "- La source de données ODBC est configurée\n"
                "- Les identifiants de connexion sont corrects");
            return;
        }
    }
    
    qDebug() << "=== CRÉATION/VÉRIFICATION DE LA TABLE UTILISATEUR ===";
    qDebug() << "Base de données connectée:" << db.isOpen();
    qDebug() << "Nom de la base:" << db.databaseName();
    
    QSqlQuery query(db);
    
    // Vérifier si la table existe
    query.prepare("SELECT COUNT(*) FROM user_tables WHERE UPPER(table_name) = 'UTILISATEUR'");
    bool tableExists = false;
    if (query.exec() && query.next()) {
        tableExists = query.value(0).toInt() > 0;
    }
    
    if (!tableExists) {
        qDebug() << "La table UTILISATEUR n'existe pas. Création en cours...";
        
        // Créer la table UTILISATEUR
        QString createTable = "CREATE TABLE UTILISATEUR ("
                              "id_utilisateur INT PRIMARY KEY,"
                              "login VARCHAR2(50) UNIQUE NOT NULL,"
                              "password VARCHAR2(100) NOT NULL,"
                              "role VARCHAR2(50) NOT NULL"
                              ")";
        
        if (!query.exec(createTable)) {
            qDebug() << "ERREUR lors de la création de la table UTILISATEUR:" << query.lastError().text();
            qDebug() << "Code erreur:" << query.lastError().nativeErrorCode();
            QMessageBox::critical(this, "Erreur", 
                QString("Impossible de créer la table UTILISATEUR:\n%1\n\nVeuillez exécuter le script create_table_utilisateur.sql dans SQL Developer.")
                .arg(query.lastError().text()));
            return;
        } else {
            qDebug() << "Table UTILISATEUR créée avec succès";
            
            // Commit pour Oracle
            if (!db.commit()) {
                qDebug() << "Erreur lors du commit:" << db.lastError().text();
            } else {
                qDebug() << "Commit réussi pour la création de la table";
            }
            
            // Créer une séquence pour l'ID (si elle n'existe pas)
            query.prepare("SELECT COUNT(*) FROM user_sequences WHERE sequence_name = 'SEQ_UTILISATEUR'");
            bool seqExists = false;
            if (query.exec() && query.next()) {
                seqExists = query.value(0).toInt() > 0;
            }
            
            if (!seqExists) {
                if (!query.exec("CREATE SEQUENCE seq_utilisateur START WITH 1 INCREMENT BY 1")) {
                    qDebug() << "Erreur création séquence:" << query.lastError().text();
                } else {
                    qDebug() << "Séquence seq_utilisateur créée";
                }
            }
            
            // Insérer des utilisateurs par défaut (mot de passe = "admin" hashé en MD5)
            // Le hash MD5 de "admin" est: 21232f297a57a5a743894a0e4a801fc3
            QString defaultPassword = "21232f297a57a5a743894a0e4a801fc3"; // MD5 de "admin"
            
            // Vérifier si les utilisateurs existent déjà
            query.prepare("SELECT COUNT(*) FROM UTILISATEUR WHERE login = 'gerant'");
            bool usersExist = false;
            if (query.exec() && query.next()) {
                usersExist = query.value(0).toInt() > 0;
            }
            
            if (!usersExist) {
                // Gérant de l'atelier
                query.prepare("INSERT INTO UTILISATEUR (id_utilisateur, login, password, role) VALUES (seq_utilisateur.NEXTVAL, :login, :password, :role)");
                query.bindValue(":login", "gerant");
                query.bindValue(":password", defaultPassword);
                query.bindValue(":role", "Gérant de l'atelier");
                if (!query.exec()) {
                    qDebug() << "Erreur insertion gerant:" << query.lastError().text();
                } else {
                    qDebug() << "Utilisateur 'gerant' créé";
                }
                
                // Technicien
                query.prepare("INSERT INTO UTILISATEUR (id_utilisateur, login, password, role) VALUES (seq_utilisateur.NEXTVAL, :login, :password, :role)");
                query.bindValue(":login", "technicien");
                query.bindValue(":password", defaultPassword);
                query.bindValue(":role", "Technicien");
                if (!query.exec()) {
                    qDebug() << "Erreur insertion technicien:" << query.lastError().text();
                } else {
                    qDebug() << "Utilisateur 'technicien' créé";
                }
                
                // Réceptionniste
                query.prepare("INSERT INTO UTILISATEUR (id_utilisateur, login, password, role) VALUES (seq_utilisateur.NEXTVAL, :login, :password, :role)");
                query.bindValue(":login", "receptionniste");
                query.bindValue(":password", defaultPassword);
                query.bindValue(":role", "Réceptionniste");
                if (!query.exec()) {
                    qDebug() << "Erreur insertion receptionniste:" << query.lastError().text();
                } else {
                    qDebug() << "Utilisateur 'receptionniste' créé";
                }
                
                // Commit pour Oracle
                if (!db.commit()) {
                    qDebug() << "Erreur lors du commit:" << db.lastError().text();
                } else {
                    qDebug() << "Utilisateurs par défaut créés et commit effectué";
                    qDebug() << "Login/Password: gerant/admin, technicien/admin, receptionniste/admin";
                }
            } else {
                qDebug() << "Les utilisateurs existent déjà dans la base de données";
            }
        }
    } else {
        qDebug() << "La table UTILISATEUR existe déjà dans la base de données";
    }
    qDebug() << "=== FIN CRÉATION/VÉRIFICATION TABLE UTILISATEUR ===";
}

bool Login::authenticateUser(const QString& login, const QString& password, const QString& role)
{
    qDebug() << "=== AUTHENTIFICATION ===";
    qDebug() << "Login:" << login << "Role:" << role;
    
    Connection& c = Connection::createInstance();
    QSqlDatabase& db = c.getDatabase();
    
    if (!db.isOpen()) {
        qDebug() << "ERREUR: Base de données non connectée";
        QMessageBox::critical(this, "Erreur", "Base de données non connectée. Impossible de vérifier les identifiants.");
        return false;
    }
    
    qDebug() << "Base de données connectée:" << db.isOpen();
    qDebug() << "Nom de la base:" << db.databaseName();
    
    QSqlQuery query(db);
    
    // Hasher le mot de passe
    QString hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
    qDebug() << "Mot de passe hashé (MD5):" << hashedPassword;
    
    // ÉTAPE 1: Vérifier si le nom d'utilisateur existe
    query.prepare("SELECT password, role FROM UTILISATEUR WHERE login = :login");
    query.bindValue(":login", login);
    
    if (!query.exec()) {
        qDebug() << "ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la vérification:\n%1").arg(query.lastError().text()));
        return false;
    }
    
    if (!query.next()) {
        // Le nom d'utilisateur n'existe pas
        qDebug() << "Authentification ÉCHOUÉE: Nom d'utilisateur incorrect";
        QMessageBox::warning(this, "Erreur d'authentification", 
            QString("❌ <b>Nom d'utilisateur incorrect</b><br><br>"
                   "Le nom d'utilisateur '<b>%1</b>' n'existe pas dans la base de données.<br><br>"
                   "Vérifiez l'orthographe et réessayez.").arg(login));
        return false;
    }
    
    // Le nom d'utilisateur existe, récupérer le mot de passe et le rôle
    QString storedPassword = query.value(0).toString();
    QString storedRole = query.value(1).toString();
    qDebug() << "Utilisateur trouvé. Rôle stocké:" << storedRole;
    
    // ÉTAPE 2: Vérifier le mot de passe
    if (storedPassword != hashedPassword) {
        qDebug() << "Authentification ÉCHOUÉE: Mot de passe incorrect";
        QMessageBox::warning(this, "Erreur d'authentification", 
            QString("❌ <b>Mot de passe incorrect</b><br><br>"
                   "Le mot de passe saisi pour l'utilisateur '<b>%1</b>' est incorrect.<br><br>"
                   "Vérifiez votre mot de passe et réessayez.").arg(login));
        return false;
    }
    
    // ÉTAPE 3: Vérifier le rôle
    if (storedRole != role) {
        qDebug() << "Authentification ÉCHOUÉE: Rôle incorrect";
        qDebug() << "Rôle saisi:" << role << "Rôle stocké:" << storedRole;
        QMessageBox::warning(this, "Erreur d'authentification", 
            QString("❌ <b>Rôle incorrect</b><br><br>"
                   "Le rôle sélectionné '<b>%1</b>' ne correspond pas à celui de l'utilisateur '<b>%2</b>'.<br><br>"
                   "Le rôle correct pour cet utilisateur est: <b>%3</b><br><br>"
                   "Veuillez sélectionner le bon rôle et réessayez.").arg(role, login, storedRole));
        return false;
    }
    
    // Toutes les vérifications sont passées
    qDebug() << "Authentification RÉUSSIE! Rôle trouvé:" << storedRole;
    qDebug() << "=== FIN AUTHENTIFICATION (SUCCÈS) ===";
    return true;
}

void Login::on_btnLogin_clicked()
{
    qDebug() << "Bouton login cliqué!";
    
    QString login = ui->txtLogin->text().trimmed();
    QString password = ui->txtPassword->text();
    QString role = ui->comboRole->currentText();
    
    qDebug() << "Login:" << login << "Role:" << role;
    
    // Validation
    if (login.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un nom d'utilisateur");
        ui->txtLogin->setFocus();
        return;
    }
    
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un mot de passe");
        ui->txtPassword->setFocus();
        return;
    }
    
    qDebug() << "Tentative d'authentification...";
    
    // Authentification (les messages d'erreur spécifiques sont gérés dans authenticateUser)
    if (authenticateUser(login, password, role)) {
        qDebug() << "Authentification réussie!";
        QMessageBox::information(this, "Succès", QString("✅ <b>Connexion réussie!</b><br><br>Bienvenue <b>%1</b> (%2)").arg(login, role));
        emit loginSuccessful(role);
    } else {
        qDebug() << "Authentification échouée!";
        // Les messages d'erreur spécifiques sont déjà affichés dans authenticateUser
        ui->txtPassword->clear();
        ui->txtPassword->setFocus();
    }
}

void Login::on_btnCancel_clicked()
{
    emit loginCancelled();
}

void Login::clearFields()
{
    ui->txtLogin->clear();
    ui->txtPassword->clear();
    ui->comboRole->setCurrentIndex(0);
    qDebug() << "Champs de login réinitialisés";
}

