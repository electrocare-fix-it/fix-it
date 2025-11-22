#include "gestionobjetelectronique.h"
#include "ui_gestionobjetelectronique.h"
#include <QRegularExpression>

gestionobjetelectronique::gestionobjetelectronique(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionobjetelectronique)
{
    ui->setupUi(this);
    openDatabase();
    tableName = detectTableName();  // Détecter le nom correct de la table
    refreshTable();
    currentSelectedId = 0;
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
    // Initialiser la connexion Oracle
    if (!Connection::initializeOracleConnection()) {
        QMessageBox::critical(this, "Base de données", 
                             "Impossible de se connecter à Oracle.\nVérifiez vos paramètres de connexion.");
        return;
    }
    
    database = Connection::getOracleConnection();
    
    if (!database.isOpen()) {
        QMessageBox::critical(this, "Base de données", "Impossible d'ouvrir la base: " + database.lastError().text());
    }
}

QString gestionobjetelectronique::detectTableName()
{
    // Essayer différentes variantes du nom de table
    QStringList tableNames = {"objet_electronique", "\"objet_electronique\"", "OBJET_ELECTRONIQUE"};
    
    for (const QString &name : tableNames) {
        QSqlQuery q(database);
        QString testSql = QString("SELECT COUNT(*) FROM %1").arg(name);
        if (q.exec(testSql)) {
            qDebug() << "Table name detected:" << name;
            return name;  // Retourner le nom qui fonctionne
        } else {
            qDebug() << "Failed to detect table with name:" << name << "Error:" << q.lastError().text();
        }
    }
    
    // Si aucune ne fonctionne, retourner la première par défaut
    qDebug() << "Using default table name: objet_electronique";
    return "objet_electronique";
}

qlonglong gestionobjetelectronique::getNextId()
{
    // Obtenir le prochain ID depuis une séquence Oracle
    // Note: Vous devez créer une séquence nommée SEQ_OBJETS_ID dans Oracle
    QSqlQuery q(database);
    if (q.exec("SELECT SEQ_OBJETS_ID.NEXTVAL FROM DUAL")) {
        if (q.next()) {
            return q.value(0).toLongLong();
        }
    }
    // Si la séquence n'existe pas, utiliser MAX+1
    QString maxSql = QString("SELECT NVL(MAX(ID_OBJET), 0) + 1 FROM %1").arg(tableName);
    if (q.exec(maxSql)) {
        if (q.next()) {
            return q.value(0).toLongLong();
        }
    }
    return 1; // Valeur par défaut
}

void gestionobjetelectronique::refreshTable()
{
    // Désactiver les signaux pour éviter les problèmes pendant le rafraîchissement
    ui->tableWidget->blockSignals(true);
    
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // Vérifier que la base de données est toujours ouverte
    if (!database.isOpen()) {
        database = Connection::getOracleConnection();
        if (!database.isOpen()) {
            QMessageBox::critical(this, "Base de données", "Impossible de rouvrir la base: " + database.lastError().text());
            ui->tableWidget->blockSignals(false);
            return;
        }
    }

    QSqlQuery q(database);
    // Utiliser le nom de table détecté
    QString sql = QString("SELECT ID_OBJET, NOM_OBJET, MARQUE, MODELE, COULEUR, NUM_SERIE, TYPE_OBJET, ETAT, "
                          "NVL(TO_CHAR(ID_CLIENT_COURANT), ''), NVL(TO_CHAR(ID_EMPLOYE_COURANT), '') "
                          "FROM %1 ORDER BY ID_OBJET").arg(tableName);
    
    if (!q.exec(sql)) {
        // Si ça échoue, essayer de redétecter le nom de table
        tableName = detectTableName();
        sql = QString("SELECT ID_OBJET, NOM_OBJET, MARQUE, MODELE, COULEUR, NUM_SERIE, TYPE_OBJET, ETAT, "
                      "NVL(TO_CHAR(ID_CLIENT_COURANT), ''), NVL(TO_CHAR(ID_EMPLOYE_COURANT), '') "
                      "FROM %1 ORDER BY ID_OBJET").arg(tableName);
        
        if (!q.exec(sql)) {
            QMessageBox::warning(this, "Lecture", "Erreur SQL: " + q.lastError().text() + "\n\nRequête: " + sql);
            ui->tableWidget->blockSignals(false);
            return;
        }
    }
    
    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 10; ++col) {
            QTableWidgetItem *it = new QTableWidgetItem(q.value(col).toString());
            if (col == 0) {
                // Garder l'ID original dans UserRole
                it->setData(Qt::UserRole, q.value(0).toLongLong());
            }
            ui->tableWidget->setItem(row, col, it);
        }
        ++row;
    }
    currentSelectedId = 0;
    
    // Réactiver les signaux
    ui->tableWidget->blockSignals(false);
}

