#include "mainwindow.h"
#include "serialportmanager.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Créer le stackedWidget programmatiquement (sans fichier UI)
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("background-color: #E0F6FF;");
    
    // Définir le stackedWidget comme widget central
    setCentralWidget(stackedWidget);
    
    // Définir la taille de la fenêtre
    resize(1388, 875);
    setWindowTitle("Système de Gestion");
    
    qDebug() << "=== CRÉATION DE LA FENÊTRE PRINCIPALE ===";
    qDebug() << "StackedWidget créé programmatiquement";
    
    // Insérer la page de login en premier (index 0)
    stackedWidget->insertWidget(0, &loginPage);
    qDebug() << "Page de login insérée à l'index 0";
    
    // Insérer les autres pages
    stackedWidget->insertWidget(1, &principale);
    stackedWidget->insertWidget(2, &objets);
    stackedWidget->insertWidget(3, &clients);
    stackedWidget->insertWidget(4, &employes);
    stackedWidget->insertWidget(5, &pieces);
    
    qDebug() << "Toutes les pages insérées. Nombre total:" << stackedWidget->count();
    
    // Connecter les signaux de la page de login
    connect(&loginPage, SIGNAL(loginSuccessful(QString)), this, SLOT(onLoginSuccessful(QString)));
    connect(&loginPage, SIGNAL(loginCancelled()), this, SLOT(onLoginCancelled()));
    
    // Connecter les signaux des autres pages
    connect(&principale, SIGNAL(homeClicked()), this, SLOT(moveHome()));
    connect(&principale, SIGNAL(openObjects()), this, SLOT(openObjectsPage()));
    connect(&principale, SIGNAL(openClients()), this, SLOT(openClientsPage()));
    connect(&principale, SIGNAL(openEmployes()), this, SLOT(openEmployesPage()));
    connect(&principale, SIGNAL(openPieces()), this, SLOT(openPiecesPage()));
    connect(&objets, SIGNAL(homeRequested()), this, SLOT(showMainMenu()));
    connect(&clients, SIGNAL(HomeCliked()), this, SLOT(showMainMenu()));
    connect(&employes, SIGNAL(homeRequested()), this, SLOT(showMainMenu()));
    connect(&pieces, SIGNAL(homeRequested()), this, SLOT(showMainMenu())); // Bouton Home de gestionpieces
    
    // Connecter les signaux RFID pour désactiver/réactiver le bouton dans le menu
    // Utiliser la syntaxe moderne de Qt pour plus de fiabilité
    connect(&pieces, &gestionpieces::accessDeniedForMenu, this, &MainWindow::disablePiecesButton, Qt::QueuedConnection);
    connect(&pieces, &gestionpieces::accessGrantedForMenu, this, &MainWindow::enablePiecesButton, Qt::QueuedConnection);
    
    qDebug() << "Signaux RFID connectes pour desactiver/reactiver le bouton";
    
    // Initialiser le flag RFID
    m_piecesButtonDisabledByRFID = false;
    
    // Initialiser le gestionnaire RFID pour le menu principal
    m_rfidManager = new SerialPortManager(this);
    setupRFIDForMainMenu();
    
    // FORCER l'affichage de la page de login au démarrage
    stackedWidget->setCurrentIndex(0);
    qDebug() << "Page de login affichée à l'index 0";
    qDebug() << "Widget actuel:" << (stackedWidget->currentWidget() ? stackedWidget->currentWidget()->objectName() : "NULL");
    qDebug() << "Est-ce la page de login?" << (stackedWidget->currentWidget() == &loginPage);
}

MainWindow::~MainWindow()
{
    // Le stackedWidget sera supprimé automatiquement car il est le widget central
}

void MainWindow::moveHome()
{
    // Retourner à la page d'authentification (déconnexion)
    stackedWidget->setCurrentIndex(0);
    qDebug() << "Retour à la page d'authentification";
    
    // Réinitialiser les champs de login
    loginPage.clearFields();
}

void MainWindow::showMainMenu()
{
    // Afficher la page principale (menu avec les 4 boutons)
    stackedWidget->setCurrentIndex(1);
    qDebug() << "Retour au menu principal depuis une page de gestion";
}

void MainWindow::openObjectsPage()
{
    stackedWidget->setCurrentIndex(2); // Index 2 car login=0, menu=1, objets=2
}

void MainWindow::openClientsPage()
{
    stackedWidget->setCurrentIndex(3); // Index 3 car login=0, menu=1, objets=2, clients=3
}

void MainWindow::openEmployesPage()
{
    stackedWidget->setCurrentIndex(4); // Index 4 car login=0, menu=1, objets=2, clients=3, employes=4
}

