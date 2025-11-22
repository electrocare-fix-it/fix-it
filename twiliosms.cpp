#include "twiliosms.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

TwilioSMS::TwilioSMS(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentReply(nullptr)
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &TwilioSMS::onSmsReplyFinished);
}

TwilioSMS::~TwilioSMS()
{
    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
}

void TwilioSMS::setCredentials(const QString& accountSid, const QString& authToken, const QString& fromNumber)
{
    m_accountSid = accountSid;
    m_authToken = authToken;
    m_fromNumber = fromNumber;
}

bool TwilioSMS::sendSMS(const QString& toNumber, const QString& message)
{
    if (m_accountSid.isEmpty() || m_authToken.isEmpty() || m_fromNumber.isEmpty()) {
        qDebug() << "Erreur: Identifiants Twilio non configurés";
        emit smsSent(false, "Les identifiants Twilio ne sont pas configurés.");
        return false;
    }

    if (toNumber.isEmpty()) {
        qDebug() << "Erreur: Numéro de téléphone vide";
        emit smsSent(false, "Le numéro de téléphone du destinataire est vide.");
        return false;
    }

    if (message.isEmpty()) {
        qDebug() << "Erreur: Message SMS vide";
        emit smsSent(false, "Le message SMS est vide.");
        return false;
    }

    QString urlString = QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(m_accountSid);
    QUrl url(urlString);

    QUrlQuery postData;
    postData.addQueryItem("From", m_fromNumber);
    postData.addQueryItem("To", toNumber);
    postData.addQueryItem("Body", message);

    QNetworkRequest request(url);
    QString credentials = QString("%1:%2").arg(m_accountSid, m_authToken);
    QByteArray authHeader = "Basic " + credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", authHeader);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postDataBytes = postData.toString(QUrl::FullyEncoded).toUtf8();
    
    qDebug() << "Envoi SMS à Twilio...";
    qDebug() << "URL:" << urlString;
    qDebug() << "To:" << toNumber;
    qDebug() << "From:" << m_fromNumber;
    qDebug() << "Message length:" << message.length();
    qDebug() << "Post data:" << postDataBytes;

    m_currentReply = m_networkManager->post(request, postDataBytes);
    
    if (!m_currentReply) {
        qDebug() << "ERREUR: m_currentReply est null après post()";
        emit smsSent(false, "Impossible de créer la requête réseau.");
        return false;
    }
    
    connect(m_currentReply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError error) {
        qDebug() << "Erreur réseau lors de l'envoi SMS:" << error;
        qDebug() << "Message d'erreur:" << m_currentReply->errorString();
    });
    
    connect(m_currentReply, &QNetworkReply::sslErrors, this, [this](const QList<QSslError>& errors) {
        qDebug() << "Erreurs SSL:" << errors;
    });

    qDebug() << "Requête POST créée, attente de la réponse...";
    return true;
}

void TwilioSMS::onSmsReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        qDebug() << "Erreur: reply est null dans onSmsReplyFinished";
        return;
    }

    bool success = false;
    QString errorMessage;

    QByteArray responseData = reply->readAll();
    qDebug() << "Réponse Twilio - Code HTTP:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Réponse Twilio - Données:" << responseData;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            qDebug() << "Erreur: Réponse JSON invalide";
            errorMessage = "Réponse invalide de l'API Twilio.";
        } else {
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << "Réponse JSON:" << jsonObj;

            if (jsonObj.contains("sid")) {
                success = true;
                qDebug() << "SMS envoyé avec succès. SID:" << jsonObj["sid"].toString();
            } else if (jsonObj.contains("message")) {
                errorMessage = jsonObj["message"].toString();
                qDebug() << "Erreur Twilio:" << errorMessage;
            } else {
                errorMessage = "Réponse inattendue de l'API Twilio.";
                qDebug() << "Réponse inattendue:" << jsonObj;
            }
        }
    } else {
        qDebug() << "Erreur réseau:" << reply->error() << "-" << reply->errorString();
        
        QJsonDocument errorDoc = QJsonDocument::fromJson(responseData);
        if (!errorDoc.isNull() && errorDoc.isObject()) {
            QJsonObject errorObj = errorDoc.object();
            if (errorObj.contains("message")) {
                errorMessage = errorObj["message"].toString();
            } else {
                errorMessage = QString("Erreur réseau: %1").arg(reply->errorString());
            }
        } else {
            errorMessage = QString("Erreur réseau: %1").arg(reply->errorString());
        }
    }

    reply->deleteLater();
    m_currentReply = nullptr;

    qDebug() << "Émission du signal smsSent - Success:" << success << "Error:" << errorMessage;
    emit smsSent(success, errorMessage);
}

