#ifndef GESTIONOBJETELECTRONIQUE_H
#define GESTIONOBJETELECTRONIQUE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVector>
#include <QPair>

namespace Ui {
class gestionobjetelectronique;
}

class gestionobjetelectronique : public QWidget
{
    Q_OBJECT

public:
    explicit gestionobjetelectronique(QWidget *parent = nullptr);
    ~gestionobjetelectronique();

signals:
    void homeRequested();

private slots:
    void on_pushButton_clicked();
    void on_btnAjouter_6_clicked();
    void on_btnModifier_6_clicked();
    void on_btnSupprimer_6_clicked();
    void on_btnRechercher_6_clicked();
    void on_btnReinitialiser_6_clicked();
    void on_tableWidget_itemSelectionChanged();
    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::gestionobjetelectronique *ui;
    QSqlDatabase database;
    QString currentSelectedReference;
    bool isTableEditMode = false;

    void openDatabase();
    void ensureTable();
    void refreshTable();
    void refreshTableWithFilters();
    void clearForm();
    bool readForm(QString &ref, QString &nom, QString &marque, QString &modele,
                  QString &couleur, QString &numeroSerie, QString &type,
                  QString &etat, QString &technicien, int &prix);
};

#endif // GESTIONOBJETELECTRONIQUE_H
