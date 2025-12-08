#include "gestionemploye.h"
#include "ui_gestionemploye.h"
#include "chatbotemploye.h"
#include "connection.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QPalette>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>

gestionemploye::gestionemploye(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionemploye)
    , m_chatbot(nullptr)
    , m_currentEmployeeId(-1)
{
    ui->setupUi(this);
    configurerValidateurs();
    
    // Configurer la plage de dates pour la date de naissance
    ui->dateNaissance_2->setDateRange(QDate(1900, 1, 1), QDate::currentDate());
    
    // FORCER les styles pour que tous les textes soient en noir
    forcerStylesTexteNoir();
    
    // Créer le chatbot et l'intégrer dans la zone dédiée
    m_chatbot = new ChatbotEmploye(this);
    if (ui->verticalLayout_7) {
        ui->verticalLayout_7->addWidget(m_chatbot);
    }
    
    // Connecter les signaux du chatbot
    connect(m_chatbot, &ChatbotEmploye::employeeSelected, 
            this, &gestionemploye::onEmployeeSelectedFromChatbot);
    connect(m_chatbot, &ChatbotEmploye::filterTableRequested,
            this, &gestionemploye::onFilterTableRequested);
    
    // Connecter les boutons principaux
    connect(ui->btnExporterPDF_2, &QPushButton::clicked, this, &gestionemploye::on_btnExporterPDF_2_clicked);
    connect(ui->btnStatistiques_2, &QPushButton::clicked, this, &gestionemploye::on_btnStatistiques_2_clicked);
    connect(ui->txtRecherche_2, &QLineEdit::textChanged, this, &gestionemploye::on_txtRecherche_2_textChanged);
    connect(ui->tableEmployes_2, &QTableWidget::itemSelectionChanged, this, &gestionemploye::on_tableEmployes_2_itemSelectionChanged);
    
    // Créer la séquence si elle n'existe pas
    creerSequenceEmploye();
    
    // Charger les employés au démarrage
    chargerEmployes();

    if (ui->comboTri) {
        ui->comboTri->setCurrentIndex(0);
    }
}

void gestionemploye::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Forcer le rafraîchissement des styles à chaque affichage
    forcerStylesTexteNoir();
    // Recharger les employés pour avoir les données à jour
    chargerEmployes();
}

