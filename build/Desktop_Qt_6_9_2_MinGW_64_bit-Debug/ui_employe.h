/********************************************************************************
** Form generated from reading UI file 'employe.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYE_H
#define UI_EMPLOYE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_employe
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QLabel *lblTitle;
    QGroupBox *groupBoxTableau;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *txtRecherche_2;
    QPushButton *btnExporterPDF_2;
    QPushButton *btnStatistiques_2;
    QTableWidget *tableEmployes_2;
    QGroupBox *groupBoxChatbot_2;
    QVBoxLayout *verticalLayout_7;
    QTextEdit *txtChatDisplay_2;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *txtChatInput_2;
    QPushButton *btnEnvoyer_2;
    QGroupBox *groupBoxFormulaire_2;
    QVBoxLayout *verticalLayout_6;
    QFormLayout *formLayout_2;
    QLabel *lblNom_2;
    QLineEdit *txtNom_2;
    QLabel *lblPrenom_2;
    QLineEdit *txtPrenom_2;
    QLabel *lblDateNaissance_2;
    QDateEdit *dateNaissance_2;
    QLabel *lblAdresse_2;
    QLineEdit *txtAdresse_2;
    QLabel *lblTelephone_2;
    QLineEdit *txtTelephone_2;
    QLabel *lblEmail_2;
    QLineEdit *txtEmail_2;
    QLabel *lblStatut_2;
    QComboBox *comboStatut_2;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnAjouter_2;
    QPushButton *btnModifier_2;
    QPushButton *btnSupprimer_2;
    QPushButton *btnVider_2;
    QPushButton *pushButton;

    void setupUi(QWidget *employe)
    {
        if (employe->objectName().isEmpty())
            employe->setObjectName("employe");
        employe->resize(1341, 863);
        stackedWidget = new QStackedWidget(employe);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(50, 10, 1178, 831));
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        lblTitle = new QLabel(page_3);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setGeometry(QRect(10, 10, 1311, 62));
        lblTitle->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C5F75; padding: 15px; background-color: #90C9A8; border-radius: 10px;"));
        lblTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBoxTableau = new QGroupBox(page_3);
        groupBoxTableau->setObjectName("groupBoxTableau");
        groupBoxTableau->setGeometry(QRect(-10, 470, 1121, 221));
        verticalLayout_5 = new QVBoxLayout(groupBoxTableau);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        txtRecherche_2 = new QLineEdit(groupBoxTableau);
        txtRecherche_2->setObjectName("txtRecherche_2");

        horizontalLayout_5->addWidget(txtRecherche_2);

        btnExporterPDF_2 = new QPushButton(groupBoxTableau);
        btnExporterPDF_2->setObjectName("btnExporterPDF_2");
        btnExporterPDF_2->setStyleSheet(QString::fromUtf8("background-color: #7EC8E3;"));

        horizontalLayout_5->addWidget(btnExporterPDF_2);

        btnStatistiques_2 = new QPushButton(groupBoxTableau);
        btnStatistiques_2->setObjectName("btnStatistiques_2");
        btnStatistiques_2->setStyleSheet(QString::fromUtf8("background-color: #C8A5E8;"));

        horizontalLayout_5->addWidget(btnStatistiques_2);


        verticalLayout_5->addLayout(horizontalLayout_5);

        tableEmployes_2 = new QTableWidget(groupBoxTableau);
        if (tableEmployes_2->columnCount() < 8)
            tableEmployes_2->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableEmployes_2->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableEmployes_2->setObjectName("tableEmployes_2");
        tableEmployes_2->setAlternatingRowColors(true);
        tableEmployes_2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableEmployes_2->setSortingEnabled(true);

        verticalLayout_5->addWidget(tableEmployes_2);

        groupBoxChatbot_2 = new QGroupBox(page_3);
        groupBoxChatbot_2->setObjectName("groupBoxChatbot_2");
        groupBoxChatbot_2->setGeometry(QRect(730, 114, 381, 351));
        verticalLayout_7 = new QVBoxLayout(groupBoxChatbot_2);
        verticalLayout_7->setObjectName("verticalLayout_7");
        txtChatDisplay_2 = new QTextEdit(groupBoxChatbot_2);
        txtChatDisplay_2->setObjectName("txtChatDisplay_2");
        txtChatDisplay_2->setReadOnly(true);

        verticalLayout_7->addWidget(txtChatDisplay_2);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        txtChatInput_2 = new QLineEdit(groupBoxChatbot_2);
        txtChatInput_2->setObjectName("txtChatInput_2");

        horizontalLayout_8->addWidget(txtChatInput_2);

        btnEnvoyer_2 = new QPushButton(groupBoxChatbot_2);
        btnEnvoyer_2->setObjectName("btnEnvoyer_2");
        btnEnvoyer_2->setStyleSheet(QString::fromUtf8("background-color:  #90C9A8;"));

        horizontalLayout_8->addWidget(btnEnvoyer_2);


        verticalLayout_7->addLayout(horizontalLayout_8);

        groupBoxFormulaire_2 = new QGroupBox(page_3);
        groupBoxFormulaire_2->setObjectName("groupBoxFormulaire_2");
        groupBoxFormulaire_2->setGeometry(QRect(10, 74, 711, 391));
        verticalLayout_6 = new QVBoxLayout(groupBoxFormulaire_2);
        verticalLayout_6->setObjectName("verticalLayout_6");
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName("formLayout_2");
        lblNom_2 = new QLabel(groupBoxFormulaire_2);
        lblNom_2->setObjectName("lblNom_2");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, lblNom_2);

        txtNom_2 = new QLineEdit(groupBoxFormulaire_2);
        txtNom_2->setObjectName("txtNom_2");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, txtNom_2);

        lblPrenom_2 = new QLabel(groupBoxFormulaire_2);
        lblPrenom_2->setObjectName("lblPrenom_2");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::LabelRole, lblPrenom_2);

        txtPrenom_2 = new QLineEdit(groupBoxFormulaire_2);
        txtPrenom_2->setObjectName("txtPrenom_2");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::FieldRole, txtPrenom_2);

        lblDateNaissance_2 = new QLabel(groupBoxFormulaire_2);
        lblDateNaissance_2->setObjectName("lblDateNaissance_2");

        formLayout_2->setWidget(2, QFormLayout::ItemRole::LabelRole, lblDateNaissance_2);

        dateNaissance_2 = new QDateEdit(groupBoxFormulaire_2);
        dateNaissance_2->setObjectName("dateNaissance_2");
        dateNaissance_2->setCalendarPopup(true);

        formLayout_2->setWidget(2, QFormLayout::ItemRole::FieldRole, dateNaissance_2);

        lblAdresse_2 = new QLabel(groupBoxFormulaire_2);
        lblAdresse_2->setObjectName("lblAdresse_2");

        formLayout_2->setWidget(3, QFormLayout::ItemRole::LabelRole, lblAdresse_2);

        txtAdresse_2 = new QLineEdit(groupBoxFormulaire_2);
        txtAdresse_2->setObjectName("txtAdresse_2");

        formLayout_2->setWidget(3, QFormLayout::ItemRole::FieldRole, txtAdresse_2);

        lblTelephone_2 = new QLabel(groupBoxFormulaire_2);
        lblTelephone_2->setObjectName("lblTelephone_2");

        formLayout_2->setWidget(4, QFormLayout::ItemRole::LabelRole, lblTelephone_2);

        txtTelephone_2 = new QLineEdit(groupBoxFormulaire_2);
        txtTelephone_2->setObjectName("txtTelephone_2");

        formLayout_2->setWidget(4, QFormLayout::ItemRole::FieldRole, txtTelephone_2);

        lblEmail_2 = new QLabel(groupBoxFormulaire_2);
        lblEmail_2->setObjectName("lblEmail_2");

        formLayout_2->setWidget(5, QFormLayout::ItemRole::LabelRole, lblEmail_2);

        txtEmail_2 = new QLineEdit(groupBoxFormulaire_2);
        txtEmail_2->setObjectName("txtEmail_2");

        formLayout_2->setWidget(5, QFormLayout::ItemRole::FieldRole, txtEmail_2);

        lblStatut_2 = new QLabel(groupBoxFormulaire_2);
        lblStatut_2->setObjectName("lblStatut_2");

        formLayout_2->setWidget(6, QFormLayout::ItemRole::LabelRole, lblStatut_2);

        comboStatut_2 = new QComboBox(groupBoxFormulaire_2);
        comboStatut_2->addItem(QString());
        comboStatut_2->addItem(QString());
        comboStatut_2->addItem(QString());
        comboStatut_2->setObjectName("comboStatut_2");

        formLayout_2->setWidget(6, QFormLayout::ItemRole::FieldRole, comboStatut_2);


        verticalLayout_6->addLayout(formLayout_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        btnAjouter_2 = new QPushButton(groupBoxFormulaire_2);
        btnAjouter_2->setObjectName("btnAjouter_2");
        btnAjouter_2->setStyleSheet(QString::fromUtf8("background-color:  #90C9A8;"));

        horizontalLayout_7->addWidget(btnAjouter_2);

        btnModifier_2 = new QPushButton(groupBoxFormulaire_2);
        btnModifier_2->setObjectName("btnModifier_2");
        btnModifier_2->setStyleSheet(QString::fromUtf8("background-color: #7EC8E3;"));

        horizontalLayout_7->addWidget(btnModifier_2);

        btnSupprimer_2 = new QPushButton(groupBoxFormulaire_2);
        btnSupprimer_2->setObjectName("btnSupprimer_2");
        btnSupprimer_2->setStyleSheet(QString::fromUtf8("background-color: #E8A5A5;"));

        horizontalLayout_7->addWidget(btnSupprimer_2);

        btnVider_2 = new QPushButton(groupBoxFormulaire_2);
        btnVider_2->setObjectName("btnVider_2");
        btnVider_2->setStyleSheet(QString::fromUtf8("background-color: #C0C0C0;"));

        horizontalLayout_7->addWidget(btnVider_2);


        verticalLayout_6->addLayout(horizontalLayout_7);

        pushButton = new QPushButton(page_3);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(940, 80, 171, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(148, 255, 191);\n"
"color: rgb(7, 7, 7);"));
        stackedWidget->addWidget(page_3);

        retranslateUi(employe);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(employe);
    } // setupUi

    void retranslateUi(QWidget *employe)
    {
        employe->setWindowTitle(QCoreApplication::translate("employe", "Form", nullptr));
        lblTitle->setText(QCoreApplication::translate("employe", "GESTION DES EMPLOY\303\211S", nullptr));
        groupBoxTableau->setTitle(QCoreApplication::translate("employe", "      Liste des Employ\303\251s", nullptr));
        txtRecherche_2->setPlaceholderText(QCoreApplication::translate("employe", "\360\237\224\215 Rechercher un employ\303\251...", nullptr));
        btnExporterPDF_2->setText(QCoreApplication::translate("employe", "\360\237\223\204 Exporter en PDF", nullptr));
        btnStatistiques_2->setText(QCoreApplication::translate("employe", "\360\237\223\210 Statistiques", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableEmployes_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("employe", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableEmployes_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("employe", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableEmployes_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("employe", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableEmployes_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("employe", "Date de naissance", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableEmployes_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("employe", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableEmployes_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("employe", "T\303\251l\303\251phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableEmployes_2->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("employe", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableEmployes_2->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("employe", "Statut", nullptr));
        groupBoxChatbot_2->setTitle(QCoreApplication::translate("employe", "\360\237\244\226 Assistant IA - Chatbot", nullptr));
        txtChatDisplay_2->setPlaceholderText(QCoreApplication::translate("employe", "Bienvenue! Posez vos questions sur la gestion des employ\303\251s...", nullptr));
        txtChatInput_2->setPlaceholderText(QCoreApplication::translate("employe", "Tapez votre message ici...", nullptr));
        btnEnvoyer_2->setText(QCoreApplication::translate("employe", "\360\237\223\244 Envoyer", nullptr));
        groupBoxFormulaire_2->setTitle(QCoreApplication::translate("employe", "Informations de l'Employ\303\251", nullptr));
        lblNom_2->setText(QCoreApplication::translate("employe", "Nom:", nullptr));
        txtNom_2->setPlaceholderText(QCoreApplication::translate("employe", "Entrez le nom", nullptr));
        lblPrenom_2->setText(QCoreApplication::translate("employe", "Pr\303\251nom:", nullptr));
        txtPrenom_2->setPlaceholderText(QCoreApplication::translate("employe", "Entrez le pr\303\251nom", nullptr));
        lblDateNaissance_2->setText(QCoreApplication::translate("employe", "Date de naissance:", nullptr));
        dateNaissance_2->setDisplayFormat(QCoreApplication::translate("employe", "dd/MM/yyyy", nullptr));
        lblAdresse_2->setText(QCoreApplication::translate("employe", "Adresse:", nullptr));
        txtAdresse_2->setPlaceholderText(QCoreApplication::translate("employe", "Entrez l'adresse", nullptr));
        lblTelephone_2->setText(QCoreApplication::translate("employe", "Num\303\251ro de t\303\251l\303\251phone:", nullptr));
        txtTelephone_2->setPlaceholderText(QCoreApplication::translate("employe", "Entrez le num\303\251ro de t\303\251l\303\251phone", nullptr));
        lblEmail_2->setText(QCoreApplication::translate("employe", "Adresse email:", nullptr));
        txtEmail_2->setPlaceholderText(QCoreApplication::translate("employe", "exemple@email.com", nullptr));
        lblStatut_2->setText(QCoreApplication::translate("employe", "Statut:", nullptr));
        comboStatut_2->setItemText(0, QCoreApplication::translate("employe", "Actif", nullptr));
        comboStatut_2->setItemText(1, QCoreApplication::translate("employe", "Inactif", nullptr));
        comboStatut_2->setItemText(2, QCoreApplication::translate("employe", "En cong\303\251", nullptr));

        btnAjouter_2->setText(QCoreApplication::translate("employe", "\342\236\225 Cr\303\251er", nullptr));
        btnModifier_2->setText(QCoreApplication::translate("employe", "\342\234\217\357\270\217 Modifier", nullptr));
        btnSupprimer_2->setText(QCoreApplication::translate("employe", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        btnVider_2->setText(QCoreApplication::translate("employe", "\360\237\224\204 Vider", nullptr));
        pushButton->setText(QCoreApplication::translate("employe", "Home ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class employe: public Ui_employe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYE_H
