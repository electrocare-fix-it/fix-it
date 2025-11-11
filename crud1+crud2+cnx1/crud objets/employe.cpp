#include "employe.h"
#include "ui_employe.h"

employe::employe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::employe)
{
    ui->setupUi(this);
}

employe::~employe()
{
    delete ui;
}
