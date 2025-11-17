#include "gestionobjetelectronique.h"
#include "ui_gestionobjetelectronique.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

gestionobjetelectronique::gestionobjetelectronique(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionobjetelectronique)
    , m_currentReference()
    , m_isTableEditMode(false)
{
    ui->setupUi(this);
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.connect()) {
        afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données: " + db.getLastError());
    }
    
    configurerValidateurs();
    actualiserTableau();
    
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &gestionobjetelectronique::on_tableWidget_itemSelectionChanged);
}

gestionobjetelectronique::~gestionobjetelectronique()
{
    delete ui;
}

void gestionobjetelectronique::on_pushButton_clicked()
{
    emit homeRequested();
}

void gestionobjetelectronique::actualiserTableau()
{
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<ObjetElectronique> objets = db.getAllObjets();
    
    ui->tableWidget->setRowCount(objets.size());
    
    for (int i = 0; i < objets.size(); ++i) {
        const ObjetElectronique& objet = objets[i];
        
        QTableWidgetItem* refItem = new QTableWidgetItem(objet.getReference());
        refItem->setData(Qt::UserRole, objet.getReference());
        ui->tableWidget->setItem(i, 0, refItem);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(objet.getNom()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(objet.getMarque()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(objet.getModele()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(objet.getCouleur()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(objet.getNumeroSerie()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(objet.getType()));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(objet.getEtat()));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(objet.getTechnicien()));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::number(objet.getPrix())));
    }
    
    m_currentReference.clear();
}

void gestionobjetelectronique::viderFormulaire()
{
    ui->lineEditReference_6->clear();
    ui->lineEditNomObjet_6->clear();
    ui->lineEditMarque_6->clear();
    ui->lineEditModele_6->clear();
    ui->lineEditCouleur_6->clear();
    ui->lineEditNumeroSerie_6->clear();
    ui->comboBoxType_6->setCurrentIndex(0);
    ui->comboBoxEtat_6->setCurrentIndex(0);
    ui->lineEditTechnicien_6->clear();
    ui->spinBoxPrix_6->setValue(0);
    m_currentReference.clear();
}

void gestionobjetelectronique::remplirFormulaire(const ObjetElectronique& objet)
{
    ui->lineEditReference_6->setText(objet.getReference());
    ui->lineEditNomObjet_6->setText(objet.getNom());
    ui->lineEditMarque_6->setText(objet.getMarque());
    ui->lineEditModele_6->setText(objet.getModele());
    ui->lineEditCouleur_6->setText(objet.getCouleur());
    ui->lineEditNumeroSerie_6->setText(objet.getNumeroSerie());
    ui->comboBoxType_6->setCurrentText(objet.getType());
    ui->comboBoxEtat_6->setCurrentText(objet.getEtat());
    ui->lineEditTechnicien_6->setText(objet.getTechnicien());
    ui->spinBoxPrix_6->setValue(objet.getPrix());
    m_currentReference = objet.getReference();
}

void gestionobjetelectronique::configurerValidateurs()
{
    ui->lineEditReference_6->setMaxLength(50);
    QRegularExpression refRegex("^[A-Za-z0-9_-]+$");
    QRegularExpressionValidator* refValidator = new QRegularExpressionValidator(refRegex, this);
    ui->lineEditReference_6->setValidator(refValidator);
    
    ui->lineEditNomObjet_6->setMaxLength(100);
    ui->lineEditMarque_6->setMaxLength(50);
    ui->lineEditModele_6->setMaxLength(100);
    ui->lineEditCouleur_6->setMaxLength(30);
    ui->lineEditNumeroSerie_6->setMaxLength(50);
    ui->lineEditTechnicien_6->setMaxLength(50);
}

