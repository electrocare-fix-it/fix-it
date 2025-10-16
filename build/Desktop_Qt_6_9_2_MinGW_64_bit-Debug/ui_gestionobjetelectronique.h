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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gestionobjetelectronique
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page_19;
    QLabel *labelTitre;
    QTableWidget *tableWidget;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_17;
    QGroupBox *groupBoxAjout_6;
    QFormLayout *formLayout_11;
    QLabel *labelReference_6;
    QLineEdit *lineEditReference_6;
    QLabel *labelNomObjet_6;
    QLineEdit *lineEditNomObjet_6;
    QLabel *labelMarque_6;
    QLineEdit *lineEditMarque_6;
    QLabel *labelModele_6;
    QLineEdit *lineEditModele_6;
    QLabel *labelCouleur_6;
    QLineEdit *lineEditCouleur_6;
    QLabel *labelNumeroSerie_6;
    QLineEdit *lineEditNumeroSerie_6;
    QLabel *labelType_6;
    QComboBox *comboBoxType_6;
    QLabel *labelEtat_6;
    QComboBox *comboBoxEtat_6;
    QLabel *labelTechnicien_6;
    QLineEdit *lineEditTechnicien_6;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *btnAjouter_6;
    QPushButton *btnModifier_6;
    QPushButton *btnSupprimer_6;
    QPushButton *btnRechercher_6;
    QPushButton *btnExportPDF_6;
    QPushButton *btnReinitialiser_6;
    QVBoxLayout *verticalLayout_17;
    QGroupBox *groupBoxSMS_6;
    QVBoxLayout *verticalLayout_18;
    QLabel *labelInfoSMS_6;
    QTextEdit *textEditSMS_6;
    QPushButton *btnGenererSMS_6;
    QPushButton *btnEnvoyerSMS_6;
    QGroupBox *groupBoxPrix_6;
    QVBoxLayout *verticalLayout_19;
    QFormLayout *formLayout_12;
    QLabel *labelProbleme_6;
    QComboBox *comboBoxProbleme_6;
    QLabel *labelPrixEstime_6;
    QSpinBox *spinBoxPrix_6;
    QPushButton *btnCalculerPrix_6;
    QGroupBox *groupBoxRecherche;
    QHBoxLayout *horizontalLayout_18;
    QLabel *labelRechercheType_6;
    QComboBox *comboBoxRechercheType_6;
    QLabel *labelRechercheMarque_6;
    QLineEdit *lineEditRechercheMarque_6;
    QLabel *labelRechercheEtat_6;
    QComboBox *comboBoxRechercheEtat_6;
    QLabel *labelRechercheModele_6;
    QLineEdit *lineEditRechercheModele_6;
    QPushButton *pushButton;
    QWidget *page_20;

    void setupUi(QWidget *gestionobjetelectronique)
    {
        if (gestionobjetelectronique->objectName().isEmpty())
            gestionobjetelectronique->setObjectName("gestionobjetelectronique");
        gestionobjetelectronique->resize(1711, 862);
        stackedWidget = new QStackedWidget(gestionobjetelectronique);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, -30, 1335, 811));
        page_19 = new QWidget();
        page_19->setObjectName("page_19");
        labelTitre = new QLabel(page_19);
        labelTitre->setObjectName("labelTitre");
        labelTitre->setGeometry(QRect(-20, 30, 1287, 51));
        labelTitre->setStyleSheet(QString::fromUtf8("font-size: 24px; font-weight: bold; color: #2C5F75; padding: 15px; background-color: #87CEEB; border-radius: 10px;"));
        labelTitre->setAlignment(Qt::AlignmentFlag::AlignCenter);
        tableWidget = new QTableWidget(page_19);
        if (tableWidget->columnCount() < 10)
            tableWidget->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(0, 720, 1231, 71));
<<<<<<< HEAD
        tableWidget->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold; background-color: #DDF6FF; gridline-color: #87CEEB; alternate-background-color: #E0F6FF;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        layoutWidget_2 = new QWidget(page_19);
        layoutWidget_2->setObjectName("layoutWidget_2");
        layoutWidget_2->setGeometry(QRect(5, 110, 1171, 537));
        horizontalLayout_17 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        horizontalLayout_17->setContentsMargins(0, 0, 0, 0);
        groupBoxAjout_6 = new QGroupBox(layoutWidget_2);
        groupBoxAjout_6->setObjectName("groupBoxAjout_6");
