#include "gestionclient.h"
#include "ui_gestionclient.h"
#include "statcirclewidget.h"
#include "agepiechartwidget.h"
#include <QMessageBox>
#include <QDate>
#include <QScrollArea>
#include <QTableWidgetItem>
#include <QHeaderView>
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
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QFont>
#include <QDateTime>
#include <QPixmap>
#include <QBuffer>
#include <QTime>
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
                        "background-color: #FFFFFF;"
                        "alternate-background-color: #F5F9FF;"
                        "gridline-color: #E0E0E0;"
                        "color: #1A1A1A;"
                        "border: 2px solid #0D47A1;"
                        "border-radius: 8px;"
                        "selection-background-color: #1976D2;"
                        "selection-color: #FFFFFF;"
                        "font-size: 11px;"
                        "}"
                        "QTableWidget::item {"
                        "border: none;"
                        "border-bottom: 1px solid #E3F2FD;"
                        "padding: 10px 8px;"
                        "color: #1A1A1A;"
                        "}"
                        "QTableWidget::item:alternate {"
                        "background-color: #F5F9FF;"
                        "}"
                        "QTableWidget::item:selected {"
                        "background-color: #1976D2;"
                        "color: #FFFFFF;"
                        "font-weight: bold;"
                        "}"
                        "QTableWidget::item:hover {"
                        "background-color: #E3F2FD;"
                        "}"
                        "QHeaderView::section {"
                        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                        "stop:0 #0D47A1, stop:1 #1565C0);"
                        "color: #FFFFFF;"
                        "font-weight: bold;"
                        "font-size: 12px;"
                        "padding: 12px 10px;"
                        "border: none;"
                        "border-right: 1px solid #0A3D7A;"
                        "border-bottom: 2px solid #0A3D7A;"
                        "text-align: left;"
                        "}"
                        "QHeaderView::section:first {"
                        "border-top-left-radius: 6px;"
                        "}"
                        "QHeaderView::section:last {"
                        "border-top-right-radius: 6px;"
                        "border-right: none;"
                        "}"
                        "QTableCornerButton::section {"
                        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                        "stop:0 #0D47A1, stop:1 #1565C0);"
                        "border-top-left-radius: 6px;"
                        "border: none;"
                        "}";
    ui->table_clients->setStyleSheet(tableStyle);
    
    QString labelStyle = "QLabel { color: #0D47A1; }";
    ui->label_search->setStyleSheet(labelStyle);
    
    ui->groupBox_listClients->setStyleSheet("QGroupBox { color: #0D47A1; font-weight: bold; } QGroupBox::title { color: #0D47A1; }");
    
    ui->table_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_clients->horizontalHeader()->setStretchLastSection(true);
    ui->table_clients->setColumnWidth(6, 150);
    ui->table_clients->setSortingEnabled(true);
    ui->table_clients->setShowGrid(false);
    ui->table_clients->setAlternatingRowColors(true);
    ui->table_clients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_clients->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_clients->verticalHeader()->setVisible(false);
    ui->table_clients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    actualiserTableau();
    
    ui->dateEdit->setDateRange(QDate(1900, 1, 1), QDate::currentDate());
    
    connect(ui->table_clients, &QTableWidget::itemSelectionChanged,
            this, &gestionclient::on_table_clients_itemSelectionChanged);
    
    connect(ui->lineEdit_search, &QLineEdit::textChanged,
            this, &gestionclient::on_searchTextChanged);
    
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