void gestionobjetelectronique::refreshTableWithFilters()
{
    // Désactiver les signaux pour éviter les problèmes pendant le rafraîchissement
    ui->tableWidget->blockSignals(true);
    
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // Vérifier que la base de données est toujours ouverte
    if (!database.isOpen()) {
        database = Connection::getOracleConnection();
        if (!database.isOpen()) {
            QMessageBox::critical(this, "Base de données", "Impossible de rouvrir la base: " + database.lastError().text());
            ui->tableWidget->blockSignals(false);
            return;
        }
    }

    QStringList where;
    QList<QVariant> binds;

    const QString type = ui->comboBoxRechercheType_6->currentText();
    if (!type.isEmpty() && type != "Tous") {
        where << "TYPE_OBJET = ?";
        binds << type;
    }

    const QString marque = ui->lineEditRechercheMarque_6->text().trimmed();
    if (!marque.isEmpty()) {
        where << "MARQUE LIKE ?";
        binds << ("%" + marque + "%");
    }

    const QString etat = ui->comboBoxRechercheEtat_6->currentText();
    if (!etat.isEmpty() && etat != "Tous") {
        where << "ETAT = ?";
        binds << etat;
    }

    const QString modele = ui->lineEditRechercheModele_6->text().trimmed();
    if (!modele.isEmpty()) {
        where << "MODELE LIKE ?";
        binds << ("%" + modele + "%");
    }

    QString sql = QString("SELECT ID_OBJET, NOM_OBJET, MARQUE, MODELE, COULEUR, NUM_SERIE, TYPE_OBJET, ETAT, "
                          "NVL(TO_CHAR(ID_CLIENT_COURANT), ''), NVL(TO_CHAR(ID_EMPLOYE_COURANT), '') "
                          "FROM %1").arg(tableName);
    if (!where.isEmpty()) sql += " WHERE " + where.join(" AND ");
    sql += " ORDER BY ID_OBJET";

    QSqlQuery q(database);
    q.prepare(sql);
    for (const QVariant &v : binds) q.addBindValue(v);
    if (!q.exec()) {
        QMessageBox::warning(this, "Recherche", "Erreur SQL: " + q.lastError().text());
        ui->tableWidget->blockSignals(false);
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 10; ++col) {
            QTableWidgetItem *it = new QTableWidgetItem(q.value(col).toString());
            if (col == 0) {
                it->setData(Qt::UserRole, q.value(0).toLongLong());
            }
            ui->tableWidget->setItem(row, col, it);
        }
        ++row;
    }
    
    // Réactiver les signaux
    ui->tableWidget->blockSignals(false);
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
    ui->lineEditTechnicien_6->clear();  // Utilisé pour ID_EMPLOYE_COURANT
    ui->spinBoxPrix_6->setValue(0);      // Utilisé pour ID_CLIENT_COURANT
}

