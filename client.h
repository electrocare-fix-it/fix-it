#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDate>

class Client
{
public:
    Client();
    Client(const QString& cin, const QString& nom, const QString& prenom,
           const QString& telephone, const QString& email, const QString& adresse,
           const QDate& dateNaissance);

    QString getCin() const;
    void setCin(const QString& cin);

    QString getNom() const;
    void setNom(const QString& nom);

    QString getPrenom() const;
    void setPrenom(const QString& prenom);

    QString getTelephone() const;
    void setTelephone(const QString& telephone);

    QString getEmail() const;
    void setEmail(const QString& email);

    QString getAdresse() const;
    void setAdresse(const QString& adresse);

    QDate getDateNaissance() const;
    void setDateNaissance(const QDate& dateNaissance);

private:
    QString m_cin;
    QString m_nom;
    QString m_prenom;
    QString m_telephone;
    QString m_email;
    QString m_adresse;
    QDate m_dateNaissance;
};

#endif // CLIENT_H