void MainWindow::openPiecesPage()
{
    // Vérifier si le bouton est désactivé par RFID avant d'ouvrir la page
    if (m_piecesButtonDisabledByRFID) {
        qDebug() << "Tentative d'acces a la page Gestion Pieces Detachees - ACCES REFUSE (bouton desactive par RFID)";
        QMessageBox::warning(this, "Accès Refusé", 
            "Vous n'avez pas l'autorisation d'accéder à la gestion des pièces détachées.\n\n"
            "Veuillez scanner une carte autorisée.");
        return; // Ne pas ouvrir la page
    }
    
    stackedWidget->setCurrentIndex(5); // Index 5 pour gestion des pièces détachées
}

void MainWindow::onLoginSuccessful(const QString& role)
{
    currentRole = role;
    setupPermissions(role);
    stackedWidget->setCurrentIndex(1); // Afficher le menu principal
}

void MainWindow::onLoginCancelled()
{
    this->close();
}

void MainWindow::setupPermissions(const QString& role)
{
    // Réinitialiser TOUS les boutons à false avant d'appliquer les nouvelles permissions
    principale.setButtonAccessible("gestionEmployeButton", false);
    principale.setButtonAccessible("gestionClientsButton", false);
    principale.setButtonAccessible("gestionObjectsButton", false);
    
    // Ne pas réinitialiser gestionPiecesButton si il est désactivé par RFID
    if (!m_piecesButtonDisabledByRFID) {
        principale.setButtonAccessible("gestionPiecesButton", false);
    }
    
    // Configurer les permissions selon le rôle
    if (role == "Gérant de l'atelier") {
        // Accès à toutes les gestions
        principale.setButtonAccessible("gestionEmployeButton", true);
        principale.setButtonAccessible("gestionClientsButton", true);
        principale.setButtonAccessible("gestionObjectsButton", true);
        // Ne pas réactiver gestionPiecesButton si il est désactivé par RFID
        if (!m_piecesButtonDisabledByRFID) {
            principale.setButtonAccessible("gestionPiecesButton", true);
        }
    } else if (role == "Technicien") {
        // Accès à gestionobjetelectronique, gestionclient et gestionpiecesdetachées
        principale.setButtonAccessible("gestionClientsButton", true);
        principale.setButtonAccessible("gestionObjectsButton", true);
        // Ne pas réactiver gestionPiecesButton si il est désactivé par RFID
        if (!m_piecesButtonDisabledByRFID) {
            principale.setButtonAccessible("gestionPiecesButton", true);
        }
    } else if (role == "Réceptionniste") {
        // Accès à gestionclient et gestionemploye
        principale.setButtonAccessible("gestionEmployeButton", true);
        principale.setButtonAccessible("gestionClientsButton", true);
    }
}

void MainWindow::disablePiecesButton()
{
    qDebug() << "========================================";
    qDebug() << "*** disablePiecesButton() APPELE ***";
    qDebug() << "Desactivation du bouton Gestion Pieces Detachees dans le menu";
    
    // Marquer que le bouton est désactivé par RFID
    m_piecesButtonDisabledByRFID = true;
    
    // Forcer la désactivation du bouton
    principale.setButtonAccessible("gestionPiecesButton", false);
    
    // Forcer la mise à jour de l'interface
    QApplication::processEvents();
    
    qDebug() << "Bouton desactive avec succes (flag RFID: true)";
    qDebug() << "========================================";
}

void MainWindow::enablePiecesButton()
{
    qDebug() << "========================================";
    qDebug() << "*** enablePiecesButton() APPELE ***";
    qDebug() << "Reactivation du bouton Gestion Pieces Detachees dans le menu";
    
    // Réinitialiser le flag RFID
    m_piecesButtonDisabledByRFID = false;
    
    // Réactiver seulement si l'utilisateur a les permissions
    if (currentRole == "Gérant de l'atelier" || currentRole == "Technicien") {
        principale.setButtonAccessible("gestionPiecesButton", true);
        qDebug() << "Bouton reactive avec succes (permissions OK)";
    } else {
        qDebug() << "Bouton non reactive (pas de permissions)";
    }
    qDebug() << "========================================";
}

