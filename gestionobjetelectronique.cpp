#include "gestionobjetelectronique.h"
#include "ui_gestionobjetelectronique.h"
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include "client.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QScrollBar>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QAbstractScrollArea>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QMap>
#include <QGroupBox>

class PieChartWidget : public QWidget
{
public:
    PieChartWidget(const QMap<QString, int>& data, const QString& title, int total, QWidget* parent = nullptr)
        : QWidget(parent), m_data(data), m_title(title), m_total(total)
    {
        setMinimumSize(400, 350);
        setStyleSheet("background-color: white; border: 1px solid #87CEEB; border-radius: 5px;");
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event)
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        QRect rect = this->rect();
        int margin = 20;
        int titleHeight = 30;
        int legendWidth = 150;
        int chartSize = qMin(rect.width() - legendWidth - 3 * margin, rect.height() - titleHeight - 2 * margin);
        
        QRect chartRect(margin, margin + titleHeight, chartSize, chartSize);
        QRect legendRect(rect.width() - legendWidth - margin, margin + titleHeight, legendWidth, chartSize);
        
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        painter.drawText(QRect(margin, margin, rect.width() - 2 * margin, titleHeight), Qt::AlignCenter, m_title);
        
        if (m_total == 0) {
            painter.drawText(chartRect, Qt::AlignCenter, "Aucune donnée");
            return;
        }
        
        QList<QColor> colors = {
            QColor("#87CEEB"), QColor("#4682B4"), QColor("#5F9EA0"), 
            QColor("#20B2AA"), QColor("#00CED1"), QColor("#1E90FF"),
            QColor("#6495ED"), QColor("#7B68EE"), QColor("#9370DB"), QColor("#BA55D3")
        };
        
        double startAngle = 0;
        int colorIndex = 0;
        int yPos = legendRect.top();
        
        QList<QString> keys = m_data.keys();
        std::sort(keys.begin(), keys.end());
        
        for (const QString& key : keys) {
            int value = m_data[key];
            double angle = (value * 360.0) / m_total;
            double percentage = (value * 100.0) / m_total;
            
            QColor color = colors[colorIndex % colors.size()];
            painter.setBrush(color);
            painter.setPen(QPen(Qt::black, 2));
            
            painter.drawPie(chartRect, startAngle * 16, angle * 16);
            
            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 9));
            QString legendText = QString("%1: %2 (%3%)").arg(key).arg(value).arg(percentage, 0, 'f', 1);
            QRect legendItemRect(legendRect.left(), yPos, legendRect.width() - 20, 20);
            painter.fillRect(legendItemRect.adjusted(0, 2, -legendRect.width() + 15, -2), color);
            painter.drawText(legendItemRect.adjusted(20, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, legendText);
            
            startAngle += angle;
            colorIndex++;
            yPos += 25;
        }
    }

private:
    QMap<QString, int> m_data;
    QString m_title;
    int m_total;
};

static QWidget* createPieChart(const QMap<QString, int>& data, const QString& title, int total)
{
    return new PieChartWidget(data, title, total);
}

static int getEtatOrder(const QString& etat)
{
    QString etatLower = etat.toLower().trimmed();
    if (etatLower == "en panne") return 1;
    if (etatLower == "en réparation") return 2;
    if (etatLower == "réparé") return 3;
    return 4;
}

static int getTypeOrder(const QString& type)
{
    QString typeLower = type.toLower().trimmed();
    if (typeLower.contains("téléphone") || typeLower.contains("telephone") || typeLower.contains("smartphone")) return 1;
    if (typeLower.contains("pc") || typeLower.contains("ordinateur") || typeLower.contains("laptop")) return 2;
    if (typeLower.contains("imprimante") || typeLower.contains("printer")) return 3;
    return 4;
}

static int getMarqueOrder(const QString& marque)
{
    QString marqueLower = marque.toLower().trimmed();
    if (marqueLower.contains("apple")) return 1;
    if (marqueLower.contains("samsung")) return 2;
    if (marqueLower.contains("hp")) return 3;
    if (marqueLower.contains("asus")) return 4;
    if (marqueLower.contains("lenovo")) return 5;
    return 6;
}

