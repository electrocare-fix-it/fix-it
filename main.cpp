#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Forcer l'utilisation du style Fusion pour que les styles CSS fonctionnent correctement
    a.setStyle("Fusion");
    
    Connection& c = Connection::createInstance();
    bool test = c.createConnection();
    
    MainWindow w;
    
    if (test) {
        w.show();
        // Message de connexion réussie (optionnel, peut être commenté)
        // QMessageBox::information(nullptr, QObject::tr("Base de données"), 
        //                         QObject::tr("Connexion réussie.\nCliquez sur OK pour continuer."), 
        //                         QMessageBox::Ok);
    } else {
        // Afficher un avertissement mais permettre à l'application de continuer
        QSqlDatabase& db = c.getDatabase();
        QSqlError error = db.lastError();
        
        QString errorText = error.text();
        QString errorCode = error.nativeErrorCode();
        
        QString errorDetails = QString("⚠️ Avertissement: Connexion à la base de données échouée.\n\n")
                              + QString("L'application va continuer, mais certaines fonctionnalités peuvent ne pas fonctionner.\n\n")
                              + QString("═══════════════════════════════════════\n")
                              + QString("DÉTAILS DE L'ERREUR:\n")
                              + QString("═══════════════════════════════════════\n\n")
                              + QString("Message: %1\n\n").arg(errorText.isEmpty() ? "Aucun message d'erreur disponible" : errorText)
                              + QString("Code erreur: %1\n\n").arg(errorCode.isEmpty() ? "N/A" : errorCode)
                              + QString("═══════════════════════════════════════\n")
                              + QString("VÉRIFICATIONS À EFFECTUER:\n")
                              + QString("═══════════════════════════════════════\n\n")
                              + QString("1. Oracle est installé et le service est démarré\n")
                              + QString("2. Le driver ODBC 'Oracle in XE' est installé\n")
                              + QString("3. La source de données 'databseqodbc' ou 'source_projet2A' est configurée\n")
                              + QString("4. Le TNS Service Name est 'XE' ou 'source_projet2A'\n")
                              + QString("5. L'utilisateur 'hiba' avec le mot de passe '0000' existe\n")
                              + QString("6. Le listener Oracle écoute sur le port 1521\n\n")
                              + QString("═══════════════════════════════════════\n")
                              + QString("CONSOLE DE DÉBOGAGE:\n")
                              + QString("═══════════════════════════════════════\n")
                              + QString("Ouvrez la console de débogage dans Qt Creator\n")
                              + QString("pour voir toutes les tentatives de connexion.\n");
        
        QMessageBox msgBox;
        msgBox.setWindowTitle(QObject::tr("Base de données - Avertissement"));
        msgBox.setText(errorDetails);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setMinimumWidth(500);
        msgBox.exec();
        
        // Continuer l'exécution même si la connexion a échoué
        w.show();
    }
    
    return a.exec();
}

