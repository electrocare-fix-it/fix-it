#include "gestionpieces.h"
#include "ui_gestionpieces.h"
#include "connection.h"
#include "serialportmanager.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QFileDialog>
#include <QPixmap>
#include <QBrush>
#include <QIcon>
#include <QDir>
#include <QPdfWriter>
#include <QPainter>
#include <QPageLayout>
#include <QPageSize>
#include <QDebug>
#include <QApplication>

gestionpieces::gestionpieces(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionpieces)
    , m_serialManager(new SerialPortManager(this))
    , m_accessGranted(false)
{
    ui->setupUi(this);

    // Harmoniser les couleurs avec le reste de l'application :
    // texte bleu foncé bien lisible sur fond clair
    this->setStyleSheet(
        "QWidget#gestionpieces { background-color: #E0F6FF; }"
        "QLabel { color: #0D47A1; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit {"
        "  color: #0D47A1;"
        "  background-color: white;"
        "}"
        "QTableWidget {"
        "  color: #0D47A1;"
        "  gridline-color: #90CAF9;"
        "}"
    );

    initialiserBase();
    
    // IMPORTANT: Le système RFID ne dépend PAS de la base de données
    // Il lit directement les IDs des cartes depuis l'Arduino via le port série
    qDebug() << "=== INITIALISATION GESTION PIECES ===";
    qDebug() << "Le système RFID fonctionne INDEPENDAMMENT de la base de données";
    qDebug() << "Les IDs des cartes sont lus directement depuis l'Arduino";
    
    // Configurer l'accès RFID
    setupRFIDAccess();
    disableAccess(); // Désactiver par défaut jusqu'à ce qu'une carte autorisée soit scannée

    connect(ui->btnAjouter_2, &QPushButton::clicked, this, &gestionpieces::ajouterPiece);
    connect(ui->btnSupprimer_2, &QPushButton::clicked, this, &gestionpieces::supprimerPiece);
    connect(ui->tablePieces_2, &QTableWidget::cellClicked, this, &gestionpieces::remplirFormulaireDepuisTable);
    connect(ui->btnNouveau_2, &QPushButton::clicked, this, &gestionpieces::viderChamps);
    connect(ui->btnChargerImage, &QPushButton::clicked, this, &gestionpieces::chargerImagePiece);
    connect(ui->btnTrierPrix, &QPushButton::clicked, this, &gestionpieces::trierPiecesParPrix);
    connect(ui->btnRechercher_2, &QPushButton::clicked, this, &gestionpieces::rechercherPieces);
    connect(ui->btnExportPDF_2, &QPushButton::clicked, this, &gestionpieces::exporterTableauPDF);
    connect(ui->pushButton, &QPushButton::clicked, this, &gestionpieces::on_pushButton_clicked); // Bouton Home

    ui->lblApercuImage->setScaledContents(true);
    ui->lblApercuImage->setMinimumSize(200, 150);
    ui->lblApercuImage->setText("Aucune image");

    afficherPieces();
}

gestionpieces::~gestionpieces()
{
    delete ui;
}

void gestionpieces::initialiserBase()
{
    // Utiliser la même connexion que le reste de l'application
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        if (!conn.createConnection()) {
            QMessageBox::critical(this, tr("Erreur de connexion"),
                                  tr("Impossible de se connecter à la base de données:\n%1")
                                      .arg(conn.getDatabase().lastError().text()));
            return;
        }
    }
    db = conn.getDatabase();

    if (!db.open()) {
        QMessageBox::critical(this, tr("Erreur de connexion"),
                              tr("Impossible de se connecter à Oracle:\n%1")
                                  .arg(db.lastError().text()));
        return;
    }

    QSqlQuery query(db);

    const QString createTable =
        "CREATE TABLE PIECE ("
        "REFERENCE VARCHAR2(50) PRIMARY KEY, "
        "NOM VARCHAR2(100) NOT NULL, "
        "CATEGORIE VARCHAR2(50), "
        "QUANTITE NUMBER(10) DEFAULT 0, "
        "PRIX NUMBER(10,3) DEFAULT 0, "
        "FOURNISSEUR VARCHAR2(100), "
        "DATE_ENTREE DATE, "
        "DATE_SORTIE DATE, "
        "CHEMIN_IMAGE VARCHAR2(500))";

    query.prepare("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'PIECE'");
    bool tableExists = false;
    if (query.exec() && query.next()) {
        tableExists = (query.value(0).toInt() > 0);
    }

    if (!tableExists) {
        if (!query.exec(createTable)) {
            QMessageBox::warning(this, tr("Avertissement"),
                                 tr("Impossible de créer la table PIECE:\n%1")
                                     .arg(query.lastError().text()));
        }
    } else {
        query.prepare("SELECT COUNT(*) FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'PIECE' AND COLUMN_NAME = 'CHEMIN_IMAGE'");
        bool columnExists = false;
        if (query.exec() && query.next()) {
            columnExists = (query.value(0).toInt() > 0);
        }
        if (!columnExists) {
            if (!query.exec("ALTER TABLE PIECE ADD CHEMIN_IMAGE VARCHAR2(500)")) {
                QMessageBox::warning(this, tr("Avertissement"),
                                     tr("Impossible d'ajouter la colonne CHEMIN_IMAGE:\n%1")
                                         .arg(query.lastError().text()));
            }
        }
    }
}