bool gestionobjetelectronique::validerFormulaire()
{
    QString ref = ui->lineEditReference_6->text().trimmed();
    if (ref.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "La référence est obligatoire.");
        ui->lineEditReference_6->setFocus();
        return false;
    }
    if (ref.length() > 50) {
        afficherMessageErreur("Erreur de validation", "La référence ne doit pas dépasser 50 caractères.");
        ui->lineEditReference_6->setFocus();
        return false;
    }
    QRegularExpression refPattern("^[A-Za-z0-9_-]+$");
    if (!refPattern.match(ref).hasMatch()) {
        afficherMessageErreur("Erreur de validation", "La référence ne doit contenir que des lettres, chiffres, tirets (-) et underscores (_).");
        ui->lineEditReference_6->setFocus();
        return false;
    }
    
    QString nom = ui->lineEditNomObjet_6->text().trimmed();
    if (nom.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "Le nom de l'objet est obligatoire.");
        ui->lineEditNomObjet_6->setFocus();
        return false;
    }
    if (nom.length() < 2) {
        afficherMessageErreur("Erreur de validation", "Le nom de l'objet doit contenir au moins 2 caractères.");
        ui->lineEditNomObjet_6->setFocus();
        return false;
    }
    
    QString marque = ui->lineEditMarque_6->text().trimmed();
    if (marque.isEmpty()) {
        afficherMessageErreur("Erreur de validation", "La marque est obligatoire.");
        ui->lineEditMarque_6->setFocus();
        return false;
    }
    if (marque.length() < 2) {
        afficherMessageErreur("Erreur de validation", "La marque doit contenir au moins 2 caractères.");
        ui->lineEditMarque_6->setFocus();
        return false;
    }
    QRegularExpression marquePattern("^[\\p{L}\\s'-]+$", QRegularExpression::UseUnicodePropertiesOption);
    if (!marquePattern.match(marque).hasMatch()) {
        afficherMessageErreur("Erreur de validation", "La marque ne doit contenir que des lettres, espaces, tirets et apostrophes.");
        ui->lineEditMarque_6->setFocus();
        return false;
    }
    
    QString type = ui->comboBoxType_6->currentText();
    if (type.isEmpty() || type == "-- Sélectionner --") {
        afficherMessageErreur("Erreur de validation", "Veuillez sélectionner un type.");
        ui->comboBoxType_6->setFocus();
        return false;
    }
    
    QString etat = ui->comboBoxEtat_6->currentText();
    if (etat.isEmpty() || etat == "-- Sélectionner --") {
        afficherMessageErreur("Erreur de validation", "Veuillez sélectionner un état.");
        ui->comboBoxEtat_6->setFocus();
        return false;
    }
    
    QString couleur = ui->lineEditCouleur_6->text().trimmed();
    if (!couleur.isEmpty()) {
        QRegularExpression couleurPattern("^[\\p{L}\\s-]+$", QRegularExpression::UseUnicodePropertiesOption);
        if (!couleurPattern.match(couleur).hasMatch()) {
            afficherMessageErreur("Erreur de validation", "La couleur ne doit contenir que des lettres, espaces et tirets.");
            ui->lineEditCouleur_6->setFocus();
            return false;
        }
    }
    
    QString numeroSerie = ui->lineEditNumeroSerie_6->text().trimmed();
    if (!numeroSerie.isEmpty()) {
        QRegularExpression snPattern("^[A-Za-z0-9_-]+$");
        if (!snPattern.match(numeroSerie).hasMatch()) {
            afficherMessageErreur("Erreur de validation", "Le numéro de série ne doit contenir que des lettres, chiffres, tirets (-) et underscores (_).");
            ui->lineEditNumeroSerie_6->setFocus();
            return false;
        }
    }
    
    QString technicien = ui->lineEditTechnicien_6->text().trimmed();
    if (!technicien.isEmpty()) {
        QRegularExpression technicienPattern("^[\\p{L}\\s'-]+$", QRegularExpression::UseUnicodePropertiesOption);
        if (!technicienPattern.match(technicien).hasMatch()) {
            afficherMessageErreur("Erreur de validation", "Le nom du technicien ne doit contenir que des lettres, espaces, tirets et apostrophes.");
            ui->lineEditTechnicien_6->setFocus();
            return false;
        }
    }
    
    int prix = ui->spinBoxPrix_6->value();
    if (prix < 0) {
        afficherMessageErreur("Erreur de validation", "Le prix ne peut pas être négatif.");
        ui->spinBoxPrix_6->setFocus();
        return false;
    }
    
    return true;
}

