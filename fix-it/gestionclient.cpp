#include "gestionclient.h"
#include "ui_gestionclient.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLabel>
#include <QGroupBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QComboBox>
#include <QTextEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include "connection.h"

gestionclient::gestionclient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionclient)
    , m_currentIndex(-1)
{
    ui->setupUi(this);
    configurerValidateurs();
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.connect()) {
        afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données: " + db.getLastError());
    }
    
    QString tableStyle = "QTableWidget {"
                        "background-color: #E3F2FD;"
                        "alternate-background-color: #BBDEFB;"
                        "gridline-color: #90CAF9;"
                        "color: #0D47A1;"
                        "}"
                        "QTableWidget::item {"
                        "border: none;"
                        "padding: 5px;"
                        "color: #0D47A1;"
                        "}"
                        "QTableWidget::item:selected {"
                        "background-color: #64B5F6;"
                        "color: white;"
                        "}"
                        "QHeaderView::section {"
                        "background-color: transparent;"
                        "color: #0D47A1;"
                        "font-weight: bold;"
                        "padding: 8px;"
                        "border: none;"
                        "border-right: 1px solid #90CAF9;"
                        "border-bottom: 1px solid #90CAF9;"
                        "}";
    ui->table_clients->setStyleSheet(tableStyle);
    
    QString labelStyle = "QLabel { color: #0D47A1; }";
    ui->label_search->setStyleSheet(labelStyle);
    
    ui->groupBox_listClients->setStyleSheet("QGroupBox { color: #0D47A1; font-weight: bold; } QGroupBox::title { color: #0D47A1; }");
    
    actualiserTableau();
    
    ui->dateEdit->setDateRange(QDate(1900, 1, 1), QDate::currentDate());
    
    connect(ui->table_clients, &QTableWidget::itemSelectionChanged,
            this, &gestionclient::on_table_clients_itemSelectionChanged);
    
    remplirComboHistorique();
    connect(ui->comboBox_selectClient, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &gestionclient::on_comboBox_selectClient_currentIndexChanged);
}

gestionclient::~gestionclient()
{
    delete ui;
}

void gestionclient::on_pushButton_clicked()
{
    emit HomeCliked();
}

void gestionclient::actualiserTableau()
{
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    ui->table_clients->setRowCount(clients.size());
    
    for (int i = 0; i < clients.size(); ++i) {
        const Client& client = clients[i];
        
        ui->table_clients->setItem(i, 0, new QTableWidgetItem(client.getCin()));
        ui->table_clients->setItem(i, 1, new QTableWidgetItem(client.getNom()));
        ui->table_clients->setItem(i, 2, new QTableWidgetItem(client.getPrenom()));
        ui->table_clients->setItem(i, 3, new QTableWidgetItem(client.getTelephone()));
        ui->table_clients->setItem(i, 4, new QTableWidgetItem(client.getEmail()));
        ui->table_clients->setItem(i, 5, new QTableWidgetItem(client.getAdresse()));
        ui->table_clients->setItem(i, 6, new QTableWidgetItem(client.getDateNaissance().toString("dd/MM/yyyy")));
    }
}

void gestionclient::viderFormulaire()
{
    ui->lineEdit_2->clear();
    ui->lineEdit->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_6->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    m_currentIndex = -1;
}

void gestionclient::remplirFormulaire(const Client& client)
{
    ui->lineEdit_2->setText(client.getNom());
    ui->lineEdit->setText(client.getPrenom());
    ui->lineEdit_3->setText(client.getCin());
    ui->lineEdit_5->setText(client.getTelephone());
    ui->lineEdit_4->setText(client.getEmail());
    ui->lineEdit_6->setText(client.getAdresse());
    ui->dateEdit->setDate(client.getDateNaissance());
}

void gestionclient::configurerValidateurs()
{
    ui->lineEdit_2->setMaxLength(50);
    ui->lineEdit->setMaxLength(50);
    
    QRegularExpression cinRegex("[0-9]*");
    QRegularExpressionValidator* cinValidator = new QRegularExpressionValidator(cinRegex, this);
    ui->lineEdit_3->setValidator(cinValidator);
    ui->lineEdit_3->setMaxLength(8);
    
    QRegularExpression telRegex("[0-9+\\-\\s]*");
    QRegularExpressionValidator* telValidator = new QRegularExpressionValidator(telRegex, this);
    ui->lineEdit_5->setValidator(telValidator);
    ui->lineEdit_5->setMaxLength(15);
    
    ui->lineEdit_4->setMaxLength(100);
    ui->lineEdit_6->setMaxLength(200);
}

