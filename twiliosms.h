#ifndef TWILIOSMS_H
#define TWILIOSMS_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TwilioSMS : public QObject
{
    Q_OBJECT

public:
    explicit TwilioSMS(QObject *parent = nullptr);
    ~TwilioSMS();

    void setCredentials(const QString& accountSid, const QString& authToken, const QString& fromNumber);
    bool sendSMS(const QString& toNumber, const QString& message);

signals:
    void smsSent(bool success, const QString& errorMessage);

private slots:
    void onSmsReplyFinished();

private:
    QString m_accountSid;
    QString m_authToken;
    QString m_fromNumber;
    QNetworkAccessManager* m_networkManager;
    QNetworkReply* m_currentReply;
};

#endif // TWILIOSMS_H