void gestionpieces::ajouterPiece()
{
    const QString ref = ui->txtReference_2->text().trimmed();
    const QString nom = ui->txtNom_2->text().trimmed();

    if (ref.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("La référence est obligatoire !"));
        return;
    }
    if (nom.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le nom est obligatoire !"));
        return;
    }

    const QString categorie = ui->cmbCategorie_2->currentText().trimmed();
    const int quantite = ui->spinQuantite_2->value();
    const double prix = ui->spinPrix_2->value();
    const QString fournisseur = ui->txtFournisseur_2->text().trimmed();
    const QDate entree = ui->dateEntree_2->date();
    const QDate sortie = ui->dateSortie_2->date();

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM PIECE WHERE REFERENCE = :ref");
    query.bindValue(":ref", ref);

    bool exists = false;
    if (query.exec() && query.next()) {
        exists = (query.value(0).toInt() > 0);
    }

    if (exists) {
        query.prepare(
            "UPDATE PIECE SET NOM = :nom, CATEGORIE = :categorie, QUANTITE = :quantite, "
            "PRIX = :prix, FOURNISSEUR = :fournisseur, DATE_ENTREE = :entree, "
            "DATE_SORTIE = :sortie, CHEMIN_IMAGE = :chemin WHERE REFERENCE = :ref");
    } else {
        query.prepare(
            "INSERT INTO PIECE (REFERENCE, NOM, CATEGORIE, QUANTITE, PRIX, "
            "FOURNISSEUR, DATE_ENTREE, DATE_SORTIE, CHEMIN_IMAGE) "
            "VALUES (:ref, :nom, :categorie, :quantite, :prix, :fournisseur, :entree, :sortie, :chemin)");
    }

    query.bindValue(":ref", ref);
    query.bindValue(":nom", nom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);
    query.bindValue(":fournisseur", fournisseur);
    query.bindValue(":entree", entree);
    query.bindValue(":sortie", sortie);
    query.bindValue(":chemin", cheminImagePiece);

    if (query.exec()) {
        QMessageBox::information(this, tr("Succès"), exists ? tr("Pièce modifiée !") : tr("Pièce ajoutée !"));

        if (!cheminImagePiece.isEmpty()) {
            cheminsImagesPieces.insert(ref, cheminImagePiece);
        } else {
            cheminsImagesPieces.remove(ref);
        }

        cheminImagePiece.clear();
        ui->lblApercuImage->setPixmap(QPixmap());
        ui->lblApercuImage->setText("Aucune image");

        afficherPieces(ordreTriCourant, texteRechercheCourant, categorieFiltreCourante);
    } else {
        QMessageBox::warning(this, tr("Erreur"),
                             (exists ? tr("Modification") : tr("Insertion")) +
                                 tr(" échouée : %1").arg(query.lastError().text()));
    }
}