bool gestionobjetelectronique::validateFields(const QString &nom, const QString &marque,
                        const QString &modele, const QString &couleur, const QString &numeroSerie,
                        const QString &type, const QString &etat)
{
    // Validation du Nom de l'objet
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le nom de l'objet est obligatoire.");
        return false;
    }
    if (nom.length() < 2) {
        QMessageBox::warning(this, "Validation", "Le nom de l'objet doit contenir au moins 2 caractères.");
        return false;
    }
    if (nom.length() > 50) {
        QMessageBox::warning(this, "Validation", "Le nom de l'objet ne doit pas dépasser 50 caractères.");
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

    // Validation du Modèle (optionnel mais si rempli, doit être valide)
    if (!modele.isEmpty()) {
        if (modele.length() > 50) {
            QMessageBox::warning(this, "Validation", "Le modèle ne doit pas dépasser 50 caractères.");
            return false;
        }
    }

    // Validation de la Couleur (optionnel mais si rempli, doit être valide)
    if (!couleur.isEmpty()) {
        if (couleur.length() > 30) {
            QMessageBox::warning(this, "Validation", "La couleur ne doit pas dépasser 30 caractères.");
            return false;
        }
    }

    // Validation du Numéro de série (optionnel mais si rempli, doit être valide)
    if (!numeroSerie.isEmpty()) {
        if (numeroSerie.length() > 50) {
            QMessageBox::warning(this, "Validation", "Le numéro de série ne doit pas dépasser 50 caractères.");
            return false;
        }
    }

    // Validation du Type
    if (type.isEmpty() || type == "-- Sélectionner --") {
        QMessageBox::warning(this, "Validation", "Veuillez sélectionner un type.");
        return false;
    }
    if (type.length() > 40) {
        QMessageBox::warning(this, "Validation", "Le type ne doit pas dépasser 40 caractères.");
        return false;
    }

    // Validation de l'État
    if (etat.isEmpty() || etat == "-- Sélectionner --") {
        QMessageBox::warning(this, "Validation", "Veuillez sélectionner un état.");
        return false;
    }
    if (etat.length() > 30) {
        QMessageBox::warning(this, "Validation", "L'état ne doit pas dépasser 30 caractères.");
        return false;
    }

    return true;
}

bool gestionobjetelectronique::readForm(qlonglong &id, QString &nom, QString &marque, QString &modele,
                  QString &couleur, QString &numeroSerie, QString &type,
                  QString &etat, qlonglong &idClient, qlonglong &idEmploye)
{
    // ID sera généré automatiquement ou lu depuis le champ référence si en mode modification
    QString refText = ui->lineEditReference_6->text().trimmed();
    if (!refText.isEmpty()) {
        id = refText.toLongLong();
    } else {
        id = 0; // Sera généré lors de l'insertion
    }
    
    nom = ui->lineEditNomObjet_6->text().trimmed();
    marque = ui->lineEditMarque_6->text().trimmed();
    modele = ui->lineEditModele_6->text().trimmed();
    couleur = ui->lineEditCouleur_6->text().trimmed();
    numeroSerie = ui->lineEditNumeroSerie_6->text().trimmed();
    type = ui->comboBoxType_6->currentText();
    etat = ui->comboBoxEtat_6->currentText();
    
    // ID_CLIENT_COURANT depuis spinBoxPrix_6 (réutilisé)
    idClient = ui->spinBoxPrix_6->value();
    // Si 0, on mettra NULL dans la base
    
    // ID_EMPLOYE_COURANT depuis lineEditTechnicien_6 (réutilisé)
    // ATTENTION: Ce champ doit contenir un nombre (ID de l'employé), pas un nom
    QString idEmployeText = ui->lineEditTechnicien_6->text().trimmed();
    if (idEmployeText.isEmpty()) {
        idEmploye = 0; // NULL si vide
    } else {
        bool ok;
        idEmploye = idEmployeText.toLongLong(&ok);
        if (!ok) {
            // Si ce n'est pas un nombre valide, mettre 0 (NULL)
            idEmploye = 0;
            // Optionnel: Afficher un avertissement
            // QMessageBox::warning(this, "Attention", "Le champ Technicien doit contenir un ID numérique. Valeur ignorée.");
        }
    }

    if (!validateFields(nom, marque, modele, couleur, numeroSerie, type, etat)) {
        // Focus sur le premier champ en erreur si possible
        if (nom.isEmpty()) ui->lineEditNomObjet_6->setFocus();
        else if (marque.isEmpty()) ui->lineEditMarque_6->setFocus();
        else if (type.isEmpty() || type == "-- Sélectionner --") ui->comboBoxType_6->setFocus();
        else if (etat.isEmpty() || etat == "-- Sélectionner --") ui->comboBoxEtat_6->setFocus();
        return false;
    }

    return true;
}

