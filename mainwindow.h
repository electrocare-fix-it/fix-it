#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "menuprincipale.h" // insérer la class menuprincipale
#include "gestionobjetelectronique.h"









QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(); // pour la boutton closeapplication
    void on_pushButton_2_clicked();// pour la boutton login
    void moveHome();//fonction pour le boutton Home
    void openObjectsPage();


private:
    Ui::MainWindow *ui;
    menuprincipale principale;// creation d'objet pour la fenetre de menuprinciple qui contient les 4 bouttons des 4 différents gestions
    gestionobjetelectronique objets;






};
#endif // MAINWINDOW_H
