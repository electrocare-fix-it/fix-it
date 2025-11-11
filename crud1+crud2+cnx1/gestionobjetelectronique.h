#ifndef GESTIONOBJETELECTRONIQUE_H
#define GESTIONOBJETELECTRONIQUE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVector>
#include <QPair>
#include <QAbstractItemView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "connection.h"

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
    qlonglong currentSelectedId;  // ID_OBJET au lieu de reference
    bool isTableEditMode = false;
    QString tableName;  // Nom correct de la table (détecté automatiquement)

    void openDatabase();
    QString detectTableName();  // Détecter le nom correct de la table
    void refreshTable();
    void refreshTableWithFilters();
    void clearForm();
    qlonglong getNextId();  // Obtenir le prochain ID depuis la séquence Oracle
    bool readForm(qlonglong &id, QString &nom, QString &marque, QString &modele,
                  QString &couleur, QString &numeroSerie, QString &type,
                  QString &etat, qlonglong &idClient, qlonglong &idEmploye);
    bool validateFields(const QString &nom, const QString &marque,
                        const QString &modele, const QString &couleur, const QString &numeroSerie,
                        const QString &type, const QString &etat);
};

#endif // GESTIONOBJETELECTRONIQUE_H
