#include "chatbotemploye.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QFrame>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include <QTextCursor>
#include "connection.h"
#include "database.h"
#include "client.h"
#include "objetelectronique.h"

// ==================== EmployeeResult ====================

int EmployeeResult::getAge() const {
    if (!dateNaissance.isValid()) return -1;
    QDate today = QDate::currentDate();
    int age = today.year() - dateNaissance.year();
    if (today.month() < dateNaissance.month() || 
        (today.month() == dateNaissance.month() && today.day() < dateNaissance.day())) {
        age--;
    }
    return age;
}

// ==================== ChatbotEmploye ====================

ChatbotEmploye::ChatbotEmploye(QWidget *parent)
    : QWidget(parent)
    , m_waitingForClarification(false)
{
    setupUI();
    applyStyles();
    
    // Message de bienvenue
    addMessage("👋 Bonjour ! Je suis votre assistant intelligent pour la gestion des employés, des clients et des objets électroniques.\n\n"
               "Je peux vous aider à :\n"
               "• Rechercher un employé par son nom et afficher ses informations (adresse, téléphone, email, statut, âge...)\n"
               "• Rechercher un client (par nom ou CIN) et consulter ses coordonnées\n"
               "• Rechercher un objet électronique (par nom, référence, marque, modèle...) et afficher ses détails (type, état, technicien, prix...)\n\n"
               "Essayez : \"Adresse du client Hiba\", \"Statut de Mohamed Omar\" ou \"Détails de l'objet iPhone 12\".", 
               false);
}

ChatbotEmploye::~ChatbotEmploye()
{
}

void ChatbotEmploye::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(14);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_inputVisible = true;
    
    // Carte principale
    QFrame *card = new QFrame(this);
    card->setObjectName("chatbotCard");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    card->setMinimumWidth(360);
    card->setStyleSheet(
        "QFrame#chatbotCard {"
        "background-color: #ffffff;"
        "border: 1px solid #d6e6f3;"
        "border-radius: 18px;"
        "}"
    );
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(12);
    cardLayout->setContentsMargins(16, 10, 16, 10);
    
    // Entête
    QLabel* titleLabel = new QLabel("💬 Assistant Intelligent - Recherche d'Employés", card);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: 700; color: #1f4f6d;");
    QLabel* subtitleLabel = new QLabel("Posez vos questions, je m'occupe du reste !", card);
    subtitleLabel->setStyleSheet("font-size: 13px; color: #4a6985;");
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    
    // Zone de chat
    m_chatDisplay = new QTextEdit(card);
    m_chatDisplay->setReadOnly(true);
    // Hauteur réduite pour laisser un espace bien visible pour la barre de choix et la saisie
    m_chatDisplay->setMinimumHeight(180);
    m_chatDisplay->setMaximumHeight(220);
    m_chatDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_chatDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_chatDisplay->setStyleSheet(
        "QTextEdit {"
        "background-color: #f7fbff;"
        "border: 2px solid #c9e0f2;"
        "border-radius: 14px;"
        "padding: 15px;"
        "font-size: 13px;"
        "color: #203347;"
        "line-height: 1.5;"
        "}"
        "QTextEdit QScrollBar:vertical {"
        "background-color: #e8f0f7;"
        "width: 12px;"
        "border-radius: 6px;"
        "}"
        "QTextEdit QScrollBar::handle:vertical {"
        "background-color: #8EC7E2;"
        "min-height: 30px;"
        "border-radius: 6px;"
        "}"
        "QTextEdit QScrollBar::handle:vertical:hover {"
        "background-color: #6BB3D4;"
        "}"
    );
    cardLayout->addWidget(m_chatDisplay, 1); // Facteur d'expansion pour prendre l'espace disponible
    
    // Zone de suggestions (cachée par défaut)
    m_suggestionsWidget = new QFrame(card);
    m_suggestionsWidget->setMinimumHeight(110);
    m_suggestionsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_suggestionsWidget->setStyleSheet(
        "QFrame {"
        "background-color: #f3f8fc;"
        "border: 1px dashed #aac9e2;"
        "border-radius: 12px;"
        "padding: 10px;"
        "}"
        "QPushButton {"
        "background-color: #e5f1fb;"
        "border: 1px solid #a7c8e5;"
        "border-radius: 9px;"
        "color: #1f4f6d;"
        "padding: 8px 10px;"
        "text-align: left;"
        "}"
        "QPushButton:hover { background-color: #d7e6f5; }"
    );
    m_suggestionsLayout = new QVBoxLayout(m_suggestionsWidget);
    m_suggestionsLayout->setSpacing(8);
    m_suggestionsLayout->setContentsMargins(6, 6, 6, 6);
    m_suggestionsWidget->hide();
    cardLayout->addWidget(m_suggestionsWidget);
    cardLayout->addSpacing(12);
    
    // Zone de saisie
    m_toggleInputButton = new QPushButton("❯", card);
    m_toggleInputButton->setFixedSize(32, 32);
    m_toggleInputButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1f84bd;"
        "color: white;"
        "border: none;"
        "border-radius: 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #166a95; }"
    );

    cardLayout->addWidget(m_toggleInputButton, 0, Qt::AlignRight);

    m_inputLayout = new QHBoxLayout();
    m_inputLayout->setSpacing(10);
    m_inputLayout->setContentsMargins(0, 4, 0, 0);

    m_inputField = new QLineEdit(card);
    m_inputField->setPlaceholderText("Posez votre question (ex : \"Adresse de Hiba Riahi\")");
    m_inputField->setMinimumWidth(210);
    m_inputField->setStyleSheet(
        "QLineEdit {"
        "padding: 11px;"
        "border: 1px solid #a7c8e5;"
        "border-radius: 10px;"
        "font-size: 13px;"
        "background-color: #ffffff;"
        "color: #203347;"
        "}"
        "QLineEdit:focus {"
        "border: 1px solid #5ea2d8;"
        "box-shadow: 0 0 0 2px rgba(94,162,216,0.18);"
        "}"
    );
    
    m_sendButton = new QPushButton("Envoyer", card);
    m_sendButton->setCursor(Qt::PointingHandCursor);
    m_sendButton->setMinimumWidth(110);
    m_sendButton->setMaximumWidth(130);
    m_sendButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1f84bd;"
        "color: #ffffff;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 10px 18px;"
        "font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #1a6f9f; }"
        "QPushButton:pressed { background-color: #155a82; }"
    );
    
    m_clearButton = new QPushButton("Effacer", card);
    m_clearButton->setCursor(Qt::PointingHandCursor);
    m_clearButton->setMinimumWidth(110);
    m_clearButton->setMaximumWidth(130);
    m_clearButton->setStyleSheet(
        "QPushButton {"
        "background-color: #d0d9e0;"
        "color: #405566;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 10px 18px;"
        "font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #c0cbd4; }"
        "QPushButton:pressed { background-color: #aab5be; }"
    );
    
    m_inputLayout->addWidget(m_inputField, 1);
    m_inputLayout->addWidget(m_sendButton, 0, Qt::AlignRight);
    m_inputLayout->addWidget(m_clearButton, 0, Qt::AlignRight);
    cardLayout->addLayout(m_inputLayout);
    
    m_mainLayout->addWidget(card);
    m_mainLayout->addStretch();
    
    // Connexions
    connect(m_sendButton, &QPushButton::clicked, this, &ChatbotEmploye::onSendButtonClicked);
    connect(m_clearButton, &QPushButton::clicked, this, &ChatbotEmploye::onClearButtonClicked);
    connect(m_inputField, &QLineEdit::returnPressed, this, &ChatbotEmploye::onSendButtonClicked);
    connect(m_toggleInputButton, &QPushButton::clicked, this, &ChatbotEmploye::onToggleInputClicked);
    updateInputVisibility();
}

