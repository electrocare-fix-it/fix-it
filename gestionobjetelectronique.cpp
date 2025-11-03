#include "gestionobjetelectronique.h"
#include "ui_gestionobjetelectronique.h"
#include <QRegularExpression>

gestionobjetelectronique::gestionobjetelectronique(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionobjetelectronique)
{
    ui->setupUi(this);
    openDatabase();
    ensureTable();
    refreshTable();
    currentSelectedReference.clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

gestionobjetelectronique::~gestionobjetelectronique()
{
    delete ui;
}

void gestionobjetelectronique::on_pushButton_clicked()
{
    emit homeRequested();
}


void gestionobjetelectronique::openDatabase()
{
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE", "objets_conn");
        database.setDatabaseName("objets.db");
    }
    if (!database.isOpen() && !database.open()) {
        QMessageBox::critical(this, "Base de données", "Impossible d'ouvrir la base: " + database.lastError().text());
    }
}

void gestionobjetelectronique::ensureTable()
{
    QSqlQuery q(database);
    const char *sql =
            "CREATE TABLE IF NOT EXISTS objets ("
            "reference TEXT PRIMARY KEY,"
            "nom TEXT,"
            "marque TEXT,"
            "modele TEXT,"
            "couleur TEXT,"
            "numero_serie TEXT,"
            "type TEXT,"
            "etat TEXT,"
            "technicien TEXT,"
            "prix INTEGER)";
    if (!q.exec(sql)) {
        QMessageBox::critical(this, "Base de données", "Échec création table: " + q.lastError().text());
    }
}

void gestionobjetelectronique::refreshTable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QSqlQuery q(database);
    if (!q.exec("SELECT reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix FROM objets ORDER BY reference")) {
        QMessageBox::warning(this, "Lecture", q.lastError().text());
        return;
    }
    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 10; ++col) {
            QTableWidgetItem *it = new QTableWidgetItem(q.value(col).toString());
            if (col == 0) {
                // keep original reference per row in UserRole
                it->setData(Qt::UserRole, q.value(0).toString());
            }
            ui->tableWidget->setItem(row, col, it);
        }
        ++row;
    }
    currentSelectedReference.clear();
}

void gestionobjetelectronique::refreshTableWithFilters()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QStringList where;
    QList<QVariant> binds;

    const QString type = ui->comboBoxRechercheType_6->currentText();
    if (!type.isEmpty() && type != "Tous") {
        where << "type = ?";
        binds << type;
    }

    const QString marque = ui->lineEditRechercheMarque_6->text().trimmed();
    if (!marque.isEmpty()) {
        where << "marque LIKE ?";
        binds << ("%" + marque + "%");
    }

    const QString etat = ui->comboBoxRechercheEtat_6->currentText();
    if (!etat.isEmpty() && etat != "Tous") {
        where << "etat = ?";
        binds << etat;
    }

    const QString modele = ui->lineEditRechercheModele_6->text().trimmed();
    if (!modele.isEmpty()) {
        where << "modele LIKE ?";
        binds << ("%" + modele + "%");
    }

    QString sql = "SELECT reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix FROM objets";
    if (!where.isEmpty()) sql += " WHERE " + where.join(" AND ");
    sql += " ORDER BY reference";

    QSqlQuery q(database);
    q.prepare(sql);
    for (const QVariant &v : binds) q.addBindValue(v);
    if (!q.exec()) {
        QMessageBox::warning(this, "Recherche", q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 10; ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(q.value(col).toString()));
        }
        ++row;
    }
}

void gestionobjetelectronique::clearForm()
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
}