QPair<QString, QString> gestionclient::getPieceIdAndStateForClient(int idClient) const
{
    QPair<QString, QString> result;
    result.first = "";
    result.second = "";
    
    QSqlDatabase& database = Connection::createInstance().getDatabase();
    if (!database.isOpen()) {
        return result;
    }
    
    QSqlQuery query(database);
    
    QString pieceQuery = "SELECT * FROM ("
                        "SELECT DISTINCT u.ID_PIECE "
                        "FROM OBJET_ELECTRONIQUE o "
                        "JOIN UTILISER u ON o.ID_OBJET = u.ID_OBJET "
                        "WHERE o.ID_CLIENT_COURANT = :id_client "
                        "ORDER BY u.DATE_UTILISATION DESC NULLS LAST"
                        ") WHERE ROWNUM <= 1";
    
    query.prepare(pieceQuery);
    query.bindValue(":id_client", idClient);
    
    if (query.exec() && query.next()) {
        QVariant pieceValue = query.value(0);
        if (!pieceValue.isNull() && pieceValue.isValid()) {
            result.first = pieceValue.toString();
        }
    }
    
    QString etatQuery = "SELECT * FROM ("
                       "SELECT e.ETAT "
                       "FROM ETAT_OBJET e "
                       "JOIN OBJET_ELECTRONIQUE o ON e.ID_OBJET = o.ID_OBJET "
                       "WHERE e.ID_CLIENT = :id_client "
                       "AND o.ID_CLIENT_COURANT = :id_client "
                       "ORDER BY e.DATE_MODIFICATION DESC"
                       ") WHERE ROWNUM <= 1";
    
    query.prepare(etatQuery);
    query.bindValue(":id_client", idClient);
    
    if (query.exec() && query.next()) {
        QVariant etatValue = query.value(0);
        if (!etatValue.isNull() && etatValue.isValid()) {
            result.second = etatValue.toString();
        }
    }
    
    if (result.second.isEmpty()) {
        QString etatFallbackQuery = "SELECT o.ETAT "
                                   "FROM OBJET_ELECTRONIQUE o "
                                   "WHERE o.ID_CLIENT_COURANT = :id_client "
                                   "AND ROWNUM = 1";
        query.prepare(etatFallbackQuery);
        query.bindValue(":id_client", idClient);
        if (query.exec() && query.next()) {
            QVariant etatValue = query.value(0);
            if (!etatValue.isNull() && etatValue.isValid()) {
                result.second = etatValue.toString();
            }
        }
    }
    
    if (result.first.isEmpty()) {
        result.first = "N/A";
    }
    if (result.second.isEmpty()) {
        result.second = "N/A";
    }
    
    return result;
}

QString gestionclient::getIdObjetForClient(int idClient) const
{
    QSqlDatabase& database = Connection::createInstance().getDatabase();
    if (!database.isOpen()) {
        return "N/A";
    }
    
    QSqlQuery query(database);
    QString idObjetQuery = "SELECT ID_OBJET "
                          "FROM OBJET_ELECTRONIQUE "
                          "WHERE ID_CLIENT_COURANT = :id_client "
                          "AND ROWNUM = 1";
    
    query.prepare(idObjetQuery);
    query.bindValue(":id_client", idClient);
    
    if (query.exec() && query.next()) {
        QVariant idObjetValue = query.value(0);
        if (!idObjetValue.isNull() && idObjetValue.isValid()) {
            return idObjetValue.toString();
        }
    }
    
    return "N/A";
}