void gestionobjetelectronique::on_btnAjouter_6_clicked()
{
    qlonglong id, idClient, idEmploye;
    QString nom, marque, modele, couleur, numeroSerie, type, etat;
    
    if (!readForm(id, nom, marque, modele, couleur, numeroSerie, type, etat, idClient, idEmploye)) return;

    // Vérifier que la base de données est toujours ouverte
    if (!database.isOpen()) {
        database = Connection::getOracleConnection();
        if (!database.isOpen()) {
            QMessageBox::critical(this, "Base de données", "Impossible d'ouvrir la base: " + database.lastError().text());
            return;
        }
    }

    // Obtenir le prochain ID
    qlonglong newId = getNextId();
    
    // Vérifier que le nom de table est valide
    if (tableName.isEmpty()) {
        tableName = detectTableName();
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur", "Impossible de détecter le nom de la table. Vérifiez votre connexion à la base de données.");
            return;
        }
    }

    // Construire la requête SQL avec gestion des valeurs NULL
    // Pour Oracle, on peut utiliser NULL directement dans la requête ou utiliser des bindings
    QString insertSql;
    
    // Construire dynamiquement la liste des colonnes et valeurs
    QStringList columns = {"ID_OBJET", "NOM_OBJET", "MARQUE"};
    QStringList values = {QString::number(newId), "?", "?"};
    QList<QVariant> bindValues;
    bindValues << nom << marque;
    
    // Ajouter MODELE
    if (!modele.isEmpty()) {
        columns << "MODELE";
        values << "?";
        bindValues << modele;
    }
    
    // Ajouter COULEUR
    if (!couleur.isEmpty()) {
        columns << "COULEUR";
        values << "?";
        bindValues << couleur;
    }
    
    // Ajouter NUM_SERIE
    if (!numeroSerie.isEmpty()) {
        columns << "NUM_SERIE";
        values << "?";
        bindValues << numeroSerie;
    }
    
    // Ajouter TYPE_OBJET et ETAT (obligatoires)
    columns << "TYPE_OBJET" << "ETAT";
    values << "?" << "?";
    bindValues << type << etat;
    
    // Ajouter ID_CLIENT_COURANT si > 0
    if (idClient > 0) {
        columns << "ID_CLIENT_COURANT";
        values << "?";
        bindValues << idClient;
    }
    
    // Ajouter ID_EMPLOYE_COURANT si > 0
    if (idEmploye > 0) {
        columns << "ID_EMPLOYE_COURANT";
        values << "?";
        bindValues << idEmploye;
    }
    
    insertSql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                .arg(tableName)
                .arg(columns.join(", "))
                .arg(values.join(", "));

    QSqlQuery q(database);
    
    if (!q.prepare(insertSql)) {
        QString errorMsg = q.lastError().text();
        QMessageBox::warning(this, "Ajout", QString("Erreur de préparation:\n%1\n\nSQL: %2").arg(errorMsg).arg(insertSql));
        return;
    }
    
    // Ajouter toutes les valeurs bindées
    for (const QVariant &val : bindValues) {
        q.addBindValue(val);
    }
    
    if (!q.exec()) {
        QString errorMsg = q.lastError().text();
        QString detailMsg = QString("Erreur SQL: %1\n\nRequête: %2\n\nID: %3\nNom: %4\nMarque: %5\nTable: %6")
                            .arg(errorMsg)
                            .arg(insertSql)
                            .arg(newId)
                            .arg(nom)
                            .arg(marque)
                            .arg(tableName);
        QMessageBox::warning(this, "Ajout", "Échec d'ajout:\n" + detailMsg);
        qDebug() << "Erreur INSERT:" << errorMsg;
        qDebug() << "SQL:" << insertSql;
        qDebug() << "Table name:" << tableName;
        qDebug() << "Bind values count:" << bindValues.size();
        return;
    }
    
    // Commit pour s'assurer que les données sont sauvegardées
    if (!database.commit()) {
        QMessageBox::warning(this, "Ajout", "Erreur lors du commit: " + database.lastError().text());
        database.rollback();
        return;
    }
    
    // Nettoyer le formulaire après l'ajout réussi
    clearForm();
    
    // Rafraîchir la table
    refreshTable();
    
    // Message de confirmation
    QMessageBox::information(this, "Ajout", "L'objet a été ajouté avec succès !");
}