bool gestionobjetelectronique::validateFields(const QString &ref, const QString &nom, const QString &marque,
                        const QString &modele, const QString &couleur, const QString &numeroSerie,
                        const QString &type, const QString &etat, const QString &technicien, int prix)
{
    // Validation de la Référence (ID unique)
    if (ref.isEmpty()) {
        QMessageBox::warning(this, "Validation", "La référence (ID unique) est obligatoire.");
        return false;
    }
    if (ref.length() > 50) {
        QMessageBox::warning(this, "Validation", "La référence ne doit pas dépasser 50 caractères.");
        return false;
    }
    // Format de référence: lettres, chiffres, tirets et underscores uniquement
    QRegularExpression refPattern("^[A-Za-z0-9_-]+$");
    if (!refPattern.match(ref).hasMatch()) {
        QMessageBox::warning(this, "Validation", "La référence ne doit contenir que des lettres, chiffres, tirets (-) et underscores (_).");
        return false;
    }

    // Validation du Nom de l'objet
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le nom de l'objet est obligatoire.");
        return false;
    }
    if (nom.length() < 2) {
        QMessageBox::warning(this, "Validation", "Le nom de l'objet doit contenir au moins 2 caractères.");
        return false;
    }
    if (nom.length() > 100) {
        QMessageBox::warning(this, "Validation", "Le nom de l'objet ne doit pas dépasser 100 caractères.");
        return false;
    }

    // Validation de la Marque
    if (marque.isEmpty()) {
        QMessageBox::warning(this, "Validation", "La marque est obligatoire.");
        return false;
    }
    if (marque.length() < 2) {
        QMessageBox::warning(this, "Validation", "La marque doit contenir au moins 2 caractères.");
        return false;
    }
    if (marque.length() > 50) {
        QMessageBox::warning(this, "Validation", "La marque ne doit pas dépasser 50 caractères.");
        return false;
    }
    // La marque ne doit contenir que des lettres, espaces, tirets et apostrophes
    QRegularExpression marquePattern("^[A-Za-zÀ-ÿ\\s'-]+$");
    if (!marquePattern.match(marque).hasMatch()) {
        QMessageBox::warning(this, "Validation", "La marque ne doit contenir que des lettres, espaces, tirets et apostrophes.");
        return false;
    }

    // Validation du Modèle (optionnel mais si rempli, doit être valide)
    if (!modele.isEmpty()) {
        if (modele.length() < 1) {
            QMessageBox::warning(this, "Validation", "Le modèle doit contenir au moins 1 caractère.");
            return false;
        }
        if (modele.length() > 100) {
            QMessageBox::warning(this, "Validation", "Le modèle ne doit pas dépasser 100 caractères.");
            return false;
        }
    }

    // Validation de la Couleur (optionnel mais si rempli, doit être valide)
    if (!couleur.isEmpty()) {
        if (couleur.length() > 30) {
            QMessageBox::warning(this, "Validation", "La couleur ne doit pas dépasser 30 caractères.");
            return false;
        }
        // La couleur ne doit contenir que des lettres, espaces et tirets
        QRegularExpression couleurPattern("^[A-Za-zÀ-ÿ\\s-]+$");
        if (!couleurPattern.match(couleur).hasMatch()) {
            QMessageBox::warning(this, "Validation", "La couleur ne doit contenir que des lettres, espaces et tirets.");
            return false;
        }
    }

    // Validation du Numéro de série (optionnel mais si rempli, doit être valide)
    if (!numeroSerie.isEmpty()) {
        if (numeroSerie.length() < 3) {
            QMessageBox::warning(this, "Validation", "Le numéro de série doit contenir au moins 3 caractères.");
            return false;
        }
        if (numeroSerie.length() > 50) {
            QMessageBox::warning(this, "Validation", "Le numéro de série ne doit pas dépasser 50 caractères.");
            return false;
        }
        // Format du numéro de série: lettres, chiffres, tirets et underscores
        QRegularExpression snPattern("^[A-Za-z0-9_-]+$");
        if (!snPattern.match(numeroSerie).hasMatch()) {
            QMessageBox::warning(this, "Validation", "Le numéro de série ne doit contenir que des lettres, chiffres, tirets (-) et underscores (_).");
            return false;
        }
    }

    // Validation du Type
    if (type.isEmpty() || type == "-- Sélectionner --") {
        QMessageBox::warning(this, "Validation", "Veuillez sélectionner un type.");
        return false;
    }

    // Validation de l'État
    if (etat.isEmpty() || etat == "-- Sélectionner --") {
        QMessageBox::warning(this, "Validation", "Veuillez sélectionner un état.");
        return false;
    }

    // Validation du Technicien (optionnel mais si rempli, doit être valide)
    if (!technicien.isEmpty()) {
        if (technicien.length() < 2) {
            QMessageBox::warning(this, "Validation", "Le nom du technicien doit contenir au moins 2 caractères.");
            return false;
        }
        if (technicien.length() > 50) {
            QMessageBox::warning(this, "Validation", "Le nom du technicien ne doit pas dépasser 50 caractères.");
            return false;
        }
        // Le nom du technicien ne doit contenir que des lettres, espaces, tirets et apostrophes
        QRegularExpression technicienPattern("^[A-Za-zÀ-ÿ\\s'-]+$");
        if (!technicienPattern.match(technicien).hasMatch()) {
            QMessageBox::warning(this, "Validation", "Le nom du technicien ne doit contenir que des lettres, espaces, tirets et apostrophes.");
            return false;
        }
    }

    // Validation du Prix (optionnel mais doit être positif si renseigné)
    if (prix < 0) {
        QMessageBox::warning(this, "Validation", "Le prix ne peut pas être négatif.");
        return false;
    }

    return true;
}