void gestionemploye::forcerStylesTexteNoir()
{
    // FORCER l'application des styles avec !important pour surcharger les styles du fichier UI
    QString globalStyle = "QLabel { color: black !important; }"
                         "QLineEdit { color: black !important; }"
                         "QDateEdit { color: black !important; }"
                         "QComboBox { color: black !important; }"
                         "QComboBox QAbstractItemView { color: black !important; }"
                         "QTextEdit { color: black !important; }"
                         "QTableWidget { color: black !important; }"
                         "QTableWidget::item { color: black !important; }"
                         "QGroupBox { color: black !important; }"
                         "QGroupBox::title { color: black !important; }"
                         "QPushButton { color: black !important; }";
    this->setStyleSheet(globalStyle);
    
    // FORCER les styles directement sur chaque widget individuellement
    // Labels
    if (ui->lblNom_2) ui->lblNom_2->setStyleSheet("color: black !important;");
    if (ui->lblPrenom_2) ui->lblPrenom_2->setStyleSheet("color: black !important;");
    if (ui->lblDateNaissance_2) ui->lblDateNaissance_2->setStyleSheet("color: black !important;");
    if (ui->lblAdresse_2) ui->lblAdresse_2->setStyleSheet("color: black !important;");
    if (ui->lblTelephone_2) ui->lblTelephone_2->setStyleSheet("color: black !important;");
    if (ui->lblEmail_2) ui->lblEmail_2->setStyleSheet("color: black !important;");
    if (ui->lblStatut_2) ui->lblStatut_2->setStyleSheet("color: black !important;");
    if (ui->lblTitle) ui->lblTitle->setStyleSheet(ui->lblTitle->styleSheet() + " color: black !important;");
    
    // Champs de saisie
    if (ui->txtNom_2) ui->txtNom_2->setStyleSheet("color: black !important;");
    if (ui->txtPrenom_2) ui->txtPrenom_2->setStyleSheet("color: black !important;");
    if (ui->dateNaissance_2) ui->dateNaissance_2->setStyleSheet("color: black !important;");
    if (ui->txtAdresse_2) ui->txtAdresse_2->setStyleSheet("color: black !important;");
    if (ui->txtTelephone_2) ui->txtTelephone_2->setStyleSheet("color: black !important;");
    if (ui->txtEmail_2) ui->txtEmail_2->setStyleSheet("color: black !important;");
    if (ui->comboStatut_2) ui->comboStatut_2->setStyleSheet("color: black !important;");
    if (ui->txtRecherche_2) ui->txtRecherche_2->setStyleSheet("color: black !important;");
    
    // GroupBox
    if (ui->groupBoxFormulaire_2) ui->groupBoxFormulaire_2->setStyleSheet("QGroupBox { color: black !important; } QGroupBox::title { color: black !important; }");
    if (ui->groupBoxTableau) ui->groupBoxTableau->setStyleSheet("QGroupBox { color: black !important; } QGroupBox::title { color: black !important; }");
    if (ui->groupBoxChatbot_2) ui->groupBoxChatbot_2->setStyleSheet("QGroupBox { color: black !important; } QGroupBox::title { color: black !important; }");
    
    // Boutons - FORCER avec !important
    QString buttonStyleForce = "color: black !important;";
    if (ui->btnAjouter_2) ui->btnAjouter_2->setStyleSheet(ui->btnAjouter_2->styleSheet() + " " + buttonStyleForce);
    if (ui->btnModifier_2) ui->btnModifier_2->setStyleSheet(ui->btnModifier_2->styleSheet() + " " + buttonStyleForce);
    if (ui->btnSupprimer_2) ui->btnSupprimer_2->setStyleSheet(ui->btnSupprimer_2->styleSheet() + " " + buttonStyleForce);
    if (ui->btnVider_2) ui->btnVider_2->setStyleSheet(ui->btnVider_2->styleSheet() + " " + buttonStyleForce);
    if (ui->btnExporterPDF_2) ui->btnExporterPDF_2->setStyleSheet(ui->btnExporterPDF_2->styleSheet() + " " + buttonStyleForce);
    if (ui->btnStatistiques_2) ui->btnStatistiques_2->setStyleSheet(ui->btnStatistiques_2->styleSheet() + " " + buttonStyleForce);
    if (ui->pushButton) ui->pushButton->setStyleSheet(ui->pushButton->styleSheet() + " color: black !important;");
    
    // Tableau - FORCER avec !important
    QString tableStyle = "QTableWidget {"
                       "background-color: #E3F2FD;"
                       "alternate-background-color: #BBDEFB;"
                       "gridline-color: #90CAF9;"
                       "color: black !important;"
                       "}"
                       "QTableWidget::item {"
                       "border: none;"
                       "padding: 5px;"
                       "color: black !important;"
                       "}"
                       "QTableWidget::item:selected {"
                       "background-color: #64B5F6;"
                       "color: black !important;"
                       "}"
                       "QHeaderView::section {"
                       "background-color: transparent;"
                       "color: black !important;"
                       "font-weight: bold;"
                       "padding: 8px;"
                       "border: none;"
                       "border-right: 1px solid #90CAF9;"
                       "border-bottom: 1px solid #90CAF9;"
                       "}";
    if (ui->tableEmployes_2) ui->tableEmployes_2->setStyleSheet(tableStyle);
    
    // Utiliser QPalette pour forcer les couleurs au niveau système
    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    this->setPalette(palette);
    
    // Forcer la palette sur tous les widgets enfants
    QList<QWidget*> allWidgets = this->findChildren<QWidget*>();
    for (QWidget* widget : allWidgets) {
        if (widget) {
            QPalette widgetPalette = widget->palette();
            widgetPalette.setColor(QPalette::WindowText, Qt::black);
            widgetPalette.setColor(QPalette::Text, Qt::black);
            widgetPalette.setColor(QPalette::ButtonText, Qt::black);
            widget->setPalette(widgetPalette);
        }
    }
    
    // Forcer le rafraîchissement visuel
    this->update();
    this->repaint();
}

gestionemploye::~gestionemploye()
{
    delete ui;
}

void gestionemploye::on_pushButton_clicked()
{
    emit homeRequested();
}

void gestionemploye::configurerValidateurs()
{
    // Limiter la longueur des champs
    ui->txtNom_2->setMaxLength(50);
    ui->txtPrenom_2->setMaxLength(50);
    ui->txtAdresse_2->setMaxLength(100);
    ui->txtEmail_2->setMaxLength(100);
    
    // Validateur pour le téléphone (chiffres, +, -, espaces)
    QRegularExpression telRegex("[0-9+\\-\\s]*");
    QRegularExpressionValidator* telValidator = new QRegularExpressionValidator(telRegex, this);
    ui->txtTelephone_2->setValidator(telValidator);
    ui->txtTelephone_2->setMaxLength(20);
}

bool gestionemploye::validerEmail(const QString& email)
{
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = regex.match(email);
    return match.hasMatch();
}

