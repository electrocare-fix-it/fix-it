#include "menuprincipale.h"
#include "ui_menuprincipale.h"

menuprincipale::menuprincipale(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menuprincipale)
{
    ui->setupUi(this);
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

<<<<<<< HEAD
void menuprincipale::on_gestionClientsButton_clicked()
{
    emit openClients();
}

=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0