bool gestionobjetelectronique::readForm(QString &ref, QString &nom, QString &marque, QString &modele,
                  QString &couleur, QString &numeroSerie, QString &type,
                  QString &etat, QString &technicien, int &prix)
{
    ref = ui->lineEditReference_6->text().trimmed();
    nom = ui->lineEditNomObjet_6->text().trimmed();
    marque = ui->lineEditMarque_6->text().trimmed();
    modele = ui->lineEditModele_6->text().trimmed();
    couleur = ui->lineEditCouleur_6->text().trimmed();
    numeroSerie = ui->lineEditNumeroSerie_6->text().trimmed();
    type = ui->comboBoxType_6->currentText();
    etat = ui->comboBoxEtat_6->currentText();
    technicien = ui->lineEditTechnicien_6->text().trimmed();
    prix = ui->spinBoxPrix_6->value();

    if (!validateFields(ref, nom, marque, modele, couleur, numeroSerie, type, etat, technicien, prix)) {
        // Focus sur le premier champ en erreur si possible
        if (ref.isEmpty()) ui->lineEditReference_6->setFocus();
        else if (nom.isEmpty()) ui->lineEditNomObjet_6->setFocus();
        else if (marque.isEmpty()) ui->lineEditMarque_6->setFocus();
        else if (type.isEmpty() || type == "-- Sélectionner --") ui->comboBoxType_6->setFocus();
        else if (etat.isEmpty() || etat == "-- Sélectionner --") ui->comboBoxEtat_6->setFocus();
        return false;
    }

    return true;
}