bool gestionemploye::validerTelephone(const QString& telephone)
{
    // Format tunisien: 8 chiffres, ou avec indicatif +216 ou 00216
    QRegularExpression regex("^(\\+216|00216)?[0-9]{8}$");
    QRegularExpressionMatch match = regex.match(telephone);
    return match.hasMatch();
}

bool gestionemploye::validerNomPrenom(const QString& texte)
{
    QString trimmedText = texte.trimmed();
    if (trimmedText.length() < 2 || trimmedText.length() > 50) {
        return false;
    }
    // Accepter les lettres (y compris accentuées), espaces, tirets et apostrophes
    QRegularExpression regex("^[\\p{L}\\s'-]+$", QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpressionMatch match = regex.match(trimmedText);
    return match.hasMatch();
}

bool gestionemploye::validerDateNaissance(const QDate& date)
{
    if (date > QDate::currentDate()) {
        return false;
    }
    int age = QDate::currentDate().year() - date.year();
    if (date.month() > QDate::currentDate().month() ||
        (date.month() == QDate::currentDate().month() && date.day() > QDate::currentDate().day())) {
        age--;
    }
    // Âge minimum 18 ans pour un employé, maximum 120 ans
    return age >= 18 && age <= 120;
}

void gestionemploye::afficherMessageErreur(const QString& titre, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(titre);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::NoIcon);
    
    QString style = "QMessageBox {"
                   "background-color: #1a1a2e;"
                   "border: 1px solid #16213e;"
                   "}"
                   "QMessageBox QLabel {"
                   "background-color: #2d2d44;"
                   "color: #f0f0f0;"
                   "font-size: 14px;"
                   "font-weight: 500;"
                   "padding: 20px 25px;"
                   "border-radius: 8px;"
                   "border: 1px solid #3d3d5c;"
                   "min-width: 350px;"
                   "line-height: 1.5;"
                   "}"
                   "QMessageBox QPushButton {"
                   "background-color: #1976D2;"
                   "color: #ffffff;"
                   "border: none;"
                   "border-radius: 6px;"
                   "padding: 10px 30px;"
                   "min-width: 100px;"
                   "min-height: 35px;"
                   "font-weight: 600;"
                   "font-size: 13px;"
                   "}"
                   "QMessageBox QPushButton:hover {"
                   "background-color: #1565C0;"
                   "}"
                   "QMessageBox QPushButton:pressed {"
                   "background-color: #0D47A1;"
                   "}";
    msgBox.setStyleSheet(style);
    msgBox.exec();
}

void gestionemploye::afficherMessageSucces(const QString& titre, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(titre);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    
    QString style = "QMessageBox {"
                   "background-color: #1a2e2a;"
                   "border: 1px solid #162e28;"
                   "}"
                   "QMessageBox QLabel {"
                   "background-color: #2d4d44;"
                   "color: #f0f0f0;"
                   "font-size: 14px;"
                   "font-weight: 500;"
                   "padding: 20px 25px;"
                   "border-radius: 8px;"
                   "border: 1px solid #3d5d54;"
                   "min-width: 350px;"
                   "line-height: 1.5;"
                   "}"
                   "QMessageBox QPushButton {"
                   "background-color: #4CAF50;"
                   "color: #ffffff;"
                   "border: none;"
                   "border-radius: 6px;"
                   "padding: 10px 30px;"
                   "min-width: 100px;"
                   "min-height: 35px;"
                   "font-weight: 600;"
                   "font-size: 13px;"
                   "}"
                   "QMessageBox QPushButton:hover {"
                   "background-color: #45a049;"
                   "}"
                   "QMessageBox QPushButton:pressed {"
                   "background-color: #3d8b40;"
                   "}";
    msgBox.setStyleSheet(style);
    msgBox.exec();
}

int gestionemploye::afficherMessageConfirmation(const QString& titre, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(titre);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    
    QString style = "QMessageBox {"
                   "background-color: #2e1a1a;"
                   "border: 1px solid #281618;"
                   "}"
                   "QMessageBox QLabel {"
                   "background-color: #4d2d2d;"
                   "color: #f0f0f0;"
                   "font-size: 14px;"
                   "font-weight: 500;"
                   "padding: 20px 25px;"
                   "border-radius: 8px;"
                   "border: 1px solid #5d3d3d;"
                   "min-width: 350px;"
                   "line-height: 1.5;"
                   "}"
                   "QMessageBox QPushButton {"
                   "color: #ffffff;"
                   "border: none;"
                   "border-radius: 6px;"
                   "padding: 10px 30px;"
                   "min-width: 100px;"
                   "min-height: 35px;"
                   "font-weight: 600;"
                   "font-size: 13px;"
                   "}"
                   "QMessageBox QPushButton[text=\"&Yes\"] {"
                   "background-color: #f44336;"
                   "}"
                   "QMessageBox QPushButton[text=\"&Yes\"]:hover {"
                   "background-color: #e53935;"
                   "}"
                   "QMessageBox QPushButton[text=\"&Yes\"]:pressed {"
                   "background-color: #d32f2f;"
                   "}"
                   "QMessageBox QPushButton[text=\"&No\"] {"
                   "background-color: #607D8B;"
                   "}"
                   "QMessageBox QPushButton[text=\"&No\"]:hover {"
                   "background-color: #546E7A;"
                   "}"
                   "QMessageBox QPushButton[text=\"&No\"]:pressed {"
                   "background-color: #455A64;"
                   "}";
    msgBox.setStyleSheet(style);
    
    return msgBox.exec();
}