bool gestionclient::validerEmail(const QString& email)
{
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = regex.match(email);
    return match.hasMatch();
}

bool gestionclient::validerTelephone(const QString& telephone)
{
    QRegularExpression regex("^(\\+216|00216)?[0-9]{8}$");
    QRegularExpressionMatch match = regex.match(telephone);
    return match.hasMatch();
}

bool gestionclient::validerCin(const QString& cin)
{
    QRegularExpression regex("^[0-9]{8}$");
    QRegularExpressionMatch match = regex.match(cin);
    return match.hasMatch();
}

bool gestionclient::validerNomPrenom(const QString& texte)
{
    QString trimmedText = texte.trimmed();
    if (trimmedText.length() < 2 || trimmedText.length() > 50) {
        return false;
    }
    // Match letters (including accented letters), spaces, hyphens, and apostrophes
    // Using \p{L} for Unicode letters which is more reliable than À-ÿ range
    QRegularExpression regex("^[\\p{L}\\s'-]+$", QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpressionMatch match = regex.match(trimmedText);
    return match.hasMatch();
}

bool gestionclient::validerDateNaissance(const QDate& date)
{
    if (date > QDate::currentDate()) {
        return false;
    }
    int age = QDate::currentDate().year() - date.year();
    if (date.month() > QDate::currentDate().month() ||
        (date.month() == QDate::currentDate().month() && date.day() > QDate::currentDate().day())) {
        age--;
    }
    return age >= 16 && age <= 120;
}

void gestionclient::afficherMessageErreur(const QString& titre, const QString& message)
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

bool gestionclient::validerFormulaire()
{
    QString nom = ui->lineEdit_2->text().trimmed();
    if (nom.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le nom du client.");
        ui->lineEdit_2->setFocus();
        return false;
    }
    if (!validerNomPrenom(nom)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du nom incorrect.\n\nLe nom doit:\n- Contenir uniquement des lettres\n- Faire entre 2 et 50 caractères\n\nExemple: Dupont");
        ui->lineEdit_2->setFocus();
        return false;
    }
    
    QString prenom = ui->lineEdit->text().trimmed();
    if (prenom.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le prénom du client.");
        ui->lineEdit->setFocus();
        return false;
    }
    if (!validerNomPrenom(prenom)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du prénom incorrect.\n\nLe prénom doit:\n- Contenir uniquement des lettres\n- Faire entre 2 et 50 caractères\n\nExemple: Jean");
        ui->lineEdit->setFocus();
        return false;
    }
    
    QString cin = ui->lineEdit_3->text().trimmed();
    if (cin.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le numéro de CIN.");
        ui->lineEdit_3->setFocus();
        return false;
    }
    if (!validerCin(cin)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du CIN incorrect.\n\nLe CIN doit contenir exactement 8 chiffres.\n\nExemple: 12345678");
        ui->lineEdit_3->setFocus();
        return false;
    }
    
    QString telephone = ui->lineEdit_5->text().trimmed();
    if (telephone.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir le numéro de téléphone.");
        ui->lineEdit_5->setFocus();
        return false;
    }
    if (!validerTelephone(telephone)) {
        afficherMessageErreur("Erreur de validation", 
            "Format du téléphone incorrect.\n\nFormats acceptés:\n- 8 chiffres: 12345678\n- Avec indicatif: +21612345678\n- Avec préfixe: 0021612345678");
        ui->lineEdit_5->setFocus();
        return false;
    }
    
    QString email = ui->lineEdit_4->text().trimmed();
    if (email.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir l'adresse email.");
        ui->lineEdit_4->setFocus();
        return false;
    }
    if (!validerEmail(email)) {
        afficherMessageErreur("Erreur de validation", 
            "Format de l'email incorrect.\n\nL'email doit contenir:\n- Un nom d'utilisateur\n- Le symbole @\n- Un domaine (exemple.com)\n\nExemple valide: client@exemple.com");
        ui->lineEdit_4->setFocus();
        return false;
    }
    
    QString adresse = ui->lineEdit_6->text().trimmed();
    if (adresse.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Veuillez saisir l'adresse du client.");
        ui->lineEdit_6->setFocus();
        return false;
    }
    if (adresse.length() < 5) {
        afficherMessageErreur("Erreur de validation", 
            "L'adresse est trop courte.\n\nVeuillez saisir au moins 5 caractères pour l'adresse complète.");
        ui->lineEdit_6->setFocus();
        return false;
    }
    
    QDate dateNaissance = ui->dateEdit->date();
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
        } else if (age < 16) {
            afficherMessageErreur("Erreur de validation", 
                "Âge invalide.\n\nL'âge minimum requis est de 16 ans.\nÂge actuel: " + QString::number(age) + " ans");
        } else {
            afficherMessageErreur("Erreur de validation", 
                "Âge invalide.\n\nL'âge maximum autorisé est de 120 ans.\nÂge actuel: " + QString::number(age) + " ans");
        }
        ui->dateEdit->setFocus();
        return false;
    }
    
    return true;
}