void gestionpieces::afficherPieces(const QString &orderBy,
                                   const QString &texteRecherche,
                                   const QString &categorieFiltre)
{
    const QString rechercheNettoyee = texteRecherche.trimmed();
    const QString categorieNettoyee = categorieFiltre.trimmed();

    QString requete = "SELECT * FROM PIECE";
    QStringList conditions;

    if (!rechercheNettoyee.isEmpty()) {
        conditions << "(LOWER(REFERENCE) LIKE :searchRef OR LOWER(NOM) LIKE :searchNom OR "
                      "LOWER(CATEGORIE) LIKE :searchCat OR LOWER(FOURNISSEUR) LIKE :searchFourn)";
    }
    if (!categorieNettoyee.isEmpty()) {
        conditions << "LOWER(CATEGORIE) = :categorieExacte";
    }
    if (!conditions.isEmpty()) {
        requete.append(" WHERE ").append(conditions.join(" AND "));
    }
    if (!orderBy.trimmed().isEmpty()) {
        requete.append(' ').append(orderBy.trimmed());
    }

    QSqlQuery query(db);
    if (!query.prepare(requete)) {
        QMessageBox::warning(this, tr("Erreur"),
                             tr("Préparation de la requête échouée : %1").arg(query.lastError().text()));
        return;
    }

    if (!rechercheNettoyee.isEmpty()) {
        const QString motif = "%" + rechercheNettoyee.toLower() + "%";
        query.bindValue(":searchRef", motif);
        query.bindValue(":searchNom", motif);
        query.bindValue(":searchCat", motif);
        query.bindValue(":searchFourn", motif);
    }
    if (!categorieNettoyee.isEmpty()) {
        query.bindValue(":categorieExacte", categorieNettoyee.toLower());
    }

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Erreur"),
                             tr("Impossible de récupérer les pièces : %1").arg(query.lastError().text()));
        return;
    }

    ui->tablePieces_2->setRowCount(0);
    if (ui->tablePieces_2->columnCount() != 9) {
        ui->tablePieces_2->setColumnCount(9);
        QStringList headers;
        headers << tr("Référence") << tr("Nom") << tr("Catégorie") << tr("Quantité") << tr("Prix")
                << tr("Fournisseur") << tr("Date entrée") << tr("Date sortie") << tr("Image");
        ui->tablePieces_2->setHorizontalHeaderLabels(headers);
    }

    int row = 0;
    while (query.next()) {
        ui->tablePieces_2->insertRow(row);

        const bool lowStock = query.value(3).toInt() < 5;
        const QString reference = query.value(0).toString();

        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            if (lowStock) item->setForeground(QBrush(Qt::red));
            ui->tablePieces_2->setItem(row, col, item);
        }

        QString cheminImage;
        QSqlRecord record = query.record();
        int cheminIndex = record.indexOf("CHEMIN_IMAGE");
        if (cheminIndex >= 0) {
            cheminImage = query.value(cheminIndex).toString().trimmed();
        } else if (record.count() > 8) {
            cheminImage = query.value(8).toString().trimmed();
        }

        if (!cheminImage.isEmpty()) {
            cheminsImagesPieces.insert(reference, cheminImage);
        } else {
            cheminsImagesPieces.remove(reference);
        }

        QTableWidgetItem *imageItem = new QTableWidgetItem();
        if (!cheminImage.isEmpty()) {
            QPixmap pixmap(cheminImage);
            if (!pixmap.isNull()) {
                QIcon icon(pixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                imageItem->setIcon(icon);
                imageItem->setTextAlignment(Qt::AlignCenter);
                ui->tablePieces_2->setRowHeight(row, 140);
            } else {
                imageItem->setText(tr("Image introuvable"));
                imageItem->setTextAlignment(Qt::AlignCenter);
            }
        } else {
            imageItem->setText(QStringLiteral("—"));
            imageItem->setTextAlignment(Qt::AlignCenter);
        }
        if (lowStock) imageItem->setForeground(QBrush(Qt::red));
        ui->tablePieces_2->setItem(row, 8, imageItem);

        ui->tablePieces_2->setColumnWidth(8, 150);
        ++row;
    }
}