void MainWindow::setupRFIDForMainMenu()
{
    qDebug() << "=== CONFIGURATION RFID POUR LE MENU PRINCIPAL ===";
    
    // Connecter les signaux RFID pour le menu principal
    connect(m_rfidManager, &SerialPortManager::accessGranted,
            this, [this](const QString &employeeName, const QString &status) {
                qDebug() << "========================================";
                qDebug() << "*** SIGNAL accessGranted RECU DANS MENU PRINCIPAL ***";
                qDebug() << "Employee:" << employeeName;
                qDebug() << "Status:" << status;
                qDebug() << "========================================";
                
                // Forcer le traitement des événements
                QApplication::processEvents();
                
                // Vérifier si on est dans le menu principal
                if (stackedWidget->currentIndex() == 1) {
                    // Message pour Hiba Riahi
                    QString message = "Accès autorisé Hiba Riahi tu as l'accès pour l'interface de la gestion pièces détachées.";
                    
                    qDebug() << "Affichage du message dans le menu principal:" << message;
                    
                    // Afficher le message dans le menu principal
                    QMessageBox *msgBox = new QMessageBox(this);
                    msgBox->setIcon(QMessageBox::Information);
                    msgBox->setWindowTitle("Accès Autorisé");
                    msgBox->setText(message);
                    msgBox->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                    msgBox->setModal(true);
                    
                    msgBox->show();
                    msgBox->raise();
                    msgBox->activateWindow();
                    
                    QApplication::processEvents();
                    
                    msgBox->exec();
                    msgBox->deleteLater();
                    
                    qDebug() << "Message affiche dans le menu principal";
                }
                
                // Réactiver le bouton dans le menu principal
                m_piecesButtonDisabledByRFID = false;
                if (currentRole == "Gérant de l'atelier" || currentRole == "Technicien") {
                    principale.setButtonAccessible("gestionPiecesButton", true);
                    qDebug() << "Bouton reactive dans le menu principal";
                }
                
                // Activer aussi l'interface dans gestionpieces si on y est
                pieces.enableAccess();
            });
    
    connect(m_rfidManager, &SerialPortManager::accessDenied,
            this, [this](const QString &employeeName, const QString &status) {
                qDebug() << "========================================";
                qDebug() << "*** SIGNAL accessDenied RECU DANS MENU PRINCIPAL ***";
                qDebug() << "Employee:" << employeeName;
                qDebug() << "Status:" << status;
                qDebug() << "========================================";
                
                // Forcer le traitement des événements
                QApplication::processEvents();
                
                // Vérifier si on est dans le menu principal
                if (stackedWidget->currentIndex() == 1) {
                    // Message pour Omar Askri
                    QString message;
                    if (employeeName.toUpper() == "OMAR_ASKRI" || employeeName.toUpper().contains("OMAR") || employeeName.toUpper().contains("ASKRI")) {
                        message = "Accès non autorisé Omar Askri tu n'as pas l'accès pour la gestion pièces détachées.";
                    } else {
                        message = "Accès non autorisé. Tu n'as pas l'accès pour la gestion pièces détachées.";
                    }
                    
                    qDebug() << "Affichage du message dans le menu principal:" << message;
                    
                    // Afficher le message dans le menu principal
                    QMessageBox *msgBox = new QMessageBox(this);
                    msgBox->setIcon(QMessageBox::Warning);
                    msgBox->setWindowTitle("Accès Refusé");
                    msgBox->setText(message);
                    msgBox->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                    msgBox->setModal(true);
                    
                    msgBox->show();
                    msgBox->raise();
                    msgBox->activateWindow();
                    
                    QApplication::processEvents();
                    
                    msgBox->exec();
                    msgBox->deleteLater();
                    
                    qDebug() << "Message affiche dans le menu principal";
                }
                
                // Désactiver le bouton dans le menu principal IMMÉDIATEMENT
                m_piecesButtonDisabledByRFID = true;
                
                // Forcer la désactivation du bouton avec le style gris
                principale.setButtonAccessible("gestionPiecesButton", false);
                
                // Forcer la mise à jour de l'interface
                QApplication::processEvents();
                
                qDebug() << "Bouton desactive dans le menu principal (gris et inutilisable)";
                qDebug() << "Flag RFID:" << m_piecesButtonDisabledByRFID;
                
                // Désactiver aussi l'interface dans gestionpieces si on y est
                pieces.disableAccess();
                
                // Forcer à nouveau la mise à jour après désactivation
                QApplication::processEvents();
            });
    
    // Tenter de se connecter à l'Arduino
    qDebug() << "Tentative de connexion à l'Arduino pour le menu principal...";
    if (m_rfidManager->connectToArduino(QString(), 9600)) {
        qDebug() << "✓✓✓ SUCCÈS: Connecté à l'Arduino pour le menu principal sur le port:" << m_rfidManager->getCurrentPort();
        qDebug() << "Le système RFID est actif dans le menu principal";
    } else {
        qDebug() << "ERREUR: Impossible de se connecter à l'Arduino pour le menu principal";
    }
}