void ChatbotEmploye::applyStyles()
{
    setStyleSheet("QWidget { background-color: transparent; }");
}

void ChatbotEmploye::updateInputVisibility()
{
    bool show = m_inputVisible;
    for (int i = 0; i < m_inputLayout->count(); ++i) {
        QWidget *widget = m_inputLayout->itemAt(i)->widget();
        if (widget) {
            widget->setVisible(show);
        }
    }
    if (show) {
        m_toggleInputButton->setText("❯");
        m_toggleInputButton->setToolTip("Masquer la saisie");
    } else {
        m_toggleInputButton->setText("❮");
        m_toggleInputButton->setToolTip("Afficher la saisie");
    }
}

void ChatbotEmploye::addMessage(const QString& message, bool isUser)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm");
    QString formattedMessage;
    
    if (isUser) {
        formattedMessage = QString(
            "<div style='margin: 8px 0; text-align: right;'>"
            "<div style='display: inline-block; background-color: #8EC7E2; color: black; "
            "padding: 10px 15px; border-radius: 12px; max-width: 80%; text-align: left;'>"
            "<b>Vous</b> (%1)<br>%2"
            "</div></div>"
        ).arg(timestamp, message.toHtmlEscaped());
    } else {
        formattedMessage = QString(
            "<div style='margin: 8px 0; text-align: left;'>"
            "<div style='display: inline-block; background-color: #e9ecef; color: #212529; "
            "padding: 10px 15px; border-radius: 12px; max-width: 80%;'>"
            "<b>🤖 Assistant</b> (%1)<br>%2"
            "</div></div>"
        ).arg(timestamp, message.toHtmlEscaped());
    }
    
    m_chatDisplay->append(formattedMessage);
    
    // Forcer le scroll vers le bas pour voir le dernier message
    QTextCursor cursor = m_chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_chatDisplay->setTextCursor(cursor);
    
    // Scroll vers le bas avec délai pour s'assurer que le contenu est rendu
    QScrollBar* scrollBar = m_chatDisplay->verticalScrollBar();
    QTimer::singleShot(50, [scrollBar]() {
        if (scrollBar) {
            scrollBar->setValue(scrollBar->maximum());
        }
    });
}

void ChatbotEmploye::onSendButtonClicked()
{
    QString query = m_inputField->text().trimmed();
    if (query.isEmpty()) return;
    
    addMessage(query, true);
    m_inputField->clear();
    
    // Si on attend une clarification
    if (m_waitingForClarification && !query.isEmpty()) {
        bool ok;
        int choice = query.toInt(&ok);
        if (ok && choice > 0 && choice <= m_pendingCandidates.size()) {
            ChatResult selected = m_pendingCandidates[choice - 1];
            m_waitingForClarification = false;
            m_pendingCandidates.clear();
            
            QString response;
            switch (selected.type) {
            case EntityType::EMPLOYEE:
                response = generateInfoResponse(selected.employee, m_currentQuery.intent);
                emit employeeSelected(selected.employee.id);
                break;
            case EntityType::CLIENT:
                response = generateClientInfoResponse(selected.client, m_currentQuery.intent);
                break;
            case EntityType::OBJECT:
                response = generateObjetInfoResponse(selected.objet, m_currentQuery.intent);
                break;
            }
            
            addMessage(response, false);
            return;
        }
    }
    
    processQuery(query);
}

void ChatbotEmploye::onClearButtonClicked()
{
    clearChat();
}

void ChatbotEmploye::onToggleInputClicked()
{
    m_inputVisible = !m_inputVisible;
    updateInputVisibility();
}

