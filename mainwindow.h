#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "menuprincipale.h"
#include "gestionobjetelectronique.h"
#include "gestionclient.h"
#include "gestionemploye.h"
#include "login.h"
#include "gestionpieces.h"

class SerialPortManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void moveHome();
    void showMainMenu();
    void openObjectsPage();
    void openClientsPage();
    void openEmployesPage();
    void openPiecesPage();
    void onLoginSuccessful(const QString& role);
    void onLoginCancelled();
    void setupPermissions(const QString& role);
    void disablePiecesButton();
    void enablePiecesButton();

private:
    QStackedWidget *stackedWidget;
    Login loginPage;
    menuprincipale principale;
    gestionobjetelectronique objets;
    gestionclient clients;
    gestionemploye employes;
    gestionpieces pieces;
    QString currentRole;
    bool m_piecesButtonDisabledByRFID; // Flag pour indiquer si le bouton est désactivé par RFID
    SerialPortManager *m_rfidManager; // Gestionnaire RFID pour le menu principal
    void setupRFIDForMainMenu(); // Configurer le RFID pour le menu principal
};

#endif // MAINWINDOW_H