void gestionobjetelectronique::on_btnAjouter_6_clicked()
{
    QString ref, nom, marque, modele, couleur, numeroSerie, type, etat, technicien; int prix = 0;
    if (!readForm(ref, nom, marque, modele, couleur, numeroSerie, type, etat, technicien, prix)) return;

    QSqlQuery q(database);
    q.prepare("INSERT INTO objets(reference, nom, marque, modele, couleur, numero_serie, type, etat, technicien, prix)"
              " VALUES(?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(ref);
    q.addBindValue(nom);
    q.addBindValue(marque);
    q.addBindValue(modele);
    q.addBindValue(couleur);
    q.addBindValue(numeroSerie);
    q.addBindValue(type);
    q.addBindValue(etat);
    q.addBindValue(technicien);
    q.addBindValue(prix);
    if (!q.exec()) {
        QMessageBox::warning(this, "Ajout", "Échec d'ajout: " + q.lastError().text());
        return;
    }
    refreshTable();
}

void gestionobjetelectronique::on_btnModifier_6_clicked()
{
    // Toggle edit mode for the table
    isTableEditMode = !isTableEditMode;
    ui->tableWidget->setEditTriggers(isTableEditMode ? QAbstractItemView::AllEditTriggers
                                                     : QAbstractItemView::NoEditTriggers);
    QMessageBox::information(this, "Mode modification",
                             isTableEditMode ? "Vous pouvez modifier directement dans le tableau."
                                             : "Mode modification désactivé.");
}

void gestionobjetelectronique::on_btnSupprimer_6_clicked()
{
    const QString ref = ui->lineEditReference_6->text().trimmed();
    if (ref.isEmpty()) {
        QMessageBox::warning(this, "Suppression", "Saisissez la référence à supprimer.");
        return;
    }
    QSqlQuery q(database);
    q.prepare("DELETE FROM objets WHERE reference=?");
    q.addBindValue(ref);
    if (!q.exec()) {
        QMessageBox::warning(this, "Suppression", "Échec de suppression: " + q.lastError().text());
        return;
    }
    clearForm();
    refreshTable();
}

void gestionobjetelectronique::on_btnRechercher_6_clicked()
{
    refreshTableWithFilters();
}

void gestionobjetelectronique::on_btnReinitialiser_6_clicked()
{
    ui->comboBoxRechercheType_6->setCurrentIndex(0);
    ui->lineEditRechercheMarque_6->clear();
    ui->comboBoxRechercheEtat_6->setCurrentIndex(0);
    ui->lineEditRechercheModele_6->clear();
    clearForm();
    refreshTable();
    currentSelectedReference.clear();
}

void gestionobjetelectronique::on_tableWidget_itemSelectionChanged()
{
    auto items = ui->tableWidget->selectedItems();
    if (items.size() < 10) return;
    // items come by row; map to fields
    ui->lineEditReference_6->setText(items[0]->text());
    currentSelectedReference = items[0]->text();
    ui->lineEditNomObjet_6->setText(items[1]->text());
    ui->lineEditMarque_6->setText(items[2]->text());
    ui->lineEditModele_6->setText(items[3]->text());
    ui->lineEditCouleur_6->setText(items[4]->text());
    ui->lineEditNumeroSerie_6->setText(items[5]->text());
    ui->comboBoxType_6->setCurrentText(items[6]->text());
    ui->comboBoxEtat_6->setCurrentText(items[7]->text());
    ui->lineEditTechnicien_6->setText(items[8]->text());
    ui->spinBoxPrix_6->setValue(items[9]->text().toInt());
}

void gestionobjetelectronique::on_tableWidget_cellChanged(int row, int /*column*/)
{
    if (!isTableEditMode) return; // only react in edit mode
    if (row < 0) return;
    // Read entire row from table
    auto get = [&](int c){ QTableWidgetItem *it = ui->tableWidget->item(row, c); return it ? it->text().trimmed() : QString(); };
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

    // Valider les champs avant de mettre à jour la base de données
    if (!validateFields(refNew, nom, marque, modele, couleur, numeroSerie, type, etat, technicien, prix)) {
        // Annuler la modification en restaurant la valeur précédente
        refreshTable();
        return;
    }

    // original reference stored in UserRole of column 0 item
    QTableWidgetItem *refItem = ui->tableWidget->item(row, 0);
    if (!refItem) return;
    const QString refOld = refItem->data(Qt::UserRole).toString().isEmpty() ? refNew : refItem->data(Qt::UserRole).toString();

    QSqlQuery q(database);
    q.prepare("UPDATE objets SET reference=?, nom=?, marque=?, modele=?, couleur=?, numero_serie=?, type=?, etat=?, technicien=?, prix=? WHERE reference=?");
    q.addBindValue(refNew);
    q.addBindValue(nom);
    q.addBindValue(marque);
    q.addBindValue(modele);
    q.addBindValue(couleur);
    q.addBindValue(numeroSerie);
    q.addBindValue(type);
    q.addBindValue(etat);
    q.addBindValue(technicien);
    q.addBindValue(prix);
    q.addBindValue(refOld);
    if (!q.exec()) {
        QMessageBox::warning(this, "Modification", "Échec de modification: " + q.lastError().text());
        refreshTable();
        return;
    }
    // update stored original ref for this row since PK may have changed
    refItem->setData(Qt::UserRole, refNew);

    // sync edited values to form
    ui->lineEditReference_6->setText(refNew);
    ui->lineEditNomObjet_6->setText(nom);
    ui->lineEditMarque_6->setText(marque);
    ui->lineEditModele_6->setText(modele);
    ui->lineEditCouleur_6->setText(couleur);
    ui->lineEditNumeroSerie_6->setText(numeroSerie);
    ui->comboBoxType_6->setCurrentText(type);
    ui->comboBoxEtat_6->setCurrentText(etat);
    ui->lineEditTechnicien_6->setText(technicien);
    ui->spinBoxPrix_6->setValue(prix);
    currentSelectedReference = refNew;
}