void gestionpieces::supprimerPiece()
{
    const QString ref = ui->txtReference_2->text().trimmed();
    if (ref.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer la référence à supprimer !"));
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM PIECE WHERE REFERENCE = :ref");
    query.bindValue(":ref", ref);

    if (query.exec()) {
        QMessageBox::information(this, tr("Supprimé"), tr("Pièce supprimée avec succès !"));
        afficherPieces(ordreTriCourant, texteRechercheCourant, categorieFiltreCourante);
    } else {
        QMessageBox::warning(this, tr("Erreur"),
                             tr("Suppression échouée : %1").arg(query.lastError().text()));
    }
}

void gestionpieces::remplirFormulaireDepuisTable(int row, int)
{
    if (row < 0 || row >= ui->tablePieces_2->rowCount())
        return;

    QTableWidgetItem *itemRef = ui->tablePieces_2->item(row, 0);
    QTableWidgetItem *itemNom = ui->tablePieces_2->item(row, 1);
    QTableWidgetItem *itemCategorie = ui->tablePieces_2->item(row, 2);
    QTableWidgetItem *itemQuantite = ui->tablePieces_2->item(row, 3);
    QTableWidgetItem *itemPrix = ui->tablePieces_2->item(row, 4);
    QTableWidgetItem *itemFournisseur = ui->tablePieces_2->item(row, 5);
    QTableWidgetItem *itemEntree = ui->tablePieces_2->item(row, 6);
    QTableWidgetItem *itemSortie = ui->tablePieces_2->item(row, 7);

    if (itemRef) {
        ui->txtReference_2->setText(itemRef->text());
        const QString ref = itemRef->text();

        QSqlQuery query(db);
        query.prepare("SELECT CHEMIN_IMAGE FROM PIECE WHERE REFERENCE = :ref");
        query.bindValue(":ref", ref);

        QString chemin;
        if (query.exec() && query.next()) {
            chemin = query.value(0).toString();
        }

        if (!chemin.isEmpty()) {
            cheminsImagesPieces.insert(ref, chemin);
        } else {
            cheminsImagesPieces.remove(ref);
        }

        cheminImagePiece = chemin;

        if (!chemin.isEmpty()) {
            QPixmap pixmap(chemin);
            if (!pixmap.isNull()) {
                ui->lblApercuImage->setPixmap(
                    pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                ui->lblApercuImage->setText(QString());
            } else {
                ui->lblApercuImage->setPixmap(QPixmap());
                ui->lblApercuImage->setText(tr("Image introuvable"));
            }
        } else {
            ui->lblApercuImage->setPixmap(QPixmap());
            ui->lblApercuImage->setText(tr("Aucune image"));
        }
    }

    if (itemNom) ui->txtNom_2->setText(itemNom->text());
    if (itemCategorie) {
        int index = ui->cmbCategorie_2->findText(itemCategorie->text());
        if (index >= 0)
            ui->cmbCategorie_2->setCurrentIndex(index);
        else
            ui->cmbCategorie_2->setCurrentText(itemCategorie->text());
    }
    if (itemQuantite) ui->spinQuantite_2->setValue(itemQuantite->text().toInt());
    if (itemPrix) ui->spinPrix_2->setValue(itemPrix->text().toDouble());
    if (itemFournisseur) ui->txtFournisseur_2->setText(itemFournisseur->text());

    if (itemEntree) {
        QDate date = QDate::fromString(itemEntree->text(), Qt::ISODate);
        if (date.isValid()) ui->dateEntree_2->setDate(date);
    }
    if (itemSortie) {
        QDate date = QDate::fromString(itemSortie->text(), Qt::ISODate);
        if (date.isValid()) ui->dateSortie_2->setDate(date);
    }
}

void gestionpieces::viderChamps()
{
    ui->txtReference_2->clear();
    ui->txtNom_2->clear();
    ui->cmbCategorie_2->setCurrentIndex(0);
    ui->spinQuantite_2->setValue(0);
    ui->spinPrix_2->setValue(0.0);
    ui->txtFournisseur_2->clear();
    ui->dateEntree_2->setDate(QDate::currentDate());
    ui->dateSortie_2->setDate(QDate::currentDate());
    cheminImagePiece.clear();
    ui->lblApercuImage->setPixmap(QPixmap());
    ui->lblApercuImage->setText(tr("Aucune image"));
}

void gestionpieces::chargerImagePiece()
{
    const QString fichier = QFileDialog::getOpenFileName(
        this,
        tr("Choisir une image de la pièce"),
        QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    if (fichier.isEmpty())
        return;

    QPixmap pixmap(fichier);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, tr("Image invalide"),
                             tr("Impossible de charger l'image sélectionnée."));
        return;
    }

    cheminImagePiece = QDir::toNativeSeparators(fichier);

    QSize labelSize = ui->lblApercuImage->size();
    if (labelSize.width() < 200) labelSize.setWidth(300);
    if (labelSize.height() < 200) labelSize.setHeight(300);

    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lblApercuImage->setPixmap(scaledPixmap);
    ui->lblApercuImage->setText(QString());
    ui->lblApercuImage->update();
}

void gestionpieces::trierPiecesParPrix()
{
    const bool triAscendantCourant = triPrixAscendant;
    const QString ordre = triAscendantCourant ? "ORDER BY PRIX ASC" : "ORDER BY PRIX DESC";
    ordreTriCourant = ordre;

    afficherPieces(ordreTriCourant, texteRechercheCourant, categorieFiltreCourante);

    if (ui->btnTrierPrix) {
        ui->btnTrierPrix->setText(triAscendantCourant ? tr("⬇️ Trier prix décroissant")
                                                      : tr("⬆️ Trier prix croissant"));
    }

    triPrixAscendant = !triPrixAscendant;
}

void gestionpieces::rechercherPieces()
{
    texteRechercheCourant = ui->txtRecherche_2->text().trimmed();
    QString categorieSelectionnee = ui->cmbFiltreCategorie_2->currentText().trimmed();
    if (categorieSelectionnee.compare("Toutes les catégories", Qt::CaseInsensitive) == 0) {
        categorieSelectionnee.clear();
    }
    categorieFiltreCourante = categorieSelectionnee;

    afficherPieces(ordreTriCourant, texteRechercheCourant, categorieFiltreCourante);
}

void gestionpieces::exporterTableauPDF()
{
    if (!ui->tablePieces_2 || ui->tablePieces_2->rowCount() == 0) {
        QMessageBox::information(this,
                                 tr("Export PDF"),
                                 tr("Aucune donnée à exporter dans le tableau."));
        return;
    }

    const QString fichier = QFileDialog::getSaveFileName(
        this,
        tr("Enregistrer le tableau au format PDF"),
        QDir::homePath() + "/pieces.pdf",
        tr("Fichiers PDF (*.pdf)"));

    if (fichier.isEmpty())
        return;

    QPdfWriter writer(fichier);
    writer.setPageOrientation(QPageLayout::Landscape);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(this,
                             tr("Export PDF"),
                             tr("Impossible de créer le fichier PDF."));
        return;
    }

    QFont fontTitre = painter.font();
    fontTitre.setPointSize(14);
    fontTitre.setBold(true);
    painter.setFont(fontTitre);
    painter.drawText(QRectF(0, 0, writer.width(), 200),
                     Qt::AlignCenter,
                     tr("Liste des Pièces Détachées"));

    QFont fontTable = painter.font();
    fontTable.setPointSize(8);
    fontTable.setBold(false);
    painter.setFont(fontTable);

    const int margin = 100;
    const int topOffset = 300;
    const QRect rectContenu(margin,
                            topOffset,
                            writer.width() - 2 * margin,
                            writer.height() - topOffset - margin);

    const int rows = ui->tablePieces_2->rowCount();
    const int cols = ui->tablePieces_2->columnCount();

    if (rows <= 0 || cols <= 0) {
        QMessageBox::warning(this,
                             tr("Export PDF"),
                             tr("Le tableau ne contient pas de données exportables."));
        return;
    }

    const double cellWidth = static_cast<double>(rectContenu.width()) / cols;
    const double cellHeight = 200.0;

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    painter.setBrush(Qt::lightGray);
    for (int col = 0; col < cols; ++col) {
        QRectF cellRect(rectContenu.left() + col * cellWidth,
                        rectContenu.top(),
                        cellWidth,
                        cellHeight);
        painter.drawRect(cellRect);

        QString headerText = ui->tablePieces_2->horizontalHeaderItem(col)
                                 ? ui->tablePieces_2->horizontalHeaderItem(col)->text()
                                 : QString();
        painter.drawText(cellRect.adjusted(5, 5, -5, -5),
                         Qt::AlignCenter | Qt::TextWordWrap,
                         headerText);
    }

    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < rows; ++row) {
        const double y = rectContenu.top() + (row + 1) * cellHeight;
        if (y + cellHeight > rectContenu.bottom())
            break;

        for (int col = 0; col < cols; ++col) {
            QRectF cellRect(rectContenu.left() + col * cellWidth,
                            y,
                            cellWidth,
                            cellHeight);
            painter.drawRect(cellRect);

            QTableWidgetItem *item = ui->tablePieces_2->item(row, col);

            if (col == 8) {
                QString ref;
                QTableWidgetItem *refItem = ui->tablePieces_2->item(row, 0);
                if (refItem)
                    ref = refItem->text();

                QString chemin = cheminsImagesPieces.value(ref);
                if (!chemin.isEmpty()) {
                    QPixmap pix(chemin);
                    if (!pix.isNull()) {
                        const int padding = 10;
                        QRectF innerRect = cellRect.adjusted(padding, padding, -padding, -padding);
                        QPixmap scaled = pix.scaled(innerRect.size().toSize(),
                                                    Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation);
                        QPointF topLeft(innerRect.left() + (innerRect.width() - scaled.width()) / 2.0,
                                        innerRect.top() + (innerRect.height() - scaled.height()) / 2.0);
                        painter.drawPixmap(topLeft, scaled);
                        continue;
                    }
                }

                QString text = item ? item->text() : QStringLiteral("—");
                if (text.isEmpty())
                    text = QStringLiteral("—");
                painter.drawText(cellRect.adjusted(5, 5, -5, -5),
                                 Qt::AlignCenter | Qt::TextWordWrap,
                                 text);
            } else {
                QString text = item ? item->text() : QString();
                painter.drawText(cellRect.adjusted(5, 5, -5, -5),
                                 Qt::AlignCenter | Qt::TextWordWrap,
                                 text);
            }
        }
    }

    painter.end();

    QMessageBox::information(this,
                             tr("Export PDF"),
                             tr("Le tableau a été exporté avec succès en PDF.\n\nChemin : %1").arg(fichier));
}

