#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QApplication>

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
    
    qDebug() << "Toutes les pages insérées. Nombre total:" << stackedWidget->count();
    
    // Connecter les signaux de la page de login
    connect(&loginPage, SIGNAL(loginSuccessful(QString)), this, SLOT(onLoginSuccessful(QString)));
    connect(&loginPage, SIGNAL(loginCancelled()), this, SLOT(onLoginCancelled()));
    
    // Connecter les signaux des autres pages
    connect(&principale, SIGNAL(homeClicked()), this, SLOT(moveHome()));
    connect(&principale, SIGNAL(openObjects()), this, SLOT(openObjectsPage()));
    connect(&principale, SIGNAL(openClients()), this, SLOT(openClientsPage()));
    connect(&principale, SIGNAL(openEmployes()), this, SLOT(openEmployesPage()));
    connect(&objets, SIGNAL(homeRequested()), this, SLOT(moveHome()));
    connect(&clients, SIGNAL(HomeCliked()), this, SLOT(moveHome()));
    connect(&employes, SIGNAL(homeRequested()), this, SLOT(moveHome()));
    
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
    // Configurer les permissions selon le rôle
    if (role == "Gérant de l'atelier") {
        // Accès à toutes les gestions
        principale.setButtonVisible("gestionEmployeButton", true);
        principale.setButtonVisible("gestionClientsButton", true);
        principale.setButtonVisible("gestionObjectsButton", true);
        principale.setButtonVisible("gestionPiecesButton", true);
    } else if (role == "Technicien") {
        // Accès à gestionobjetelectronique, gestionclient, gestionpiecesdetachées
        principale.setButtonVisible("gestionEmployeButton", false);
        principale.setButtonVisible("gestionClientsButton", true);
        principale.setButtonVisible("gestionObjectsButton", true);
        principale.setButtonVisible("gestionPiecesButton", true);
    } else if (role == "Réceptionniste") {
        // Accès à gestionclient et gestionemploye
        principale.setButtonVisible("gestionEmployeButton", true);
        principale.setButtonVisible("gestionClientsButton", true);
        principale.setButtonVisible("gestionObjectsButton", false);
        principale.setButtonVisible("gestionPiecesButton", false);
    }
}
