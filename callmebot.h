#ifndef CALLMEBOT_H
#define CALLMEBOT_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class CallMeBot : public QObject
{
    Q_OBJECT

public:
    explicit CallMeBot(QObject *parent = nullptr);
    ~CallMeBot();

    void setCredentials(const QString& phoneNumber, const QString& apiKey);
    bool sendWhatsApp(const QString& toNumber, const QString& message);

signals:
    void messageSent(bool success, const QString& errorMessage);

private slots:
    void onReplyFinished();

private:
    QString m_phoneNumber;
    QString m_apiKey;
    QNetworkAccessManager* m_networkManager;
    QNetworkReply* m_currentReply;
};

#endif // CALLMEBOT_H