void gestionpieces::on_pushButton_clicked()
{
    // Émettre le signal pour retourner au menu principal
    emit homeRequested();
}

void gestionpieces::setupRFIDAccess()
{
    // Connecter les signaux du gestionnaire de port série
    connect(m_serialManager, &SerialPortManager::accessGranted,
            this, [this](const QString &employeeName, const QString &status) {
                qDebug() << "========================================";
                qDebug() << "*** SIGNAL accessGranted RECU ***";
                qDebug() << "Employee:" << employeeName;
                qDebug() << "Status:" << status;
                qDebug() << "========================================";
                
                // Forcer le traitement des événements
                QApplication::processEvents();
                
                m_currentEmployee = employeeName;
                
                // ACTIVER L'ACCÈS IMMÉDIATEMENT
                enableAccess();
                qDebug() << "Interface activee";
                
                // ÉMETTRE LE SIGNAL AVANT D'AFFICHER LE MESSAGE
                qDebug() << "*** EMISSION DU SIGNAL accessGrantedForMenu (AVANT MESSAGE) ***";
                emit accessGrantedForMenu();
                qDebug() << "Signal accessGrantedForMenu emis - Le bouton sera reactive";
                
                // Forcer le traitement des événements pour que le signal soit traité
                QApplication::processEvents();
                
                // Message pour Hiba Riahi
                QString message = "Bienvenue Hiba Riahi tu as l'accès pour l'interface de la gestion pièces détachées.";
                
                qDebug() << "Preparation du message:" << message;
                
                // Afficher directement avec QMessageBox pour forcer l'affichage
                QMessageBox *msgBox = new QMessageBox(this);
                msgBox->setIcon(QMessageBox::Information);
                msgBox->setWindowTitle("Accès Autorisé");
                msgBox->setText(message);
                msgBox->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                msgBox->setModal(true);
                
                qDebug() << "Ouverture du QMessageBox...";
                msgBox->show();
                msgBox->raise();
                msgBox->activateWindow();
                
                QApplication::processEvents();
                
                msgBox->exec();
                msgBox->deleteLater();
                
                qDebug() << "Message affiche et ferme";
            });
    
    connect(m_serialManager, &SerialPortManager::accessDenied,
            this, [this](const QString &employeeName, const QString &status) {
                qDebug() << "========================================";
                qDebug() << "*** SIGNAL accessDenied RECU ***";
                qDebug() << "Employee:" << employeeName;
                qDebug() << "Status:" << status;
                qDebug() << "========================================";
                
                // Forcer le traitement des événements
                QApplication::processEvents();
                
                m_currentEmployee = employeeName;
                
                // DÉSACTIVER L'ACCÈS IMMÉDIATEMENT
                disableAccess();
                qDebug() << "Interface desactivee (grises)";
                
                // ÉMETTRE LE SIGNAL AVANT D'AFFICHER LE MESSAGE (IMPORTANT!)
                qDebug() << "*** EMISSION DU SIGNAL accessDeniedForMenu (AVANT MESSAGE) ***";
                emit accessDeniedForMenu();
                qDebug() << "Signal accessDeniedForMenu emis - Le bouton sera desactive";
                
                // Forcer le traitement des événements pour que le signal soit traité
                QApplication::processEvents();
                
                // Message spécifique pour Omar Askri
                QString message;
                if (employeeName.toUpper() == "OMAR_ASKRI" || employeeName.toUpper().contains("OMAR") || employeeName.toUpper().contains("ASKRI")) {
                    message = "Accès non autorisé Omar Askri tu n'as pas l'accès pour la gestion pièces détachées.";
                } else {
                    message = "Accès non autorisé. Tu n'as pas l'accès pour la gestion pièces détachées.";
                }
                
                qDebug() << "Preparation du message:" << message;
                
                // Afficher directement avec QMessageBox pour forcer l'affichage
                QMessageBox *msgBox = new QMessageBox(this);
                msgBox->setIcon(QMessageBox::Warning);
                msgBox->setWindowTitle("Accès Refusé");
                msgBox->setText(message);
                msgBox->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                msgBox->setModal(true);
                
                qDebug() << "Ouverture du QMessageBox...";
                msgBox->show();
                msgBox->raise();
                msgBox->activateWindow();
                
                QApplication::processEvents();
                
                msgBox->exec();
                msgBox->deleteLater();
                
                qDebug() << "Message affiche et ferme";
            });
    
    connect(m_serialManager, &SerialPortManager::errorOccurred,
            this, [this](const QString &error) {
                QMessageBox::warning(this, "Erreur RFID", 
                    QString("Erreur de communication avec l'Arduino:\n%1\n\nL'accès sera désactivé par défaut.").arg(error));
                disableAccess();
            });
    
    // Tenter de se connecter à l'Arduino (essaiera automatiquement tous les ports)
    QStringList ports = m_serialManager->getAvailablePorts();
    qDebug() << "=== TEST CONNEXION ARDUINO RFID ===";
    qDebug() << "Ports série disponibles:" << ports;
    
    if (ports.isEmpty()) {
        qDebug() << "ERREUR: Aucun port série disponible pour l'Arduino";
        QMessageBox::information(this, "Arduino non détecté", 
            "Aucun port série n'a été détecté.\n\n"
            "Vérifiez que:\n"
            "1. L'Arduino est connecté via USB\n"
            "2. Les pilotes USB sont installés\n"
            "3. Le port COM est visible dans le Gestionnaire de périphériques");
        disableAccess();
    } else {
        // Essayer de se connecter (essaiera automatiquement tous les ports disponibles)
        qDebug() << "Tentative de connexion (essaiera tous les ports disponibles)...";
        qDebug() << "NOTE: Le système RFID ne dépend PAS de la base de données";
        qDebug() << "Les IDs des cartes sont lus directement depuis l'Arduino";
        
        if (m_serialManager->connectToArduino(QString(), 9600)) { // Port vide = essaie tous les ports
            qDebug() << "✓✓✓ SUCCÈS: Connecté à l'Arduino sur le port:" << m_serialManager->getCurrentPort();
            qDebug() << "En attente de scan de carte RFID...";
            qDebug() << "=== CONNEXION RFID ACTIVE ===";
            qDebug() << "Le signal readyRead devrait se declencher quand une carte est scannee";
            
            // Vérifier que la connexion est vraiment active
            if (m_serialManager->isConnected()) {
                qDebug() << "✓ Connexion verifiee: ACTIVE";
                
                // Afficher un message de confirmation de connexion
                QMessageBox::information(this, "Connexion RFID", 
                    QString("Connexion à l'Arduino établie sur le port %1.\n\n"
                           "Scannez votre carte RFID pour accéder à l'interface.\n\n"
                           "Carte Hiba Riahi (8B 8C ED 00) → Accès autorisé\n"
                           "Carte Omar Askri (03 D1 10 2D) → Accès refusé")
                    .arg(m_serialManager->getCurrentPort()));
            } else {
                qDebug() << "!!! ATTENTION: Connexion verifiee: INACTIVE !!!";
            }
        } else {
            qDebug() << "ERREUR: Impossible de se connecter à l'Arduino après avoir essayé tous les ports";
            QMessageBox::warning(this, "Erreur de Connexion", 
                "Impossible de se connecter à l'Arduino.\n\n"
                "Vérifiez que:\n"
                "1. L'Arduino est branché via USB\n"
                "2. Le code Arduino est téléversé\n"
                "3. Aucun autre programme n'utilise le port série\n"
                "4. Le Moniteur Série Arduino est fermé");
            disableAccess();
        }
    }
}

