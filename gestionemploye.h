#ifndef GESTIONEMPLOYE_H
#define GESTIONEMPLOYE_H

#include <QWidget>
#include <QDate>
#include <QRegularExpressionValidator>
#include <QShowEvent>

namespace Ui {
class gestionemploye;
}

class ChatbotEmploye;

class gestionemploye : public QWidget
{
    Q_OBJECT

public:
    explicit gestionemploye(QWidget *parent = nullptr);
    ~gestionemploye();

private slots:
    void on_pushButton_clicked();
    void on_btnAjouter_2_clicked();
    void on_btnModifier_2_clicked();
    void on_btnSupprimer_2_clicked();
    void on_btnVider_2_clicked();
    void on_btnExporterPDF_2_clicked();
    void on_btnStatistiques_2_clicked();
    void on_txtRecherche_2_textChanged(const QString& text);
    void on_tableEmployes_2_itemSelectionChanged();
    void on_comboTri_currentIndexChanged(int index);
    void onEmployeeSelectedFromChatbot(int employeeId);
    void onFilterTableRequested(const QString& filter);

signals:
    void homeRequested();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::gestionemploye *ui;
    ChatbotEmploye* m_chatbot;
    int m_currentEmployeeId;  // ID de l'employé actuellement sélectionné pour modification
    
    void configurerValidateurs();
    void remplirFormulaire(int employeeId);
    void forcerStylesTexteNoir();
    bool validerFormulaire();
    bool validerEmail(const QString& email);
    bool validerTelephone(const QString& telephone);
    bool validerNomPrenom(const QString& texte);
    bool validerDateNaissance(const QDate& date);
    void afficherMessageErreur(const QString& titre, const QString& message);
    void afficherMessageSucces(const QString& titre, const QString& message);
    int afficherMessageConfirmation(const QString& titre, const QString& message);
    void chargerEmployes();
    void actualiserTableau();
    bool creerSequenceEmploye();
    int obtenirProchainIdEmploye();
    void appliquerTriDepuisSelection();
};

#endif // GESTIONEMPLOYE_H