int gestionclient::trouverClientParCin(const QString& cin)
{
    DatabaseManager& db = DatabaseManager::getInstance();
    if (db.clientExists(cin)) {
        QList<Client> clients = db.getAllClients();
        for (int i = 0; i < clients.size(); ++i) {
            if (clients[i].getCin() == cin) {
                return i;
            }
        }
    }
    return -1;
}

void gestionclient::afficherMessageSucces(const QString& titre, const QString& message)
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

void gestionclient::on_btn_addClient_clicked()
{
    if (!validerFormulaire()) {
        return;
    }
    
    QString cin = ui->lineEdit_3->text().trimmed();
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (db.clientExists(cin)) {
        afficherMessageErreur("Erreur", "Un client avec ce CIN existe déjà.");
        return;
    }
    
    Client client(
        cin,
        ui->lineEdit_2->text().trimmed(),
        ui->lineEdit->text().trimmed(),
        ui->lineEdit_5->text().trimmed(),
        ui->lineEdit_4->text().trimmed(),
        ui->lineEdit_6->text().trimmed(),
        ui->dateEdit->date()
    );
    
    if (!db.insertClient(client)) {
        afficherMessageErreur("Erreur", "Erreur lors de l'ajout du client: " + db.getLastError());
        return;
    }
    
    envoyerNotificationEmail(client, "ajout");
    
    actualiserTableau();
    remplirComboHistorique();
    viderFormulaire();
    
    afficherMessageSucces("Succès", "Client ajouté avec succès.");
}

void gestionclient::on_btn_modifier_clicked()
{
    if (m_currentIndex == -1) {
        afficherMessageErreur("Attention", "Veuillez sélectionner un client à modifier.");
        return;
    }
    
    if (!validerFormulaire()) {
        return;
    }
    
    QString cin = ui->lineEdit_3->text().trimmed();
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    if (m_currentIndex >= 0 && m_currentIndex < clients.size()) {
        QString oldCin = clients[m_currentIndex].getCin();
        if (cin != oldCin && db.clientExists(cin)) {
            afficherMessageErreur("Erreur", "Un autre client avec ce CIN existe déjà.");
            return;
        }
    }
    
    Client client(
        cin,
        ui->lineEdit_2->text().trimmed(),
        ui->lineEdit->text().trimmed(),
        ui->lineEdit_5->text().trimmed(),
        ui->lineEdit_4->text().trimmed(),
        ui->lineEdit_6->text().trimmed(),
        ui->dateEdit->date()
    );
    
    if (m_currentIndex >= 0 && m_currentIndex < clients.size()) {
        QString oldCin = clients[m_currentIndex].getCin();
        if (cin != oldCin) {
            if (!db.deleteClient(oldCin)) {
                afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
                return;
            }
            if (!db.insertClient(client)) {
                afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
                return;
            }
        } else {
            if (!db.updateClient(client)) {
                afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
                return;
            }
        }
    }
    
    envoyerNotificationEmail(client, "modification");
    
    actualiserTableau();
    remplirComboHistorique();
    viderFormulaire();
    
    afficherMessageSucces("Succès", "Client modifié avec succès.");
}

void gestionclient::on_btn_supprimer_clicked()
{
    if (m_currentIndex == -1) {
        afficherMessageErreur("Attention", "Veuillez sélectionner un client à supprimer.");
        return;
    }
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Êtes-vous sûr de vouloir supprimer ce client ?");
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
        DatabaseManager& db = DatabaseManager::getInstance();
        QList<Client> clients = db.getAllClients();
        
        if (m_currentIndex >= 0 && m_currentIndex < clients.size()) {
            QString cin = clients[m_currentIndex].getCin();
            if (!db.deleteClient(cin)) {
                afficherMessageErreur("Erreur", "Erreur lors de la suppression: " + db.getLastError());
                return;
            }
        }
        
        actualiserTableau();
        viderFormulaire();
        afficherMessageSucces("Succès", "Client supprimé avec succès.");
    }
}