void gestionpieces::enableAccess()
{
    qDebug() << "=== enableAccess APPELE ===";
    m_accessGranted = true;
    
    // Activer tous les widgets de l'interface (boutons CRUD)
    ui->btnAjouter_2->setEnabled(true);
    ui->btnSupprimer_2->setEnabled(true);
    ui->btnNouveau_2->setEnabled(true);
    ui->btnChargerImage->setEnabled(true);
    ui->btnTrierPrix->setEnabled(true);
    ui->btnRechercher_2->setEnabled(true);
    ui->btnExportPDF_2->setEnabled(true);
    
    // Activer les champs de saisie
    ui->txtReference_2->setEnabled(true);
    ui->txtNom_2->setEnabled(true);
    ui->cmbCategorie_2->setEnabled(true);
    ui->spinQuantite_2->setEnabled(true);
    ui->spinPrix_2->setEnabled(true);
    ui->txtFournisseur_2->setEnabled(true);
    ui->dateEntree_2->setEnabled(true);
    ui->dateSortie_2->setEnabled(true);
    ui->tablePieces_2->setEnabled(true);
    
    // SUPPRIMER TOUS LES STYLES GRIS - Réinitialiser les styles à vide pour utiliser les styles par défaut
    // Cela permettra aux styles du constructeur (setStyleSheet global) de s'appliquer
    ui->btnAjouter_2->setStyleSheet("");
    ui->btnSupprimer_2->setStyleSheet("");
    ui->btnNouveau_2->setStyleSheet("");
    ui->btnChargerImage->setStyleSheet("");
    ui->btnTrierPrix->setStyleSheet("");
    ui->btnRechercher_2->setStyleSheet("");
    ui->btnExportPDF_2->setStyleSheet("");
    
    // Réinitialiser les styles des champs de saisie
    ui->txtReference_2->setStyleSheet("");
    ui->txtNom_2->setStyleSheet("");
    ui->cmbCategorie_2->setStyleSheet("");
    ui->spinQuantite_2->setStyleSheet("");
    ui->spinPrix_2->setStyleSheet("");
    ui->txtFournisseur_2->setStyleSheet("");
    ui->dateEntree_2->setStyleSheet("");
    ui->dateSortie_2->setStyleSheet("");
    ui->tablePieces_2->setStyleSheet("");
    
    // Réappliquer le style global pour restaurer l'apparence normale
    // Les widgets hériteront du style du parent (gestionpieces)
    this->setStyleSheet(
        "QWidget#gestionpieces { background-color: #E0F6FF; }"
        "QLabel { color: #0D47A1; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit {"
        "  color: #0D47A1;"
        "  background-color: white;"
        "}"
        "QTableWidget {"
        "  color: #0D47A1;"
        "  gridline-color: #90CAF9;"
        "}"
    );
    
    // Forcer la mise à jour visuelle de tous les widgets
    ui->btnAjouter_2->update();
    ui->btnSupprimer_2->update();
    ui->btnNouveau_2->update();
    ui->btnChargerImage->update();
    ui->btnTrierPrix->update();
    ui->btnRechercher_2->update();
    ui->btnExportPDF_2->update();
    
    ui->txtReference_2->update();
    ui->txtNom_2->update();
    ui->cmbCategorie_2->update();
    ui->spinQuantite_2->update();
    ui->spinPrix_2->update();
    ui->txtFournisseur_2->update();
    ui->dateEntree_2->update();
    ui->dateSortie_2->update();
    ui->tablePieces_2->update();
    
    // Forcer la mise à jour visuelle globale
    QApplication::processEvents();
    this->repaint();
    
    qDebug() << "Tous les widgets sont maintenant ACTIVES - Styles gris supprimes - Interface restauree a l'etat initial";
}