bool gestionemploye::validerFormulaire()
{
    // Validation du nom
    QString nom = ui->txtNom_2->text().trimmed();
    if (nom.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le nom de l'employé.");
        ui->txtNom_2->setFocus();
        return false;
    }
    if (!validerNomPrenom(nom)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du nom incorrect.\n\nLe nom doit:\n- Contenir uniquement des lettres\n- Faire entre 2 et 50 caractères\n\nExemple: Dupont");
        ui->txtNom_2->setFocus();
        return false;
    }
    
    // Validation du prénom
    QString prenom = ui->txtPrenom_2->text().trimmed();
    if (prenom.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le prénom de l'employé.");
        ui->txtPrenom_2->setFocus();
        return false;
    }
    if (!validerNomPrenom(prenom)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du prénom incorrect.\n\nLe prénom doit:\n- Contenir uniquement des lettres\n- Faire entre 2 et 50 caractères\n\nExemple: Jean");
        ui->txtPrenom_2->setFocus();
        return false;
    }
    
    // Validation de la date de naissance
    QDate dateNaissance = ui->dateNaissance_2->date();
    if (!validerDateNaissance(dateNaissance)) {
        QDate currentDate = QDate::currentDate();
        int age = currentDate.year() - dateNaissance.year();
        if (dateNaissance.month() > currentDate.month() ||
            (dateNaissance.month() == currentDate.month() && dateNaissance.day() > currentDate.day())) {
            age--;
        }
        
        if (dateNaissance > currentDate) {
            afficherMessageErreur("Erreur de validation", 
                "Date de naissance invalide.\n\nLa date de naissance ne peut pas être dans le futur.");
        } else if (age < 18) {
            afficherMessageErreur("Erreur de validation", 
                "Âge invalide.\n\nL'âge minimum requis pour un employé est de 18 ans.\nÂge actuel: " + QString::number(age) + " ans");
        } else {
            afficherMessageErreur("Erreur de validation", 
                "Âge invalide.\n\nL'âge maximum autorisé est de 120 ans.\nÂge actuel: " + QString::number(age) + " ans");
        }
        ui->dateNaissance_2->setFocus();
        return false;
    }
    
    // Validation de l'adresse
    QString adresse = ui->txtAdresse_2->text().trimmed();
    if (adresse.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir l'adresse de l'employé.");
        ui->txtAdresse_2->setFocus();
        return false;
    }
    if (adresse.length() < 5) {
        afficherMessageErreur("Erreur de validation", 
            "L'adresse est trop courte.\n\nVeuillez saisir au moins 5 caractères pour l'adresse complète.");
        ui->txtAdresse_2->setFocus();
        return false;
    }
    
    // Validation du téléphone
    QString telephone = ui->txtTelephone_2->text().trimmed();
    if (telephone.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le numéro de téléphone.");
        ui->txtTelephone_2->setFocus();
        return false;
    }
    if (!validerTelephone(telephone)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du téléphone incorrect.\n\nFormats acceptés:\n- 8 chiffres: 12345678\n- Avec indicatif: +21612345678\n- Avec préfixe: 0021612345678");
        ui->txtTelephone_2->setFocus();
        return false;
    }
    
    // Validation de l'email
    QString email = ui->txtEmail_2->text().trimmed();
    if (email.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir l'adresse email.");
        ui->txtEmail_2->setFocus();
        return false;
    }
    if (!validerEmail(email)) {
        afficherMessageErreur("Erreur de validation", 
            "Format de l'email incorrect.\n\nL'email doit contenir:\n- Un nom d'utilisateur\n- Le symbole @\n- Un domaine (exemple.com)\n\nExemple valide: employe@exemple.com");
        ui->txtEmail_2->setFocus();
        return false;
    }
    
    // Validation du statut (doit être sélectionné)
    if (ui->comboStatut_2->currentText().isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez sélectionner un statut pour l'employé.");
        ui->comboStatut_2->setFocus();
        return false;
    }
    
    return true;
}

