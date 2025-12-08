#ifndef CHATBOTEMPLOYE_H
#define CHATBOTEMPLOYE_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QMap>
#include <QDate>

// Structure pour représenter un employé trouvé
struct EmployeeResult {
    int id;
    QString nom;
    QString prenom;
    QString adresse;
    QString telephone;
    QString email;
    QDate dateNaissance;
    QString statut;
    
    QString getFullName() const { return prenom + " " + nom; }
    int getAge() const;
    bool isActive() const { return statut.toUpper().contains("ACTIF"); }
};

// Structure pour représenter un client
struct ClientResult {
    QString cin;
    QString nom;
    QString prenom;
    QString telephone;
    QString email;
    QString adresse;
    QDate dateNaissance;

    QString getFullName() const { return prenom + " " + nom; }
};

// Structure pour représenter un objet électronique
struct ObjetResult {
    QString reference;
    QString nom;
    QString marque;
    QString modele;
    QString couleur;
    QString numeroSerie;
    QString type;
    QString etat;
    QString technicien;
    int prix = 0;

    QString getDisplayName() const { return nom.isEmpty() ? reference : nom; }
};

// Structure pour représenter une intention détectée
enum class IntentType {
    SEARCH_INFO,        // Recherche d'information générale
    GET_ADDRESS,        // Obtenir l'adresse
    GET_PHONE,          // Obtenir le téléphone
    GET_EMAIL,          // Obtenir l'email
    GET_STATUS,         // Obtenir le statut
    GET_AGE,            // Obtenir l'âge
    GET_SUMMARY,        // Synthèse complète
    FILTER_TABLE,       // Filtrer le tableau
    UNKNOWN             // Intention non reconnue
};

enum class EntityType {
    EMPLOYEE,
    CLIENT,
    OBJECT
};

struct ChatResult {
    EntityType type = EntityType::EMPLOYEE;
    EmployeeResult employee;
    ClientResult client;
    ObjetResult objet;

    QString displayName() const {
        switch (type) {
        case EntityType::EMPLOYEE:
            return employee.getFullName();
        case EntityType::CLIENT:
            return client.getFullName();
        case EntityType::OBJECT:
            return objet.getDisplayName();
        }
        return {};
    }
};

struct ParsedQuery {
    QString searchTerm;        // Terme recherché (nom, référence...)
    IntentType intent;         // Intention détectée
    QString originalQuery;     // Requête originale
    EntityType entityType;     // Type d'entité ciblé
    bool isAmbiguous;          // Si plusieurs employés trouvés
    QList<ChatResult> candidates; // Résultats candidats
};

QT_BEGIN_NAMESPACE
class QTextEdit;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
QT_END_NAMESPACE

class ChatbotEmploye : public QWidget
{
    Q_OBJECT

public:
    explicit ChatbotEmploye(QWidget *parent = nullptr);
    ~ChatbotEmploye();

    // Méthodes publiques pour interagir avec le chatbot
    void addMessage(const QString& message, bool isUser = true);
    void processQuery(const QString& query);
    QString processQueryText(const QString& query);
    void clearChat();

signals:
    void employeeSelected(int employeeId);  // Signal émis quand un employé est sélectionné
    void filterTableRequested(const QString& filter);  // Signal pour filtrer le tableau

private slots:
    void onSendButtonClicked();
    void onClearButtonClicked();
    void onSuggestionClicked(int employeeIndex);
    void onToggleInputClicked();

private:
    // Interface utilisateur
    QTextEdit* m_chatDisplay;
    QLineEdit* m_inputField;
    QPushButton* m_sendButton;
    QPushButton* m_clearButton;
    QPushButton* m_toggleInputButton;
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_inputLayout;
    QVBoxLayout* m_suggestionsLayout;
    QWidget* m_suggestionsWidget;
    
    // État du chatbot
    ParsedQuery m_currentQuery;
    bool m_waitingForClarification;
    QList<ChatResult> m_pendingCandidates;
    
    bool m_inputVisible;

    // Traitement de langage naturel
    ParsedQuery parseQuery(const QString& query);
    IntentType detectIntent(const QString& query);
    EntityType detectEntityType(const QString& query) const;
    QString extractNameFromQuery(const QString& query);
    
    // Recherche dans la base de données
    QList<EmployeeResult> searchEmployees(const QString& name);
    QList<EmployeeResult> searchEmployeesFuzzy(const QString& name);
    EmployeeResult getEmployeeById(int id);
    EmployeeResult getEmployeeByFullName(const QString& nom, const QString& prenom);
    QList<ClientResult> searchClients(const QString& nameOrCin);
    QList<ObjetResult> searchObjets(const QString& term);
    QList<ChatResult> buildChatResults(EntityType type, const QString& term);
    
    // Génération de réponses
    QString generateResponse(const ParsedQuery& parsedQuery);
    QString generateAmbiguityResponse(const QList<ChatResult>& candidates);
    QString generateInfoResponse(const EmployeeResult& employee, IntentType intent);
    QString generateClientInfoResponse(const ClientResult& client, IntentType intent);
    QString generateObjetInfoResponse(const ObjetResult& objet, IntentType intent);
    QString generateSummaryResponse(const EmployeeResult& employee);
    
    // Utilitaires
    void showSuggestions(const QList<ChatResult>& candidates);
    void hideSuggestions();
    QString formatEmployeeInfo(const EmployeeResult& employee, bool detailed = false);
    QString formatClientInfo(const ClientResult& client, bool detailed = false);
    QString formatObjetInfo(const ObjetResult& objet, bool detailed = false);
    int calculateAge(const QDate& birthDate);
    
    // Styles
    void setupUI();
    void applyStyles();
    void updateInputVisibility();
};

#endif // CHATBOTEMPLOYE_H