void ChatbotEmploye::clearChat()
{
    m_chatDisplay->clear();
    m_waitingForClarification = false;
    m_pendingCandidates.clear();
    hideSuggestions();
    
    addMessage("👋 Bonjour ! Je suis votre assistant intelligent pour la gestion des employés, des clients et des objets électroniques.\n\n"
               "Je peux vous aider à :\n"
               "• Rechercher un employé par son nom et afficher ses informations (adresse, téléphone, email, statut, âge...)\n"
               "• Rechercher un client (par nom ou CIN) et consulter ses coordonnées\n"
               "• Rechercher un objet électronique (par nom, référence, marque, modèle...) et afficher ses détails (type, état, technicien, prix...)\n\n"
               "Essayez : \"Adresse du client Hiba\", \"Statut de Mohamed Omar\" ou \"Détails de l'objet iPhone 12\".", 
               false);
}

void ChatbotEmploye::processQuery(const QString& query)
{
    qDebug() << "=== Traitement de la requête ===";
    qDebug() << "Requête originale:" << query;
    
    ParsedQuery parsed = parseQuery(query);
    m_currentQuery = parsed;
    
    qDebug() << "Terme extrait:" << parsed.searchTerm;
    qDebug() << "Intention:" << static_cast<int>(parsed.intent);
    qDebug() << "Nombre de candidats:" << parsed.candidates.size();
    
    QString response = generateResponse(parsed);
    addMessage(response, false);
}

QString ChatbotEmploye::processQueryText(const QString& query)
{
    qDebug() << "=== Traitement (texte seul) ===";
    qDebug() << "Requête originale:" << query;
    
    ParsedQuery parsed = parseQuery(query);
    m_currentQuery = parsed;
    
    qDebug() << "Terme extrait:" << parsed.searchTerm;
    qDebug() << "Intention:" << static_cast<int>(parsed.intent);
    qDebug() << "Nombre de candidats:" << parsed.candidates.size();
    
    return generateResponse(parsed);
}

ParsedQuery ChatbotEmploye::parseQuery(const QString& query)
{
    ParsedQuery result;
    result.originalQuery = query;
    result.intent = detectIntent(query);
    result.entityType = detectEntityType(query);
    result.searchTerm = extractNameFromQuery(query);
    result.isAmbiguous = false;
    
    if (!result.searchTerm.isEmpty()) {
        result.candidates = buildChatResults(result.entityType, result.searchTerm);
        
        if (result.candidates.isEmpty() && result.entityType == EntityType::EMPLOYEE) {
            auto clientCandidates = buildChatResults(EntityType::CLIENT, result.searchTerm);
            if (!clientCandidates.isEmpty()) {
                result.candidates = clientCandidates;
                result.entityType = EntityType::CLIENT;
            } else {
                auto objetCandidates = buildChatResults(EntityType::OBJECT, result.searchTerm);
                if (!objetCandidates.isEmpty()) {
                    result.candidates = objetCandidates;
                    result.entityType = EntityType::OBJECT;
                }
            }
        }
        
        result.isAmbiguous = (result.candidates.size() > 1);
    }
    
    return result;
}

IntentType ChatbotEmploye::detectIntent(const QString& query)
{
    QString lowerQuery = query.toLower();
    
    // Adresse
    if (lowerQuery.contains("adresse") || lowerQuery.contains("habite") || 
        lowerQuery.contains("vit") || lowerQuery.contains("demeure")) {
        return IntentType::GET_ADDRESS;
    }
    
    // Téléphone
    if (lowerQuery.contains("téléphone") || lowerQuery.contains("telephone") || 
        lowerQuery.contains("numéro") || lowerQuery.contains("numero") || 
        lowerQuery.contains("tel") || lowerQuery.contains("appeler")) {
        return IntentType::GET_PHONE;
    }
    
    // Email
    if (lowerQuery.contains("email") || lowerQuery.contains("e-mail") || 
        lowerQuery.contains("courriel") || lowerQuery.contains("mail")) {
        return IntentType::GET_EMAIL;
    }
    
    // Statut
    if (lowerQuery.contains("statut") || lowerQuery.contains("état") || 
        lowerQuery.contains("situation") || lowerQuery.contains("actif") || 
        lowerQuery.contains("congé") || lowerQuery.contains("conges")) {
        return IntentType::GET_STATUS;
    }
    
    // Âge
    if (lowerQuery.contains("âge") || lowerQuery.contains("age") || 
        lowerQuery.contains("vieill") || lowerQuery.contains("né en") || 
        lowerQuery.contains("ne en")) {
        return IntentType::GET_AGE;
    }
    
    // Synthèse
    if (lowerQuery.contains("synthèse") || lowerQuery.contains("synthese") || 
        lowerQuery.contains("résumé") || lowerQuery.contains("resume") || 
        lowerQuery.contains("dossier") || lowerQuery.contains("complet") || 
        lowerQuery.contains("tout") || lowerQuery.contains("toutes les informations")) {
        return IntentType::GET_SUMMARY;
    }
    
    // Recherche simple / filtre tableau
    if (lowerQuery.contains("recherche") || lowerQuery.contains("cherche") || 
        lowerQuery.contains("trouve") || lowerQuery.contains("affiche") || 
        lowerQuery.contains("montre")) {
        return IntentType::FILTER_TABLE;
    }
    
    // Par défaut : recherche d'information générale
    return IntentType::SEARCH_INFO;
}

EntityType ChatbotEmploye::detectEntityType(const QString& query) const
{
    QString lowerQuery = query.toLower();
    if (lowerQuery.contains("client") || lowerQuery.contains("cliente")) {
        return EntityType::CLIENT;
    }
    if (lowerQuery.contains("objet") || lowerQuery.contains("appareil") ||
        lowerQuery.contains("machine") || lowerQuery.contains("électronique") ||
        lowerQuery.contains("electronique")) {
        return EntityType::OBJECT;
    }
    return EntityType::EMPLOYEE;
}

