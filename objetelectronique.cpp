#include "objetelectronique.h"

ObjetElectronique::ObjetElectronique()
    : m_prix(0)
{
}

ObjetElectronique::ObjetElectronique(const QString& reference, const QString& nom, const QString& marque,
                                      const QString& modele, const QString& couleur, const QString& numeroSerie,
                                      const QString& type, const QString& etat, const QString& technicien, int prix)
    : m_reference(reference)
    , m_nom(nom)
    , m_marque(marque)
    , m_modele(modele)
    , m_couleur(couleur)
    , m_numeroSerie(numeroSerie)
    , m_type(type)
    , m_etat(etat)
    , m_technicien(technicien)
    , m_prix(prix)
{
}

QString ObjetElectronique::getReference() const
{
    return m_reference;
}

void ObjetElectronique::setReference(const QString& reference)
{
    m_reference = reference;
}

QString ObjetElectronique::getNom() const
{
    return m_nom;
}

void ObjetElectronique::setNom(const QString& nom)
{
    m_nom = nom;
}

QString ObjetElectronique::getMarque() const
{
    return m_marque;
}

void ObjetElectronique::setMarque(const QString& marque)
{
    m_marque = marque;
}

QString ObjetElectronique::getModele() const
{
    return m_modele;
}

void ObjetElectronique::setModele(const QString& modele)
{
    m_modele = modele;
}

QString ObjetElectronique::getCouleur() const
{
    return m_couleur;
}

void ObjetElectronique::setCouleur(const QString& couleur)
{
    m_couleur = couleur;
}

QString ObjetElectronique::getNumeroSerie() const
{
    return m_numeroSerie;
}

void ObjetElectronique::setNumeroSerie(const QString& numeroSerie)
{
    m_numeroSerie = numeroSerie;
}

QString ObjetElectronique::getType() const
{
    return m_type;
}

void ObjetElectronique::setType(const QString& type)
{
    m_type = type;
}

QString ObjetElectronique::getEtat() const
{
    return m_etat;
}

void ObjetElectronique::setEtat(const QString& etat)
{
    m_etat = etat;
}

QString ObjetElectronique::getTechnicien() const
{
    return m_technicien;
}

void ObjetElectronique::setTechnicien(const QString& technicien)
{
    m_technicien = technicien;
}

int ObjetElectronique::getPrix() const
{
    return m_prix;
}

void ObjetElectronique::setPrix(int prix)
{
    m_prix = prix;
}