void gestionclient::actualiserTableau()
{
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    ui->table_clients->setRowCount(clients.size());
    
    for (int i = 0; i < clients.size(); ++i) {
        const Client& client = clients[i];
        
        ui->table_clients->setItem(i, 0, new QTableWidgetItem(client.getCin().isEmpty() ? "N/A" : client.getCin()));
        ui->table_clients->setItem(i, 1, new QTableWidgetItem(client.getNom().isEmpty() ? "N/A" : client.getNom()));
        ui->table_clients->setItem(i, 2, new QTableWidgetItem(client.getPrenom().isEmpty() ? "N/A" : client.getPrenom()));
        ui->table_clients->setItem(i, 3, new QTableWidgetItem(client.getTelephone().isEmpty() ? "N/A" : client.getTelephone()));
        ui->table_clients->setItem(i, 4, new QTableWidgetItem(client.getEmail().isEmpty() ? "N/A" : client.getEmail()));
        ui->table_clients->setItem(i, 5, new QTableWidgetItem(client.getAdresse().isEmpty() ? "N/A" : client.getAdresse()));
        
        QString dateStr;
        if (client.getDateNaissance().isValid()) {
            dateStr = client.getDateNaissance().toString("dd/MM/yyyy");
        } else {
            dateStr = "Non renseignée";
        }
        ui->table_clients->setItem(i, 6, new QTableWidgetItem(dateStr));
        
        int idClient = db.getClientIdByCin(client.getCin());
        QPair<QString, QString> pieceInfo = getPieceIdAndStateForClient(idClient);
        ui->table_clients->setItem(i, 7, new QTableWidgetItem(pieceInfo.first));
        
        QString idObjet = getIdObjetForClient(idClient);
        ui->table_clients->setItem(i, 8, new QTableWidgetItem(idObjet));
    }
    
    ui->table_clients->resizeColumnsToContents();
    ui->table_clients->setColumnWidth(6, 150);
    ui->table_clients->sortItems(1, Qt::AscendingOrder);
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

QString gestionclient::normalizeString(const QString& str) const
{
    QString normalized = str.normalized(QString::NormalizationForm_KD);
    QString result;
    for (QChar c : normalized) {
        if (c.category() != QChar::Mark_NonSpacing) {
            result.append(c.toLower());
        }
    }
    return result;
}

void gestionclient::on_searchTextChanged(const QString& text)
{
    QString searchText = text.trimmed();
    
    if (searchText.isEmpty()) {
        actualiserTableau();
        return;
    }
    
    performSearch(searchText);
}

void gestionclient::on_btn_search_clicked()
{
    QString searchText = ui->lineEdit_search->text().trimmed();
    performSearch(searchText);
}

void gestionclient::performSearch(const QString& searchText)
{
    if (searchText.isEmpty()) {
        actualiserTableau();
        return;
    }
    
    QString normalizedSearch = normalizeString(searchText);
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> allClients = db.getAllClients();
    
    QList<Client> filteredClients;
    for (const Client& client : allClients) {
        bool matches = false;
        
        if (normalizeString(client.getNom()).contains(normalizedSearch) ||
            normalizeString(client.getPrenom()).contains(normalizedSearch) ||
            normalizeString(client.getCin()).contains(normalizedSearch) ||
            normalizeString(client.getTelephone()).contains(normalizedSearch) ||
            normalizeString(client.getEmail()).contains(normalizedSearch) ||
            normalizeString(client.getAdresse()).contains(normalizedSearch)) {
            matches = true;
        }
        
        if (!matches && client.getDateNaissance().isValid()) {
            QString dateStr = client.getDateNaissance().toString("dd/MM/yyyy");
            QString dateStrAlt = client.getDateNaissance().toString("dd-MM-yyyy");
            if (dateStr.contains(searchText) || dateStrAlt.contains(searchText)) {
                matches = true;
            }
        }
        
        if (matches) {
            filteredClients.append(client);
        }
    }
    
    ui->table_clients->setRowCount(filteredClients.size());
    for (int i = 0; i < filteredClients.size(); ++i) {
        const Client& client = filteredClients[i];
        ui->table_clients->setItem(i, 0, new QTableWidgetItem(client.getCin().isEmpty() ? "N/A" : client.getCin()));
        ui->table_clients->setItem(i, 1, new QTableWidgetItem(client.getNom().isEmpty() ? "N/A" : client.getNom()));
        ui->table_clients->setItem(i, 2, new QTableWidgetItem(client.getPrenom().isEmpty() ? "N/A" : client.getPrenom()));
        ui->table_clients->setItem(i, 3, new QTableWidgetItem(client.getTelephone().isEmpty() ? "N/A" : client.getTelephone()));
        ui->table_clients->setItem(i, 4, new QTableWidgetItem(client.getEmail().isEmpty() ? "N/A" : client.getEmail()));
        ui->table_clients->setItem(i, 5, new QTableWidgetItem(client.getAdresse().isEmpty() ? "N/A" : client.getAdresse()));
        
        QString dateStr;
        if (client.getDateNaissance().isValid()) {
            dateStr = client.getDateNaissance().toString("dd/MM/yyyy");
        } else {
            dateStr = "Non renseignée";
        }
        ui->table_clients->setItem(i, 6, new QTableWidgetItem(dateStr));
        
        DatabaseManager& db = DatabaseManager::getInstance();
        int idClient = db.getClientIdByCin(client.getCin());
        QPair<QString, QString> pieceInfo = getPieceIdAndStateForClient(idClient);
        ui->table_clients->setItem(i, 7, new QTableWidgetItem(pieceInfo.first));
        
        QString idObjet = getIdObjetForClient(idClient);
        ui->table_clients->setItem(i, 8, new QTableWidgetItem(idObjet));
    }
    
    ui->table_clients->resizeColumnsToContents();
    ui->table_clients->setColumnWidth(6, 150);
    
    if (filteredClients.size() > 0) {
        ui->table_clients->sortItems(1, Qt::AscendingOrder);
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
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    QPixmap logoPixmap(":/res/equipe.png");
    if (logoPixmap.isNull()) {
        logoPixmap = QPixmap("equipe.png");
    }
    
    QTextDocument document;
    QString html = "<html><head><meta charset='UTF-8'>";
    html += "<style>";
    html += "body { font-family: 'Arial', 'Helvetica', sans-serif; margin: 0; padding: 25px; background-color: #F8F9FA; }";
    html += ".header { text-align: center; margin-bottom: 30px; padding: 20px; background-color: #FFFFFF; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }";
    html += ".logo { max-width: 150px; max-height: 80px; margin-bottom: 15px; }";
    html += "h1 { color: #0D47A1; font-size: 26px; margin: 10px 0; font-weight: bold; letter-spacing: 1px; }";
    html += ".info { color: #616161; font-size: 12px; margin: 5px 0; }";
    html += ".table-wrapper { background-color: #FFFFFF; padding: 15px; border-radius: 8px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); margin-top: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; margin: 0; font-size: 10px; }";
    html += "th { background-color: #0D47A1; color: #FFFFFF; padding: 14px 12px; text-align: left; font-weight: bold; font-size: 11px; border: 1px solid #0A3D7A; }";
    html += "td { padding: 12px 12px; border: 1px solid #E0E0E0; background-color: #FFFFFF; color: #212121; font-size: 10px; }";
    html += "tr:nth-child(even) td { background-color: #F5F9FF; }";
    html += "tr:hover td { background-color: #E3F2FD; }";
    html += ".footer { margin-top: 30px; padding: 20px; background-color: #FFFFFF; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); text-align: center; }";
    html += ".total { font-weight: bold; color: #0D47A1; font-size: 14px; margin-bottom: 8px; }";
    html += ".footer-text { color: #616161; font-size: 11px; }";
    html += "</style></head><body>";
    
    html += "<div class='header'>";
    if (!logoPixmap.isNull()) {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        logoPixmap.save(&buffer, "PNG");
        QString base64Logo = QString::fromLatin1(byteArray.toBase64().data());
        html += "<img src='data:image/png;base64," + base64Logo + "' class='logo' alt='Logo' />";
    }
    html += "<h1>📋 LISTE DES CLIENTS</h1>";
    html += "<div class='info'>Date d'exportation: " + QDate::currentDate().toString("dd/MM/yyyy") + "</div>";
    html += "<div class='info'>Heure: " + QTime::currentTime().toString("HH:mm:ss") + "</div>";
    html += "</div>";
    
    html += "<div class='table-wrapper'>";
    html += "<table>";
    html += "<thead><tr>";
    html += "<th>CIN</th><th>Nom</th><th>Prénom</th><th>Téléphone</th><th>Email</th>";
    html += "<th>Adresse</th><th>Date de Naissance</th><th>Réf Pièce</th><th>ID Objet</th>";
    html += "</tr></thead><tbody>";
    
    for (int row = 0; row < ui->table_clients->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->table_clients->columnCount(); ++col) {
            QTableWidgetItem* item = ui->table_clients->item(row, col);
            QString text = item ? item->text() : "N/A";
            if (text.isEmpty()) {
                text = "N/A";
            }
            html += "<td>" + text.toHtmlEscaped() + "</td>";
        }
        html += "</tr>";
    }
    
    html += "</tbody></table>";
    html += "</div>";
    
    html += "<div class='footer'>";
    html += "<div class='total'>Total: " + QString::number(ui->table_clients->rowCount()) + " client(s)</div>";
    html += "<div class='footer-text'>Système de Gestion - Document généré automatiquement</div>";
    html += "</div>";
    
    html += "</body></html>";
    
    document.setHtml(html);
    document.setPageSize(printer.pageRect(QPrinter::Point).size());
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
    QDate currentDate = QDate::currentDate();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    
    QMap<QString, int> ageGroups;
    ageGroups["Moins de 20 ans"] = 0;
    ageGroups["20-30 ans"] = 0;
    ageGroups["31-40 ans"] = 0;
    ageGroups["41-50 ans"] = 0;
    ageGroups["51-60 ans"] = 0;
    ageGroups["Plus de 60 ans"] = 0;
    ageGroups["Âge non renseigné"] = 0;
    
    for (const Client& client : clients) {
        if (client.getDateNaissance().isValid()) {
            int age = currentDate.year() - client.getDateNaissance().year();
            int monthDiff = currentDate.month() - client.getDateNaissance().month();
            int dayDiff = currentDate.day() - client.getDateNaissance().day();
            
            if (monthDiff < 0 || (monthDiff == 0 && dayDiff < 0)) {
                age--;
            }
            
            if (age < 20) {
                ageGroups["Moins de 20 ans"]++;
            } else if (age >= 20 && age <= 30) {
                ageGroups["20-30 ans"]++;
            } else if (age >= 31 && age <= 40) {
                ageGroups["31-40 ans"]++;
            } else if (age >= 41 && age <= 50) {
                ageGroups["41-50 ans"]++;
            } else if (age >= 51 && age <= 60) {
                ageGroups["51-60 ans"]++;
            } else {
                ageGroups["Plus de 60 ans"]++;
            }
        } else {
            ageGroups["Âge non renseigné"]++;
        }
    }
    
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques des Clients par Âge");
    dialog->setMinimumSize(1000, 700);
    dialog->setStyleSheet(
        "QDialog {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #E3F2FD, stop:1 #BBDEFB);"
        "}"
    );
    
    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    QLabel* titleLabel = new QLabel("📊 Répartition des Clients par Tranche d'Âge", dialog);
    titleLabel->setStyleSheet(
        "QLabel {"
        "font-size: 28px;"
        "font-weight: bold;"
        "color: #0D47A1;"
        "padding: 15px 0px;"
        "background-color: rgba(255, 255, 255, 0.7);"
        "border-radius: 10px;"
        "}"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(titleLabel);
    
    QLabel* updateLabel = new QLabel(dialog);
    QString updateText = QString("🕒 Mise à jour: %1")
                        .arg(currentDateTime.toString("dd/MM/yyyy HH:mm:ss"));
    updateLabel->setText(updateText);
    updateLabel->setStyleSheet(
        "QLabel {"
        "font-size: 11px;"
        "color: #424242;"
        "font-weight: 500;"
        "padding: 8px 12px;"
        "background-color: rgba(255, 255, 255, 0.9);"
        "border-radius: 8px;"
        "border: 1px solid #BBDEFB;"
        "}"
    );
    updateLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    headerLayout->addWidget(updateLabel);
    
    mainLayout->addLayout(headerLayout);
    
    QFrame* chartContainer = new QFrame(dialog);
    chartContainer->setStyleSheet(
        "QFrame {"
        "background-color: white;"
        "border: 3px solid #1976D2;"
        "border-radius: 15px;"
        "padding: 20px;"
        "}"
    );
    
    QVBoxLayout* containerLayout = new QVBoxLayout(chartContainer);
    containerLayout->setContentsMargins(10, 10, 10, 10);
    
    QLabel* subtitleLabel = new QLabel("Distribution des clients selon leur âge", chartContainer);
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #424242;"
        "padding: 10px;"
        "}"
    );
    subtitleLabel->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(subtitleLabel);
    
    AgePieChartWidget* pieChart = new AgePieChartWidget(ageGroups, totalClients, chartContainer);
    containerLayout->addWidget(pieChart);
    
    mainLayout->addWidget(chartContainer);
    mainLayout->addStretch();
    
    QPushButton* closeBtn = new QPushButton("Fermer", dialog);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #1976D2, stop:1 #0D47A1);"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "padding: 12px 50px;"
        "border-radius: 8px;"
        "min-width: 150px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #2196F3, stop:1 #1565C0);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #1565C0, stop:1 #0A3D7A);"
        "}"
    );
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    dialog->exec();
    delete dialog;
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
    
    DatabaseManager& db = DatabaseManager::getInstance();
    int idClient = db.getClientIdByCin(client.getCin());
    
    QString objetsInfo = "";
    if (idClient > 0) {
        QSqlDatabase& database = Connection::createInstance().getDatabase();
        if (database.isOpen()) {
            QSqlQuery query(database);
            QString objetsQuery = "SELECT o.ID_OBJET, o.NOM_OBJET, o.MARQUE, o.MODELE, "
                                 "NVL(e.ETAT, o.ETAT) AS ETAT_OBJET "
                                 "FROM OBJET_ELECTRONIQUE o "
                                 "LEFT JOIN ETAT_OBJET e ON o.ID_OBJET = e.ID_OBJET AND e.ID_CLIENT = :id_client "
                                 "WHERE o.ID_CLIENT_COURANT = :id_client "
                                 "ORDER BY o.ID_OBJET";
            query.prepare(objetsQuery);
            query.bindValue(":id_client", idClient);
            
            if (query.exec()) {
                int count = 0;
                while (query.next() && count < 10) {
                    QString refObjet = query.value(0).toString();
                    QString nomObjet = query.value(1).toString();
                    QString marque = query.value(2).toString();
                    QString modele = query.value(3).toString();
                    QString etat = query.value(4).toString();
                    if (etat.isEmpty()) {
                        etat = "Non défini";
                    }
                    
                    objetsInfo += QString("- Réf: %1 - État: %2\n")
                                 .arg(refObjet)
                                 .arg(etat);
                    count++;
                }
            }
        }
    }
    
    QString sujet;
    QString message;
    
    QDate dateAjout = QDate::currentDate();
    QDate dateRappel = dateAjout.addDays(4);
    QString dateRappelStr = dateRappel.toString("dd/MM/yyyy");
    
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
        ).arg(client.getNom())
         .arg(client.getPrenom())
         .arg(client.getCin())
         .arg(client.getTelephone())
         .arg(client.getEmail())
         .arg(client.getAdresse());
    }
    
    if (!objetsInfo.isEmpty()) {
        message += "État de vos objets:\n";
        message += objetsInfo;
        message += "\n";
    }
    
    if (typeOperation == "ajout") {
        message += QString("Nous vous rappelons que vous pouvez nous contacter le %1 pour toute question concernant vos objets.\n\n")
                   .arg(dateRappelStr);
        message += "Merci de votre confiance.\n\n";
    } else if (typeOperation == "modification") {
        message += "Si vous n'avez pas effectué cette modification, veuillez nous contacter immédiatement.\n\n";
    }
    
    message += "Pour toute assistance, contactez-nous au: 29839200\n\n";
    message += "Cordialement,\n";
    message += "L'équipe de gestion";
    
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
                if (!historique.contains("📦 HISTORIQUE DES OBJETS:")) {
                    historique += QString("📦 HISTORIQUE DES OBJETS:\n");
                    historique += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
                }
                
                if (tableName == "POSSEDER") {
                    QString histQuery = "SELECT o.ID_OBJET, o.NOM_OBJET, o.MARQUE, p.DATE_DEBUT, p.DATE_FIN, "
                                       "NVL(e.ETAT, o.ETAT) AS ETAT_OBJET "
                                       "FROM POSSEDER p "
                                       "JOIN OBJET_ELECTRONIQUE o ON p.ID_OBJET = o.ID_OBJET "
                                       "LEFT JOIN ETAT_OBJET e ON o.ID_OBJET = e.ID_OBJET AND p.ID_CLIENT = e.ID_CLIENT "
                                       "WHERE p.ID_CLIENT = (SELECT ID_CLIENT FROM CLIENT WHERE CIN = :cin) "
                                       "ORDER BY p.DATE_DEBUT DESC";
                    query.prepare(histQuery);
                    query.bindValue(":cin", cin);
                    
                    if (query.exec()) {
                        int count = 0;
                        while (query.next() && count < 10) {
                            QString refObjet = query.value(0).toString();
                            QString etatObjet = query.value(5).toString();
                            if (etatObjet.isEmpty()) {
                                etatObjet = "Non défini";
                            }
                            historique += QString("• %1 - %2 (Réf: %3, Du %4 au %5) - État: %6\n")
                                         .arg(query.value(1).toString())
                                         .arg(query.value(2).toString())
                                         .arg(refObjet)
                                         .arg(query.value(3).toDate().toString("dd/MM/yyyy"))
                                         .arg(query.value(4).toDate().toString("dd/MM/yyyy"))
                                         .arg(etatObjet);
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

void gestionclient::traiterCinArduino(const QString& cin)
{
    qDebug() << "=== traiterCinArduino appele avec CIN:" << cin;
    
    if (!ui->textEdit_history) {
        qDebug() << "ERREUR: textEdit_history est null!";
        return;
    }
    
    ui->textEdit_history->clear();
    ui->textEdit_history->setPlainText("CIN recu depuis Arduino: " + cin + "\n");
    ui->textEdit_history->append("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.isConnected()) {
        ui->textEdit_history->append("ERREUR: Base de donnees non connectee!");
        qDebug() << "Base de donnees non connectee";
        return;
    }
    
    qDebug() << "Recherche des objets pour le CIN:" << cin;
    QList<QPair<QString, QString>> objets = db.getObjetsByClientCin(cin);
    qDebug() << "Nombre d'objets trouves:" << objets.size();
    
    if (objets.isEmpty()) {
        ui->textEdit_history->append("\nAucun objet electronique trouve pour ce CIN.\n");
        ui->textEdit_history->append("Verifiez que le CIN existe dans la base de donnees.");
        ui->lineEdit_2->setText("Aucun objet trouve pour le CIN: " + cin);
        qDebug() << "Aucun objet trouve pour le CIN:" << cin;
        return;
    }
    
    ui->textEdit_history->append("\nObjets electroniques trouves:\n");
    
    QString resultat = "CIN: " + cin + " | ";
    for (int i = 0; i < objets.size(); i++) {
        QString ligneObjet = QString("• ID Objet: %1 - Etat: %2\n").arg(objets[i].first).arg(objets[i].second);
        ui->textEdit_history->append(ligneObjet);
        qDebug() << "Objet" << i << ":" << objets[i].first << "-" << objets[i].second;
        
        resultat += "ID: " + objets[i].first + " - Etat: " + objets[i].second;
        if (i < objets.size() - 1) {
            resultat += " | ";
        }
    }
    
    ui->lineEdit_2->setText(resultat);
    qDebug() << "Resultats affiches dans lineEdit_2:" << resultat;
    qDebug() << "Contenu de textEdit_history:" << ui->textEdit_history->toPlainText();
}

