#include "client.h"

Client::Client()
    : m_dateNaissance(QDate::currentDate())
{
}

Client::Client(const QString& cin, const QString& nom, const QString& prenom,
               const QString& telephone, const QString& email, const QString& adresse,
               const QDate& dateNaissance)
    : m_cin(cin)
    , m_nom(nom)
    , m_prenom(prenom)
    , m_telephone(telephone)
    , m_email(email)
    , m_adresse(adresse)
    , m_dateNaissance(dateNaissance)
{
}

QString Client::getCin() const
{
    return m_cin;
}

void Client::setCin(const QString& cin)
{
    m_cin = cin;
}

QString Client::getNom() const
{
    return m_nom;
}

void Client::setNom(const QString& nom)
{
    m_nom = nom;
}

QString Client::getPrenom() const
{
    return m_prenom;
}

void Client::setPrenom(const QString& prenom)
{
    m_prenom = prenom;
}

QString Client::getTelephone() const
{
    return m_telephone;
}

void Client::setTelephone(const QString& telephone)
{
    m_telephone = telephone;
}

QString Client::getEmail() const
{
    return m_email;
}

void Client::setEmail(const QString& email)
{
    m_email = email;
}

QString Client::getAdresse() const
{
    return m_adresse;
}

void Client::setAdresse(const QString& adresse)
{
    m_adresse = adresse;
}

QDate Client::getDateNaissance() const
{
    return m_dateNaissance;
}

void Client::setDateNaissance(const QDate& dateNaissance)
{
    m_dateNaissance = dateNaissance;
}


