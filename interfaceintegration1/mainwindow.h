#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "gestionclient.h"
#include "gestionemploye.h"
#include "gestionobjetelectronique.h"
#include "gestionpiecesdetachees.h"


#include <QMainWindow>

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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();//gestion piecesdetachees
    void moveHome();

private:
    Ui::MainWindow *ui;

    // creation des objets
    gestionclient client;  //pour la fenetre de l'interface client
    gestionemploye employe;//pour la fentre de l'interface employe
    gestionobjetelectronique electro;//pour la fenetre de l'interface objet electronique
    gestionpiecesdetachees detaches;//pour la fenetre de l'interface pieces detachees



};
#endif // MAINWINDOW_H