QString ChatbotEmploye::extractNameFromQuery(const QString& query)
{
    QString lowerQuery = query.toLower();
    
    // Patterns spécifiques pour client/objet
    QRegularExpression customPatterns[] = {
        QRegularExpression(R"(client(?:e)?\s+(?:nomm[eé]?\s+)?([a-z0-9\s'\-]+))", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(R"(objet(?:\sélectronique)?\s+(?:nomm[eé]?\s+)?([a-z0-9\s'\-]+))", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(R"((?:réf(?:erence)?|reference)\s+([a-z0-9\-]+))", QRegularExpression::CaseInsensitiveOption)
    };
    
    for (const auto& pattern : customPatterns) {
        QRegularExpressionMatch match = pattern.match(query);
        if (match.hasMatch()) {
            QString captured = match.captured(1).trimmed();
            captured.replace(QRegularExpression("\\s+"), " ");
            if (!captured.isEmpty()) {
                return captured;
            }
        }
    }
    
    // Patterns génériques pour personnes
    QRegularExpression personPatterns[] = {
        QRegularExpression(R"((?:quelle|quel|qui|où|ou)\s+(?:est|sont|a|ont)\s+(?:l'|la|le|les)?\s*(?:adresse|téléphone|telephone|email|statut|âge|age)\s+(?:de|du|des|d')\s+([a-z]+(?:\s+[a-z]+)?))", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(R"((?:informations|info|détails|details|données|donnees)\s+(?:sur|de|du|des|pour)\s+([a-z]+(?:\s+[a-z]+)?))", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(R"((?:nom|prénom|prenom|appelle|s'appelle)\s+(?:est|d'|de|du|des|le|la|les)?\s*([a-z]+(?:\s+[a-z]+)?))", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression(R"(([A-Z][a-z]+\s+[A-Z][a-z]+))"),
        QRegularExpression(R"(([A-Z][a-z]+))")
    };
    
    for (const auto& pattern : personPatterns) {
        QRegularExpressionMatch match = pattern.match(query);
        if (match.hasMatch()) {
            QString name = match.captured(1).trimmed();
            if (!name.isEmpty() && name.length() > 2) {
                QStringList words = name.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                QStringList capitalizedWords;
                for (const QString& word : words) {
                    capitalizedWords.append(word[0].toUpper() + word.mid(1).toLower());
                }
                return capitalizedWords.join(" ");
            }
        }
    }
    
    // Fallback : extraire les mots significatifs
    QStringList words = query.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QStringList names;
    
    QStringList ignoreWords = {
        "quelle","quel","qui","où","ou","est","sont","a","ont","donner","donne","donnez","moi",
        "l'","la","le","les","de","du","des","d'","sur","pour","avec","montrer","montre",
        "adresse","téléphone","telephone","email","statut","âge","age","informations","info","détails","details","données","donnees",
        "client","clients","objet","objets","electronique","électronique","appareil","appareils","machine","machines",
        "marque","modele","modèle","reference","référence","statistique","synthese","résumé","resume"
    };
    
    for (const QString& word : words) {
        QString cleaned = word;
        cleaned.remove(QRegularExpression("^[^a-zA-Z0-9]+|[^a-zA-Z0-9]+$"));
        QString lowerWord = cleaned.toLower();
        if (cleaned.length() > 1 && !ignoreWords.contains(lowerWord)) {
            if (lowerWord.size() == cleaned.size()) {
                names.append(cleaned[0].toUpper() + cleaned.mid(1).toLower());
            } else {
                names.append(cleaned);
            }
        }
    }
    
    if (names.size() >= 2) {
        return names.join(" ");
    } else if (names.size() == 1) {
        return names.first();
    }
    
    return "";
}

QList<EmployeeResult> ChatbotEmploye::searchEmployees(const QString& name)
{
    QList<EmployeeResult> results;
    
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        qDebug() << "Base de données non connectée - tentative de reconnexion...";
        if (!conn.createConnection()) {
            qDebug() << "Échec de la reconnexion:" << conn.getDatabase().lastError().text();
            return results;
        }
    }
    
    QSqlQuery query(conn.getDatabase());
    
    // Diviser le nom en mots (peut être "hiba riahi" ou "Hiba Riahi")
    QStringList nameWords = name.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    
    if (nameWords.isEmpty()) {
        return results;
    }
    
    // Si un seul mot, chercher dans nom et prénom
    if (nameWords.size() == 1) {
        QString word = nameWords[0];
        QString searchPattern = "%" + word + "%";
        query.prepare(
            "SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut "
            "FROM EMPLOYE "
            "WHERE UPPER(nom) LIKE UPPER(:pattern) OR UPPER(prenom) LIKE UPPER(:pattern) "
            "ORDER BY nom, prenom"
        );
        query.bindValue(":pattern", searchPattern);
    } else {
        // Si deux mots, chercher comme nom complet (prénom nom ou nom prénom)
        QString word1 = nameWords[0];
        QString word2 = nameWords[1];
        query.prepare(
            "SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut "
            "FROM EMPLOYE "
            "WHERE (UPPER(prenom) LIKE UPPER(:word1) AND UPPER(nom) LIKE UPPER(:word2)) "
            "OR (UPPER(nom) LIKE UPPER(:word1) AND UPPER(prenom) LIKE UPPER(:word2)) "
            "OR (UPPER(prenom || ' ' || nom) LIKE UPPER(:pattern)) "
            "OR (UPPER(nom || ' ' || prenom) LIKE UPPER(:pattern)) "
            "ORDER BY nom, prenom"
        );
        query.bindValue(":word1", "%" + word1 + "%");
        query.bindValue(":word2", "%" + word2 + "%");
        query.bindValue(":pattern", "%" + name + "%");
    }
    
    qDebug() << "Recherche d'employé avec:" << name;
    
    if (!query.exec()) {
        qDebug() << "Erreur de recherche:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        return results;
    }
    
    while (query.next()) {
        EmployeeResult emp;
        emp.id = query.value(0).toInt();
        emp.nom = query.value(1).toString();
        emp.prenom = query.value(2).toString();
        emp.dateNaissance = query.value(3).toDate();
        emp.adresse = query.value(4).toString();
        emp.telephone = query.value(5).toString();
        emp.email = query.value(6).toString();
        emp.statut = query.value(7).toString();
        results.append(emp);
        qDebug() << "Employé trouvé:" << emp.getFullName() << "(ID:" << emp.id << ")";
    }
    
    qDebug() << "Nombre d'employés trouvés:" << results.size();
    
    return results;
}

QList<EmployeeResult> ChatbotEmploye::searchEmployeesFuzzy(const QString& name)
{
    QList<EmployeeResult> results;
    
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        return results;
    }
    
    QSqlQuery query(conn.getDatabase());
    
    // Recherche floue : chaque mot du nom recherché
    QStringList nameWords = name.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QStringList conditions;
    QVariantList bindValues;
    
    for (const QString& word : nameWords) {
        if (word.length() > 2) {
            conditions.append("(UPPER(nom) LIKE UPPER(:word) OR UPPER(prenom) LIKE UPPER(:word))");
            bindValues.append("%" + word + "%");
        }
    }
    
    if (conditions.isEmpty()) return results;
    
    QString sql = QString(
        "SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut "
        "FROM EMPLOYE "
        "WHERE %1 "
        "ORDER BY nom, prenom"
    ).arg(conditions.join(" OR "));
    
    query.prepare(sql);
    for (int i = 0; i < bindValues.size(); ++i) {
        query.bindValue(":word", bindValues[i]);
    }
    
    if (!query.exec()) {
        qDebug() << "Erreur de recherche floue:" << query.lastError().text();
        return results;
    }
    
    while (query.next()) {
        EmployeeResult emp;
        emp.id = query.value(0).toInt();
        emp.nom = query.value(1).toString();
        emp.prenom = query.value(2).toString();
        emp.dateNaissance = query.value(3).toDate();
        emp.adresse = query.value(4).toString();
        emp.telephone = query.value(5).toString();
        emp.email = query.value(6).toString();
        emp.statut = query.value(7).toString();
        results.append(emp);
    }
    
    return results;
}

QList<ClientResult> ChatbotEmploye::searchClients(const QString& nameOrCin)
{
    QList<ClientResult> results;
    QString term = nameOrCin.trimmed();
    if (term.isEmpty()) {
        return results;
    }

    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.isConnected() && !db.connect()) {
        qDebug() << "Impossible de se connecter pour rechercher des clients";
        return results;
    }

    QList<Client> clients = db.getAllClients();
    QString lowerTerm = term.toLower();

    for (const Client& client : clients) {
        QString fullName = (client.getPrenom() + " " + client.getNom()).toLower();
        QString invertedName = (client.getNom() + " " + client.getPrenom()).toLower();

        if (fullName.contains(lowerTerm) ||
            invertedName.contains(lowerTerm) ||
            client.getCin().toLower().contains(lowerTerm)) {
            ClientResult result;
            result.cin = client.getCin();
            result.nom = client.getNom();
            result.prenom = client.getPrenom();
            result.telephone = client.getTelephone();
            result.email = client.getEmail();
            result.adresse = client.getAdresse();
            result.dateNaissance = client.getDateNaissance();
            results.append(result);
        }
    }

    return results;
}

QList<ObjetResult> ChatbotEmploye::searchObjets(const QString& term)
{
    QList<ObjetResult> results;
    QString searchTerm = term.trimmed();
    if (searchTerm.isEmpty()) {
        return results;
    }

    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.isConnected() && !db.connect()) {
        qDebug() << "Impossible de se connecter pour rechercher des objets";
        return results;
    }

    QList<ObjetElectronique> objets = db.getAllObjets();
    QString lowerTerm = searchTerm.toLower();

    for (const ObjetElectronique& objet : objets) {
        QStringList fields = {
            objet.getReference(),
            objet.getNom(),
            objet.getMarque(),
            objet.getModele(),
            objet.getCouleur(),
            objet.getNumeroSerie(),
            objet.getType(),
            objet.getTechnicien()
        };

        bool match = false;
        for (const QString& field : fields) {
            if (field.toLower().contains(lowerTerm)) {
                match = true;
                break;
            }
        }

        if (match) {
            ObjetResult result;
            result.reference = objet.getReference();
            result.nom = objet.getNom();
            result.marque = objet.getMarque();
            result.modele = objet.getModele();
            result.couleur = objet.getCouleur();
            result.numeroSerie = objet.getNumeroSerie();
            result.type = objet.getType();
            result.etat = objet.getEtat();
            result.technicien = objet.getTechnicien();
            result.prix = objet.getPrix();
            results.append(result);
        }
    }

    return results;
}

QList<ChatResult> ChatbotEmploye::buildChatResults(EntityType type, const QString& term)
{
    QList<ChatResult> chatResults;
    switch (type) {
    case EntityType::EMPLOYEE: {
        QList<EmployeeResult> employees = searchEmployees(term);
        if (employees.isEmpty()) {
            employees = searchEmployeesFuzzy(term);
        }
        for (const EmployeeResult& emp : employees) {
            ChatResult result;
            result.type = EntityType::EMPLOYEE;
            result.employee = emp;
            chatResults.append(result);
        }
        break;
    }
    case EntityType::CLIENT: {
        QList<ClientResult> clients = searchClients(term);
        for (const ClientResult& client : clients) {
            ChatResult result;
            result.type = EntityType::CLIENT;
            result.client = client;
            chatResults.append(result);
        }
        break;
    }
    case EntityType::OBJECT: {
        QList<ObjetResult> objets = searchObjets(term);
        for (const ObjetResult& objet : objets) {
            ChatResult result;
            result.type = EntityType::OBJECT;
            result.objet = objet;
            chatResults.append(result);
        }
        break;
    }
    }
    return chatResults;
}

EmployeeResult ChatbotEmploye::getEmployeeById(int id)
{
    EmployeeResult emp;
    
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        return emp;
    }
    
    QSqlQuery query(conn.getDatabase());
    query.prepare(
        "SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut "
        "FROM EMPLOYE "
        "WHERE id_employe = :id"
    );
    query.bindValue(":id", id);
    
    if (query.exec() && query.next()) {
        emp.id = query.value(0).toInt();
        emp.nom = query.value(1).toString();
        emp.prenom = query.value(2).toString();
        emp.dateNaissance = query.value(3).toDate();
        emp.adresse = query.value(4).toString();
        emp.telephone = query.value(5).toString();
        emp.email = query.value(6).toString();
        emp.statut = query.value(7).toString();
    }
    
    return emp;
}