gestionobjetelectronique::gestionobjetelectronique(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gestionobjetelectronique)
    , m_currentReference()
    , m_isTableEditMode(false)
    , m_callMeBot(new CallMeBot(this))
{
    ui->setupUi(this);
    
    ui->groupBoxPrix_6->setVisible(true);
    ui->groupBoxPrix_6->setEnabled(true);
    
    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.connect()) {
        afficherMessageErreur("Erreur", "Impossible de se connecter à la base de données: " + db.getLastError());
    }

    m_callMeBot->setCredentials("21654619393", "6479484");

    qDebug() << "Connexion du signal messageSent...";
    bool connected = connect(m_callMeBot, &CallMeBot::messageSent, this, [this](bool success, const QString& errorMessage) {
        qDebug() << "=== SIGNAL messageSent REÇU ===";
        qDebug() << "Success:" << success;
        qDebug() << "Error Message:" << errorMessage;
        
        if (success) {
            qDebug() << "WhatsApp envoyé avec succès!";
            afficherMessageSucces("WhatsApp envoyé", "Le message WhatsApp a été envoyé avec succès au client.");
        } else {
            qDebug() << "Échec de l'envoi WhatsApp:" << errorMessage;
            QString fullError = QString("Erreur lors de l'envoi du message WhatsApp:\n\n%1\n\nVérifiez:\n- Votre connexion Internet\n- Les identifiants CallMeBot\n- Le format du numéro de téléphone").arg(errorMessage);
            afficherMessageErreur("Erreur d'envoi WhatsApp", fullError);
        }
        qDebug() << "=== FIN SIGNAL messageSent ===";
    });
    
    if (!connected) {
        qDebug() << "ERREUR: Impossible de connecter le signal messageSent!";
    } else {
        qDebug() << "Signal messageSent connecté avec succès.";
    }

    configurerValidateurs();
    actualiserTableau();
    
    remplirComboClient();
    remplirComboObjets();
    remplirComboPiecesDetachees();
    
    connect(ui->comboBoxProbleme_6, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > 0) {
            int idPiece = ui->comboBoxProbleme_6->itemData(index).toInt();
            if (idPiece > 0) {
                DatabaseManager& db = DatabaseManager::getInstance();
                double prix = db.getPrixPieceDetachee(idPiece);
                ui->spinBoxPrix_6->setValue(static_cast<int>(prix));
            }
        }
    });
    
    connect(ui->comboBoxProbleme_6, &QComboBox::activated, this, [this](int index) {
        if (index == 0 && ui->comboBoxProbleme_6->count() == 1) {
            qDebug() << "ComboBox vide détecté, rafraîchissement des pièces détachées...";
            remplirComboPiecesDetachees();
        }
    });

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setVisible(true);
    
    // Définir une hauteur fixe petite pour forcer le scrollbar (120px pour voir environ 3-4 lignes)
    ui->tableWidget->setFixedHeight(120);
    
    // Configuration du scrollbar - AsNeeded pour qu'il apparaisse automatiquement
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Mode de défilement - important pour que le scroll fonctionne
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    
    // S'assurer que le widget peut défiler et que le scrollbar est activé
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // S'assurer que le scrollbar est activé après le chargement
    QTimer::singleShot(300, this, [this]() {
        QScrollBar* vScrollBar = ui->tableWidget->verticalScrollBar();
        if (vScrollBar) {
            vScrollBar->setEnabled(true);
            // Ne pas manipuler min/max, laisser Qt gérer
        }
    });
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setFocusPolicy(Qt::StrongFocus);
    ui->tableWidget->setTabKeyNavigation(true);
    ui->tableWidget->setAutoScroll(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(80);
    ui->tableWidget->verticalHeader()->setVisible(true);

    QString scrollbarStyle = "QScrollBar:vertical {"
                            "background: #E0E0E0;"
                            "width: 20px;"
                            "border: 1px solid #B0B0B0;"
                            "border-radius: 10px;"
                            "}"
                            "QScrollBar::handle:vertical {"
                            "background: #A0A0A0;"
                            "min-height: 30px;"
                            "border-radius: 8px;"
                            "margin: 2px;"
                            "}"
                            "QScrollBar::handle:vertical:hover {"
                            "background: #808080;"
                            "}"
                            "QScrollBar::handle:vertical:pressed {"
                            "background: #606060;"
                            "}"
                            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                            "height: 15px;"
                            "background: #D0D0D0;"
                            "border-radius: 5px;"
                            "}"
                            "QScrollBar::add-line:vertical:hover, QScrollBar::sub-line:vertical:hover {"
                            "background: #B0B0B0;"
                            "}"
                            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                            "background: #E8E8E8;"
                            "}";
    ui->tableWidget->setStyleSheet(ui->tableWidget->styleSheet() + scrollbarStyle);

    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &gestionobjetelectronique::on_tableWidget_itemSelectionChanged);
    // Note: on_btnCalculerPrix_6_clicked et on_btnExportPDF_6_clicked sont auto-connectés par Qt
    // grâce à la convention de nommage on_<objectName>_<signalName>()
    
    remplirComboClient();
    remplirComboObjets();
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

    std::sort(objets.begin(), objets.end(),
              [](const ObjetElectronique& a, const ObjetElectronique& b) {
                  int etatA = getEtatOrder(a.getEtat());
                  int etatB = getEtatOrder(b.getEtat());
                  if (etatA != etatB) {
                      return etatA < etatB;
                  }
                  
                  int typeA = getTypeOrder(a.getType());
                  int typeB = getTypeOrder(b.getType());
                  if (typeA != typeB) {
                      return typeA < typeB;
                  }
                  
                  int marqueA = getMarqueOrder(a.getMarque());
                  int marqueB = getMarqueOrder(b.getMarque());
                  if (marqueA != marqueB) {
                      return marqueA < marqueB;
                  }
                  
                  return a.getReference() < b.getReference();
              });

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

    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(80);
    ui->tableWidget->setVisible(true);
    
    // Le scrollbar sera géré automatiquement par Qt
    
    if (objets.size() > 0) {
        ui->tableWidget->setFocus();
        ui->tableWidget->selectRow(0);
        ui->tableWidget->setCurrentCell(0, 0);
        ui->tableWidget->scrollToTop();
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
    ui->comboBoxProbleme_6->setCurrentIndex(0);
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
    
    DatabaseManager& db = DatabaseManager::getInstance();
    int clientId = db.getClientIdByObjetReference(objet.getReference());
    if (clientId > 0) {
        for (int i = 0; i < ui->comboBoxType_7->count(); ++i) {
            if (ui->comboBoxType_7->itemData(i).toInt() == clientId) {
                ui->comboBoxType_7->setCurrentIndex(i);
                break;
            }
        }
    } else {
        ui->comboBoxType_7->setCurrentIndex(0);
    }
    
    int idPiece = db.getIdPieceByObjetReference(objet.getReference());
    if (idPiece > 0) {
        for (int i = 0; i < ui->comboBoxProbleme_6->count(); ++i) {
            if (ui->comboBoxProbleme_6->itemData(i).toInt() == idPiece) {
                ui->comboBoxProbleme_6->setCurrentIndex(i);
                break;
            }
        }
    } else {
        ui->comboBoxProbleme_6->setCurrentIndex(0);
    }
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

    int idClient = ui->comboBoxType_7->currentData().toInt();
    if (idClient <= 0) {
        idClient = -1;
    }
    
    int idPiece = ui->comboBoxProbleme_6->currentData().toInt();
    if (idPiece <= 0) {
        idPiece = -1;
    }
    
    qDebug() << "Ajout objet - Référence:" << objet.getReference() << "ID Client:" << idClient << "ID Piece:" << idPiece;

    if (!db.insertObjet(objet, idClient, idPiece)) {
        afficherMessageErreur("Erreur", "Erreur lors de l'ajout de l'objet: " + db.getLastError());
        return;
    }
    
    qDebug() << "Objet ajouté avec succès. Vérification ID_CLIENT_COURANT...";
    int verifyIdClient = db.getClientIdByObjetReference(objet.getReference());
    qDebug() << "ID_CLIENT_COURANT vérifié après insertion:" << verifyIdClient;

    actualiserTableau();
    viderFormulaire();
    remplirComboClient();
    remplirComboObjets();
    remplirComboPiecesDetachees();
    
    afficherMessageSucces("Succès", "Objet ajouté avec succès.");
}

