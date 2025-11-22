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
    addMessage("👋 Bonjour ! Je suis votre assistant intelligent pour la gestion des employés.\n\n"
               "Je peux vous aider à :\n"
               "• Rechercher un employé par son nom\n"
               "• Obtenir des informations spécifiques (adresse, téléphone, email, statut)\n"
               "• Générer une synthèse complète d'un employé\n\n"
               "Essayez : \"Quelle est l'adresse de Marie ?\" ou \"Informations sur Jean Dupont\"", 
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
    cardLayout->setSpacing(16);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    
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
    m_chatDisplay->setMinimumHeight(250);
    m_chatDisplay->setMaximumHeight(400);
    m_chatDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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
    
    // Zone de saisie
    m_inputLayout = new QHBoxLayout();
    m_inputLayout->setSpacing(10);
    m_inputLayout->setContentsMargins(0, 0, 0, 0);
    
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
    m_sendButton->setMinimumWidth(120);
    m_sendButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1f84bd;"
        "color: #ffffff;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 12px 22px;"
        "font-weight: 600;"
        "}"
        "QPushButton:hover { background-color: #1a6f9f; }"
        "QPushButton:pressed { background-color: #155a82; }"
    );
    
    m_clearButton = new QPushButton("Effacer", card);
    m_clearButton->setCursor(Qt::PointingHandCursor);
    m_clearButton->setMinimumWidth(120);
    m_clearButton->setStyleSheet(
        "QPushButton {"
        "background-color: #d0d9e0;"
        "color: #405566;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 12px 22px;"
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
}

void ChatbotEmploye::applyStyles()
{
    setStyleSheet("QWidget { background-color: transparent; }");
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
            EmployeeResult selected = m_pendingCandidates[choice - 1];
            m_waitingForClarification = false;
            m_pendingCandidates.clear();
            
            // Générer la réponse pour l'employé sélectionné
            ParsedQuery newQuery;
            newQuery.employeeName = selected.getFullName();
            newQuery.intent = m_currentQuery.intent;
            newQuery.originalQuery = query;
            newQuery.isAmbiguous = false;
            newQuery.candidates = {selected};
            
            QString response = generateInfoResponse(selected, m_currentQuery.intent);
            addMessage(response, false);
            
            // Émettre le signal pour sélectionner l'employé dans le tableau
            emit employeeSelected(selected.id);
            return;
        }
    }
    
    processQuery(query);
}

void ChatbotEmploye::onClearButtonClicked()
{
    clearChat();
}

void ChatbotEmploye::clearChat()
{
    m_chatDisplay->clear();
    m_waitingForClarification = false;
    m_pendingCandidates.clear();
    hideSuggestions();
    
    addMessage("👋 Bonjour ! Je suis votre assistant intelligent pour la gestion des employés.\n\n"
               "Je peux vous aider à :\n"
               "• Rechercher un employé par son nom\n"
               "• Obtenir des informations spécifiques (adresse, téléphone, email, statut)\n"
               "• Générer une synthèse complète d'un employé\n\n"
               "Essayez : \"Quelle est l'adresse de Marie ?\" ou \"Informations sur Jean Dupont\"", 
               false);
}

void ChatbotEmploye::processQuery(const QString& query)
{
    qDebug() << "=== Traitement de la requête ===";
    qDebug() << "Requête originale:" << query;
    
    ParsedQuery parsed = parseQuery(query);
    m_currentQuery = parsed;
    
    qDebug() << "Nom extrait:" << parsed.employeeName;
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
    
    qDebug() << "Nom extrait:" << parsed.employeeName;
    qDebug() << "Intention:" << static_cast<int>(parsed.intent);
    qDebug() << "Nombre de candidats:" << parsed.candidates.size();
    
    return generateResponse(parsed);
}