void gestionpieces::disableAccess()
{
    qDebug() << "=== disableAccess APPELE ===";
    m_accessGranted = false;
    
    // Désactiver tous les widgets de l'interface
    ui->btnAjouter_2->setEnabled(false);
    ui->btnSupprimer_2->setEnabled(false);
    ui->btnNouveau_2->setEnabled(false);
    ui->btnChargerImage->setEnabled(false);
    ui->btnTrierPrix->setEnabled(false);
    ui->btnRechercher_2->setEnabled(false);
    ui->btnExportPDF_2->setEnabled(false);
    
    // Désactiver les champs de saisie
    ui->txtReference_2->setEnabled(false);
    ui->txtNom_2->setEnabled(false);
    ui->cmbCategorie_2->setEnabled(false);
    ui->spinQuantite_2->setEnabled(false);
    ui->spinPrix_2->setEnabled(false);
    ui->txtFournisseur_2->setEnabled(false);
    ui->dateEntree_2->setEnabled(false);
    ui->dateSortie_2->setEnabled(false);
    ui->tablePieces_2->setEnabled(false);
    
    // Appliquer un style gris pour rendre visuellement l'interface désactivée
    QString disabledStyle = 
        "QPushButton { background-color: #dfe3e8; color: #8a8f99; border: 2px dashed #b5bcc5; border-radius: 15px; padding: 12px; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateEdit { background-color: #f0f0f0; color: #8a8f99; }"
        "QTableWidget { background-color: #f0f0f0; color: #8a8f99; }";
    
    // Appliquer le style aux widgets principaux
    ui->btnAjouter_2->setStyleSheet(disabledStyle);
    ui->btnSupprimer_2->setStyleSheet(disabledStyle);
    ui->btnNouveau_2->setStyleSheet(disabledStyle);
    
    // Forcer la mise à jour visuelle
    QApplication::processEvents();
    
    qDebug() << "Tous les widgets sont maintenant DESACTIVES (grises)";
}

void gestionpieces::showAccessMessage(const QString &message, bool isGranted)
{
    qDebug() << "=== showAccessMessage APPELE ===";
    qDebug() << "Message:" << message;
    qDebug() << "isGranted:" << isGranted;
    
    // Forcer le traitement des événements avant d'afficher
    QApplication::processEvents();
    
    // Créer le message box
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(message);
    msgBox->setIcon(isGranted ? QMessageBox::Information : QMessageBox::Warning);
    msgBox->setWindowTitle(isGranted ? "Accès Autorisé" : "Accès Refusé");
    
    // Forcer la fenêtre au premier plan et visible
    msgBox->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    msgBox->setModal(true);
    
    // S'assurer que la fenêtre est visible
    msgBox->show();
    msgBox->raise();
    msgBox->activateWindow();
    
    qDebug() << "Ouverture du QMessageBox...";
    
    // Forcer l'affichage
    QApplication::processEvents();
    
    // Afficher le message
    msgBox->exec();
    
    // Nettoyer
    msgBox->deleteLater();
    
    qDebug() << "QMessageBox ferme";
}


