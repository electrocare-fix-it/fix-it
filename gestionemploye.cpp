#include "gestionemploye.h"
#include "ui_gestionemploye.h"

gestionemploye::gestionemploye(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionemploye)
{
    ui->setupUi(this);
}

gestionemploye::~gestionemploye()
{
    delete ui;
}

void gestionemploye::on_pushButton_clicked()
{
    emit HomeRequested();
}