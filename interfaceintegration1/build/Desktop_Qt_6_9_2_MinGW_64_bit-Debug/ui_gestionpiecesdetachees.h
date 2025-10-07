/********************************************************************************
** Form generated from reading UI file 'gestionpiecesdetachees.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTIONPIECESDETACHEES_H
#define UI_GESTIONPIECESDETACHEES_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gestionpiecesdetachees
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGroupBox *groupBoxFormulaire;
    QGridLayout *gridLayout_2;
    QLabel *lblReference_2;
    QDoubleSpinBox *spinPrix_2;
    QSpinBox *spinQuantite_2;
    QLineEdit *txtNom_2;
    QLabel *lblDateSortie_2;
    QLineEdit *txtFournisseur_2;
    QDateEdit *dateSortie_2;
    QLabel *lblCategorie_2;
    QLabel *lblQuantite_2;
    QLabel *lblPrix_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnAjouter_2;
    QPushButton *btnModifier_2;
    QPushButton *btnSupprimer_2;
    QPushButton *btnNouveau_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lblDateEntree_2;
    QLabel *lblFournisseur_2;
    QLineEdit *txtReference_2;
    QLabel *lblNom_2;
    QComboBox *cmbCategorie_2;
    QDateEdit *dateEntree_2;
    QGroupBox *groupBoxRecherche;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lblRecherche_2;
    QLineEdit *txtRecherche_2;
    QLabel *lblFiltreCategorie_2;
    QComboBox *cmbFiltreCategorie_2;
    QPushButton *btnRechercher_2;
    QLabel *lblTitre;
    QGroupBox *groupBoxTableau;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tablePieces_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lblAlerteStock_2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnExportPDF_2;
    QPushButton *btnExportExcel_2;
    QPushButton *pushButton;

    void setupUi(QWidget *gestionpiecesdetachees)
    {
        if (gestionpiecesdetachees->objectName().isEmpty())
            gestionpiecesdetachees->setObjectName("gestionpiecesdetachees");
        gestionpiecesdetachees->resize(1219, 822);
        stackedWidget = new QStackedWidget(gestionpiecesdetachees);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(10, 10, 1211, 801));
        page = new QWidget();
        page->setObjectName("page");
        groupBoxFormulaire = new QGroupBox(page);
        groupBoxFormulaire->setObjectName("groupBoxFormulaire");
        groupBoxFormulaire->setGeometry(QRect(20, 79, 1041, 311));
        gridLayout_2 = new QGridLayout(groupBoxFormulaire);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setHorizontalSpacing(15);
        gridLayout_2->setVerticalSpacing(12);
        lblReference_2 = new QLabel(groupBoxFormulaire);
        lblReference_2->setObjectName("lblReference_2");

        gridLayout_2->addWidget(lblReference_2, 0, 0, 1, 1);

        spinPrix_2 = new QDoubleSpinBox(groupBoxFormulaire);
        spinPrix_2->setObjectName("spinPrix_2");
        spinPrix_2->setDecimals(3);
        spinPrix_2->setMinimum(0.000000000000000);
        spinPrix_2->setMaximum(999999.989999999990687);

        gridLayout_2->addWidget(spinPrix_2, 2, 1, 1, 1);

        spinQuantite_2 = new QSpinBox(groupBoxFormulaire);
        spinQuantite_2->setObjectName("spinQuantite_2");
        spinQuantite_2->setMinimum(0);
        spinQuantite_2->setMaximum(99999);

        gridLayout_2->addWidget(spinQuantite_2, 1, 3, 1, 1);

        txtNom_2 = new QLineEdit(groupBoxFormulaire);
        txtNom_2->setObjectName("txtNom_2");

        gridLayout_2->addWidget(txtNom_2, 0, 3, 1, 1);

        lblDateSortie_2 = new QLabel(groupBoxFormulaire);
        lblDateSortie_2->setObjectName("lblDateSortie_2");

        gridLayout_2->addWidget(lblDateSortie_2, 3, 2, 1, 1);

        txtFournisseur_2 = new QLineEdit(groupBoxFormulaire);
        txtFournisseur_2->setObjectName("txtFournisseur_2");

        gridLayout_2->addWidget(txtFournisseur_2, 2, 3, 1, 1);

        dateSortie_2 = new QDateEdit(groupBoxFormulaire);
        dateSortie_2->setObjectName("dateSortie_2");
        dateSortie_2->setCalendarPopup(true);
        dateSortie_2->setDate(QDate(2025, 10, 7));

        gridLayout_2->addWidget(dateSortie_2, 3, 3, 1, 1);

        lblCategorie_2 = new QLabel(groupBoxFormulaire);
        lblCategorie_2->setObjectName("lblCategorie_2");

        gridLayout_2->addWidget(lblCategorie_2, 1, 0, 1, 1);

        lblQuantite_2 = new QLabel(groupBoxFormulaire);
        lblQuantite_2->setObjectName("lblQuantite_2");

        gridLayout_2->addWidget(lblQuantite_2, 1, 2, 1, 1);

        lblPrix_2 = new QLabel(groupBoxFormulaire);
        lblPrix_2->setObjectName("lblPrix_2");

        gridLayout_2->addWidget(lblPrix_2, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(10);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        btnAjouter_2 = new QPushButton(groupBoxFormulaire);
        btnAjouter_2->setObjectName("btnAjouter_2");
        btnAjouter_2->setMinimumSize(QSize(120, 40));
        btnAjouter_2->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;"));

        horizontalLayout_4->addWidget(btnAjouter_2);

        btnModifier_2 = new QPushButton(groupBoxFormulaire);
        btnModifier_2->setObjectName("btnModifier_2");
        btnModifier_2->setMinimumSize(QSize(120, 40));
        btnModifier_2->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6;"));

        horizontalLayout_4->addWidget(btnModifier_2);

        btnSupprimer_2 = new QPushButton(groupBoxFormulaire);
        btnSupprimer_2->setObjectName("btnSupprimer_2");
        btnSupprimer_2->setMinimumSize(QSize(120, 40));
        btnSupprimer_2->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6;"));

        horizontalLayout_4->addWidget(btnSupprimer_2);

        btnNouveau_2 = new QPushButton(groupBoxFormulaire);
        btnNouveau_2->setObjectName("btnNouveau_2");
        btnNouveau_2->setMinimumSize(QSize(120, 40));
        btnNouveau_2->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;"));

        horizontalLayout_4->addWidget(btnNouveau_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        gridLayout_2->addLayout(horizontalLayout_4, 4, 0, 1, 4);

        lblDateEntree_2 = new QLabel(groupBoxFormulaire);
        lblDateEntree_2->setObjectName("lblDateEntree_2");

        gridLayout_2->addWidget(lblDateEntree_2, 3, 0, 1, 1);

        lblFournisseur_2 = new QLabel(groupBoxFormulaire);
        lblFournisseur_2->setObjectName("lblFournisseur_2");

        gridLayout_2->addWidget(lblFournisseur_2, 2, 2, 1, 1);

        txtReference_2 = new QLineEdit(groupBoxFormulaire);
        txtReference_2->setObjectName("txtReference_2");

        gridLayout_2->addWidget(txtReference_2, 0, 1, 1, 1);

        lblNom_2 = new QLabel(groupBoxFormulaire);
        lblNom_2->setObjectName("lblNom_2");

        gridLayout_2->addWidget(lblNom_2, 0, 2, 1, 1);

        cmbCategorie_2 = new QComboBox(groupBoxFormulaire);
        cmbCategorie_2->addItem(QString());
        cmbCategorie_2->addItem(QString());
        cmbCategorie_2->addItem(QString());
        cmbCategorie_2->addItem(QString());
        cmbCategorie_2->addItem(QString());
        cmbCategorie_2->addItem(QString());
        cmbCategorie_2->setObjectName("cmbCategorie_2");
        cmbCategorie_2->setEditable(true);

        gridLayout_2->addWidget(cmbCategorie_2, 1, 1, 1, 1);

        dateEntree_2 = new QDateEdit(groupBoxFormulaire);
        dateEntree_2->setObjectName("dateEntree_2");
        dateEntree_2->setCalendarPopup(true);
        dateEntree_2->setDate(QDate(2025, 10, 7));

        gridLayout_2->addWidget(dateEntree_2, 3, 1, 1, 1);

        groupBoxRecherche = new QGroupBox(page);
        groupBoxRecherche->setObjectName("groupBoxRecherche");
        groupBoxRecherche->setGeometry(QRect(20, 394, 1041, 111));
        horizontalLayout_5 = new QHBoxLayout(groupBoxRecherche);
        horizontalLayout_5->setSpacing(10);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        lblRecherche_2 = new QLabel(groupBoxRecherche);
        lblRecherche_2->setObjectName("lblRecherche_2");

        horizontalLayout_5->addWidget(lblRecherche_2);

        txtRecherche_2 = new QLineEdit(groupBoxRecherche);
        txtRecherche_2->setObjectName("txtRecherche_2");
        txtRecherche_2->setStyleSheet(QString::fromUtf8("background-color: #5DADE2;"));

        horizontalLayout_5->addWidget(txtRecherche_2);

        lblFiltreCategorie_2 = new QLabel(groupBoxRecherche);
        lblFiltreCategorie_2->setObjectName("lblFiltreCategorie_2");

        horizontalLayout_5->addWidget(lblFiltreCategorie_2);

        cmbFiltreCategorie_2 = new QComboBox(groupBoxRecherche);
        cmbFiltreCategorie_2->addItem(QString());
        cmbFiltreCategorie_2->addItem(QString());
        cmbFiltreCategorie_2->addItem(QString());
        cmbFiltreCategorie_2->addItem(QString());
        cmbFiltreCategorie_2->addItem(QString());
        cmbFiltreCategorie_2->setObjectName("cmbFiltreCategorie_2");

        horizontalLayout_5->addWidget(cmbFiltreCategorie_2);

        btnRechercher_2 = new QPushButton(groupBoxRecherche);
        btnRechercher_2->setObjectName("btnRechercher_2");
        btnRechercher_2->setMinimumSize(QSize(120, 35));
        btnRechercher_2->setStyleSheet(QString::fromUtf8("background-color: #5DADE2;"));

        horizontalLayout_5->addWidget(btnRechercher_2);

        lblTitre = new QLabel(page);
        lblTitre->setObjectName("lblTitre");
        lblTitre->setGeometry(QRect(-150, -10, 1401, 51));
        lblTitre->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C5F75; padding: 15px; background-color: #87CEEB; border-radius: 10px;"));
        lblTitre->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBoxTableau = new QGroupBox(page);
        groupBoxTableau->setObjectName("groupBoxTableau");
        groupBoxTableau->setGeometry(QRect(20, 510, 1041, 221));
        verticalLayout_3 = new QVBoxLayout(groupBoxTableau);
        verticalLayout_3->setObjectName("verticalLayout_3");
        tablePieces_2 = new QTableWidget(groupBoxTableau);
        if (tablePieces_2->columnCount() < 8)
            tablePieces_2->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tablePieces_2->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tablePieces_2->setObjectName("tablePieces_2");
        tablePieces_2->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tablePieces_2->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tablePieces_2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tablePieces_2->setSortingEnabled(true);

        verticalLayout_3->addWidget(tablePieces_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        lblAlerteStock_2 = new QLabel(groupBoxTableau);
        lblAlerteStock_2->setObjectName("lblAlerteStock_2");
        lblAlerteStock_2->setStyleSheet(QString::fromUtf8("color: #FF5722; font-weight: bold;"));

        horizontalLayout_6->addWidget(lblAlerteStock_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        btnExportPDF_2 = new QPushButton(groupBoxTableau);
        btnExportPDF_2->setObjectName("btnExportPDF_2");
        btnExportPDF_2->setMinimumSize(QSize(140, 35));

        horizontalLayout_6->addWidget(btnExportPDF_2);

        btnExportExcel_2 = new QPushButton(groupBoxTableau);
        btnExportExcel_2->setObjectName("btnExportExcel_2");
        btnExportExcel_2->setMinimumSize(QSize(140, 35));

        horizontalLayout_6->addWidget(btnExportExcel_2);


        verticalLayout_3->addLayout(horizontalLayout_6);

        pushButton = new QPushButton(page);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(950, 50, 111, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(148, 255, 191);\n"
"color: rgb(7, 7, 7);"));
        stackedWidget->addWidget(page);

        retranslateUi(gestionpiecesdetachees);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(gestionpiecesdetachees);
    } // setupUi

    void retranslateUi(QWidget *gestionpiecesdetachees)
    {
        gestionpiecesdetachees->setWindowTitle(QCoreApplication::translate("gestionpiecesdetachees", "Form", nullptr));
        groupBoxFormulaire->setTitle(QCoreApplication::translate("gestionpiecesdetachees", "Informations de la Pi\303\250ce", nullptr));
        lblReference_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "R\303\251f\303\251rence :", nullptr));
        txtNom_2->setPlaceholderText(QCoreApplication::translate("gestionpiecesdetachees", "Ex: \303\211cran LCD", nullptr));
        lblDateSortie_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Date de sortie :", nullptr));
        txtFournisseur_2->setPlaceholderText(QCoreApplication::translate("gestionpiecesdetachees", "Nom du fournisseur", nullptr));
        lblCategorie_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Cat\303\251gorie :", nullptr));
        lblQuantite_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Quantit\303\251 en stock :", nullptr));
        lblPrix_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Prix unitaire (DT) :", nullptr));
        btnAjouter_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\342\236\225 Ajouter", nullptr));
        btnModifier_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\342\234\217\357\270\217 Modifier", nullptr));
        btnSupprimer_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        btnNouveau_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\360\237\224\204 Nouveau", nullptr));
        lblDateEntree_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Date d'entr\303\251e :", nullptr));
        lblFournisseur_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Fournisseur :", nullptr));
        txtReference_2->setPlaceholderText(QCoreApplication::translate("gestionpiecesdetachees", "Ex: REF-2024-001", nullptr));
        lblNom_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Nom de la pi\303\250ce :", nullptr));
        cmbCategorie_2->setItemText(0, QCoreApplication::translate("gestionpiecesdetachees", "\303\211crans", nullptr));
        cmbCategorie_2->setItemText(1, QCoreApplication::translate("gestionpiecesdetachees", "Batteries", nullptr));
        cmbCategorie_2->setItemText(2, QCoreApplication::translate("gestionpiecesdetachees", "Connecteurs", nullptr));
        cmbCategorie_2->setItemText(3, QCoreApplication::translate("gestionpiecesdetachees", "C\303\242bles", nullptr));
        cmbCategorie_2->setItemText(4, QCoreApplication::translate("gestionpiecesdetachees", "Circuits imprim\303\251s", nullptr));
        cmbCategorie_2->setItemText(5, QCoreApplication::translate("gestionpiecesdetachees", "Composants \303\251lectroniques", nullptr));

        groupBoxRecherche->setTitle(QCoreApplication::translate("gestionpiecesdetachees", "Recherche et Filtrage", nullptr));
        lblRecherche_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\360\237\224\215 Rechercher :", nullptr));
        txtRecherche_2->setPlaceholderText(QCoreApplication::translate("gestionpiecesdetachees", "Rechercher par r\303\251f\303\251rence, nom, cat\303\251gorie ou fournisseur...", nullptr));
        lblFiltreCategorie_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Cat\303\251gorie :", nullptr));
        cmbFiltreCategorie_2->setItemText(0, QCoreApplication::translate("gestionpiecesdetachees", "Toutes les cat\303\251gories", nullptr));
        cmbFiltreCategorie_2->setItemText(1, QCoreApplication::translate("gestionpiecesdetachees", "\303\211crans", nullptr));
        cmbFiltreCategorie_2->setItemText(2, QCoreApplication::translate("gestionpiecesdetachees", "Batteries", nullptr));
        cmbFiltreCategorie_2->setItemText(3, QCoreApplication::translate("gestionpiecesdetachees", "Connecteurs", nullptr));
        cmbFiltreCategorie_2->setItemText(4, QCoreApplication::translate("gestionpiecesdetachees", "C\303\242bles", nullptr));

        btnRechercher_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\360\237\224\215 Rechercher", nullptr));
        lblTitre->setText(QCoreApplication::translate("gestionpiecesdetachees", "Gestion des Pi\303\250ces D\303\251tach\303\251es", nullptr));
        groupBoxTableau->setTitle(QCoreApplication::translate("gestionpiecesdetachees", "Liste des Pi\303\250ces D\303\251tach\303\251es", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tablePieces_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("gestionpiecesdetachees", "R\303\251f\303\251rence", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tablePieces_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("gestionpiecesdetachees", "Nom de la pi\303\250ce", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tablePieces_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("gestionpiecesdetachees", "Cat\303\251gorie", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tablePieces_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("gestionpiecesdetachees", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tablePieces_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("gestionpiecesdetachees", "Prix unitaire (DT)", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tablePieces_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("gestionpiecesdetachees", "Fournisseur", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tablePieces_2->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("gestionpiecesdetachees", "Date d'entr\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tablePieces_2->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("gestionpiecesdetachees", "Date de sortie", nullptr));
        lblAlerteStock_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\342\232\240\357\270\217 Alerte stock minimum : Affichage des pi\303\250ces en quantit\303\251 faible", nullptr));
        btnExportPDF_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\360\237\223\204 Exporter PDF", nullptr));
        btnExportExcel_2->setText(QCoreApplication::translate("gestionpiecesdetachees", "\360\237\223\212 Exporter Excel", nullptr));
        pushButton->setText(QCoreApplication::translate("gestionpiecesdetachees", "Home", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gestionpiecesdetachees: public Ui_gestionpiecesdetachees {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONPIECESDETACHEES_H
