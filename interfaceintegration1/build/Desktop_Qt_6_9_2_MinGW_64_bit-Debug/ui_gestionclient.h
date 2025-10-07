/********************************************************************************
** Form generated from reading UI file 'gestionclient.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTIONCLIENT_H
#define UI_GESTIONCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gestionclient
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGroupBox *groupBox_history;
    QVBoxLayout *verticalLayout_history;
    QLabel *label_selectClient;
    QComboBox *comboBox_selectClient;
    QLabel *label_historyDetails;
    QTextEdit *textEdit_history;
    QPushButton *btn_showHistory;
    QGroupBox *groupBox_listClients;
    QVBoxLayout *verticalLayout_list;
    QHBoxLayout *searchLayout;
    QLabel *label_search;
    QLineEdit *lineEdit_search;
    QPushButton *btn_search_2;
    QPushButton *btn_search;
    QTableWidget *table_clients;
    QHBoxLayout *buttonsLayout;
    QPushButton *btn_modifier;
    QPushButton *btn_supprimer;
    QPushButton *btn_modifier_2;
    QPushButton *btn_notifier;
    QGroupBox *groupBox_addClient;
    QVBoxLayout *verticalLayout_addClient;
    QLabel *label_5;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLabel *label;
    QLineEdit *lineEdit_3;
    QLabel *label_2;
    QLineEdit *lineEdit_5;
    QLabel *label_3;
    QLineEdit *lineEdit_4;
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QLabel *label_7;
    QDateEdit *dateEdit;
    QPushButton *btn_addClient;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;
    QLabel *lblTitle;

    void setupUi(QWidget *gestionclient)
    {
        if (gestionclient->objectName().isEmpty())
            gestionclient->setObjectName("gestionclient");
        gestionclient->resize(1711, 877);
        stackedWidget = new QStackedWidget(gestionclient);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 20, 1271, 841));
        page = new QWidget();
        page->setObjectName("page");
        groupBox_history = new QGroupBox(page);
        groupBox_history->setObjectName("groupBox_history");
        groupBox_history->setGeometry(QRect(590, 89, 561, 391));
        groupBox_history->setStyleSheet(QString::fromUtf8("color: #4682B4;"));
        verticalLayout_history = new QVBoxLayout(groupBox_history);
        verticalLayout_history->setSpacing(12);
        verticalLayout_history->setObjectName("verticalLayout_history");
        verticalLayout_history->setContentsMargins(15, 15, 15, 15);
        label_selectClient = new QLabel(groupBox_history);
        label_selectClient->setObjectName("label_selectClient");

        verticalLayout_history->addWidget(label_selectClient);

        comboBox_selectClient = new QComboBox(groupBox_history);
        comboBox_selectClient->setObjectName("comboBox_selectClient");
        comboBox_selectClient->setEditable(false);

        verticalLayout_history->addWidget(comboBox_selectClient);

        label_historyDetails = new QLabel(groupBox_history);
        label_historyDetails->setObjectName("label_historyDetails");

        verticalLayout_history->addWidget(label_historyDetails);

        textEdit_history = new QTextEdit(groupBox_history);
        textEdit_history->setObjectName("textEdit_history");
        textEdit_history->setReadOnly(true);

        verticalLayout_history->addWidget(textEdit_history);

        btn_showHistory = new QPushButton(groupBox_history);
        btn_showHistory->setObjectName("btn_showHistory");
        btn_showHistory->setMinimumSize(QSize(0, 55));
        QFont font;
        font.setBold(true);
        btn_showHistory->setFont(font);
        btn_showHistory->setStyleSheet(QString::fromUtf8("background-color: #5DADE2;\n"
"color:balck;"));

        verticalLayout_history->addWidget(btn_showHistory);

        groupBox_listClients = new QGroupBox(page);
        groupBox_listClients->setObjectName("groupBox_listClients");
        groupBox_listClients->setGeometry(QRect(0, 500, 1151, 241));
        verticalLayout_list = new QVBoxLayout(groupBox_listClients);
        verticalLayout_list->setSpacing(10);
        verticalLayout_list->setObjectName("verticalLayout_list");
        verticalLayout_list->setContentsMargins(10, 10, 10, 10);
        searchLayout = new QHBoxLayout();
        searchLayout->setSpacing(10);
        searchLayout->setObjectName("searchLayout");
        label_search = new QLabel(groupBox_listClients);
        label_search->setObjectName("label_search");

        searchLayout->addWidget(label_search);

        lineEdit_search = new QLineEdit(groupBox_listClients);
        lineEdit_search->setObjectName("lineEdit_search");

        searchLayout->addWidget(lineEdit_search);

        btn_search_2 = new QPushButton(groupBox_listClients);
        btn_search_2->setObjectName("btn_search_2");
        btn_search_2->setMinimumSize(QSize(120, 55));
        btn_search_2->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6;"));

        searchLayout->addWidget(btn_search_2);

        btn_search = new QPushButton(groupBox_listClients);
        btn_search->setObjectName("btn_search");
        btn_search->setMinimumSize(QSize(120, 55));
        btn_search->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;"));

        searchLayout->addWidget(btn_search);


        verticalLayout_list->addLayout(searchLayout);

        table_clients = new QTableWidget(groupBox_listClients);
        if (table_clients->columnCount() < 7)
            table_clients->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_clients->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        table_clients->setObjectName("table_clients");
        table_clients->setAlternatingRowColors(true);
        table_clients->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        table_clients->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        table_clients->setSortingEnabled(true);

        verticalLayout_list->addWidget(table_clients);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setSpacing(15);
        buttonsLayout->setObjectName("buttonsLayout");
        btn_modifier = new QPushButton(groupBox_listClients);
        btn_modifier->setObjectName("btn_modifier");
        btn_modifier->setMinimumSize(QSize(150, 55));
        btn_modifier->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6;"));

        buttonsLayout->addWidget(btn_modifier);

        btn_supprimer = new QPushButton(groupBox_listClients);
        btn_supprimer->setObjectName("btn_supprimer");
        btn_supprimer->setMinimumSize(QSize(150, 55));
        btn_supprimer->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6;"));

        buttonsLayout->addWidget(btn_supprimer);

        btn_modifier_2 = new QPushButton(groupBox_listClients);
        btn_modifier_2->setObjectName("btn_modifier_2");
        btn_modifier_2->setMinimumSize(QSize(150, 55));
        btn_modifier_2->setStyleSheet(QString::fromUtf8("background-color: #5DADE2;"));

        buttonsLayout->addWidget(btn_modifier_2);

        btn_notifier = new QPushButton(groupBox_listClients);
        btn_notifier->setObjectName("btn_notifier");
        btn_notifier->setMinimumSize(QSize(150, 55));
        btn_notifier->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;"));

        buttonsLayout->addWidget(btn_notifier);


        verticalLayout_list->addLayout(buttonsLayout);

        groupBox_addClient = new QGroupBox(page);
        groupBox_addClient->setObjectName("groupBox_addClient");
        groupBox_addClient->setGeometry(QRect(20, 60, 561, 431));
        groupBox_addClient->setStyleSheet(QString::fromUtf8("\n"
"color: #4682B4;"));
        verticalLayout_addClient = new QVBoxLayout(groupBox_addClient);
        verticalLayout_addClient->setSpacing(8);
        verticalLayout_addClient->setObjectName("verticalLayout_addClient");
        verticalLayout_addClient->setContentsMargins(15, 15, 15, 15);
        label_5 = new QLabel(groupBox_addClient);
        label_5->setObjectName("label_5");

        verticalLayout_addClient->addWidget(label_5);

        lineEdit_2 = new QLineEdit(groupBox_addClient);
        lineEdit_2->setObjectName("lineEdit_2");

        verticalLayout_addClient->addWidget(lineEdit_2);

        label_4 = new QLabel(groupBox_addClient);
        label_4->setObjectName("label_4");

        verticalLayout_addClient->addWidget(label_4);

        lineEdit = new QLineEdit(groupBox_addClient);
        lineEdit->setObjectName("lineEdit");

        verticalLayout_addClient->addWidget(lineEdit);

        label = new QLabel(groupBox_addClient);
        label->setObjectName("label");

        verticalLayout_addClient->addWidget(label);

        lineEdit_3 = new QLineEdit(groupBox_addClient);
        lineEdit_3->setObjectName("lineEdit_3");

        verticalLayout_addClient->addWidget(lineEdit_3);

        label_2 = new QLabel(groupBox_addClient);
        label_2->setObjectName("label_2");

        verticalLayout_addClient->addWidget(label_2);

        lineEdit_5 = new QLineEdit(groupBox_addClient);
        lineEdit_5->setObjectName("lineEdit_5");

        verticalLayout_addClient->addWidget(lineEdit_5);

        label_3 = new QLabel(groupBox_addClient);
        label_3->setObjectName("label_3");

        verticalLayout_addClient->addWidget(label_3);

        lineEdit_4 = new QLineEdit(groupBox_addClient);
        lineEdit_4->setObjectName("lineEdit_4");

        verticalLayout_addClient->addWidget(lineEdit_4);

        label_6 = new QLabel(groupBox_addClient);
        label_6->setObjectName("label_6");

        verticalLayout_addClient->addWidget(label_6);

        lineEdit_6 = new QLineEdit(groupBox_addClient);
        lineEdit_6->setObjectName("lineEdit_6");

        verticalLayout_addClient->addWidget(lineEdit_6);

        label_7 = new QLabel(groupBox_addClient);
        label_7->setObjectName("label_7");

        verticalLayout_addClient->addWidget(label_7);

        dateEdit = new QDateEdit(groupBox_addClient);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        verticalLayout_addClient->addWidget(dateEdit);

        btn_addClient = new QPushButton(groupBox_addClient);
        btn_addClient->setObjectName("btn_addClient");
        btn_addClient->setMinimumSize(QSize(0, 55));
        btn_addClient->setFont(font);
        btn_addClient->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;\n"
"color:black;"));

        verticalLayout_addClient->addWidget(btn_addClient);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_addClient->addItem(verticalSpacer);

        pushButton = new QPushButton(page);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(890, 60, 261, 29));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(148, 255, 191);\n"
"color: rgb(7, 7, 7);"));
        lblTitle = new QLabel(page);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setGeometry(QRect(-10, -10, 1311, 62));
        lblTitle->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C5F75; padding: 15px; background-color: #87CEEB; border-radius: 10px;"));
        lblTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        stackedWidget->addWidget(page);

        retranslateUi(gestionclient);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(gestionclient);
    } // setupUi

    void retranslateUi(QWidget *gestionclient)
    {
        gestionclient->setWindowTitle(QCoreApplication::translate("gestionclient", "Form", nullptr));
        groupBox_history->setTitle(QCoreApplication::translate("gestionclient", "\360\237\223\232 Historique des Clients", nullptr));
        label_selectClient->setText(QCoreApplication::translate("gestionclient", "\360\237\224\275 S\303\251lectionner un Client:", nullptr));
        label_historyDetails->setText(QCoreApplication::translate("gestionclient", "\360\237\223\235 D\303\251tails de l'Historique:", nullptr));
        textEdit_history->setPlaceholderText(QCoreApplication::translate("gestionclient", "L'historique du client s\303\251lectionn\303\251 s'affichera ici...", nullptr));
        btn_showHistory->setText(QCoreApplication::translate("gestionclient", "\360\237\224\215 Afficher Historique", nullptr));
        groupBox_listClients->setTitle(QCoreApplication::translate("gestionclient", "\360\237\223\213 Liste des Clients", nullptr));
        label_search->setText(QCoreApplication::translate("gestionclient", "\360\237\224\215 Recherche:", nullptr));
        lineEdit_search->setPlaceholderText(QCoreApplication::translate("gestionclient", "Rechercher par nom, CIN, t\303\251l\303\251phone, email...", nullptr));
        btn_search_2->setText(QCoreApplication::translate("gestionclient", "statistiques", nullptr));
        btn_search->setText(QCoreApplication::translate("gestionclient", "Rechercher", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table_clients->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("gestionclient", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table_clients->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("gestionclient", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table_clients->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("gestionclient", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table_clients->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("gestionclient", "T\303\251l\303\251phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table_clients->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("gestionclient", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table_clients->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("gestionclient", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_clients->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("gestionclient", "Date de Naissance", nullptr));
        btn_modifier->setText(QCoreApplication::translate("gestionclient", "\342\234\217\357\270\217 Modifier", nullptr));
        btn_supprimer->setText(QCoreApplication::translate("gestionclient", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        btn_modifier_2->setText(QCoreApplication::translate("gestionclient", "\360\237\223\204 Exportation PDF", nullptr));
        btn_notifier->setText(QCoreApplication::translate("gestionclient", "\360\237\223\247 Notifier", nullptr));
        groupBox_addClient->setTitle(QCoreApplication::translate("gestionclient", "\342\236\225 Ajouter un Client", nullptr));
        label_5->setText(QCoreApplication::translate("gestionclient", "\360\237\221\244 Nom du Client:", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("gestionclient", "Entrez le nom du client", nullptr));
        label_4->setText(QCoreApplication::translate("gestionclient", "\360\237\221\244 Pr\303\251nom du Client:", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("gestionclient", "Entrez le pr\303\251nom du client", nullptr));
        label->setText(QCoreApplication::translate("gestionclient", "\360\237\206\224 CIN:", nullptr));
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("gestionclient", "Num\303\251ro de CIN", nullptr));
        label_2->setText(QCoreApplication::translate("gestionclient", "\360\237\223\236 T\303\251l\303\251phone:", nullptr));
        lineEdit_5->setPlaceholderText(QCoreApplication::translate("gestionclient", "+216 XX XXX XXX", nullptr));
        label_3->setText(QCoreApplication::translate("gestionclient", "\360\237\223\247 Email:", nullptr));
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("gestionclient", "email@exemple.com", nullptr));
        label_6->setText(QCoreApplication::translate("gestionclient", "\360\237\223\215 Adresse:", nullptr));
        lineEdit_6->setPlaceholderText(QCoreApplication::translate("gestionclient", "Adresse compl\303\250te", nullptr));
        label_7->setText(QCoreApplication::translate("gestionclient", "\360\237\223\205 Date de Naissance:", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("gestionclient", "dd/MM/yyyy", nullptr));
        btn_addClient->setText(QCoreApplication::translate("gestionclient", "\342\234\205 Ajouter le Client", nullptr));
        pushButton->setText(QCoreApplication::translate("gestionclient", "Home ", nullptr));
        lblTitle->setText(QCoreApplication::translate("gestionclient", "GESTION DES CLIENTS", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gestionclient: public Ui_gestionclient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONCLIENT_H