EmployeeResult ChatbotEmploye::getEmployeeByFullName(const QString& nom, const QString& prenom)
{
    EmployeeResult emp;
    
    Connection& conn = Connection::createInstance();
    if (!conn.getDatabase().isOpen()) {
        return emp;
    }
    
    QSqlQuery query(conn.getDatabase());
    query.prepare(
        "SELECT id_employe, nom, prenom, date_naissance, adresse, num_tel, email, statut "
        "FROM EMPLOYE "
        "WHERE UPPER(nom) = UPPER(:nom) AND UPPER(prenom) = UPPER(:prenom)"
    );
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    
    if (query.exec() && query.next()) {
        emp.id = query.value(0).toInt();
        emp.nom = query.value(1).toString();
        emp.prenom = query.value(2).toString();
        emp.dateNaissance = query.value(3).toDate();
        emp.adresse = query.value(4).toString();
        emp.telephone = query.value(5).toString();
        emp.email = query.value(6).toString();
        emp.statut = query.value(7).toString();
    }
    
    return emp;
}

QString ChatbotEmploye::generateResponse(const ParsedQuery& parsedQuery)
{
    qDebug() << "=== Génération de la réponse ===";
    qDebug() << "Terme recherché:" << parsedQuery.searchTerm;
    qDebug() << "Candidats trouvés:" << parsedQuery.candidates.size();
    
    if (parsedQuery.searchTerm.isEmpty()) {
        qDebug() << "Aucun terme exploitable";
        return "❌ Je n'ai pas pu identifier la personne ou l'objet mentionné dans votre question.\n\n"
               "💡 Exemples :\n"
               "• \"Adresse du client Hiba\"\n"
               "• \"Statut de Mohamed Omar\"\n"
               "• \"Détails de l'objet PC-001\"";
    }
    
    if (parsedQuery.candidates.isEmpty()) {
        qDebug() << "Aucun résultat trouvé pour:" << parsedQuery.searchTerm;
        return QString("❌ Je n'ai trouvé aucun résultat correspondant à « %1 ».\n\n"
                       "💡 Vérifiez l'orthographe ou précisez s'il s'agit d'un client, d'un employé ou d'un objet.")
            .arg(parsedQuery.searchTerm);
    }
    
    if (parsedQuery.isAmbiguous) {
        m_waitingForClarification = true;
        m_pendingCandidates = parsedQuery.candidates;
        showSuggestions(parsedQuery.candidates);
        return generateAmbiguityResponse(parsedQuery.candidates);
    }
    
    const ChatResult& target = parsedQuery.candidates.first();
    
    if (parsedQuery.intent == IntentType::FILTER_TABLE && target.type == EntityType::EMPLOYEE) {
        emit filterTableRequested(target.employee.getFullName());
        return QString("✅ J'ai trouvé %1 (ID: %2).\n\n"
                       "📋 Le tableau a été filtré pour afficher cet employé.")
            .arg(target.employee.getFullName(), QString::number(target.employee.id));
    }
    
    switch (target.type) {
    case EntityType::EMPLOYEE:
        emit employeeSelected(target.employee.id);
        return generateInfoResponse(target.employee, parsedQuery.intent);
    case EntityType::CLIENT:
        return generateClientInfoResponse(target.client, parsedQuery.intent);
    case EntityType::OBJECT:
        return generateObjetInfoResponse(target.objet, parsedQuery.intent);
    }
    
    return "❌ Je n'ai pas réussi à formuler une réponse pour cette requête.";
}

