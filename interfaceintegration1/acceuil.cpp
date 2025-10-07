#include "acceuil.h"
#include "ui_acceuil.h"

acceuil::acceuil(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::acceuil)
{
    ui->setupUi(this);
}

acceuil::~acceuil()
{
    delete ui;
}
