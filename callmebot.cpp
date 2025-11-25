#include "callmebot.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QDebug>

CallMeBot::CallMeBot(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentReply(nullptr)
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &CallMeBot::onReplyFinished);
}

CallMeBot::~CallMeBot()
{
    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
}

void CallMeBot::setCredentials(const QString& phoneNumber, const QString& apiKey)
{
    m_phoneNumber = phoneNumber;
    m_apiKey = apiKey;
}

bool CallMeBot::sendWhatsApp(const QString& toNumber, const QString& message)
{
    if (m_phoneNumber.isEmpty() || m_apiKey.isEmpty()) {
        qDebug() << "Erreur: Identifiants CallMeBot non configurés";
        emit messageSent(false, "Les identifiants CallMeBot ne sont pas configurés.");
        return false;
    }

    if (toNumber.isEmpty()) {
        qDebug() << "Erreur: Numéro de téléphone vide";
        emit messageSent(false, "Le numéro de téléphone du destinataire est vide.");
        return false;
    }

    if (message.isEmpty()) {
        qDebug() << "Erreur: Message vide";
        emit messageSent(false, "Le message est vide.");
        return false;
    }

    QString cleanToNumber = toNumber;
    if (cleanToNumber.startsWith("+")) {
        cleanToNumber = cleanToNumber.mid(1);
    }
    
    cleanToNumber = cleanToNumber.trimmed();
    cleanToNumber = cleanToNumber.remove(QRegularExpression("[^0-9]"));

    QUrl url("https://api.callmebot.com/whatsapp.php");
    QUrlQuery query;
    query.addQueryItem("phone", cleanToNumber);
    query.addQueryItem("text", message);
    query.addQueryItem("apikey", m_apiKey);
    
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0");

    qDebug() << "=== ENVOI WHATSAPP CALLMEBOT ===";
    qDebug() << "URL complète:" << url.toString();
    qDebug() << "Numéro destinataire:" << cleanToNumber;
    qDebug() << "API Key:" << m_apiKey;
    qDebug() << "Longueur message:" << message.length();
    qDebug() << "Message (premiers 200 chars):" << message.left(200);

    m_currentReply = m_networkManager->get(request);
    
    if (!m_currentReply) {
        qDebug() << "ERREUR: m_currentReply est null après get()";
        emit messageSent(false, "Impossible de créer la requête réseau.");
        return false;
    }
    
    connect(m_currentReply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError error) {
        qDebug() << "Erreur réseau lors de l'envoi WhatsApp:" << error;
        qDebug() << "Message d'erreur:" << m_currentReply->errorString();
    });
    
    connect(m_currentReply, &QNetworkReply::sslErrors, this, [this](const QList<QSslError>& errors) {
        qDebug() << "Erreurs SSL:" << errors;
    });

    qDebug() << "Requête GET créée, attente de la réponse...";
    return true;
}

void CallMeBot::onReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        qDebug() << "Erreur: reply est null dans onReplyFinished";
        return;
    }

    bool success = false;
    QString errorMessage;

    QByteArray responseData = reply->readAll();
    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Réponse CallMeBot - Code HTTP:" << httpCode;
    qDebug() << "Réponse CallMeBot - Données:" << responseData;

    if (reply->error() == QNetworkReply::NoError) {
        QString response = QString::fromUtf8(responseData).trimmed();
        qDebug() << "Réponse CallMeBot (texte):" << response;
        
        if (httpCode == 200) {
            if (response.contains("OK", Qt::CaseInsensitive) || 
                response.contains("success", Qt::CaseInsensitive) ||
                response.contains("Message sent", Qt::CaseInsensitive) ||
                response.isEmpty()) {
                success = true;
                qDebug() << "WhatsApp envoyé avec succès via CallMeBot";
            } else if (response.contains("error", Qt::CaseInsensitive) || 
                       response.contains("Error", Qt::CaseInsensitive) ||
                       response.contains("failed", Qt::CaseInsensitive)) {
                errorMessage = response.isEmpty() ? "Erreur inconnue de CallMeBot" : response;
                qDebug() << "Erreur CallMeBot:" << errorMessage;
            } else {
                success = true;
                qDebug() << "Réponse reçue (considérée comme succès):" << response;
            }
        } else {
            errorMessage = QString("Code HTTP %1: %2").arg(httpCode).arg(response);
            qDebug() << "Erreur HTTP:" << errorMessage;
        }
    } else {
        QString response = QString::fromUtf8(responseData).trimmed();
        qDebug() << "Erreur réseau:" << reply->error() << "-" << reply->errorString();
        if (!response.isEmpty()) {
            errorMessage = QString("Erreur réseau: %1\nRéponse: %2").arg(reply->errorString()).arg(response);
        } else {
            errorMessage = QString("Erreur réseau: %1").arg(reply->errorString());
        }
    }

    reply->deleteLater();
    m_currentReply = nullptr;

    qDebug() << "Émission du signal messageSent - Success:" << success << "Error:" << errorMessage;
    emit messageSent(success, errorMessage);
}