QString ChatbotEmploye::generateAmbiguityResponse(const QList<ChatResult>& candidates)
{
    QString response = QString("🔍 J'ai trouvé %1 résultat(s) correspondant(s) :\n\n").arg(candidates.size());
    
    for (int i = 0; i < candidates.size(); ++i) {
        const ChatResult& candidate = candidates[i];
        switch (candidate.type) {
        case EntityType::EMPLOYEE:
            response += QString("%1. 👔 **%2** (Employé · ID %3 · Statut %4)\n")
                        .arg(i + 1)
                        .arg(candidate.employee.getFullName())
                        .arg(candidate.employee.id)
                        .arg(candidate.employee.statut);
            break;
        case EntityType::CLIENT:
            response += QString("%1. 🧾 **%2** (Client · CIN %3)\n")
                        .arg(i + 1)
                        .arg(candidate.client.getFullName())
                        .arg(candidate.client.cin);
            break;
        case EntityType::OBJECT:
            response += QString("%1. 🛠️ **%2** (Objet · Réf %3 · %4)\n")
                        .arg(i + 1)
                        .arg(candidate.objet.getDisplayName())
                        .arg(candidate.objet.reference)
                        .arg(candidate.objet.etat);
            break;
        }
    }
    
    response += "\n💡 **Tapez le numéro (1, 2, etc.)** pour sélectionner l'entrée souhaitée.";
    
    return response;
}