void gestionemploye::on_btnAjouter_2_clicked()
{
    if (!validerFormulaire()) {
        return; // La validation a déjà affiché le message d'erreur
    }
    
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données.");
            return;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    
    // Créer la séquence si nécessaire
    creerSequenceEmploye();
    
    // Préparer l'insertion
    query.prepare(
        "INSERT INTO EMPLOYE (id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut) "
        "VALUES (SEQ_EMPLOYE.NEXTVAL, :nom, :prenom, TO_DATE(:date_naissance, 'YYYY-MM-DD'), :adresse, :num_tel, :email, :statut)"
    );
    
    query.bindValue(":nom", ui->txtNom_2->text().trimmed());
    query.bindValue(":prenom", ui->txtPrenom_2->text().trimmed());
    query.bindValue(":date_naissance", ui->dateNaissance_2->date().toString("yyyy-MM-dd"));
    query.bindValue(":adresse", ui->txtAdresse_2->text().trimmed());
    query.bindValue(":num_tel", ui->txtTelephone_2->text().trimmed());
    query.bindValue(":email", ui->txtEmail_2->text().trimmed());
    query.bindValue(":statut", ui->comboStatut_2->currentText());
    
    if (!query.exec()) {
        afficherMessageErreur("Erreur", "Erreur lors de l'ajout de l'employé:\n" + query.lastError().text());
        qDebug() << "Erreur SQL:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        return;
    }
    
    // Commit pour Oracle
    if (!conn.getDatabase().commit()) {
        qDebug() << "Erreur lors du commit:" << conn.getDatabase().lastError().text();
    }
    
    afficherMessageSucces("Succès", "Employé ajouté avec succès.");
    
    // Vider le formulaire
    on_btnVider_2_clicked();
    
    // Recharger la liste des employés
    chargerEmployes();
}

void gestionemploye::on_btnModifier_2_clicked()
{
    if (m_currentEmployeeId <= 0) {
        afficherMessageErreur("Erreur", "Veuillez sélectionner un employé dans le tableau pour le modifier.");
        return;
    }
    
    if (!validerFormulaire()) {
        return; // La validation a déjà affiché le message d'erreur
    }
    
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données.");
            return;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    
    // Préparer la mise à jour
    query.prepare(
        "UPDATE EMPLOYE SET nom = :nom, prenom = :prenom, date_naissance = TO_DATE(:date_naissance, 'YYYY-MM-DD'), "
        "adresse = :adresse, num_tel = :num_tel, email = :email, statut = :statut "
        "WHERE id_employe = :id"
    );
    
    query.bindValue(":id", m_currentEmployeeId);
    query.bindValue(":nom", ui->txtNom_2->text().trimmed());
    query.bindValue(":prenom", ui->txtPrenom_2->text().trimmed());
    query.bindValue(":date_naissance", ui->dateNaissance_2->date().toString("yyyy-MM-dd"));
    query.bindValue(":adresse", ui->txtAdresse_2->text().trimmed());
    query.bindValue(":num_tel", ui->txtTelephone_2->text().trimmed());
    query.bindValue(":email", ui->txtEmail_2->text().trimmed());
    query.bindValue(":statut", ui->comboStatut_2->currentText());
    
    if (!query.exec()) {
        afficherMessageErreur("Erreur", "Erreur lors de la modification de l'employé:\n" + query.lastError().text());
        qDebug() << "Erreur SQL:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        return;
    }
    
    // Commit pour Oracle
    if (!conn.getDatabase().commit()) {
        qDebug() << "Erreur lors du commit:" << conn.getDatabase().lastError().text();
    }
    
    afficherMessageSucces("Succès", "Employé modifié avec succès.");
    
    // Vider le formulaire et réinitialiser l'ID
    m_currentEmployeeId = -1;
    on_btnVider_2_clicked();
    
    // Recharger la liste des employés
    chargerEmployes();
}

