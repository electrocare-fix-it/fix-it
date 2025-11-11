#include "gestionclient.h"
#include "ui_gestionclient.h"

gestionclient::gestionclient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionclient)
{
    ui->setupUi(this);
}

gestionclient::~gestionclient()
{
    delete ui;
}

void gestionclient::on_pushButton_clicked()
{
    emit HomeCliked();
}

