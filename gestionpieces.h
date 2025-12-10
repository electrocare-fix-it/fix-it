#ifndef GESTIONPIECES_H
#define GESTIONPIECES_H

#include <QWidget>
#include <QSqlDatabase>
#include <QHash>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class gestionpieces; }
QT_END_NAMESPACE

class SerialPortManager;

class gestionpieces : public QWidget
{
    Q_OBJECT

public:
    explicit gestionpieces(QWidget *parent = nullptr);
    ~gestionpieces();
    
    // Méthodes publiques pour activer/désactiver l'accès depuis MainWindow
    void enableAccess();
    void disableAccess();

private slots:
    void ajouterPiece();
    void afficherPieces(const QString &orderBy = QString(),
                        const QString &texteRecherche = QString(),
                        const QString &categorieFiltre = QString());
    void supprimerPiece();
    void remplirFormulaireDepuisTable(int row, int column);
    void viderChamps();
    void chargerImagePiece();
    void trierPiecesParPrix();
    void rechercherPieces();
    void exporterTableauPDF();
    void on_pushButton_clicked(); // Bouton Home

signals:
    void homeRequested(); // Signal pour retourner au menu principal
    void accessDeniedForMenu(); // Signal pour désactiver le bouton dans le menu principal
    void accessGrantedForMenu(); // Signal pour réactiver le bouton dans le menu principal

private:
    void initialiserBase();
    void setupRFIDAccess();
    void showAccessMessage(const QString &message, bool isGranted);

    Ui::gestionpieces *ui;
    QSqlDatabase db;
    QString cheminImagePiece;
    QHash<QString, QString> cheminsImagesPieces;
    bool triPrixAscendant = true;
    QString texteRechercheCourant;
    QString categorieFiltreCourante;
    QString ordreTriCourant;
    
    SerialPortManager *m_serialManager;
    bool m_accessGranted;
    QString m_currentEmployee;
};

#endif // GESTIONPIECES_H


