#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ADD USER CODE HERE
    ui->stackedWidget->insertWidget(1,&principale);
    ui->stackedWidget->insertWidget(2,&objets);
<<<<<<< HEAD
    ui->stackedWidget->insertWidget(3,&clients);
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0









    connect(&principale,SIGNAL(homeClicked()),this,SLOT(moveHome()));
    connect(&principale,SIGNAL(openObjects()),this,SLOT(openObjectsPage()));
<<<<<<< HEAD
    connect(&principale,SIGNAL(openClients()),this,SLOT(openClientsPage()));
    connect(&objets,SIGNAL(homeRequested()),this,SLOT(moveHome()));
    connect(&clients,SIGNAL(HomeCliked()),this,SLOT(moveHome()));
=======
    connect(&objets,SIGNAL(homeRequested()),this,SLOT(moveHome()));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::moveHome()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::openObjectsPage()
{
    ui->stackedWidget->setCurrentIndex(2);
}

<<<<<<< HEAD
void MainWindow::openClientsPage()
{
    ui->stackedWidget->setCurrentIndex(3);
}

=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
