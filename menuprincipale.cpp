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