void gestionobjetelectronique::on_btnModifier_6_clicked()
{
    if (m_currentReference.isEmpty()) {
        afficherMessageErreur("Attention", "Veuillez sélectionner un objet à modifier.");
        return;
    }

    ObjetElectronique objet;
    if (!lireFormulaire(objet)) {
        return;
    }

    DatabaseManager& db = DatabaseManager::getInstance();
    int idClient = ui->comboBoxType_7->currentData().toInt();
    if (idClient <= 0) {
        idClient = -1; // NULL
    }

    int idPiece = ui->comboBoxProbleme_6->currentData().toInt();
    if (idPiece <= 0) {
        idPiece = -1;
    }
    
    if (!db.updateObjet(objet, idClient, idPiece)) {
        afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
        return;
    }

    actualiserTableau();
    viderFormulaire();
    remplirComboClient();
    remplirComboObjets();
    remplirComboPiecesDetachees();
    
    afficherMessageSucces("Succès", "Objet modifié avec succès.");
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
        remplirComboObjets();
        afficherMessageSucces("Succès", "Objet supprimé avec succès.");
    }
}

void gestionobjetelectronique::on_btnRechercher_6_clicked()
{
    QString type = ui->comboBoxRechercheType_6->currentText();
    QString marque = ui->lineEditRechercheMarque_6->text().trimmed();
    QString etat = ui->comboBoxRechercheEtat_6->currentText();
    QString modele = ui->lineEdit->text().trimmed();

    DatabaseManager& db = DatabaseManager::getInstance();
    QList<ObjetElectronique> allObjets = db.getAllObjets();

    if (allObjets.isEmpty()) {
        ui->tableWidget->setRowCount(0);
        afficherMessageErreur("Information", "Aucun objet électronique trouvé dans la base de données.");
        return;
    }

    QList<ObjetElectronique> filteredObjets;
    for (const ObjetElectronique& objet : allObjets) {
        bool matches = true;

        if (!type.isEmpty() && type != "Tous" && objet.getType() != type) {
            matches = false;
        }
        if (!marque.isEmpty() && !objet.getMarque().contains(marque, Qt::CaseInsensitive)) {
            matches = false;
        }
        if (!etat.isEmpty() && etat != "Tous" && objet.getEtat().compare(etat, Qt::CaseInsensitive) != 0) {
            matches = false;
        }
        if (!modele.isEmpty() && !objet.getModele().contains(modele, Qt::CaseInsensitive)) {
            matches = false;
        }

        if (matches) {
            filteredObjets.append(objet);
        }
    }

    if (filteredObjets.isEmpty()) {
        ui->tableWidget->setRowCount(0);
        afficherMessageErreur("Information", "Aucun objet ne correspond aux critères de recherche.");
        return;
    }

    std::sort(filteredObjets.begin(), filteredObjets.end(),
              [](const ObjetElectronique& a, const ObjetElectronique& b) {
                  int etatA = getEtatOrder(a.getEtat());
                  int etatB = getEtatOrder(b.getEtat());
                  if (etatA != etatB) {
                      return etatA < etatB;
                  }
                  
                  int typeA = getTypeOrder(a.getType());
                  int typeB = getTypeOrder(b.getType());
                  if (typeA != typeB) {
                      return typeA < typeB;
                  }
                  
                  int marqueA = getMarqueOrder(a.getMarque());
                  int marqueB = getMarqueOrder(b.getMarque());
                  if (marqueA != marqueB) {
                      return marqueA < marqueB;
                  }
                  
                  return a.getReference() < b.getReference();
              });

    ui->tableWidget->clearContents();
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

    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(80);
    ui->tableWidget->setVisible(true);
    
    // Le scrollbar sera géré automatiquement par Qt
    
    if (filteredObjets.size() > 0) {
        ui->tableWidget->setFocus();
        ui->tableWidget->selectRow(0);
        ui->tableWidget->setCurrentCell(0, 0);
        ui->tableWidget->scrollToTop();
    }
    
    ui->tableWidget->update();
    ui->tableWidget->show();
}