void gestionclient::on_table_clients_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = ui->table_clients->selectedItems();
    if (selectedItems.isEmpty()) {
        viderFormulaire();
        m_currentIndex = -1;
        return;
    }
    
    int row = selectedItems.first()->row();
    QTableWidgetItem* cinItem = ui->table_clients->item(row, 0);
    if (cinItem) {
        QString cin = cinItem->text();
        DatabaseManager& db = DatabaseManager::getInstance();
        Client client = db.getClientByCin(cin);
        
        if (!client.getCin().isEmpty()) {
            m_currentIndex = row;
            remplirFormulaire(client);
        }
    }
}

void gestionclient::on_btn_search_clicked()
{
    QString searchText = ui->lineEdit_search->text().trimmed().toLower();
    
    if (searchText.isEmpty()) {
        actualiserTableau();
        return;
    }
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> allClients = db.getAllClients();
    
    QList<Client> filteredClients;
    for (const Client& client : allClients) {
        if (client.getNom().toLower().contains(searchText) ||
            client.getPrenom().toLower().contains(searchText) ||
            client.getCin().toLower().contains(searchText) ||
            client.getTelephone().contains(searchText) ||
            client.getEmail().toLower().contains(searchText) ||
            client.getAdresse().toLower().contains(searchText)) {
            filteredClients.append(client);
        }
    }
    
    ui->table_clients->setRowCount(filteredClients.size());
    for (int i = 0; i < filteredClients.size(); ++i) {
        const Client& client = filteredClients[i];
        ui->table_clients->setItem(i, 0, new QTableWidgetItem(client.getCin()));
        ui->table_clients->setItem(i, 1, new QTableWidgetItem(client.getNom()));
        ui->table_clients->setItem(i, 2, new QTableWidgetItem(client.getPrenom()));
        ui->table_clients->setItem(i, 3, new QTableWidgetItem(client.getTelephone()));
        ui->table_clients->setItem(i, 4, new QTableWidgetItem(client.getEmail()));
        ui->table_clients->setItem(i, 5, new QTableWidgetItem(client.getAdresse()));
        ui->table_clients->setItem(i, 6, new QTableWidgetItem(client.getDateNaissance().toString("dd/MM/yyyy")));
    }
}

void gestionclient::on_btn_modifier_2_clicked()
{
    exporterPDF();
}

void gestionclient::exporterPDF()
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
    html += "<h1 style='text-align: center; color: #2C5F75;'>Liste des Clients</h1>";
    html += "<p style='text-align: right;'>Date: " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<table border='1' cellpadding='5' cellspacing='0' style='width: 100%; border-collapse: collapse;'>";
    html += "<tr style='background-color: #90C9A8;'>";
    html += "<th>CIN</th><th>Nom</th><th>Prénom</th><th>Téléphone</th><th>Email</th><th>Adresse</th><th>Date de Naissance</th>";
    html += "</tr>";
    
    for (int row = 0; row < ui->table_clients->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->table_clients->columnCount(); ++col) {
            QTableWidgetItem* item = ui->table_clients->item(row, col);
            QString text = item ? item->text() : "";
            html += "<td>" + text.toHtmlEscaped() + "</td>";
        }
        html += "</tr>";
    }
    
    html += "</table>";
    html += "<p style='margin-top: 20px;'>Total: " + QString::number(ui->table_clients->rowCount()) + " client(s)</p>";
    html += "</body></html>";
    
    document.setHtml(html);
    document.print(&printer);
    
    afficherMessageSucces("Succès", "Le fichier PDF a été exporté avec succès:\n" + fileName);
}

void gestionclient::on_btn_search_2_clicked()
{
    afficherStatistiques();
}