<<<<<<< HEAD
        groupBoxAjout_6->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #1A3A4A;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        formLayout_11 = new QFormLayout(groupBoxAjout_6);
        formLayout_11->setObjectName("formLayout_11");
        formLayout_11->setHorizontalSpacing(10);
        formLayout_11->setVerticalSpacing(12);
        formLayout_11->setContentsMargins(15, 20, 15, 15);
        labelReference_6 = new QLabel(groupBoxAjout_6);
        labelReference_6->setObjectName("labelReference_6");
<<<<<<< HEAD
        labelReference_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(0, QFormLayout::ItemRole::LabelRole, labelReference_6);

        lineEditReference_6 = new QLineEdit(groupBoxAjout_6);
        lineEditReference_6->setObjectName("lineEditReference_6");

        formLayout_11->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEditReference_6);

        labelNomObjet_6 = new QLabel(groupBoxAjout_6);
        labelNomObjet_6->setObjectName("labelNomObjet_6");
<<<<<<< HEAD
        labelNomObjet_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(1, QFormLayout::ItemRole::LabelRole, labelNomObjet_6);

        lineEditNomObjet_6 = new QLineEdit(groupBoxAjout_6);
        lineEditNomObjet_6->setObjectName("lineEditNomObjet_6");

        formLayout_11->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEditNomObjet_6);

        labelMarque_6 = new QLabel(groupBoxAjout_6);
        labelMarque_6->setObjectName("labelMarque_6");
<<<<<<< HEAD
        labelMarque_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(2, QFormLayout::ItemRole::LabelRole, labelMarque_6);

        lineEditMarque_6 = new QLineEdit(groupBoxAjout_6);
        lineEditMarque_6->setObjectName("lineEditMarque_6");

        formLayout_11->setWidget(2, QFormLayout::ItemRole::FieldRole, lineEditMarque_6);

        labelModele_6 = new QLabel(groupBoxAjout_6);
        labelModele_6->setObjectName("labelModele_6");
<<<<<<< HEAD
        labelModele_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(3, QFormLayout::ItemRole::LabelRole, labelModele_6);

        lineEditModele_6 = new QLineEdit(groupBoxAjout_6);
        lineEditModele_6->setObjectName("lineEditModele_6");

        formLayout_11->setWidget(3, QFormLayout::ItemRole::FieldRole, lineEditModele_6);

        labelCouleur_6 = new QLabel(groupBoxAjout_6);
        labelCouleur_6->setObjectName("labelCouleur_6");
<<<<<<< HEAD
        labelCouleur_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(4, QFormLayout::ItemRole::LabelRole, labelCouleur_6);

        lineEditCouleur_6 = new QLineEdit(groupBoxAjout_6);
        lineEditCouleur_6->setObjectName("lineEditCouleur_6");

        formLayout_11->setWidget(4, QFormLayout::ItemRole::FieldRole, lineEditCouleur_6);

        labelNumeroSerie_6 = new QLabel(groupBoxAjout_6);
        labelNumeroSerie_6->setObjectName("labelNumeroSerie_6");
<<<<<<< HEAD
        labelNumeroSerie_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(5, QFormLayout::ItemRole::LabelRole, labelNumeroSerie_6);

        lineEditNumeroSerie_6 = new QLineEdit(groupBoxAjout_6);
        lineEditNumeroSerie_6->setObjectName("lineEditNumeroSerie_6");

        formLayout_11->setWidget(5, QFormLayout::ItemRole::FieldRole, lineEditNumeroSerie_6);

        labelType_6 = new QLabel(groupBoxAjout_6);
        labelType_6->setObjectName("labelType_6");