void gestionemploye::on_btnSupprimer_2_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Êtes-vous sûr de vouloir supprimer cet employé ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    
    QString style = "QMessageBox {"
                   "background-color: #2e1a1a;"
                   "border: 1px solid #281618;"
                   "}"
                   "QMessageBox QLabel {"
                   "background-color: #4d2d2d;"
                   "color: #f0f0f0;"
                   "font-size: 14px;"
                   "font-weight: 500;"
                   "padding: 20px 25px;"
                   "border-radius: 8px;"
                   "border: 1px solid #5d3d3d;"
                   "min-width: 350px;"
                   "line-height: 1.5;"
                   "}"
                   "QMessageBox QPushButton {"
                   "color: #ffffff;"
                   "border: none;"
                   "border-radius: 6px;"
                   "padding: 10px 30px;"
                   "min-width: 100px;"
                   "min-height: 35px;"
                   "font-weight: 600;"
                   "font-size: 13px;"
                   "}"
                   "QMessageBox QPushButton[text=\"&Yes\"] {"
                   "background-color: #f44336;"
                   "}"
                   "QMessageBox QPushButton[text=\"&Yes\"]:hover {"
                   "background-color: #e53935;"
                   "}"
                   "QMessageBox QPushButton[text=\"&Yes\"]:pressed {"
                   "background-color: #d32f2f;"
                   "}"
                   "QMessageBox QPushButton[text=\"&No\"] {"
                   "background-color: #607D8B;"
                   "}"
                   "QMessageBox QPushButton[text=\"&No\"]:hover {"
                   "background-color: #546E7A;"
                   "}"
                   "QMessageBox QPushButton[text=\"&No\"]:pressed {"
                   "background-color: #455A64;"
                   "}";
    msgBox.setStyleSheet(style);
    
    int reply = msgBox.exec();
    
    if (reply == QMessageBox::Yes) {
        if (m_currentEmployeeId <= 0) {
            afficherMessageErreur("Erreur", "Veuillez sélectionner un employé dans le tableau pour le supprimer.");
            return;
        }
        
        Connection& conn = Connection::createInstance();
        if (!conn.getDatabase().isOpen()) {
            if (!conn.createConnection()) {
                afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données.");
                return;
            }
        }
        
        QSqlQuery query(conn.getDatabase());
        query.prepare("DELETE FROM EMPLOYE WHERE id_employe = :id");
        query.bindValue(":id", m_currentEmployeeId);
        
        if (!query.exec()) {
            afficherMessageErreur("Erreur", "Erreur lors de la suppression de l'employé:\n" + query.lastError().text());
            qDebug() << "Erreur SQL:" << query.lastError().text();
            return;
        }
        
        // Commit pour Oracle
        if (!conn.getDatabase().commit()) {
            qDebug() << "Erreur lors du commit:" << conn.getDatabase().lastError().text();
        }
        
        afficherMessageSucces("Succès", "Employé supprimé avec succès.");
        
        // Vider le formulaire et réinitialiser l'ID
        m_currentEmployeeId = -1;
        on_btnVider_2_clicked();
        
        // Recharger la liste des employés
        chargerEmployes();
    }
}

void gestionemploye::on_btnVider_2_clicked()
{
    ui->txtNom_2->clear();
    ui->txtPrenom_2->clear();
    ui->dateNaissance_2->setDate(QDate::currentDate());
    ui->txtAdresse_2->clear();
    ui->txtTelephone_2->clear();
    ui->txtEmail_2->clear();
    ui->comboStatut_2->setCurrentIndex(0);
}

void gestionemploye::onEmployeeSelectedFromChatbot(int employeeId)
{
    // Charger les informations de l'employé sélectionné dans le formulaire
    remplirFormulaire(employeeId);
    
    // Basculer vers la page principale pour voir les informations
    ui->stackedWidget->setCurrentIndex(0);
    
    // Sélectionner la ligne dans le tableau
    for (int row = 0; row < ui->tableEmployes_2->rowCount(); ++row) {
        QTableWidgetItem* idItem = ui->tableEmployes_2->item(row, 0);
        if (idItem && idItem->text().toInt() == employeeId) {
            ui->tableEmployes_2->selectRow(row);
            ui->tableEmployes_2->scrollToItem(idItem);
            break;
        }
    }
}

void gestionemploye::onFilterTableRequested(const QString& filter)
{
    // Filtrer le tableau avec le nom de l'employé
    ui->txtRecherche_2->setText(filter);
    
    // Basculer vers la page principale pour voir le tableau filtré
    ui->stackedWidget->setCurrentIndex(0);
}

void gestionemploye::chargerEmployes()
{
    actualiserTableau();
}