void gestionobjetelectronique::on_btnModifier_6_clicked()
{
    // Modifier l'objet depuis le formulaire
    const QString refText = ui->lineEditReference_6->text().trimmed();
    if (refText.isEmpty()) {
        QMessageBox::warning(this, "Modification", "Sélectionnez d'abord un objet à modifier depuis le tableau.");
        return;
    }
    
    bool ok;
    qlonglong id = refText.toLongLong(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Modification", "ID invalide. Veuillez sélectionner un objet depuis le tableau.");
        return;
    }
    
    qlonglong idClient, idEmploye;
    QString nom, marque, modele, couleur, numeroSerie, type, etat;
    
    if (!readForm(id, nom, marque, modele, couleur, numeroSerie, type, etat, idClient, idEmploye)) {
        return;
    }
    
    // Vérifier que la base de données est toujours ouverte
    if (!database.isOpen()) {
        database = Connection::getOracleConnection();
        if (!database.isOpen()) {
            QMessageBox::critical(this, "Base de données", "Impossible d'ouvrir la base: " + database.lastError().text());
            return;
        }
    }
    
    // Vérifier que le nom de table est valide
    if (tableName.isEmpty()) {
        tableName = detectTableName();
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur", "Impossible de détecter le nom de la table. Vérifiez votre connexion à la base de données.");
            return;
        }
    }
    
    // Construire la requête UPDATE avec gestion des valeurs NULL
    QStringList setClauses;
    QList<QVariant> bindValues;
    
    setClauses << "NOM_OBJET = ?";
    bindValues << nom;
    
    setClauses << "MARQUE = ?";
    bindValues << marque;
    
    // Gérer MODELE (peut être NULL)
    if (modele.isEmpty()) {
        setClauses << "MODELE = NULL";
    } else {
        setClauses << "MODELE = ?";
        bindValues << modele;
    }
    
    // Gérer COULEUR (peut être NULL)
    if (couleur.isEmpty()) {
        setClauses << "COULEUR = NULL";
    } else {
        setClauses << "COULEUR = ?";
        bindValues << couleur;
    }
    
    // Gérer NUM_SERIE (peut être NULL)
    if (numeroSerie.isEmpty()) {
        setClauses << "NUM_SERIE = NULL";
    } else {
        setClauses << "NUM_SERIE = ?";
        bindValues << numeroSerie;
    }
    
    setClauses << "TYPE_OBJET = ?";
    bindValues << type;
    
    setClauses << "ETAT = ?";
    bindValues << etat;
    
    // Gérer ID_CLIENT_COURANT (peut être NULL)
    if (idClient > 0) {
        setClauses << "ID_CLIENT_COURANT = ?";
        bindValues << idClient;
    } else {
        setClauses << "ID_CLIENT_COURANT = NULL";
    }
    
    // Gérer ID_EMPLOYE_COURANT (peut être NULL)
    if (idEmploye > 0) {
        setClauses << "ID_EMPLOYE_COURANT = ?";
        bindValues << idEmploye;
    } else {
        setClauses << "ID_EMPLOYE_COURANT = NULL";
    }
    
    // Fonction helper pour échapper les apostrophes
    auto escapeSql = [](const QString &str) -> QString {
        QString result = str;
        return result.replace("'", "''");
    };
    
    // Construire la requête UPDATE avec toutes les valeurs directement dans le SQL
    // Pour éviter les problèmes avec QODBC et les valeurs NULL
    QString updateSql = QString("UPDATE %1 SET NOM_OBJET = '%2', MARQUE = '%3'")
                        .arg(tableName)
                        .arg(escapeSql(nom))
                        .arg(escapeSql(marque));
    
    // Ajouter MODELE
    if (modele.isEmpty()) {
        updateSql += ", MODELE = NULL";
    } else {
        updateSql += QString(", MODELE = '%1'").arg(escapeSql(modele));
    }
    
    // Ajouter COULEUR
    if (couleur.isEmpty()) {
        updateSql += ", COULEUR = NULL";
    } else {
        updateSql += QString(", COULEUR = '%1'").arg(escapeSql(couleur));
    }
    
    // Ajouter NUM_SERIE
    if (numeroSerie.isEmpty()) {
        updateSql += ", NUM_SERIE = NULL";
    } else {
        updateSql += QString(", NUM_SERIE = '%1'").arg(escapeSql(numeroSerie));
    }
    
    // Ajouter TYPE_OBJET et ETAT
    updateSql += QString(", TYPE_OBJET = '%1', ETAT = '%2'")
                 .arg(escapeSql(type))
                 .arg(escapeSql(etat));
    
    // Ajouter ID_CLIENT_COURANT
    if (idClient > 0) {
        updateSql += QString(", ID_CLIENT_COURANT = %1").arg(idClient);
    } else {
        updateSql += ", ID_CLIENT_COURANT = NULL";
    }
    
    // Ajouter ID_EMPLOYE_COURANT
    if (idEmploye > 0) {
        updateSql += QString(", ID_EMPLOYE_COURANT = %1").arg(idEmploye);
    } else {
        updateSql += ", ID_EMPLOYE_COURANT = NULL";
    }
    
    // Ajouter le WHERE
    updateSql += QString(" WHERE ID_OBJET = %1").arg(id);
    
    QSqlQuery q(database);
    
    if (!q.exec(updateSql)) {
        QString errorMsg = q.lastError().text();
        QString detailMsg = QString("Erreur SQL: %1\n\nRequête: %2\n\nID: %3\nTable: %4")
                            .arg(errorMsg)
                            .arg(updateSql)
                            .arg(id)
                            .arg(tableName);
        QMessageBox::warning(this, "Modification", "Échec de modification:\n" + detailMsg);
        qDebug() << "Erreur UPDATE:" << errorMsg;
        qDebug() << "SQL:" << updateSql;
        qDebug() << "Table name:" << tableName;
        return;
    }
    
    // Commit pour s'assurer que les données sont sauvegardées
    if (!database.commit()) {
        QMessageBox::warning(this, "Modification", "Erreur lors du commit: " + database.lastError().text());
        database.rollback();
        return;
    }
    
    clearForm();
    refreshTable();
    QMessageBox::information(this, "Modification", "L'objet a été modifié avec succès !");
}