<<<<<<< HEAD
        labelType_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(6, QFormLayout::ItemRole::LabelRole, labelType_6);

        comboBoxType_6 = new QComboBox(groupBoxAjout_6);
        comboBoxType_6->addItem(QString());
        comboBoxType_6->addItem(QString());
        comboBoxType_6->addItem(QString());
        comboBoxType_6->addItem(QString());
        comboBoxType_6->addItem(QString());
        comboBoxType_6->addItem(QString());
        comboBoxType_6->setObjectName("comboBoxType_6");

        formLayout_11->setWidget(6, QFormLayout::ItemRole::FieldRole, comboBoxType_6);

        labelEtat_6 = new QLabel(groupBoxAjout_6);
        labelEtat_6->setObjectName("labelEtat_6");
<<<<<<< HEAD
        labelEtat_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(7, QFormLayout::ItemRole::LabelRole, labelEtat_6);

        comboBoxEtat_6 = new QComboBox(groupBoxAjout_6);
        comboBoxEtat_6->addItem(QString());
        comboBoxEtat_6->addItem(QString());
        comboBoxEtat_6->addItem(QString());
        comboBoxEtat_6->addItem(QString());
        comboBoxEtat_6->addItem(QString());
        comboBoxEtat_6->setObjectName("comboBoxEtat_6");

        formLayout_11->setWidget(7, QFormLayout::ItemRole::FieldRole, comboBoxEtat_6);

        labelTechnicien_6 = new QLabel(groupBoxAjout_6);
        labelTechnicien_6->setObjectName("labelTechnicien_6");
<<<<<<< HEAD
        labelTechnicien_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_11->setWidget(8, QFormLayout::ItemRole::LabelRole, labelTechnicien_6);

        lineEditTechnicien_6 = new QLineEdit(groupBoxAjout_6);
        lineEditTechnicien_6->setObjectName("lineEditTechnicien_6");

        formLayout_11->setWidget(8, QFormLayout::ItemRole::FieldRole, lineEditTechnicien_6);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        btnAjouter_6 = new QPushButton(groupBoxAjout_6);
        btnAjouter_6->setObjectName("btnAjouter_6");
<<<<<<< HEAD
        btnAjouter_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB; padding: 10px; color: #1A3A4A; font-weight: bold;"));
=======
        btnAjouter_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB; padding: 10px;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_16->addWidget(btnAjouter_6);

        btnModifier_6 = new QPushButton(groupBoxAjout_6);
        btnModifier_6->setObjectName("btnModifier_6");
<<<<<<< HEAD
        btnModifier_6->setStyleSheet(QString::fromUtf8("padding: 10px; background-color: #5DADE2; color: #1A3A4A; font-weight: bold;"));