void gestionemploye::actualiserTableau()
{
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            qDebug() << "Impossible de se connecter à la base de données";
            return;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    
    if (!query.exec("SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut FROM EMPLOYE ORDER BY id_employe")) {
        qDebug() << "Erreur lors du chargement des employés:" << query.lastError().text();
        afficherMessageErreur("Erreur", "Erreur lors du chargement des employés:\n" + query.lastError().text());
        return;
    }
    
    // Vider le tableau
    ui->tableEmployes_2->setRowCount(0);
    
    // Remplir le tableau
    int row = 0;
    while (query.next()) {
        ui->tableEmployes_2->insertRow(row);
        
        ui->tableEmployes_2->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableEmployes_2->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableEmployes_2->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        
        QDate dateNaissance = query.value(3).toDate();
        ui->tableEmployes_2->setItem(row, 3, new QTableWidgetItem(dateNaissance.toString("dd/MM/yyyy")));
        
        ui->tableEmployes_2->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableEmployes_2->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableEmployes_2->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableEmployes_2->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
        
        row++;
    }
    
    // Ajuster les colonnes pour occuper toute la largeur disponible
    if (ui->tableEmployes_2->horizontalHeader()) {
        ui->tableEmployes_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    appliquerTriDepuisSelection();
}

bool gestionemploye::creerSequenceEmploye()
{
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            return false;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    
    // Vérifier si la séquence existe
    query.prepare("SELECT COUNT(*) FROM user_sequences WHERE sequence_name = 'SEQ_EMPLOYE'");
    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification de la séquence:" << query.lastError().text();
        return false;
    }
    
    bool sequenceExists = false;
    if (query.next()) {
        sequenceExists = query.value(0).toInt() > 0;
    }
    
    if (!sequenceExists) {
        // Créer la séquence
        if (!query.exec("CREATE SEQUENCE SEQ_EMPLOYE START WITH 1 INCREMENT BY 1 NOCACHE")) {
            qDebug() << "Erreur lors de la création de la séquence:" << query.lastError().text();
            return false;
        }
        qDebug() << "Séquence SEQ_EMPLOYE créée";
    }
    
    return true;
}

int gestionemploye::obtenirProchainIdEmploye()
{
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            return -1;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    if (query.exec("SELECT SEQ_EMPLOYE.NEXTVAL FROM DUAL")) {
        if (query.next()) {
            return query.value(0).toInt();
        }
    }
    
    return -1;
}

void gestionemploye::on_btnExporterPDF_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }
    
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    
    QTextDocument document;
    QString html = "<html><head><meta charset='UTF-8'></head><body>";
    html += "<h1 style='text-align: center; color: #2C5F75;'>Liste des Employés</h1>";
    html += "<p style='text-align: right;'>Date: " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<table border='1' cellpadding='5' cellspacing='0' style='width: 100%; border-collapse: collapse;'>";
    html += "<tr style='background-color: #90C9A8;'>";
    html += "<th>ID</th><th>Nom</th><th>Prénom</th><th>Date de naissance</th><th>Adresse</th><th>Téléphone</th><th>Email</th><th>Statut</th>";
    html += "</tr>";
    
    for (int row = 0; row < ui->tableEmployes_2->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->tableEmployes_2->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableEmployes_2->item(row, col);
            QString text = item ? item->text() : "";
            html += "<td>" + text.toHtmlEscaped() + "</td>";
        }
        html += "</tr>";
    }
    
    html += "</table>";
    html += "<p style='margin-top: 20px;'>Total: " + QString::number(ui->tableEmployes_2->rowCount()) + " employé(s)</p>";
    html += "</body></html>";
    
    document.setHtml(html);
    document.print(&printer);
    
    afficherMessageSucces("Succès", "Le fichier PDF a été exporté avec succès:\n" + fileName);
}

