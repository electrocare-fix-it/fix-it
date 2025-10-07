#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ADD USER CODE HERE
    ui->stackedWidget->insertWidget(1,&employe);
    ui->stackedWidget->insertWidget(2,&client);
    ui->stackedWidget->insertWidget(3,&electro);
    ui->stackedWidget->insertWidget(4,&detaches);

    connect(&employe,SIGNAL(HomeCliked()),this,SLOT(moveHome()));
    connect(&client,SIGNAL(HomeCliked()),this,SLOT(moveHome()));
    connect(&electro,SIGNAL(HomeCliked()),this,SLOT(moveHome()));
    connect(&detaches,SIGNAL(HomeCliked()),this,SLOT(moveHome()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//pour le boutton qui ferme l'application
{
    this->close();
}


void MainWindow::on_pushButton_2_clicked()//pour la gestion de l'emlpoyé
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_3_clicked()//pour la gestion client
{
     ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);//pour la gestion objet electronique
}

void MainWindow::moveHome()
{
    ui->stackedWidget->setCurrentIndex(0);
}




void MainWindow::on_pushButton_5_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}