QString ChatbotEmploye::generateInfoResponse(const EmployeeResult& employee, IntentType intent)
{
    switch (intent) {
        case IntentType::GET_ADDRESS:
            return QString("📍 L'adresse de **%1** (ID: %2) est :\n\n%3")
                   .arg(employee.getFullName(), QString::number(employee.id), employee.adresse);
        
        case IntentType::GET_PHONE:
            return QString("📞 Le numéro de téléphone de **%1** (ID: %2) est :\n\n**%3**")
                   .arg(employee.getFullName(), QString::number(employee.id), employee.telephone);
        
        case IntentType::GET_EMAIL:
            return QString("📧 L'adresse email de **%1** (ID: %2) est :\n\n**%3**")
                   .arg(employee.getFullName(), QString::number(employee.id), employee.email);
        
        case IntentType::GET_STATUS:
            return QString("📊 Le statut de **%1** (ID: %2) est :\n\n**%3**")
                   .arg(employee.getFullName(), QString::number(employee.id), employee.statut);
        
        case IntentType::GET_AGE:
            {
                int age = employee.getAge();
                if (age > 0) {
                    return QString("🎂 **%1** (ID: %2) a **%3 ans**.\n\n"
                                  "📅 Date de naissance : %4")
                           .arg(employee.getFullName(), QString::number(employee.id), 
                                QString::number(age), employee.dateNaissance.toString("dd/MM/yyyy"));
                } else {
                    return QString("❌ La date de naissance de **%1** n'est pas disponible.")
                           .arg(employee.getFullName());
                }
            }
        
        case IntentType::GET_SUMMARY:
            return generateSummaryResponse(employee);
        
        case IntentType::SEARCH_INFO:
        default:
            return formatEmployeeInfo(employee, true);
    }
}

QString ChatbotEmploye::generateClientInfoResponse(const ClientResult& client, IntentType intent)
{
    switch (intent) {
    case IntentType::GET_ADDRESS:
        return QString("📍 L'adresse du client **%1** (CIN: %2) est :\n\n%3")
                .arg(client.getFullName(), client.cin, client.adresse);
    case IntentType::GET_PHONE:
        return QString("📞 Le numéro de téléphone de **%1** est : **%2**")
                .arg(client.getFullName(), client.telephone);
    case IntentType::GET_EMAIL:
        return QString("📧 L'adresse email de **%1** est : **%2**")
                .arg(client.getFullName(), client.email);
    case IntentType::GET_AGE: {
        int age = calculateAge(client.dateNaissance);
        if (age > 0) {
            return QString("🎂 **%1** a **%2 ans**.\n\n📅 Date de naissance : %3")
                    .arg(client.getFullName())
                    .arg(age)
                    .arg(client.dateNaissance.toString("dd/MM/yyyy"));
        }
        break;
    }
    case IntentType::GET_STATUS:
        return QString("ℹ️ Les clients n'ont pas de statut particulier.\n\nVoici les informations disponibles :\n\n%1")
                .arg(formatClientInfo(client, true));
    case IntentType::GET_SUMMARY:
    case IntentType::FILTER_TABLE:
    case IntentType::SEARCH_INFO:
    default:
        return formatClientInfo(client, true);
    }

    return formatClientInfo(client, true);
}

QString ChatbotEmploye::generateObjetInfoResponse(const ObjetResult& objet, IntentType intent)
{
    switch (intent) {
    case IntentType::GET_STATUS:
        return QString("🛠️ L'état actuel de l'objet **%1** (réf. %2) est : **%3**")
                .arg(objet.getDisplayName(), objet.reference, objet.etat);
    case IntentType::GET_ADDRESS:
    case IntentType::GET_PHONE:
    case IntentType::GET_EMAIL:
    case IntentType::GET_AGE:
        return QString("ℹ️ Les objets électroniques ne possèdent pas cette information spécifique.\n\n%1")
                .arg(formatObjetInfo(objet, true));
    case IntentType::GET_SUMMARY:
    case IntentType::FILTER_TABLE:
    case IntentType::SEARCH_INFO:
    default:
        return formatObjetInfo(objet, true);
    }
}

QString ChatbotEmploye::generateSummaryResponse(const EmployeeResult& employee)
{
    QString summary = QString("📋 **SYNTHÈSE - %1** (ID: %2)\n\n").arg(employee.getFullName(), QString::number(employee.id));
    
    summary += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    // Statut
    QString statusIcon = employee.isActive() ? "✅" : "⏸️";
    summary += QString("%1 **Statut** : %2\n\n").arg(statusIcon, employee.statut);
    
    // Coordonnées
    summary += "📧 **Email** : " + employee.email + "\n";
    summary += "📞 **Téléphone** : " + employee.telephone + "\n";
    summary += "📍 **Adresse** : " + employee.adresse + "\n\n";
    
    // Date de naissance et âge
    if (employee.dateNaissance.isValid()) {
        int age = employee.getAge();
        summary += QString("🎂 **Date de naissance** : %1 (%2 ans)\n\n")
                  .arg(employee.dateNaissance.toString("dd/MM/yyyy"), QString::number(age));
    }
    
    summary += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    // Recommandations (si applicable)
    if (!employee.isActive()) {
        summary += "💡 **Note** : Cet employé n'est pas actif actuellement.\n";
    }
    
    return summary;
}

