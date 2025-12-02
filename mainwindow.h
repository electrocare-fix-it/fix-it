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

private:
    QStackedWidget *stackedWidget;
    Login loginPage;
    menuprincipale principale;
    gestionobjetelectronique objets;
    gestionclient clients;
    gestionemploye employes;
    gestionpieces pieces;
    QString currentRole;
};

#endif // MAINWINDOW_H