=======
        btnModifier_6->setStyleSheet(QString::fromUtf8("padding: 10px;\n"
"background-color: #5DADE2;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_16->addWidget(btnModifier_6);

        btnSupprimer_6 = new QPushButton(groupBoxAjout_6);
        btnSupprimer_6->setObjectName("btnSupprimer_6");
<<<<<<< HEAD
        btnSupprimer_6->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6; padding: 10px; color: #1A3A4A; font-weight: bold;"));
=======
        btnSupprimer_6->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6; padding: 10px;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_16->addWidget(btnSupprimer_6);

        btnRechercher_6 = new QPushButton(groupBoxAjout_6);
        btnRechercher_6->setObjectName("btnRechercher_6");
<<<<<<< HEAD
        btnRechercher_6->setStyleSheet(QString::fromUtf8("padding: 10px; background-color: #5DADE2; color: #1A3A4A; font-weight: bold;"));
=======
        btnRechercher_6->setStyleSheet(QString::fromUtf8("padding: 10px;\n"
"background-color: #5DADE2;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_16->addWidget(btnRechercher_6);

        btnExportPDF_6 = new QPushButton(groupBoxAjout_6);
        btnExportPDF_6->setObjectName("btnExportPDF_6");
<<<<<<< HEAD
        btnExportPDF_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB; padding: 10px; color: #1A3A4A; font-weight: bold;"));
=======
        btnExportPDF_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB; padding: 10px;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_16->addWidget(btnExportPDF_6);

        btnReinitialiser_6 = new QPushButton(groupBoxAjout_6);
        btnReinitialiser_6->setObjectName("btnReinitialiser_6");
<<<<<<< HEAD
        btnReinitialiser_6->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6; padding: 10px; color: #1A3A4A; font-weight: bold;"));
=======
        btnReinitialiser_6->setStyleSheet(QString::fromUtf8("background-color: #B0E0E6; padding: 10px;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_16->addWidget(btnReinitialiser_6);


        formLayout_11->setLayout(10, QFormLayout::ItemRole::SpanningRole, horizontalLayout_16);


        horizontalLayout_17->addWidget(groupBoxAjout_6);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setObjectName("verticalLayout_17");
        groupBoxSMS_6 = new QGroupBox(layoutWidget_2);
        groupBoxSMS_6->setObjectName("groupBoxSMS_6");
<<<<<<< HEAD
        groupBoxSMS_6->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #1A3A4A;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        verticalLayout_18 = new QVBoxLayout(groupBoxSMS_6);
        verticalLayout_18->setSpacing(10);
        verticalLayout_18->setObjectName("verticalLayout_18");
        labelInfoSMS_6 = new QLabel(groupBoxSMS_6);
        labelInfoSMS_6->setObjectName("labelInfoSMS_6");
<<<<<<< HEAD
        labelInfoSMS_6->setStyleSheet(QString::fromUtf8("font-size: 11px; color: #1A3A4A; font-weight: bold;"));
=======
        labelInfoSMS_6->setStyleSheet(QString::fromUtf8("font-size: 11px; color: #5A9FBF;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        labelInfoSMS_6->setWordWrap(true);

        verticalLayout_18->addWidget(labelInfoSMS_6);

        textEditSMS_6 = new QTextEdit(groupBoxSMS_6);
        textEditSMS_6->setObjectName("textEditSMS_6");

        verticalLayout_18->addWidget(textEditSMS_6);

        btnGenererSMS_6 = new QPushButton(groupBoxSMS_6);
        btnGenererSMS_6->setObjectName("btnGenererSMS_6");
<<<<<<< HEAD
        btnGenererSMS_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB; color: #1A3A4A; font-weight: bold;"));
=======
        btnGenererSMS_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        verticalLayout_18->addWidget(btnGenererSMS_6);

        btnEnvoyerSMS_6 = new QPushButton(groupBoxSMS_6);
        btnEnvoyerSMS_6->setObjectName("btnEnvoyerSMS_6");
<<<<<<< HEAD
        btnEnvoyerSMS_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        verticalLayout_18->addWidget(btnEnvoyerSMS_6);


        verticalLayout_17->addWidget(groupBoxSMS_6);

        groupBoxPrix_6 = new QGroupBox(layoutWidget_2);
        groupBoxPrix_6->setObjectName("groupBoxPrix_6");
<<<<<<< HEAD
        groupBoxPrix_6->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #1A3A4A;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        verticalLayout_19 = new QVBoxLayout(groupBoxPrix_6);
        verticalLayout_19->setObjectName("verticalLayout_19");
        formLayout_12 = new QFormLayout();
        formLayout_12->setObjectName("formLayout_12");
        labelProbleme_6 = new QLabel(groupBoxPrix_6);
        labelProbleme_6->setObjectName("labelProbleme_6");
<<<<<<< HEAD
        labelProbleme_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_12->setWidget(0, QFormLayout::ItemRole::LabelRole, labelProbleme_6);

        comboBoxProbleme_6 = new QComboBox(groupBoxPrix_6);
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->addItem(QString());
        comboBoxProbleme_6->setObjectName("comboBoxProbleme_6");

        formLayout_12->setWidget(0, QFormLayout::ItemRole::FieldRole, comboBoxProbleme_6);

        labelPrixEstime_6 = new QLabel(groupBoxPrix_6);
        labelPrixEstime_6->setObjectName("labelPrixEstime_6");
<<<<<<< HEAD
        labelPrixEstime_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        formLayout_12->setWidget(1, QFormLayout::ItemRole::LabelRole, labelPrixEstime_6);

        spinBoxPrix_6 = new QSpinBox(groupBoxPrix_6);
        spinBoxPrix_6->setObjectName("spinBoxPrix_6");
        spinBoxPrix_6->setMaximum(10000);

        formLayout_12->setWidget(1, QFormLayout::ItemRole::FieldRole, spinBoxPrix_6);


        verticalLayout_19->addLayout(formLayout_12);

        btnCalculerPrix_6 = new QPushButton(groupBoxPrix_6);
        btnCalculerPrix_6->setObjectName("btnCalculerPrix_6");
<<<<<<< HEAD
        btnCalculerPrix_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB; color: #1A3A4A; font-weight: bold;"));
=======
        btnCalculerPrix_6->setStyleSheet(QString::fromUtf8("background-color: #87CEEB;"));
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        verticalLayout_19->addWidget(btnCalculerPrix_6);


        verticalLayout_17->addWidget(groupBoxPrix_6);


        horizontalLayout_17->addLayout(verticalLayout_17);

        groupBoxRecherche = new QGroupBox(page_19);
        groupBoxRecherche->setObjectName("groupBoxRecherche");
        groupBoxRecherche->setGeometry(QRect(0, 650, 1181, 71));
<<<<<<< HEAD
        groupBoxRecherche->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #1A3A4A;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        horizontalLayout_18 = new QHBoxLayout(groupBoxRecherche);
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        labelRechercheType_6 = new QLabel(groupBoxRecherche);
        labelRechercheType_6->setObjectName("labelRechercheType_6");
<<<<<<< HEAD
        labelRechercheType_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_18->addWidget(labelRechercheType_6);

        comboBoxRechercheType_6 = new QComboBox(groupBoxRecherche);
        comboBoxRechercheType_6->addItem(QString());
        comboBoxRechercheType_6->addItem(QString());
        comboBoxRechercheType_6->addItem(QString());
        comboBoxRechercheType_6->addItem(QString());
        comboBoxRechercheType_6->setObjectName("comboBoxRechercheType_6");

        horizontalLayout_18->addWidget(comboBoxRechercheType_6);

        labelRechercheMarque_6 = new QLabel(groupBoxRecherche);
        labelRechercheMarque_6->setObjectName("labelRechercheMarque_6");
<<<<<<< HEAD
        labelRechercheMarque_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_18->addWidget(labelRechercheMarque_6);

        lineEditRechercheMarque_6 = new QLineEdit(groupBoxRecherche);
        lineEditRechercheMarque_6->setObjectName("lineEditRechercheMarque_6");

        horizontalLayout_18->addWidget(lineEditRechercheMarque_6);

        labelRechercheEtat_6 = new QLabel(groupBoxRecherche);
        labelRechercheEtat_6->setObjectName("labelRechercheEtat_6");
<<<<<<< HEAD
        labelRechercheEtat_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_18->addWidget(labelRechercheEtat_6);

        comboBoxRechercheEtat_6 = new QComboBox(groupBoxRecherche);
        comboBoxRechercheEtat_6->addItem(QString());
        comboBoxRechercheEtat_6->addItem(QString());
        comboBoxRechercheEtat_6->addItem(QString());
        comboBoxRechercheEtat_6->addItem(QString());
        comboBoxRechercheEtat_6->setObjectName("comboBoxRechercheEtat_6");

        horizontalLayout_18->addWidget(comboBoxRechercheEtat_6);

        labelRechercheModele_6 = new QLabel(groupBoxRecherche);
        labelRechercheModele_6->setObjectName("labelRechercheModele_6");
<<<<<<< HEAD
        labelRechercheModele_6->setStyleSheet(QString::fromUtf8("color: #1A3A4A; font-weight: bold;"));
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

        horizontalLayout_18->addWidget(labelRechercheModele_6);

        lineEditRechercheModele_6 = new QLineEdit(groupBoxRecherche);
        lineEditRechercheModele_6->setObjectName("lineEditRechercheModele_6");

        horizontalLayout_18->addWidget(lineEditRechercheModele_6);

        pushButton = new QPushButton(page_19);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(920, 90, 90, 21));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(148, 255, 191);\n"
"color: rgb(7, 7, 7);"));
        stackedWidget->addWidget(page_19);
        page_20 = new QWidget();
        page_20->setObjectName("page_20");
        stackedWidget->addWidget(page_20);

        retranslateUi(gestionobjetelectronique);

        QMetaObject::connectSlotsByName(gestionobjetelectronique);
    } // setupUi

    void retranslateUi(QWidget *gestionobjetelectronique)
    {
        gestionobjetelectronique->setWindowTitle(QCoreApplication::translate("gestionobjetelectronique", "Form", nullptr));
        labelTitre->setText(QCoreApplication::translate("gestionobjetelectronique", "GESTION DES OBJETS \303\211LECTRONIQUES", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("gestionobjetelectronique", "R\303\251f\303\251rence", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("gestionobjetelectronique", "Nom Objet", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("gestionobjetelectronique", "Marque", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("gestionobjetelectronique", "Mod\303\250le", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("gestionobjetelectronique", "Couleur", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("gestionobjetelectronique", "N\302\260 S\303\251rie", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("gestionobjetelectronique", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("gestionobjetelectronique", "\303\211tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("gestionobjetelectronique", "Technicien", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("gestionobjetelectronique", "Prix (TND)", nullptr));
        groupBoxAjout_6->setTitle(QCoreApplication::translate("gestionobjetelectronique", " Informations de l'Objet ", nullptr));
        labelReference_6->setText(QCoreApplication::translate("gestionobjetelectronique", "R\303\251f\303\251rence (ID unique):", nullptr));
        lineEditReference_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Ex: REF-001", nullptr));
        labelNomObjet_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Nom de l'objet:", nullptr));
        lineEditNomObjet_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Ex: iPhone 12", nullptr));
        labelMarque_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Marque:", nullptr));
        lineEditMarque_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Ex: Apple, Samsung, HP...", nullptr));
        labelModele_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Mod\303\250le:", nullptr));
        lineEditModele_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Ex: Pro Max, Galaxy S21...", nullptr));
        labelCouleur_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Couleur:", nullptr));
        lineEditCouleur_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Ex: Noir, Blanc, Bleu...", nullptr));
        labelNumeroSerie_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Num\303\251ro de s\303\251rie:", nullptr));
        lineEditNumeroSerie_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Ex: SN123456789", nullptr));
        labelType_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Type:", nullptr));
        comboBoxType_6->setItemText(0, QCoreApplication::translate("gestionobjetelectronique", "-- S\303\251lectionner --", nullptr));
        comboBoxType_6->setItemText(1, QCoreApplication::translate("gestionobjetelectronique", "PC", nullptr));
        comboBoxType_6->setItemText(2, QCoreApplication::translate("gestionobjetelectronique", "T\303\251l\303\251phone", nullptr));
        comboBoxType_6->setItemText(3, QCoreApplication::translate("gestionobjetelectronique", "Imprimante", nullptr));
        comboBoxType_6->setItemText(4, QCoreApplication::translate("gestionobjetelectronique", "Tablette", nullptr));
        comboBoxType_6->setItemText(5, QCoreApplication::translate("gestionobjetelectronique", "Autre", nullptr));

        labelEtat_6->setText(QCoreApplication::translate("gestionobjetelectronique", "\303\211tat:", nullptr));
        comboBoxEtat_6->setItemText(0, QCoreApplication::translate("gestionobjetelectronique", "-- S\303\251lectionner --", nullptr));
        comboBoxEtat_6->setItemText(1, QCoreApplication::translate("gestionobjetelectronique", "En panne", nullptr));
        comboBoxEtat_6->setItemText(2, QCoreApplication::translate("gestionobjetelectronique", "En r\303\251paration", nullptr));
        comboBoxEtat_6->setItemText(3, QCoreApplication::translate("gestionobjetelectronique", "R\303\251par\303\251", nullptr));
        comboBoxEtat_6->setItemText(4, QCoreApplication::translate("gestionobjetelectronique", "En attente de pi\303\250ces", nullptr));

        labelTechnicien_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Technicien:", nullptr));
        lineEditTechnicien_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Nom du technicien", nullptr));
        btnAjouter_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Ajouter", nullptr));
        btnModifier_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Modifier", nullptr));
        btnSupprimer_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Supprimer", nullptr));
        btnRechercher_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Rechercher", nullptr));
        btnExportPDF_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Export PDF", nullptr));
        btnReinitialiser_6->setText(QCoreApplication::translate("gestionobjetelectronique", "R\303\251initialiser", nullptr));
        groupBoxSMS_6->setTitle(QCoreApplication::translate("gestionobjetelectronique", " SMS Rendez-vous ", nullptr));
        labelInfoSMS_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Le SMS sera g\303\251n\303\251r\303\251 automatiquement selon l'appareil et le probl\303\250me", nullptr));
        textEditSMS_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Le message SMS appara\303\256tra ici...", nullptr));
        btnGenererSMS_6->setText(QCoreApplication::translate("gestionobjetelectronique", "G\303\251n\303\251rer SMS", nullptr));
        btnEnvoyerSMS_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Envoyer SMS", nullptr));
        groupBoxPrix_6->setTitle(QCoreApplication::translate("gestionobjetelectronique", " Prix Estim\303\251 ", nullptr));
        labelProbleme_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Probl\303\250me:", nullptr));
        comboBoxProbleme_6->setItemText(0, QCoreApplication::translate("gestionobjetelectronique", "-- S\303\251lectionner --", nullptr));
        comboBoxProbleme_6->setItemText(1, QCoreApplication::translate("gestionobjetelectronique", "\303\211cran cass\303\251", nullptr));
        comboBoxProbleme_6->setItemText(2, QCoreApplication::translate("gestionobjetelectronique", "Batterie d\303\251fectueuse", nullptr));
        comboBoxProbleme_6->setItemText(3, QCoreApplication::translate("gestionobjetelectronique", "Probl\303\250me logiciel", nullptr));
        comboBoxProbleme_6->setItemText(4, QCoreApplication::translate("gestionobjetelectronique", "Ne s'allume pas", nullptr));
        comboBoxProbleme_6->setItemText(5, QCoreApplication::translate("gestionobjetelectronique", "Probl\303\250me de charge", nullptr));
        comboBoxProbleme_6->setItemText(6, QCoreApplication::translate("gestionobjetelectronique", "Autre", nullptr));

        labelPrixEstime_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Prix (TND):", nullptr));
        spinBoxPrix_6->setSuffix(QCoreApplication::translate("gestionobjetelectronique", " TND", nullptr));
        btnCalculerPrix_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Calculer Prix Estim\303\251", nullptr));
        groupBoxRecherche->setTitle(QCoreApplication::translate("gestionobjetelectronique", " Recherche Tri ", nullptr));
        labelRechercheType_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Par Type:", nullptr));
        comboBoxRechercheType_6->setItemText(0, QCoreApplication::translate("gestionobjetelectronique", "Tous", nullptr));
        comboBoxRechercheType_6->setItemText(1, QCoreApplication::translate("gestionobjetelectronique", "PC", nullptr));
        comboBoxRechercheType_6->setItemText(2, QCoreApplication::translate("gestionobjetelectronique", "T\303\251l\303\251phone", nullptr));
        comboBoxRechercheType_6->setItemText(3, QCoreApplication::translate("gestionobjetelectronique", "Imprimante", nullptr));

        labelRechercheMarque_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Par Marque:", nullptr));
        lineEditRechercheMarque_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Entrez la marque...", nullptr));
        labelRechercheEtat_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Par \303\211tat:", nullptr));
        comboBoxRechercheEtat_6->setItemText(0, QCoreApplication::translate("gestionobjetelectronique", "Tous", nullptr));
        comboBoxRechercheEtat_6->setItemText(1, QCoreApplication::translate("gestionobjetelectronique", "En panne", nullptr));
        comboBoxRechercheEtat_6->setItemText(2, QCoreApplication::translate("gestionobjetelectronique", "En r\303\251paration", nullptr));
        comboBoxRechercheEtat_6->setItemText(3, QCoreApplication::translate("gestionobjetelectronique", "R\303\251par\303\251", nullptr));

        labelRechercheModele_6->setText(QCoreApplication::translate("gestionobjetelectronique", "Par Mod\303\250le:", nullptr));
        lineEditRechercheModele_6->setPlaceholderText(QCoreApplication::translate("gestionobjetelectronique", "Entrez le mod\303\250le...", nullptr));
        pushButton->setText(QCoreApplication::translate("gestionobjetelectronique", "Home", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gestionobjetelectronique: public Ui_gestionobjetelectronique {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONOBJETELECTRONIQUE_H
