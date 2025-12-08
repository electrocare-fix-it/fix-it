#ifndef GESTIONPIECES_H
#define GESTIONPIECES_H

#include <QWidget>
#include <QSqlDatabase>
#include <QHash>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class gestionpieces; }
QT_END_NAMESPACE

class gestionpieces : public QWidget
{
    Q_OBJECT

public:
    explicit gestionpieces(QWidget *parent = nullptr);
    ~gestionpieces();

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

private:
    void initialiserBase();

    Ui::gestionpieces *ui;
    QSqlDatabase db;
    QString cheminImagePiece;
    QHash<QString, QString> cheminsImagesPieces;
    bool triPrixAscendant = true;
    QString texteRechercheCourant;
    QString categorieFiltreCourante;
    QString ordreTriCourant;
};

#endif // GESTIONPIECES_H