bool gestionobjetelectronique::lireFormulaire(ObjetElectronique& objet)
{
    if (!validerFormulaire()) {
        return false;
    }
    
    objet = ObjetElectronique(
        ui->lineEditReference_6->text().trimmed(),
        ui->lineEditNomObjet_6->text().trimmed(),
        ui->lineEditMarque_6->text().trimmed(),
        ui->lineEditModele_6->text().trimmed(),
        ui->lineEditCouleur_6->text().trimmed(),
        ui->lineEditNumeroSerie_6->text().trimmed(),
        ui->comboBoxType_6->currentText(),
        ui->comboBoxEtat_6->currentText(),
        ui->lineEditTechnicien_6->text().trimmed(),
        ui->spinBoxPrix_6->value()
    );
    
    return true;
}

void gestionobjetelectronique::afficherMessageErreur(const QString& titre, const QString& message)
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

void gestionobjetelectronique::afficherMessageSucces(const QString& titre, const QString& message)
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

void gestionobjetelectronique::on_btnAjouter_6_clicked()
{
    ObjetElectronique objet;
    if (!lireFormulaire(objet)) {
        return;
    }
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (db.objetExists(objet.getReference())) {
        afficherMessageErreur("Erreur", "Un objet avec cette référence existe déjà.");
        return;
    }
    
    if (!db.insertObjet(objet)) {
        afficherMessageErreur("Erreur", "Erreur lors de l'ajout de l'objet: " + db.getLastError());
        return;
    }
    
    actualiserTableau();
    viderFormulaire();
    
    afficherMessageSucces("Succès", "Objet ajouté avec succès.");
}

void gestionobjetelectronique::on_btnModifier_6_clicked()
{
    m_isTableEditMode = !m_isTableEditMode;
    ui->tableWidget->setEditTriggers(m_isTableEditMode ? QAbstractItemView::AllEditTriggers
                                                       : QAbstractItemView::NoEditTriggers);
    
    if (m_isTableEditMode) {
        afficherMessageSucces("Mode modification", "Vous pouvez modifier directement dans le tableau.");
    } else {
        QMessageBox::information(this, "Mode modification", "Mode modification désactivé.");
    }
}

void gestionobjetelectronique::on_btnSupprimer_6_clicked()
{
    if (m_currentReference.isEmpty()) {
        afficherMessageErreur("Attention", "Veuillez sélectionner un objet à supprimer.");
        return;
    }
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Êtes-vous sûr de vouloir supprimer cet objet ?");
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
        if (!db.deleteObjet(m_currentReference)) {
            afficherMessageErreur("Erreur", "Erreur lors de la suppression: " + db.getLastError());
            return;
        }
        
        actualiserTableau();
        viderFormulaire();
        afficherMessageSucces("Succès", "Objet supprimé avec succès.");
    }
}

void gestionobjetelectronique::on_btnRechercher_6_clicked()
{
    QString type = ui->comboBoxRechercheType_6->currentText();
    QString marque = ui->lineEditRechercheMarque_6->text().trimmed();
    QString etat = ui->comboBoxRechercheEtat_6->currentText();
    QString modele = ui->lineEditRechercheModele_6->text().trimmed();
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<ObjetElectronique> allObjets = db.getAllObjets();
    
    QList<ObjetElectronique> filteredObjets;
    for (const ObjetElectronique& objet : allObjets) {
        bool matches = true;
        
        if (!type.isEmpty() && type != "Tous" && objet.getType() != type) {
            matches = false;
        }
        if (!marque.isEmpty() && !objet.getMarque().contains(marque, Qt::CaseInsensitive)) {
            matches = false;
        }
        if (!etat.isEmpty() && etat != "Tous" && objet.getEtat() != etat) {
            matches = false;
        }
        if (!modele.isEmpty() && !objet.getModele().contains(modele, Qt::CaseInsensitive)) {
            matches = false;
        }
        
        if (matches) {
            filteredObjets.append(objet);
        }
    }
    
    ui->tableWidget->setRowCount(filteredObjets.size());
    for (int i = 0; i < filteredObjets.size(); ++i) {
        const ObjetElectronique& objet = filteredObjets[i];
        
        QTableWidgetItem* refItem = new QTableWidgetItem(objet.getReference());
        refItem->setData(Qt::UserRole, objet.getReference());
        ui->tableWidget->setItem(i, 0, refItem);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(objet.getNom()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(objet.getMarque()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(objet.getModele()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(objet.getCouleur()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(objet.getNumeroSerie()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(objet.getType()));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(objet.getEtat()));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(objet.getTechnicien()));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::number(objet.getPrix())));
    }
}

