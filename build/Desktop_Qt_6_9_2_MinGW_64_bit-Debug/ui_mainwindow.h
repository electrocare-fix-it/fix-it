/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton_2;
    QLabel *label_5;
    QLabel *label_6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1388, 875);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color: #E0F6FF;\n"
""));
        page = new QWidget();
        page->setObjectName("page");
        pushButton = new QPushButton(page);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(960, 700, 191, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;\n"
"font: 14pt \"Segoe UI\";\n"
""));
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(-30, 140, 1211, 91));
        label->setStyleSheet(QString::fromUtf8("font: 36pt \"Segoe UI\";"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(370, 310, 91, 20));
        label_2->setStyleSheet(QString::fromUtf8("color:black;\n"
"font: 14pt \"Segoe UI\";"));
        label_3 = new QLabel(page);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(370, 380, 111, 21));
        label_3->setStyleSheet(QString::fromUtf8("color:black;\n"
"font: 14pt \"Segoe UI\";"));
        label_4 = new QLabel(page);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(370, 450, 111, 21));
        label_4->setStyleSheet(QString::fromUtf8("color:balck;\n"
"font: 14pt \"Segoe UI\";"));
        lineEdit = new QLineEdit(page);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(510, 300, 201, 31));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(234, 234, 234);\n"
"color: black;\n"
""));
        lineEdit_2 = new QLineEdit(page);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(510, 380, 201, 31));
        lineEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(234, 234, 234);\n"
"color: black;"));
        lineEdit_3 = new QLineEdit(page);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(510, 460, 201, 31));
        lineEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(234, 234, 234);\n"
"color:black;"));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(550, 540, 131, 31));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;\n"
"font: 14pt \"Segoe UI\";\n"
""));
        label_5 = new QLabel(page);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 40, 261, 171));
        label_5->setStyleSheet(QString::fromUtf8("image: url(:/res/background.png);"));
        label_6 = new QLabel(page);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(940, 40, 191, 171));
        label_6->setStyleSheet(QString::fromUtf8("image: url(:/res/equipe.png);"));
        stackedWidget->addWidget(page);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1388, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "close application ", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "WELCOME", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "LOGGIN", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Role", nullptr));
        lineEdit->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "login", nullptr));
        label_5->setText(QString());
        label_6->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
