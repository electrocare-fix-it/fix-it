#include "menuprincipale.h"
#include "ui_menuprincipale.h"
#include <QPushButton>

menuprincipale::menuprincipale(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menuprincipale)
{
    ui->setupUi(this);
}

void menuprincipale::setButtonAccessible(const QString& buttonName, bool enabled)
{
    QPushButton* button = this->findChild<QPushButton*>(buttonName);
    if (!button) {
        return;
    }

    button->setVisible(true);
    button->setEnabled(enabled);

    if (enabled) {
        // Style bleu clair pour les boutons autorisés
        button->setStyleSheet(
            "QPushButton {"
            "background-color: #8EC7E2;"
            "color: #212529;"
            "border: 2px solid #6BB3D4;"
            "border-radius: 15px;"
            "padding: 12px;"
            "font-weight: 600;"
            "font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "background-color: #7ABEDB;"
            "border-color: #5AA3C4;"
            "}"
            "QPushButton:pressed {"
            "background-color: #6BB3D4;"
            "border-color: #4A93B4;"
            "}"
        );
    } else {
        // Style gris pour les boutons non autorisés
        button->setStyleSheet(
            "QPushButton {"
            "background-color: #dfe3e8;"
            "color: #8a8f99;"
            "border: 2px dashed #b5bcc5;"
            "border-radius: 15px;"
            "padding: 12px;"
            "font-weight: 600;"
            "}"
        );
    }
}

menuprincipale::~menuprincipale()
{
    delete ui;
}

void menuprincipale::on_homeButton_clicked()
{
    emit homeClicked();
}

void menuprincipale::on_gestionObjectsButton_clicked()
{
    emit openObjects();
}

void menuprincipale::on_gestionClientsButton_clicked()
{
    emit openClients();
}

void menuprincipale::on_gestionEmployeButton_clicked()
{
    emit openEmployes();
}

void menuprincipale::on_gestionPiecesButton_clicked()
{
    emit openPieces();
}