void gestionobjetelectronique::on_btnSupprimer_6_clicked()
{
    const QString refText = ui->lineEditReference_6->text().trimmed();
    if (refText.isEmpty()) {
        QMessageBox::warning(this, "Suppression", "Saisissez l'ID de l'objet à supprimer.");
        return;
    }
    
    bool ok;
    qlonglong id = refText.toLongLong(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Suppression", "ID invalide. Veuillez entrer un nombre valide.");
        return;
    }
    
    // Confirmation avant suppression
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", 
                                                              "Êtes-vous sûr de vouloir supprimer l'objet avec l'ID: " + QString::number(id) + " ?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }
    
    // Vérifier que la base de données est toujours ouverte
    if (!database.isOpen()) {
        database = Connection::getOracleConnection();
        if (!database.isOpen()) {
            QMessageBox::critical(this, "Base de données", "Impossible d'ouvrir la base: " + database.lastError().text());
            return;
        }
    }
    
    // Vérifier que le nom de table est valide
    if (tableName.isEmpty()) {
        tableName = detectTableName();
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur", "Impossible de détecter le nom de la table. Vérifiez votre connexion à la base de données.");
            return;
        }
    }
    
    // Utiliser une requête SQL directe pour DELETE (sans bind)
    QString deleteSql = QString("DELETE FROM %1 WHERE ID_OBJET = %2")
                        .arg(tableName)
                        .arg(id);
    
    QSqlQuery q(database);
    
    if (!q.exec(deleteSql)) {
        QString errorMsg = q.lastError().text();
        // Essayer de redétecter le nom de table si ça échoue
        QString originalTableName = tableName;
        tableName = detectTableName();
        
        if (tableName != originalTableName) {
            deleteSql = QString("DELETE FROM %1 WHERE ID_OBJET = %2")
                        .arg(tableName)
                        .arg(id);
            if (!q.exec(deleteSql)) {
                QString detailMsg = QString("Erreur SQL: %1\n\nRequête: %2\n\nID: %3\nTable: %4\n\nVérifiez:\n- Que l'ID existe dans la table\n- Qu'il n'y a pas de contraintes de clé étrangère\n- Que vous avez les permissions de suppression")
                                    .arg(q.lastError().text())
                                    .arg(deleteSql)
                                    .arg(id)
                                    .arg(tableName);
                QMessageBox::warning(this, "Suppression", "Échec de suppression:\n" + detailMsg);
                qDebug() << "Erreur DELETE:" << q.lastError().text();
                qDebug() << "SQL:" << deleteSql;
                qDebug() << "Table name:" << tableName;
                return;
            }
        } else {
            QString detailMsg = QString("Erreur SQL: %1\n\nRequête: %2\n\nID: %3\nTable: %4\n\nVérifiez:\n- Que l'ID existe dans la table\n- Qu'il n'y a pas de contraintes de clé étrangère\n- Que vous avez les permissions de suppression")
                                .arg(errorMsg)
                                .arg(deleteSql)
                                .arg(id)
                                .arg(tableName);
            QMessageBox::warning(this, "Suppression", "Échec de suppression:\n" + detailMsg);
            qDebug() << "Erreur DELETE:" << errorMsg;
            qDebug() << "SQL:" << deleteSql;
            qDebug() << "Table name:" << tableName;
            return;
        }
    }
    
    // Vérifier si une ligne a été supprimée
    // Note: numRowsAffected() peut ne pas fonctionner avec tous les drivers Oracle
    // On peut aussi vérifier en faisant un SELECT après
    int rowsAffected = q.numRowsAffected();
    
    // Si numRowsAffected() retourne -1 (non supporté), on vérifie autrement
    if (rowsAffected == -1) {
        // Vérifier si l'objet existe encore
        QSqlQuery checkQ(database);
        QString checkSql = QString("SELECT COUNT(*) FROM %1 WHERE ID_OBJET = ?").arg(tableName);
        if (checkQ.prepare(checkSql)) {
            checkQ.addBindValue(id);
            if (checkQ.exec() && checkQ.next()) {
                if (checkQ.value(0).toInt() > 0) {
                    QMessageBox::warning(this, "Suppression", "La suppression a échoué. L'objet existe toujours dans la base de données.");
                    return;
                }
            }
        }
    } else if (rowsAffected == 0) {
        QMessageBox::warning(this, "Suppression", "Aucun objet trouvé avec cet ID.");
        return;
    }
    
    // Commit pour s'assurer que les données sont sauvegardées
    if (!database.commit()) {
        QMessageBox::warning(this, "Suppression", "Erreur lors du commit: " + database.lastError().text());
        database.rollback();
        return;
    }
    
    clearForm();
    refreshTable();
    QMessageBox::information(this, "Suppression", "L'objet a été supprimé avec succès !");
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
    currentSelectedId = 0;
}

