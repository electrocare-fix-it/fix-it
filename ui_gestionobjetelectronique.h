/********************************************************************************
** Form generated from reading UI file 'gestionobjetelectronique.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTIONOBJETELECTRONIQUE_H
#define UI_GESTIONOBJETELECTRONIQUE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gestionobjetelectronique
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QFrame *contentFrame;
    QVBoxLayout *contentLayout;
    QLabel *infoLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *gestionobjetelectronique)
    {
        if (gestionobjetelectronique->objectName().isEmpty())
            gestionobjetelectronique->setObjectName("gestionobjetelectronique");
        gestionobjetelectronique->resize(800, 600);
        gestionobjetelectronique->setStyleSheet(QString::fromUtf8("\n"
"    QWidget {\n"
"     background-color: #DDF6FF;\n"
"    }\n"
"    QPushButton {\n"
"     background-color: #8EC7E2;\n"
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
        verticalLayout = new QVBoxLayout(gestionobjetelectronique);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(gestionobjetelectronique);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #0F2A3D; margin: 20px;"));

        verticalLayout->addWidget(titleLabel);

        contentFrame = new QFrame(gestionobjetelectronique);
        contentFrame->setObjectName("contentFrame");
        contentFrame->setStyleSheet(QString::fromUtf8("QFrame { background-color: rgba(255, 255, 255, 0.7); border-radius: 10px; }"));
        contentLayout = new QVBoxLayout(contentFrame);
        contentLayout->setObjectName("contentLayout");
        infoLabel = new QLabel(contentFrame);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; color: #0F2A3D; margin: 20px;"));

        contentLayout->addWidget(infoLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        contentLayout->addItem(verticalSpacer);


        verticalLayout->addWidget(contentFrame);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(gestionobjetelectronique);
        pushButton->setObjectName("pushButton");
        pushButton->setStyleSheet(QString::fromUtf8("font-size: 14px; padding: 10px 20px;"));

        buttonLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(gestionobjetelectronique);

        QMetaObject::connectSlotsByName(gestionobjetelectronique);
    } // setupUi

    void retranslateUi(QWidget *gestionobjetelectronique)
    {
        gestionobjetelectronique->setWindowTitle(QCoreApplication::translate("gestionobjetelectronique", "Gestion Objets \303\211lectroniques", nullptr));
        titleLabel->setText(QCoreApplication::translate("gestionobjetelectronique", "Gestion des Objets \303\211lectroniques", nullptr));
        infoLabel->setText(QCoreApplication::translate("gestionobjetelectronique", "Interface de gestion des objets \303\251lectroniques", nullptr));
        pushButton->setText(QCoreApplication::translate("gestionobjetelectronique", "Retour au Menu Principal", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gestionobjetelectronique: public Ui_gestionobjetelectronique {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONOBJETELECTRONIQUE_H
