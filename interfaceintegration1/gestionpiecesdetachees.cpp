#include "gestionpiecesdetachees.h"
#include "ui_gestionpiecesdetachees.h"

gestionpiecesdetachees::gestionpiecesdetachees(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionpiecesdetachees)
{
    ui->setupUi(this);
}

gestionpiecesdetachees::~gestionpiecesdetachees()
{
    delete ui;
}

void gestionpiecesdetachees::on_pushButton_clicked()
{
    emit HomeCliked();
}

