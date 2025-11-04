#ifndef GESTIONCLIENT_H
#define GESTIONCLIENT_H

#include <QWidget>
#include <QList>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include "client.h"
#include "database.h"

namespace Ui {
class gestionclient;
}

class gestionclient : public QWidget
{
    Q_OBJECT

public:
    explicit gestionclient(QWidget *parent = nullptr);
    ~gestionclient();

private:
    Ui::gestionclient *ui;
    int m_currentIndex;

    void actualiserTableau();
    void viderFormulaire();
    void remplirFormulaire(const Client& client);
    bool validerFormulaire();
    int trouverClientParCin(const QString& cin);
    void configurerValidateurs();
    bool validerEmail(const QString& email);
    bool validerTelephone(const QString& telephone);
    bool validerCin(const QString& cin);
    bool validerNomPrenom(const QString& texte);
    bool validerDateNaissance(const QDate& date);
    void afficherMessageErreur(const QString& titre, const QString& message);
    void afficherMessageSucces(const QString& titre, const QString& message);

signals:
    void HomeCliked();

private slots:
    void on_pushButton_clicked();
    void on_btn_addClient_clicked();
    void on_btn_modifier_clicked();
    void on_btn_supprimer_clicked();
    void on_table_clients_itemSelectionChanged();
    void on_btn_search_clicked();
};

#endif // GESTIONCLIENT_H
