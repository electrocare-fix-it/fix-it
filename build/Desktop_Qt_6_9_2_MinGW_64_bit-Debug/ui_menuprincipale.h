/********************************************************************************
** Form generated from reading UI file 'menuprincipale.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENUPRINCIPALE_H
#define UI_MENUPRINCIPALE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_menuprincipale
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayout;
    QLabel *titleLabel;
    QPushButton *homeButton;
    QFrame *contentFrame;
    QGridLayout *gridLayout;
    QPushButton *gestionEmployeButton;
    QPushButton *gestionClientsButton;
    QPushButton *gestionObjectsButton;
    QPushButton *gestionPiecesButton;
    QLabel *footerLabel;

    void setupUi(QWidget *menuprincipale)
    {
        if (menuprincipale->objectName().isEmpty())
            menuprincipale->setObjectName("menuprincipale");
        menuprincipale->resize(1325, 714);
        menuprincipale->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow, QWidget {\n"
"     background-color: #DDF6FF; /* light cyan like screenshot */\n"
"    }\n"
"    QPushButton {\n"
"     background-color: #8EC7E2; /* blue buttons */\n"
"     color: #0F2A3D;\n"
"     border: 1px solid #6FB6D6;\n"
"     border-radius: 6px;\n"
"     padding: 6px 12px;\n"
"     font-weight: normal;\n"
"    }\n"
"    QPushButton:hover {\n"
"     background-color: #7ABEDB;\n"
"    }\n"
"    QPushButton:pressed {\n"
"     background-color: #6BB3D4;\n"
"    }\n"
"   "));
        verticalLayout = new QVBoxLayout(menuprincipale);
        verticalLayout->setObjectName("verticalLayout");
        headerFrame = new QFrame(menuprincipale);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame { background-color: transparent; border: none; }"));
        horizontalLayout = new QHBoxLayout(headerFrame);
        horizontalLayout->setObjectName("horizontalLayout");
        titleLabel = new QLabel(headerFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("\n"
"           QLabel {\n"
"            color: #2C3E50;\n"
"            font-size: 28px;\n"
"            font-weight: bold;\n"
"            padding: 10px;\n"
"           }\n"
"          "));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(titleLabel);

        homeButton = new QPushButton(headerFrame);
        homeButton->setObjectName("homeButton");
        homeButton->setMinimumSize(QSize(100, 40));
        homeButton->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(homeButton);


        verticalLayout->addWidget(headerFrame);

        contentFrame = new QFrame(menuprincipale);
        contentFrame->setObjectName("contentFrame");
        contentFrame->setStyleSheet(QString::fromUtf8("\n"
"        QFrame {\n"
"         background-color: transparent;\n"
"        }\n"
"       "));
        gridLayout = new QGridLayout(contentFrame);
        gridLayout->setSpacing(30);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(100, -1, 100, -1);
        gestionEmployeButton = new QPushButton(contentFrame);
        gestionEmployeButton->setObjectName("gestionEmployeButton");
        gestionEmployeButton->setMinimumSize(QSize(280, 120));
        gestionEmployeButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(gestionEmployeButton, 0, 0, 1, 1);

        gestionClientsButton = new QPushButton(contentFrame);
        gestionClientsButton->setObjectName("gestionClientsButton");
        gestionClientsButton->setMinimumSize(QSize(280, 120));
        gestionClientsButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(gestionClientsButton, 0, 1, 1, 1);

        gestionObjectsButton = new QPushButton(contentFrame);
        gestionObjectsButton->setObjectName("gestionObjectsButton");
        gestionObjectsButton->setMinimumSize(QSize(280, 120));
        gestionObjectsButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(gestionObjectsButton, 1, 0, 1, 1);

        gestionPiecesButton = new QPushButton(contentFrame);
        gestionPiecesButton->setObjectName("gestionPiecesButton");
        gestionPiecesButton->setMinimumSize(QSize(280, 120));
        gestionPiecesButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(gestionPiecesButton, 1, 1, 1, 1);


        verticalLayout->addWidget(contentFrame);

        footerLabel = new QLabel(menuprincipale);
        footerLabel->setObjectName("footerLabel");
        footerLabel->setStyleSheet(QString::fromUtf8("\n"
"        QLabel {\n"
"         color: #2C3E50;\n"
"         font-size: 12px;\n"
"         padding: 10px;\n"
"        }\n"
"       "));
        footerLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(footerLabel);


        retranslateUi(menuprincipale);

        QMetaObject::connectSlotsByName(menuprincipale);
    } // setupUi

    void retranslateUi(QWidget *menuprincipale)
    {
        menuprincipale->setWindowTitle(QCoreApplication::translate("menuprincipale", "Menu Principal - Gestion", nullptr));
        titleLabel->setText(QCoreApplication::translate("menuprincipale", "MENU PRINCIPAL", nullptr));
        homeButton->setText(QCoreApplication::translate("menuprincipale", "Home", nullptr));
        gestionEmployeButton->setText(QCoreApplication::translate("menuprincipale", "\360\237\221\245 Gestion Employ\303\251s", nullptr));
        gestionClientsButton->setText(QCoreApplication::translate("menuprincipale", "\360\237\221\244 Gestion Clients", nullptr));
        gestionObjectsButton->setText(QCoreApplication::translate("menuprincipale", "\360\237\223\261 Gestion Objets \303\211lectroniques", nullptr));
        gestionPiecesButton->setText(QCoreApplication::translate("menuprincipale", "\360\237\224\247 Gestion Pi\303\250ces D\303\251tach\303\251es", nullptr));
        footerLabel->setText(QCoreApplication::translate("menuprincipale", "Syst\303\250me de Gestion - Version 1.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class menuprincipale: public Ui_menuprincipale {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENUPRINCIPALE_H