void gestionobjetelectronique::on_tableWidget_itemSelectionChanged()
{
    auto items = ui->tableWidget->selectedItems();
    if (items.size() < 10) return;
    // items come by row; map to fields
    ui->lineEditReference_6->setText(items[0]->text());
    currentSelectedId = items[0]->data(Qt::UserRole).toLongLong();
    ui->lineEditNomObjet_6->setText(items[1]->text());
    ui->lineEditMarque_6->setText(items[2]->text());
    ui->lineEditModele_6->setText(items[3]->text());
    ui->lineEditCouleur_6->setText(items[4]->text());
    ui->lineEditNumeroSerie_6->setText(items[5]->text());
    ui->comboBoxType_6->setCurrentText(items[6]->text());
    ui->comboBoxEtat_6->setCurrentText(items[7]->text());
    // ID_CLIENT_COURANT dans spinBoxPrix_6
    QString idClientStr = items[8]->text();
    ui->spinBoxPrix_6->setValue(idClientStr.isEmpty() ? 0 : idClientStr.toInt());
    // ID_EMPLOYE_COURANT dans lineEditTechnicien_6
    ui->lineEditTechnicien_6->setText(items[9]->text());
}

void gestionobjetelectronique::on_tableWidget_cellChanged(int row, int /*column*/)
{
    if (!isTableEditMode) return; // only react in edit mode
    if (row < 0 || row >= ui->tableWidget->rowCount()) return;
    
    // Vérifier que la base de données est toujours ouverte
    if (!database.isOpen()) {
        database = Connection::getOracleConnection();
        if (!database.isOpen()) {
            QMessageBox::critical(this, "Base de données", "Impossible d'ouvrir la base: " + database.lastError().text());
            refreshTable();
            return;
        }
    }
    
    // Read entire row from table
    auto get = [&](int c){ QTableWidgetItem *it = ui->tableWidget->item(row, c); return it ? it->text().trimmed() : QString(); };
    qlonglong idNew = get(0).toLongLong();
    QString nom = get(1);
    QString marque = get(2);
    QString modele = get(3);
    QString couleur = get(4);
    QString numeroSerie = get(5);
    QString type = get(6);
    QString etat = get(7);
    qlonglong idClient = get(8).toLongLong();
    qlonglong idEmploye = get(9).toLongLong();

    // Valider les champs avant de mettre à jour la base de données
    if (!validateFields(nom, marque, modele, couleur, numeroSerie, type, etat)) {
        // Annuler la modification en restaurant la valeur précédente
        refreshTable();
        return;
    }

    // original ID stored in UserRole of column 0 item
    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    if (!idItem) return;
    const qlonglong idOld = idItem->data(Qt::UserRole).toLongLong();

    // Vérifier que le nom de table est valide
    if (tableName.isEmpty()) {
        tableName = detectTableName();
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur", "Impossible de détecter le nom de la table. Vérifiez votre connexion à la base de données.");
            refreshTable();
            return;
        }
    }
    
    // Fonction helper pour échapper les apostrophes
    auto escapeSql = [](const QString &str) -> QString {
        QString result = str;
        return result.replace("'", "''");
    };
    
    // Construire la requête UPDATE avec toutes les valeurs directement dans le SQL
    // Pour éviter les problèmes avec QODBC et les valeurs NULL
    QString updateSql = QString("UPDATE %1 SET NOM_OBJET = '%2', MARQUE = '%3'")
                        .arg(tableName)
                        .arg(escapeSql(nom))
                        .arg(escapeSql(marque));
    
    // Ajouter MODELE
    if (modele.isEmpty()) {
        updateSql += ", MODELE = NULL";
    } else {
        updateSql += QString(", MODELE = '%1'").arg(escapeSql(modele));
    }
    
    // Ajouter COULEUR
    if (couleur.isEmpty()) {
        updateSql += ", COULEUR = NULL";
    } else {
        updateSql += QString(", COULEUR = '%1'").arg(escapeSql(couleur));
    }
    
    // Ajouter NUM_SERIE
    if (numeroSerie.isEmpty()) {
        updateSql += ", NUM_SERIE = NULL";
    } else {
        updateSql += QString(", NUM_SERIE = '%1'").arg(escapeSql(numeroSerie));
    }
    
    // Ajouter TYPE_OBJET et ETAT
    updateSql += QString(", TYPE_OBJET = '%1', ETAT = '%2'")
                 .arg(escapeSql(type))
                 .arg(escapeSql(etat));
    
    // Ajouter ID_CLIENT_COURANT
    if (idClient > 0) {
        updateSql += QString(", ID_CLIENT_COURANT = %1").arg(idClient);
    } else {
        updateSql += ", ID_CLIENT_COURANT = NULL";
    }
    
    // Ajouter ID_EMPLOYE_COURANT
    if (idEmploye > 0) {
        updateSql += QString(", ID_EMPLOYE_COURANT = %1").arg(idEmploye);
    } else {
        updateSql += ", ID_EMPLOYE_COURANT = NULL";
    }
    
    // Ajouter le WHERE
    updateSql += QString(" WHERE ID_OBJET = %1").arg(idOld);
    
    QSqlQuery q(database);
    
    if (!q.exec(updateSql)) {
        QString errorMsg = q.lastError().text();
        QString detailMsg = QString("Erreur SQL: %1\n\nRequête: %2\n\nID: %3\nTable: %4")
                            .arg(errorMsg)
                            .arg(updateSql)
                            .arg(idOld)
                            .arg(tableName);
        QMessageBox::warning(this, "Modification", "Échec de modification:\n" + detailMsg);
        qDebug() << "Erreur UPDATE:" << errorMsg;
        qDebug() << "SQL:" << updateSql;
        qDebug() << "Table name:" << tableName;
        refreshTable();
        return;
    }
    
    // Commit pour s'assurer que les données sont sauvegardées
    if (!database.commit()) {
        QMessageBox::warning(this, "Modification", "Erreur lors du commit: " + database.lastError().text());
        database.rollback();
        refreshTable();
        return;
    }
    
    // update stored original ID for this row
    idItem->setData(Qt::UserRole, idNew);

    // Désactiver temporairement les signaux pour éviter les boucles infinies
    ui->tableWidget->blockSignals(true);
    
    // sync edited values to form
    ui->lineEditReference_6->setText(QString::number(idNew));
    ui->lineEditNomObjet_6->setText(nom);
    ui->lineEditMarque_6->setText(marque);
    ui->lineEditModele_6->setText(modele);
    ui->lineEditCouleur_6->setText(couleur);
    ui->lineEditNumeroSerie_6->setText(numeroSerie);
    ui->comboBoxType_6->setCurrentText(type);
    ui->comboBoxEtat_6->setCurrentText(etat);
    ui->spinBoxPrix_6->setValue(idClient);
    ui->lineEditTechnicien_6->setText(QString::number(idEmploye));
    currentSelectedId = idNew;
    
    // Réactiver les signaux
    ui->tableWidget->blockSignals(false);
}
