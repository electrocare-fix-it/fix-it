#ifndef GESTIONOBJETELECTRONIQUE_H
#define GESTIONOBJETELECTRONIQUE_H

#include <QWidget>
#include <QList>
#include <QRegularExpressionValidator>
#include <QAbstractItemView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "objetelectronique.h"
#include "database.h"
#include "callmebot.h"
#include "arduinoserial.h"

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
    void on_btnCalculerPrix_6_clicked();
    void on_btnExportPDF_6_clicked();
    void on_btnGenererSMS_6_clicked();
    void on_btnEnvoyerSMS_6_clicked();
    void on_pushButton_2_clicked();
    void on_tableWidget_itemSelectionChanged();
    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::gestionobjetelectronique *ui;
    QString m_currentReference;
    bool m_isTableEditMode;
    CallMeBot* m_callMeBot;
    ArduinoSerial* m_arduinoSerial;

    void actualiserTableau();
    void viderFormulaire();
    void remplirFormulaire(const ObjetElectronique& objet);
    bool validerFormulaire();
    bool lireFormulaire(ObjetElectronique& objet);
    void configurerValidateurs();
    void afficherMessageErreur(const QString& titre, const QString& message);
    void afficherMessageSucces(const QString& titre, const QString& message);
    int calculerPrixEstime(const QString& probleme);
    void exporterPDF();
    void remplirComboClient();
    void remplirComboObjets();
    void remplirComboPiecesDetachees();
    QString genererWhatsApp(const ObjetElectronique& objet);
    void afficherStatistiques();
    void traiterCinArduino(const QString& cin);
};

#endif // GESTIONOBJETELECTRONIQUE_H
