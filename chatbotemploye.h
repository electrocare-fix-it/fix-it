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

struct ParsedQuery {
    QString employeeName;      // Nom ou prénom extrait
    IntentType intent;         // Intention détectée
    QString originalQuery;     // Requête originale
    bool isAmbiguous;          // Si plusieurs employés trouvés
    QList<EmployeeResult> candidates; // Employés candidats
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

private:
    // Interface utilisateur
    QTextEdit* m_chatDisplay;
    QLineEdit* m_inputField;
    QPushButton* m_sendButton;
    QPushButton* m_clearButton;
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_inputLayout;
    QVBoxLayout* m_suggestionsLayout;
    QWidget* m_suggestionsWidget;
    
    // État du chatbot
    ParsedQuery m_currentQuery;
    bool m_waitingForClarification;
    QList<EmployeeResult> m_pendingCandidates;
    
    // Traitement de langage naturel
    ParsedQuery parseQuery(const QString& query);
    IntentType detectIntent(const QString& query);
    QString extractEmployeeName(const QString& query);
    
    // Recherche dans la base de données
    QList<EmployeeResult> searchEmployees(const QString& name);
    QList<EmployeeResult> searchEmployeesFuzzy(const QString& name);
    EmployeeResult getEmployeeById(int id);
    EmployeeResult getEmployeeByFullName(const QString& nom, const QString& prenom);
    
    // Génération de réponses
    QString generateResponse(const ParsedQuery& parsedQuery);
    QString generateAmbiguityResponse(const QList<EmployeeResult>& candidates);
    QString generateInfoResponse(const EmployeeResult& employee, IntentType intent);
    QString generateSummaryResponse(const EmployeeResult& employee);
    
    // Utilitaires
    void showSuggestions(const QList<EmployeeResult>& candidates);
    void hideSuggestions();
    QString formatEmployeeInfo(const EmployeeResult& employee, bool detailed = false);
    int calculateAge(const QDate& birthDate);
    
    // Styles
    void setupUI();
    void applyStyles();
};

#endif // CHATBOTEMPLOYE_H