void gestionemploye::on_btnStatistiques_2_clicked()
{
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données.");
            return;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    
    // Statistiques générales
    int totalEmployes = 0;
    int actifs = 0;
    int enConges = 0;
    int autres = 0;
    
    if (query.exec("SELECT COUNT(*) FROM EMPLOYE")) {
        if (query.next()) {
            totalEmployes = query.value(0).toInt();
        }
    }
    
    if (query.exec("SELECT COUNT(*) FROM EMPLOYE WHERE UPPER(statut) LIKE '%ACTIF%'")) {
        if (query.next()) {
            actifs = query.value(0).toInt();
        }
    }
    
    if (query.exec("SELECT COUNT(*) FROM EMPLOYE WHERE UPPER(statut) LIKE '%CONGÉ%' OR UPPER(statut) LIKE '%CONGES%'")) {
        if (query.next()) {
            enConges = query.value(0).toInt();
        }
    }
    
    autres = totalEmployes - actifs - enConges;
    
    QString stats = QString(
        "📊 <b>STATISTIQUES DES EMPLOYÉS</b><br><br>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━<br><br>"
        "👥 <b>Total des employés:</b> %1<br><br>"
        "✅ <b>Employés actifs:</b> %2 (%3%)<br>"
        "⏸️ <b>En congé:</b> %4 (%5%)<br>"
        "📋 <b>Autres statuts:</b> %6 (%7%)<br><br>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    )
    .arg(totalEmployes)
    .arg(actifs)
    .arg(totalEmployes > 0 ? QString::number((actifs * 100.0) / totalEmployes, 'f', 1) : "0")
    .arg(enConges)
    .arg(totalEmployes > 0 ? QString::number((enConges * 100.0) / totalEmployes, 'f', 1) : "0")
    .arg(autres)
    .arg(totalEmployes > 0 ? QString::number((autres * 100.0) / totalEmployes, 'f', 1) : "0");
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Statistiques");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(stats);
    msgBox.setIcon(QMessageBox::Information);
    
    QString style = "QMessageBox { background-color: #f8f9fa; }"
                   "QMessageBox QLabel { color: #212529; font-size: 14px; padding: 15px; }"
                   "QMessageBox QPushButton { background-color: #8EC7E2; color: black; padding: 10px 20px; border-radius: 5px; }"
                   "QMessageBox QPushButton:hover { background-color: #7ABEDB; }";
    msgBox.setStyleSheet(style);
    
    msgBox.exec();
}

void gestionemploye::on_txtRecherche_2_textChanged(const QString& text)
{
    QString searchText = text.toLower();
    
    for (int row = 0; row < ui->tableEmployes_2->rowCount(); ++row) {
        bool match = false;
        
        for (int col = 0; col < ui->tableEmployes_2->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableEmployes_2->item(row, col);
            if (item && item->text().toLower().contains(searchText)) {
                match = true;
                break;
            }
        }
        
        ui->tableEmployes_2->setRowHidden(row, !match);
    }
}

void gestionemploye::on_tableEmployes_2_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableEmployes_2->selectedItems();
    if (selectedItems.isEmpty()) {
        m_currentEmployeeId = -1;
        return;
    }
    
    int row = selectedItems.first()->row();
    QTableWidgetItem* idItem = ui->tableEmployes_2->item(row, 0);
    if (idItem) {
        int employeeId = idItem->text().toInt();
        remplirFormulaire(employeeId);
    }
}

void gestionemploye::on_comboTri_currentIndexChanged(int)
{
    appliquerTriDepuisSelection();
}

void gestionemploye::remplirFormulaire(int employeeId)
{
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            qDebug() << "Impossible de se connecter à la base de données";
            return;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    query.prepare(
        "SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut "
        "FROM EMPLOYE WHERE id_employe = :id"
    );
    query.bindValue(":id", employeeId);
    
    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération de l'employé:" << query.lastError().text();
        return;
    }
    
    if (query.next()) {
        m_currentEmployeeId = query.value(0).toInt();
        ui->txtNom_2->setText(query.value(1).toString());
        ui->txtPrenom_2->setText(query.value(2).toString());
        
        QDate dateNaissance = query.value(3).toDate();
        if (dateNaissance.isValid()) {
            ui->dateNaissance_2->setDate(dateNaissance);
        }
        
        ui->txtAdresse_2->setText(query.value(4).toString());
        ui->txtTelephone_2->setText(query.value(5).toString());
        ui->txtEmail_2->setText(query.value(6).toString());
        
        // Trouver l'index du statut dans le comboBox
        QString statut = query.value(7).toString();
        int index = ui->comboStatut_2->findText(statut);
        if (index >= 0) {
            ui->comboStatut_2->setCurrentIndex(index);
        } else {
            ui->comboStatut_2->setCurrentText(statut);
        }
    }
}

void gestionemploye::appliquerTriDepuisSelection()
{
    if (!ui->tableEmployes_2 || !ui->comboTri) {
        return;
    }

    int colonne = 0;
    Qt::SortOrder ordre = Qt::AscendingOrder;

    switch (ui->comboTri->currentIndex()) {
    case 0: // ID croissant
        colonne = 0;
        ordre = Qt::AscendingOrder;
        break;
    case 1: // ID décroissant
        colonne = 0;
        ordre = Qt::DescendingOrder;
        break;
    case 2: // Nom A-Z
        colonne = 1;
        ordre = Qt::AscendingOrder;
        break;
    case 3: // Nom Z-A
        colonne = 1;
        ordre = Qt::DescendingOrder;
        break;
    case 4: // Prénom A-Z
        colonne = 2;
        ordre = Qt::AscendingOrder;
        break;
    case 5: // Prénom Z-A
        colonne = 2;
        ordre = Qt::DescendingOrder;
        break;
    default:
        colonne = 0;
        ordre = Qt::AscendingOrder;
        break;
    }

    ui->tableEmployes_2->sortItems(colonne, ordre);
    if (ui->tableEmployes_2->horizontalHeader()) {
        ui->tableEmployes_2->horizontalHeader()->setSortIndicator(colonne, ordre);
        ui->tableEmployes_2->horizontalHeader()->setSortIndicatorShown(true);
    }
}