ParsedQuery ChatbotEmploye::parseQuery(const QString& query)
{
    ParsedQuery result;
    result.originalQuery = query;
    result.intent = detectIntent(query);
    result.employeeName = extractEmployeeName(query);
    result.isAmbiguous = false;
    
    // Rechercher les employés
    if (!result.employeeName.isEmpty()) {
        result.candidates = searchEmployees(result.employeeName);
        
        // Si recherche floue nécessaire
        if (result.candidates.isEmpty()) {
            result.candidates = searchEmployeesFuzzy(result.employeeName);
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

QString ChatbotEmploye::extractEmployeeName(const QString& query)
{
    QString lowerQuery = query.toLower();
    
    // Patterns pour extraire les noms (améliorés pour gérer les minuscules)
    QRegularExpression patterns[] = {
        // Pattern 1: "quelle est l'adresse de hiba riahi"
        QRegularExpression(R"((?:quelle|quel|qui|où|ou)\s+(?:est|sont|a|ont)\s+(?:l'|la|le|les)?\s*(?:adresse|téléphone|telephone|email|statut|âge|age)\s+(?:de|du|des|d')\s+([a-z]+(?:\s+[a-z]+)?))", QRegularExpression::CaseInsensitiveOption),
        // Pattern 2: "informations sur hiba riahi"
        QRegularExpression(R"((?:informations|info|détails|details|données|donnees)\s+(?:sur|de|du|des|pour)\s+([a-z]+(?:\s+[a-z]+)?))", QRegularExpression::CaseInsensitiveOption),
        // Pattern 3: "nom/prénom est hiba riahi"
        QRegularExpression(R"((?:nom|prénom|prenom|appelle|s'appelle)\s+(?:est|d'|de|du|des|le|la|les)?\s*([a-z]+(?:\s+[a-z]+)?))", QRegularExpression::CaseInsensitiveOption),
        // Pattern 4: Nom complet avec majuscules "Hiba Riahi"
        QRegularExpression(R"(([A-Z][a-z]+\s+[A-Z][a-z]+))"),
        // Pattern 5: Prénom ou nom seul avec majuscule "Hiba"
        QRegularExpression(R"(([A-Z][a-z]+))")
    };
    
    for (const auto& pattern : patterns) {
        QRegularExpressionMatch match = pattern.match(query);
        if (match.hasMatch()) {
            QString name = match.captured(1).trimmed();
            if (!name.isEmpty() && name.length() > 2) {
                // Capitaliser la première lettre de chaque mot
                QStringList words = name.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                QStringList capitalizedWords;
                for (const QString& word : words) {
                    if (word.length() > 0) {
                        capitalizedWords.append(word[0].toUpper() + word.mid(1).toLower());
                    }
                }
                return capitalizedWords.join(" ");
            }
        }
    }
    
    // Si aucun pattern ne correspond, chercher des mots (même en minuscules)
    QStringList words = query.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QStringList names;
    
    // Mots à ignorer
    QStringList ignoreWords = {"quelle", "quel", "qui", "où", "ou", "est", "sont", "a", "ont", 
                               "l'", "la", "le", "les", "de", "du", "des", "d'", "sur", "pour",
                               "adresse", "téléphone", "telephone", "email", "statut", "âge", "age",
                               "informations", "info", "détails", "details", "données", "donnees"};
    
    for (const QString& word : words) {
        QString lowerWord = word.toLower();
        if (word.length() > 2 && !ignoreWords.contains(lowerWord)) {
            // Accepter les mots qui ne sont pas des mots-clés
            names.append(word[0].toUpper() + word.mid(1).toLower());
        }
    }
    
    if (names.size() >= 2) {
        return names[0] + " " + names[1];
    } else if (names.size() == 1) {
        return names[0];
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
    qDebug() << "Nom recherché:" << parsedQuery.employeeName;
    qDebug() << "Candidats trouvés:" << parsedQuery.candidates.size();
    
    // Si aucun nom n'a été trouvé
    if (parsedQuery.employeeName.isEmpty()) {
        qDebug() << "Aucun nom extrait de la requête";
        return "❌ Je n'ai pas pu identifier le nom de l'employé dans votre question.\n\n"
               "💡 Exemples de questions valides :\n"
               "• \"Quelle est l'adresse de Marie ?\"\n"
               "• \"Informations sur Jean Dupont\"\n"
               "• \"Téléphone de Paul Martin\"\n"
               "• \"Quelle est l'adresse de hiba riahi\"";
    }
    
    // Si aucun employé trouvé
    if (parsedQuery.candidates.isEmpty()) {
        qDebug() << "Aucun employé trouvé pour:" << parsedQuery.employeeName;
        return QString("❌ Je n'ai trouvé aucun employé correspondant à \"%1\".\n\n"
                      "💡 Vérifiez l'orthographe ou essayez avec le prénom ou le nom seul.\n\n"
                      "💬 Essayez aussi : \"Informations sur %1\"")
                      .arg(parsedQuery.employeeName);
    }
    
    // Si ambiguïté (plusieurs résultats)
    if (parsedQuery.isAmbiguous) {
        m_waitingForClarification = true;
        m_pendingCandidates = parsedQuery.candidates;
        showSuggestions(parsedQuery.candidates);
        return generateAmbiguityResponse(parsedQuery.candidates);
    }
    
    // Un seul résultat trouvé
    EmployeeResult employee = parsedQuery.candidates.first();
    
    // Si intention de filtrer le tableau
    if (parsedQuery.intent == IntentType::FILTER_TABLE) {
        emit filterTableRequested(employee.getFullName());
        return QString("✅ J'ai trouvé %1 (ID: %2).\n\n"
                      "📋 Le tableau a été filtré pour afficher cet employé.")
                      .arg(employee.getFullName(), QString::number(employee.id));
    }
    
    // Générer la réponse selon l'intention
    emit employeeSelected(employee.id);
    return generateInfoResponse(employee, parsedQuery.intent);
}

QString ChatbotEmploye::generateAmbiguityResponse(const QList<EmployeeResult>& candidates)
{
    QString response = QString("🔍 J'ai trouvé %1 employé(s) portant ce nom :\n\n").arg(candidates.size());
    
    for (int i = 0; i < candidates.size(); ++i) {
        const EmployeeResult& emp = candidates[i];
        response += QString("%1. **%2** (ID: %3, Statut: %4)\n")
                   .arg(i + 1)
                   .arg(emp.getFullName())
                   .arg(emp.id)
                   .arg(emp.statut);
    }
    
    response += "\n💡 **Tapez le numéro (1, 2, etc.)** pour sélectionner l'employé souhaité.";
    
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

void ChatbotEmploye::showSuggestions(const QList<EmployeeResult>& candidates)
{
    // Nettoyer les suggestions précédentes
    QLayoutItem* item;
    while ((item = m_suggestionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Ajouter un titre
    QLabel* titleLabel = new QLabel("💡 Sélectionnez un employé :", m_suggestionsWidget);
    titleLabel->setStyleSheet("font-weight: bold; color: #495057; margin-bottom: 5px;");
    m_suggestionsLayout->addWidget(titleLabel);
    
    // Créer un bouton pour chaque candidat
    for (int i = 0; i < candidates.size(); ++i) {
        const EmployeeResult& emp = candidates[i];
        QPushButton* btn = new QPushButton(
            QString("%1. %2 (ID: %3) - %4")
            .arg(i + 1)
            .arg(emp.getFullName())
            .arg(emp.id)
            .arg(emp.statut),
            m_suggestionsWidget
        );
        
        btn->setStyleSheet(
            "QPushButton {"
            "text-align: left;"
            "padding: 10px;"
            "background-color: #e9ecef;"
            "border: 2px solid #8EC7E2;"
            "border-radius: 6px;"
            "color: #212529;"
            "font-size: 12px;"
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
    
    EmployeeResult selected = m_pendingCandidates[employeeIndex];
    m_waitingForClarification = false;
    m_pendingCandidates.clear();
    hideSuggestions();
    
    // Générer la réponse
    QString response = generateInfoResponse(selected, m_currentQuery.intent);
    addMessage(response, false);
    
    // Émettre le signal
    emit employeeSelected(selected.id);
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