void gestionclient::afficherStatistiques()
{
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    int totalClients = clients.size();
    int avecEmail = 0;
    int avecTelephone = 0;
    
    for (const Client& client : clients) {
        if (!client.getEmail().isEmpty()) avecEmail++;
        if (!client.getTelephone().isEmpty()) avecTelephone++;
    }
    
    QString stats = QString(
        "<div style='color: #1a237e;'>"
        "📊 <b style='color: #1a237e;'>STATISTIQUES DES CLIENTS</b><br><br>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━<br><br>"
        "👥 <b style='color: #1a237e;'>Total des clients:</b> <span style='color: #1a237e;'>%1</span><br><br>"
        "📧 <b style='color: #1a237e;'>Clients avec email:</b> <span style='color: #1a237e;'>%2 (%3%)</span><br>"
        "📱 <b style='color: #1a237e;'>Clients avec téléphone:</b> <span style='color: #1a237e;'>%4 (%5%)</span><br><br>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        "</div>"
    )
    .arg(totalClients)
    .arg(avecEmail)
    .arg(totalClients > 0 ? QString::number((avecEmail * 100.0) / totalClients, 'f', 1) : "0")
    .arg(avecTelephone)
    .arg(totalClients > 0 ? QString::number((avecTelephone * 100.0) / totalClients, 'f', 1) : "0");
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Statistiques");
    msgBox.setText(stats);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void gestionclient::on_btn_notifier_clicked()
{
    if (m_currentIndex == -1) {
        afficherMessageErreur("Attention", "Veuillez sélectionner un client à notifier.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    if (m_currentIndex >= 0 && m_currentIndex < clients.size()) {
        Client client = clients[m_currentIndex];
        envoyerNotificationEmail(client, "notification");
        afficherMessageSucces("Succès", "Notification envoyée à " + client.getEmail());
    }
}

void gestionclient::envoyerNotificationEmail(const Client& client, const QString& typeOperation)
{
    if (client.getEmail().isEmpty() || !validerEmail(client.getEmail())) {
        qDebug() << "Email invalide ou vide pour le client" << client.getNom();
        return;
    }
    
    QString sujet;
    QString message;
    
    if (typeOperation == "ajout") {
        sujet = "Bienvenue - Inscription confirmée";
        message = QString(
            "Bonjour %1 %2,\n\n"
            "Votre inscription a été enregistrée avec succès dans notre système.\n\n"
            "Détails de votre compte:\n"
            "- CIN: %3\n"
            "- Nom: %1\n"
            "- Prénom: %2\n"
            "- Téléphone: %4\n"
            "- Email: %5\n"
            "- Adresse: %6\n\n"
            "Merci de votre confiance.\n\n"
            "Cordialement,\n"
            "L'équipe de gestion"
        ).arg(client.getNom())
         .arg(client.getPrenom())
         .arg(client.getCin())
         .arg(client.getTelephone())
         .arg(client.getEmail())
         .arg(client.getAdresse());
    } else if (typeOperation == "modification") {
        sujet = "Mise à jour de vos informations";
        message = QString(
            "Bonjour %1 %2,\n\n"
            "Vos informations ont été mises à jour avec succès.\n\n"
            "Nouvelles informations:\n"
            "- CIN: %3\n"
            "- Nom: %1\n"
            "- Prénom: %2\n"
            "- Téléphone: %4\n"
            "- Email: %5\n"
            "- Adresse: %6\n\n"
            "Si vous n'avez pas effectué cette modification, veuillez nous contacter immédiatement.\n\n"
            "Cordialement,\n"
            "L'équipe de gestion"
        ).arg(client.getNom())
         .arg(client.getPrenom())
         .arg(client.getCin())
         .arg(client.getTelephone())
         .arg(client.getEmail())
         .arg(client.getAdresse());
    } else {
        sujet = "Notification importante";
        message = QString(
            "Bonjour %1 %2,\n\n"
            "Ceci est une notification concernant votre compte.\n\n"
            "Informations de votre compte:\n"
            "- CIN: %3\n"
            "- Nom: %1\n"
            "- Prénom: %2\n"
            "- Téléphone: %4\n"
            "- Email: %5\n"
            "- Adresse: %6\n\n"
            "Cordialement,\n"
            "L'équipe de gestion"
        ).arg(client.getNom())
         .arg(client.getPrenom())
         .arg(client.getCin())
         .arg(client.getTelephone())
         .arg(client.getEmail())
         .arg(client.getAdresse());
    }
    
    QUrl mailtoUrl;
    mailtoUrl.setScheme("mailto");
    mailtoUrl.setPath(client.getEmail());
    QUrlQuery query;
    query.addQueryItem("subject", sujet);
    query.addQueryItem("body", message);
    mailtoUrl.setQuery(query);
    
    QDesktopServices::openUrl(mailtoUrl);
    
    qDebug() << "Notification email préparée pour:" << client.getEmail();
}

void gestionclient::remplirComboHistorique()
{
    ui->comboBox_selectClient->clear();
    ui->comboBox_selectClient->addItem("-- Sélectionner un client --", "");
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    for (const Client& client : clients) {
        QString displayText = QString("%1 %2 (%3)").arg(client.getNom()).arg(client.getPrenom()).arg(client.getCin());
        ui->comboBox_selectClient->addItem(displayText, client.getCin());
    }
}

void gestionclient::on_btn_showHistory_clicked()
{
    QString cin = ui->comboBox_selectClient->currentData().toString();
    if (cin.isEmpty()) {
        afficherMessageErreur("Attention", "Veuillez sélectionner un client.");
        return;
    }
    
    afficherHistorique(cin);
}

void gestionclient::on_comboBox_selectClient_currentIndexChanged(int index)
{
    if (index > 0) {
        QString cin = ui->comboBox_selectClient->itemData(index).toString();
        if (!cin.isEmpty()) {
            afficherHistorique(cin);
        }
    } else {
        ui->textEdit_history->clear();
    }
}

void gestionclient::afficherHistorique(const QString& cin)
{
    DatabaseManager& db = DatabaseManager::getInstance();
    Client client = db.getClientByCin(cin);
    
    if (client.getCin().isEmpty()) {
        ui->textEdit_history->setPlainText("Client non trouvé.");
        return;
    }
    
    QString historique = QString(
        "═══════════════════════════════════════════════════════════\n"
        "📋 HISTORIQUE DU CLIENT\n"
        "═══════════════════════════════════════════════════════════\n\n"
        "👤 INFORMATIONS PERSONNELLES:\n"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        "• CIN: %1\n"
        "• Nom: %2\n"
        "• Prénom: %3\n"
        "• Date de naissance: %4\n"
        "• Téléphone: %5\n"
        "• Email: %6\n"
        "• Adresse: %7\n\n"
    ).arg(client.getCin())
     .arg(client.getNom())
     .arg(client.getPrenom())
     .arg(client.getDateNaissance().toString("dd/MM/yyyy"))
     .arg(client.getTelephone())
     .arg(client.getEmail())
     .arg(client.getAdresse());
    
    QSqlDatabase& database = Connection::createInstance().getDatabase();
    if (database.isOpen()) {
        QSqlQuery query(database);
        
        QStringList tableNames = {"POSSEDER", "REPARER"};
        bool hasHistory = false;
        
        for (const QString& tableName : tableNames) {
            QString checkQuery = QString("SELECT COUNT(*) FROM %1 WHERE ID_CLIENT = (SELECT ID_CLIENT FROM CLIENT WHERE CIN = :cin)")
                                .arg(tableName);
            
            query.prepare(checkQuery);
            query.bindValue(":cin", cin);
            
            if (query.exec() && query.next() && query.value(0).toInt() > 0) {
                hasHistory = true;
                historique += QString("📦 HISTORIQUE DES OBJETS:\n");
                historique += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                
                if (tableName == "POSSEDER") {
                    QString histQuery = "SELECT o.NOM_OBJET, o.MARQUE, p.DATE_DEBUT, p.DATE_FIN "
                                       "FROM POSSEDER p "
                                       "JOIN OBJET_ELECTRONIQUE o ON p.ID_OBJET = o.ID_OBJET "
                                       "WHERE p.ID_CLIENT = (SELECT ID_CLIENT FROM CLIENT WHERE CIN = :cin) "
                                       "ORDER BY p.DATE_DEBUT DESC";
                    query.prepare(histQuery);
                    query.bindValue(":cin", cin);
                    
                    if (query.exec()) {
                        int count = 0;
                        while (query.next() && count < 10) {
                            historique += QString("• %1 - %2 (Du %3 au %4)\n")
                                         .arg(query.value(0).toString())
                                         .arg(query.value(1).toString())
                                         .arg(query.value(2).toDate().toString("dd/MM/yyyy"))
                                         .arg(query.value(3).toDate().toString("dd/MM/yyyy"));
                            count++;
                        }
                    }
                }
                
                historique += "\n";
                break;
            }
        }
        
        if (!hasHistory) {
            historique += "📦 HISTORIQUE DES OBJETS:\n";
            historique += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            historique += "Aucun objet enregistré pour ce client.\n\n";
        }
    }
    
    historique += "═══════════════════════════════════════════════════════════\n";
    historique += QString("Date de consultation: %1\n").arg(QDate::currentDate().toString("dd/MM/yyyy"));
    historique += "═══════════════════════════════════════════════════════════\n";
    
    ui->textEdit_history->setPlainText(historique);
}

