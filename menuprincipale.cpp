#include "menuprincipale.h"
#include "ui_menuprincipale.h"
#include <QPushButton>

menuprincipale::menuprincipale(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menuprincipale)
{
    ui->setupUi(this);
}

void menuprincipale::setButtonVisible(const QString& buttonName, bool visible)
{
    QPushButton* button = this->findChild<QPushButton*>(buttonName);
    if (button) {
        button->setVisible(visible);
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