void gestionobjetelectronique::on_btnReinitialiser_6_clicked()
{
    ui->comboBoxRechercheType_6->setCurrentIndex(0);
    ui->lineEditRechercheMarque_6->clear();
    ui->comboBoxRechercheEtat_6->setCurrentIndex(0);
    ui->lineEdit->clear();
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

int gestionobjetelectronique::calculerPrixEstime(const QString& probleme)
{
    if (probleme.isEmpty() || probleme == "-- Sélectionner --") {
        return 0;
    }
    
    int currentIndex = ui->comboBoxProbleme_6->currentIndex();
    if (currentIndex > 0) {
        int idPiece = ui->comboBoxProbleme_6->itemData(currentIndex).toInt();
        if (idPiece > 0) {
            DatabaseManager& db = DatabaseManager::getInstance();
            double prix = db.getPrixPieceDetachee(idPiece);
            return static_cast<int>(prix);
        }
    }
    
    return 0;
}

void gestionobjetelectronique::on_btnCalculerPrix_6_clicked()
{
    int currentIndex = ui->comboBoxProbleme_6->currentIndex();
    if (currentIndex <= 0) {
        afficherMessageErreur("Erreur", "Veuillez sélectionner une pièce détachée.");
        return;
    }
    
    int idPiece = ui->comboBoxProbleme_6->itemData(currentIndex).toInt();
    if (idPiece <= 0) {
        afficherMessageErreur("Erreur", "Pièce détachée invalide.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::getInstance();
    double prix = db.getPrixPieceDetachee(idPiece);
    
    if (prix > 0) {
        ui->spinBoxPrix_6->setValue(static_cast<int>(prix));
        QString nomPiece = ui->comboBoxProbleme_6->currentText();
        afficherMessageSucces("Prix estimé", QString("Le prix pour '%1' est de %2 TND.")
                              .arg(nomPiece).arg(prix, 0, 'f', 2));
    } else {
        afficherMessageErreur("Erreur", "Impossible de récupérer le prix de la pièce détachée.");
    }
}

void gestionobjetelectronique::exporterPDF()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString exportDir = desktopPath + "/export pdf";
    
    QDir dir;
    if (!dir.exists(exportDir)) {
        dir.mkpath(exportDir);
    }
    
    QString fileName = exportDir + "/Objets_Electroniques_" + 
                       QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss") + ".pdf";
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<ObjetElectronique> objets = db.getAllObjets();
    
    if (objets.isEmpty()) {
        afficherMessageErreur("Erreur", "Aucun objet à exporter. Veuillez ajouter des objets avant d'exporter le PDF.");
        return;
    }
    
    std::sort(objets.begin(), objets.end(),
        [](const ObjetElectronique& a, const ObjetElectronique& b) {
            int etatA = getEtatOrder(a.getEtat());
            int etatB = getEtatOrder(b.getEtat());
            if (etatA != etatB) {
                return etatA < etatB;
            }
            
            int typeA = getTypeOrder(a.getType());
            int typeB = getTypeOrder(b.getType());
            if (typeA != typeB) {
                return typeA < typeB;
            }
            
            int marqueA = getMarqueOrder(a.getMarque());
            int marqueB = getMarqueOrder(b.getMarque());
            if (marqueA != marqueB) {
                return marqueA < marqueB;
            }
            
            return a.getReference() < b.getReference();
        });
    
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);
    
    QTextDocument document;
    QString html = "<!DOCTYPE HTML>";
    html += "<html><head><meta charset='UTF-8'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; }";
    html += "h1 { text-align: center; color: #2C5F75; margin-bottom: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; font-size: 9px; }";
    html += "th { background-color: #87CEEB; color: #1A3A4A; font-weight: bold; border: 1px solid #000; padding: 8px; text-align: left; }";
    html += "td { border: 1px solid #000; padding: 6px; }";
    html += "tr:nth-child(even) { background-color: #f2f2f2; }";
    html += ".total { margin-top: 20px; font-weight: bold; text-align: right; }";
    html += "</style>";
    html += "</head><body>";
    html += "<h1>GESTION DES OBJETS ÉLECTRONIQUES</h1>";
    html += "<p style='text-align: right; margin-bottom: 20px;'>Date: " + 
            QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss") + "</p>";
    html += "<table>";
    html += "<tr>";
    html += "<th>Référence</th>";
    html += "<th>Nom Objet</th>";
    html += "<th>Marque</th>";
    html += "<th>Modèle</th>";
    html += "<th>Couleur</th>";
    html += "<th>N° Série</th>";
    html += "<th>Type</th>";
    html += "<th>État</th>";
    html += "<th>Technicien</th>";
    html += "<th style='text-align: right;'>Prix (TND)</th>";
    html += "</tr>";
    
    for (const ObjetElectronique& objet : objets) {
        html += "<tr>";
        html += "<td>" + objet.getReference().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getNom().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getMarque().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getModele().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getCouleur().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getNumeroSerie().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getType().toHtmlEscaped() + "</td>";
        html += "<td>" + objet.getEtat().toHtmlEscaped() + "</td>";
        html += "<td>" + (objet.getTechnicien().isEmpty() ? QString("-") : objet.getTechnicien().toHtmlEscaped()) + "</td>";
        html += "<td style='text-align: right;'>" + QString::number(objet.getPrix()) + "</td>";
        html += "</tr>";
    }
    
    html += "</table>";
    html += "<p class='total'>Total: " + QString::number(objets.size()) + " objet(s)</p>";
    html += "</body></html>";
    
    document.setHtml(html);
    document.setPageSize(printer.pageRect(QPrinter::Point).size());
    document.print(&printer);
    
    afficherMessageSucces("Succès", "Le fichier PDF a été exporté avec succès dans le dossier 'export pdf' sur le bureau:\n" + fileName);
}

void gestionobjetelectronique::on_btnExportPDF_6_clicked()
{
    exporterPDF();
}

void gestionobjetelectronique::remplirComboClient()
{
    ui->comboBoxType_7->clear();
    ui->comboBoxType_7->addItem("-- Sélectionner --", -1);
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<Client> clients = db.getAllClients();
    
    for (const Client& client : clients) {
        QString displayText = QString("%1 %2 (%3)").arg(client.getNom()).arg(client.getPrenom()).arg(client.getCin());
        int clientId = db.getClientIdByCin(client.getCin());
        ui->comboBoxType_7->addItem(displayText, clientId);
    }
}

void gestionobjetelectronique::remplirComboObjets()
{
    ui->comboBoxType_9->clear();
    ui->comboBoxType_9->addItem("-- Sélectionner Objet Electronique --", QString());
    
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<ObjetElectronique> objets = db.getAllObjets();
    
    for (const ObjetElectronique& objet : objets) {
        QString displayText = QString("%1 - %2 (%3)").arg(objet.getReference())
                                                      .arg(objet.getNom())
                                                      .arg(objet.getMarque());
        ui->comboBoxType_9->addItem(displayText, objet.getReference());
    }
}

void gestionobjetelectronique::remplirComboPiecesDetachees()
{
    ui->comboBoxProbleme_6->clear();
    ui->comboBoxProbleme_6->addItem("-- Sélectionner --", -1);
    
    DatabaseManager& db = DatabaseManager::getInstance();
    
    if (!db.isConnected()) {
        qDebug() << "remplirComboPiecesDetachees: Base de données non connectée, tentative de connexion...";
        if (!db.connect()) {
            qDebug() << "remplirComboPiecesDetachees: Impossible de se connecter à la base de données";
            QMessageBox::warning(this, "Erreur", "Impossible de se connecter à la base de données pour charger les pièces détachées.");
            return;
        }
    }
    
    QList<DatabaseManager::PieceDetachee> pieces = db.getAllPiecesDetachees();
    
    qDebug() << "remplirComboPiecesDetachees: Nombre de pièces trouvées:" << pieces.size();
    
    if (pieces.isEmpty()) {
        qDebug() << "remplirComboPiecesDetachees: Aucune pièce détachée trouvée!";
        QString errorMsg = db.getLastError();
        if (!errorMsg.isEmpty()) {
            qDebug() << "remplirComboPiecesDetachees: Erreur de la base de données:" << errorMsg;
        }
    }
    
    for (const DatabaseManager::PieceDetachee& piece : pieces) {
        if (piece.id > 0 && !piece.nom.isEmpty()) {
            QString displayText = QString("%1 (%2 TND)").arg(piece.nom).arg(piece.prix, 0, 'f', 2);
            ui->comboBoxProbleme_6->addItem(displayText, piece.id);
            qDebug() << "remplirComboPiecesDetachees: Ajout de" << displayText << "avec ID" << piece.id;
        }
    }
    
    qDebug() << "remplirComboPiecesDetachees: ComboBox rempli avec" << ui->comboBoxProbleme_6->count() << "éléments";
    
    if (ui->comboBoxProbleme_6->count() == 1) {
        qDebug() << "remplirComboPiecesDetachees: ATTENTION - Seul l'élément '-- Sélectionner --' est présent!";
    }
}

void gestionobjetelectronique::on_btnGenererSMS_6_clicked()
{
    QString reference = ui->comboBoxType_9->currentData().toString();
    
    if (reference.isEmpty()) {
        afficherMessageErreur("Erreur", "Veuillez sélectionner un objet électronique.");
        ui->textEditSMS_6->clear();
        return;
    }
    
    DatabaseManager& db = DatabaseManager::getInstance();
    ObjetElectronique objet = db.getObjetByReference(reference);
    
    if (objet.getReference().isEmpty()) {
        afficherMessageErreur("Erreur", "Objet introuvable dans la base de données.");
        ui->textEditSMS_6->clear();
        return;
    }
    
    QString message = genererWhatsApp(objet);
    ui->textEditSMS_6->setPlainText(message);
}

void gestionobjetelectronique::on_btnEnvoyerSMS_6_clicked()
{
    QString reference = ui->comboBoxType_9->currentData().toString();
    
    if (reference.isEmpty()) {
        afficherMessageErreur("Erreur", "Veuillez sélectionner un objet électronique.");
        return;
    }
    
    QString messageWhatsApp = ui->textEditSMS_6->toPlainText();
    if (messageWhatsApp.isEmpty()) {
        afficherMessageErreur("Erreur", "Veuillez d'abord générer le message WhatsApp.");
        return;
    }
    
    messageWhatsApp = messageWhatsApp.trimmed();
    if (messageWhatsApp.isEmpty()) {
        afficherMessageErreur("Erreur", "Le message WhatsApp est vide.");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::getInstance();
    ObjetElectronique objet = db.getObjetByReference(reference);
    
    if (objet.getReference().isEmpty()) {
        afficherMessageErreur("Erreur", "Objet introuvable dans la base de données.");
        return;
    }
    
    int idClient = db.getClientIdByObjetReference(reference);
    qDebug() << "ID Client récupéré pour référence" << reference << ":" << idClient;
    
    if (idClient <= 0) {
        QString errorMsg = QString("Aucun client associé à cet objet électronique (Réf: %1).\n\nVérifiez que vous avez bien sélectionné un client lors de l'ajout de l'objet.").arg(reference);
        afficherMessageErreur("Erreur", errorMsg);
        return;
    }
    
    Client client = db.getClientById(idClient);
    if (client.getTelephone().isEmpty()) {
        afficherMessageErreur("Erreur", "Le client n'a pas de numéro de téléphone enregistré.");
        return;
    }
    
    QString phoneNumber = client.getTelephone().trimmed();
    if (phoneNumber.isEmpty()) {
        afficherMessageErreur("Erreur", "Le client n'a pas de numéro de téléphone enregistré.");
        return;
    }
    
    if (!phoneNumber.startsWith("+")) {
        if (phoneNumber.startsWith("0")) {
            phoneNumber = "+216" + phoneNumber.mid(1);
        } else {
            phoneNumber = "+216" + phoneNumber;
        }
    }
    
    qDebug() << "=== DÉBUT ENVOI WHATSAPP ===";
    qDebug() << "Numéro de téléphone:" << phoneNumber;
    qDebug() << "Longueur du message:" << messageWhatsApp.length();
    qDebug() << "Message (premiers 100 caractères):" << messageWhatsApp.left(100);
    
    if (m_callMeBot == nullptr) {
        qDebug() << "ERREUR: m_callMeBot est null!";
        afficherMessageErreur("Erreur", "Le service WhatsApp n'est pas initialisé.");
        return;
    }
    
    bool sendResult = m_callMeBot->sendWhatsApp(phoneNumber, messageWhatsApp);
    qDebug() << "Résultat de sendWhatsApp:" << sendResult;
    
    if (!sendResult) {
        qDebug() << "ERREUR: sendWhatsApp a retourné false";
        afficherMessageErreur("Erreur", "Impossible d'initier l'envoi du message WhatsApp. Vérifiez les logs de la console.");
    } else {
        qDebug() << "Envoi WhatsApp initié avec succès, attente de la réponse...";
        QMessageBox::information(this, "Envoi en cours", "L'envoi du message WhatsApp est en cours. Vous recevrez une notification lorsque l'envoi sera terminé.");
    }
    
    qDebug() << "=== FIN ENVOI WHATSAPP ===";
}

void gestionobjetelectronique::on_pushButton_2_clicked()
{
    afficherStatistiques();
}

void gestionobjetelectronique::afficherStatistiques()
{
    DatabaseManager& db = DatabaseManager::getInstance();
    QList<ObjetElectronique> objets = db.getAllObjets();
    
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques des Objets Électroniques");
    dialog->setMinimumSize(1200, 800);
    dialog->setStyleSheet("QDialog { background-color: #F5F5F5; }");
    
    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    
    QLabel* titreLabel = new QLabel("Statistiques des Objets Électroniques", dialog);
    titreLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2C5F75; padding: 10px;");
    titreLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titreLabel);
    
    QScrollArea* scrollArea = new QScrollArea(dialog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #F5F5F5; }");
    
    QWidget* scrollContent = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(scrollContent);
    
    int totalObjets = objets.size();
    
    if (totalObjets == 0) {
        QLabel* emptyLabel = new QLabel("Aucun objet électronique enregistré.", scrollContent);
        emptyLabel->setStyleSheet("font-size: 14px; color: #888; padding: 20px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
        
        scrollArea->setWidget(scrollContent);
        mainLayout->addWidget(scrollArea);
        
        QPushButton* closeBtn = new QPushButton("Fermer", dialog);
        closeBtn->setStyleSheet("QPushButton { background-color: #87CEEB; color: #1A3A4A; font-weight: bold; padding: 8px; border-radius: 5px; }");
        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
        mainLayout->addWidget(closeBtn);
        
        dialog->exec();
        delete dialog;
        return;
    }
    
    QMap<QString, int> statsParEtat;
    QMap<QString, int> statsParType;
    QMap<QString, int> statsParMarque;
    QMap<QString, int> statsParTechnicien;
    double prixTotal = 0.0;
    int objetsAvecPrix = 0;
    
    for (const ObjetElectronique& objet : objets) {
        QString etat = objet.getEtat();
        statsParEtat[etat]++;
        
        QString type = objet.getType();
        if (!type.isEmpty()) {
            statsParType[type]++;
        }
        
        QString marque = objet.getMarque();
        if (!marque.isEmpty()) {
            statsParMarque[marque]++;
        }
        
        QString technicien = objet.getTechnicien();
        if (!technicien.isEmpty()) {
            statsParTechnicien[technicien]++;
        }
        
        if (objet.getPrix() > 0) {
            prixTotal += objet.getPrix();
            objetsAvecPrix++;
        }
    }
    
    QGroupBox* overviewBox = new QGroupBox("📊 Vue d'ensemble", scrollContent);
    overviewBox->setStyleSheet("QGroupBox { font-weight: bold; color: #2C5F75; border: 2px solid #87CEEB; border-radius: 5px; margin-top: 10px; padding-top: 10px; }");
    QVBoxLayout* overviewLayout = new QVBoxLayout(overviewBox);
    QLabel* overviewLabel = new QLabel(QString("Nombre total d'objets : <b>%1</b><br>Prix total estimé : <b>%2 TND</b><br>Prix moyen : <b>%3 TND</b>")
                                       .arg(totalObjets)
                                       .arg(prixTotal, 0, 'f', 2)
                                       .arg(objetsAvecPrix > 0 ? prixTotal / objetsAvecPrix : 0.0, 0, 'f', 2), overviewBox);
    overviewLabel->setStyleSheet("font-size: 14px; padding: 10px;");
    overviewLayout->addWidget(overviewLabel);
    contentLayout->addWidget(overviewBox);
    
    auto addStatSection = [&](const QMap<QString, int>& data, const QString& title, const QString& icon) {
        if (data.isEmpty()) return;
        
        QGroupBox* sectionBox = new QGroupBox(QString("%1 %2").arg(icon).arg(title), scrollContent);
        sectionBox->setStyleSheet("QGroupBox { font-weight: bold; color: #2C5F75; border: 2px solid #87CEEB; border-radius: 5px; margin-top: 10px; padding-top: 10px; }");
        QHBoxLayout* sectionLayout = new QHBoxLayout(sectionBox);
        
        QTextEdit* tableText = new QTextEdit(sectionBox);
        tableText->setReadOnly(true);
        tableText->setMaximumWidth(400);
        tableText->setStyleSheet("QTextEdit { background-color: white; border: 1px solid #87CEEB; border-radius: 5px; padding: 10px; font-size: 12px; }");
        
        QString tableHtml = "<table style='width: 100%; border-collapse: collapse;'>";
        tableHtml += "<tr style='background-color: #87CEEB;'><th style='padding: 8px; text-align: left;'>" + 
                     (title.contains("Technicien") ? "Technicien" : title.split(" ").last()) + 
                     "</th><th style='padding: 8px; text-align: right;'>Nombre</th>";
        if (!title.contains("Technicien")) {
            tableHtml += "<th style='padding: 8px; text-align: right;'>Pourcentage</th>";
        }
        tableHtml += "</tr>";
        
        QList<QString> keys = data.keys();
        if (title.contains("État")) {
            std::sort(keys.begin(), keys.end(), [](const QString& a, const QString& b) {
                return getEtatOrder(a) < getEtatOrder(b);
            });
        } else {
            std::sort(keys.begin(), keys.end());
        }
        
        for (const QString& key : keys) {
            int count = data[key];
            tableHtml += QString("<tr><td style='padding: 8px;'>%1</td><td style='padding: 8px; text-align: right;'>%2</td>")
                        .arg(key).arg(count);
            if (!title.contains("Technicien")) {
                double pourcentage = (count * 100.0) / totalObjets;
                tableHtml += QString("<td style='padding: 8px; text-align: right;'>%1%</td>")
                            .arg(pourcentage, 0, 'f', 1);
            }
            tableHtml += "</tr>";
        }
        tableHtml += "</table>";
        tableText->setHtml(tableHtml);
        
        QWidget* chartView = createPieChart(data, title, totalObjets);
        
        sectionLayout->addWidget(tableText);
        sectionLayout->addWidget(chartView);
        contentLayout->addWidget(sectionBox);
    };
    
    addStatSection(statsParEtat, "Répartition par État", "📈");
    addStatSection(statsParType, "Répartition par Type", "🔧");
    addStatSection(statsParMarque, "Répartition par Marque", "🏷️");
    addStatSection(statsParTechnicien, "Répartition par Technicien", "👨‍🔧");
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
    
    QPushButton* closeBtn = new QPushButton("Fermer", dialog);
    closeBtn->setStyleSheet("QPushButton { background-color: #87CEEB; color: #1A3A4A; font-weight: bold; padding: 8px; border-radius: 5px; }");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    mainLayout->addWidget(closeBtn);
    
    dialog->exec();
    delete dialog;
}

QString gestionobjetelectronique::genererWhatsApp(const ObjetElectronique& objet)
{
    QString message;
    
    message += "Bonjour,\n\n";
    
    message += QString("Votre %1 %2 %3").arg(objet.getType())
                                         .arg(objet.getMarque())
                                         .arg(objet.getModele());
    
    if (!objet.getNom().isEmpty()) {
        message += QString(" (%1)").arg(objet.getNom());
    }
    
    message += QString(" (Réf: %1)").arg(objet.getReference());
    message += " est actuellement ";
    
    QString etat = objet.getEtat().toLower();
    if (etat == "en panne") {
        message += "en panne";
    } else if (etat == "en réparation") {
        message += "en cours de réparation";
    } else if (etat == "réparé") {
        message += "réparé et prêt à être récupéré";
    } else if (etat == "en attente de pièces") {
        message += "en attente de pièces de rechange";
    } else {
        message += objet.getEtat().toLower();
    }
    message += ".\n\n";
    
    if (!objet.getTechnicien().isEmpty()) {
        message += QString("Technicien responsable: %1\n").arg(objet.getTechnicien());
    }
    
    if (objet.getPrix() > 0) {
        message += QString("Prix estimé: %1 TND\n").arg(objet.getPrix());
    }
    
    message += "\n";
    if (etat == "réparé") {
        message += "Vous pouvez venir récupérer votre appareil à notre atelier.\n\n";
        message += "Merci de votre confiance !";
    } else if (etat == "en réparation") {
        message += "Nous vous tiendrons informé de l'avancement des réparations.\n\n";
        message += "Merci de votre patience.";
    } else if (etat == "en panne") {
        message += "Nous procéderons au diagnostic et vous contacterons prochainement.\n\n";
        message += "Merci de votre confiance.";
    } else if (etat == "en attente de pièces") {
        message += "Nous attendons la réception des pièces nécessaires.\n\n";
        message += "Nous vous contacterons dès leur arrivée.";
    } else {
        message += "Nous vous tiendrons informé de l'évolution de votre dossier.\n\n";
        message += "Merci de votre confiance.";
    }
    
    return message;
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
        if (!db.insertObjet(objet, -1)) {
            afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
            actualiserTableau();
            return;
        }
    } else {
        if (!db.updateObjet(objet, -1)) {
            afficherMessageErreur("Erreur", "Erreur lors de la modification: " + db.getLastError());
            actualiserTableau();
            return;
        }
    }

    refItem->setData(Qt::UserRole, refNew);
    remplirFormulaire(objet);
}