void ChatbotEmploye::showSuggestions(const QList<ChatResult>& candidates)
{
    // Nettoyer les suggestions précédentes
    QLayoutItem* item;
    while ((item = m_suggestionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Ajouter un titre
    QLabel* titleLabel = new QLabel("💡 Sélectionnez l'entrée souhaitée :", m_suggestionsWidget);
    titleLabel->setStyleSheet("font-weight: bold; color: #495057; margin-bottom: 5px;");
    m_suggestionsLayout->addWidget(titleLabel);
    
    // Créer un bouton pour chaque candidat
    for (int i = 0; i < candidates.size(); ++i) {
        const ChatResult& candidate = candidates[i];
        QString text;
        switch (candidate.type) {
        case EntityType::EMPLOYEE:
            text = QString("%1. 👔 %2 (ID %3) - %4")
                    .arg(i + 1)
                    .arg(candidate.employee.getFullName())
                    .arg(candidate.employee.id)
                    .arg(candidate.employee.statut);
            break;
        case EntityType::CLIENT:
            text = QString("%1. 🧾 %2 (CIN %3)")
                    .arg(i + 1)
                    .arg(candidate.client.getFullName())
                    .arg(candidate.client.cin);
            break;
        case EntityType::OBJECT:
            text = QString("%1. 🛠️ %2 (Réf %3 - %4)")
                    .arg(i + 1)
                    .arg(candidate.objet.getDisplayName())
                    .arg(candidate.objet.reference)
                    .arg(candidate.objet.etat);
            break;
        }

        QPushButton* btn = new QPushButton(text, m_suggestionsWidget);
        
        btn->setStyleSheet(
            "QPushButton {"
            "text-align: left;"
            "padding: 6px 10px;"
            "min-height: 30px;"
            "background-color: #e9ecef;"
            "border: 2px solid #8EC7E2;"
            "border-radius: 6px;"
            "color: #212529;"
            "font-size: 13px;"
            "}"
            "QPushButton:hover {"
            "background-color: #8EC7E2;"
            "border-color: #6BB3D4;"
            "}"
        );
        
        connect(btn, &QPushButton::clicked, this, [this, i]() {
            onSuggestionClicked(i);
        });
        
        m_suggestionsLayout->addWidget(btn);
    }
    
    m_suggestionsWidget->show();
}

void ChatbotEmploye::hideSuggestions()
{
    m_suggestionsWidget->hide();
    
    // Nettoyer
    QLayoutItem* item;
    while ((item = m_suggestionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void ChatbotEmploye::onSuggestionClicked(int employeeIndex)
{
    if (employeeIndex < 0 || employeeIndex >= m_pendingCandidates.size()) {
        return;
    }
    
    ChatResult selected = m_pendingCandidates[employeeIndex];
    m_waitingForClarification = false;
    m_pendingCandidates.clear();
    hideSuggestions();
    
    QString response;
    switch (selected.type) {
    case EntityType::EMPLOYEE:
        response = generateInfoResponse(selected.employee, m_currentQuery.intent);
        emit employeeSelected(selected.employee.id);
        break;
    case EntityType::CLIENT:
        response = generateClientInfoResponse(selected.client, m_currentQuery.intent);
        break;
    case EntityType::OBJECT:
        response = generateObjetInfoResponse(selected.objet, m_currentQuery.intent);
        break;
    }

    addMessage(response, false);
}

QString ChatbotEmploye::formatEmployeeInfo(const EmployeeResult& employee, bool detailed)
{
    QString info = QString("👤 **%1** (ID: %2)\n\n").arg(employee.getFullName(), QString::number(employee.id));
    
    if (detailed) {
        info += "📊 **Statut** : " + employee.statut + "\n";
        info += "📧 **Email** : " + employee.email + "\n";
        info += "📞 **Téléphone** : " + employee.telephone + "\n";
        info += "📍 **Adresse** : " + employee.adresse + "\n";
        
        if (employee.dateNaissance.isValid()) {
            int age = employee.getAge();
            info += QString("🎂 **Date de naissance** : %1 (%2 ans)\n")
                   .arg(employee.dateNaissance.toString("dd/MM/yyyy"), QString::number(age));
        }
    }
    
    return info;
}

QString ChatbotEmploye::formatClientInfo(const ClientResult& client, bool detailed)
{
    QString info = QString("🧾 **Client : %1** (CIN: %2)\n\n")
                   .arg(client.getFullName(), client.cin);
    info += "📞 **Téléphone** : " + client.telephone + "\n";
    info += "📧 **Email** : " + client.email + "\n";
    info += "📍 **Adresse** : " + client.adresse + "\n";
    if (client.dateNaissance.isValid()) {
        info += "🎂 **Date de naissance** : " + client.dateNaissance.toString("dd/MM/yyyy") + "\n";
    }
    if (detailed) {
        info += "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }
    return info;
}

QString ChatbotEmploye::formatObjetInfo(const ObjetResult& objet, bool detailed)
{
    QString info = QString("🛠️ **Objet : %1** (Réf: %2)\n\n")
                   .arg(objet.getDisplayName(), objet.reference);
    info += "🏷️ **Marque / Modèle** : " + objet.marque + " / " + objet.modele + "\n";
    info += "🎨 **Couleur** : " + objet.couleur + "\n";
    info += "🔢 **N° de série** : " + objet.numeroSerie + "\n";
    info += "📚 **Type** : " + objet.type + "\n";
    info += "📊 **État** : " + objet.etat + "\n";
    if (!objet.technicien.isEmpty()) {
        info += "👷 **Technicien assigné** : " + objet.technicien + "\n";
    }
    if (objet.prix > 0) {
        info += "💰 **Prix estimé** : " + QString::number(objet.prix) + " DT\n";
    }
    if (detailed) {
        info += "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }
    return info;
}

int ChatbotEmploye::calculateAge(const QDate& birthDate)
{
    if (!birthDate.isValid()) return -1;
    QDate today = QDate::currentDate();
    int age = today.year() - birthDate.year();
    if (today.month() < birthDate.month() || 
        (today.month() == birthDate.month() && today.day() < birthDate.day())) {
        age--;
    }
    return age;
}