void gestionobjetelectronique::on_btnReinitialiser_6_clicked()
{
    ui->comboBoxRechercheType_6->setCurrentIndex(0);
    ui->lineEditRechercheMarque_6->clear();
    ui->comboBoxRechercheEtat_6->setCurrentIndex(0);
    ui->lineEditRechercheModele_6->clear();
    viderFormulaire();
    actualiserTableau();
}

void gestionobjetelectronique::on_tableWidget_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        viderFormulaire();
        return;
    }
    
    int row = selectedItems.first()->row();
    QTableWidgetItem* refItem = ui->tableWidget->item(row, 0);
    if (refItem) {
        QString reference = refItem->text();
        DatabaseManager& db = DatabaseManager::getInstance();
        ObjetElectronique objet = db.getObjetByReference(reference);
        
        if (!objet.getReference().isEmpty()) {
            remplirFormulaire(objet);
        }
    }
}

void gestionobjetelectronique::on_tableWidget_cellChanged(int row, int column)
{
    Q_UNUSED(column)
    if (!m_isTableEditMode || row < 0) {
        return;
    }
    
    auto get = [&](int c) {
        QTableWidgetItem* it = ui->tableWidget->item(row, c);
        return it ? it->text().trimmed() : QString();
    };
    
    QString refNew = get(0);
    QString nom = get(1);
    QString marque = get(2);
    QString modele = get(3);
    QString couleur = get(4);
    QString numeroSerie = get(5);
    QString type = get(6);
    QString etat = get(7);
    QString technicien = get(8);
    int prix = get(9).toInt();
    
    QRegularExpression refPattern("^[A-Za-z0-9_-]+$");
    QRegularExpression marquePattern("^[\\p{L}\\s'-]+$", QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpression snPattern("^[A-Za-z0-9_-]+$");
    
    if (refNew.isEmpty() || refNew.length() > 50 || !refPattern.match(refNew).hasMatch() ||
        nom.isEmpty() || nom.length() < 2 ||
        marque.isEmpty() || marque.length() < 2 || !marquePattern.match(marque).hasMatch() ||
        type.isEmpty() || type == "-- Sélectionner --" ||
        etat.isEmpty() || etat == "-- Sélectionner --" ||
        (!numeroSerie.isEmpty() && (numeroSerie.length() < 3 || !snPattern.match(numeroSerie).hasMatch())) ||
        prix < 0) {
        actualiserTableau();
        return;
    }
    
    QTableWidgetItem* refItem = ui->tableWidget->item(row, 0);
    if (!refItem) {
        return;
    }
    
    QString oldReference = refItem->data(Qt::UserRole).toString();
    if (oldReference.isEmpty()) {
        oldReference = refNew;
    }
    
    ObjetElectronique objet(refNew, nom, marque, modele, couleur, numeroSerie, type, etat, technicien, prix);
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (refNew != oldReference) {
        if (db.objetExists(refNew)) {
            afficherMessageErreur("Erreur", "Un objet avec cette référence existe déjà.");
            actualiserTableau();
            return;
        }
        if (!db.deleteObjet(oldReference)) {
            afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
            actualiserTableau();
            return;
        }
        if (!db.insertObjet(objet)) {
            afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
            actualiserTableau();
            return;
        }
    } else {
        if (!db.updateObjet(objet)) {
            afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
            actualiserTableau();
            return;
        }
    }
    
    refItem->setData(Qt::UserRole, refNew);
    remplirFormulaire(objet);
}
