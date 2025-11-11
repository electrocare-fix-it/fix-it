#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    Connection& c = Connection::createInstance();
    bool test = c.createConnection();
    
    MainWindow w;
    
    if (test) {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Base de données"), 
                                QObject::tr("Connexion réussie.\nCliquez sur OK pour continuer."), 
                                QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Base de données"), 
                             QObject::tr("Échec de la connexion.\nCliquez sur OK pour quitter."), 
                             QMessageBox::Ok);
        return 1;
    }
    
    return a.exec();
}

