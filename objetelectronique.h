#ifndef OBJETELECTRONIQUE_H
#define OBJETELECTRONIQUE_H

#include <QString>

class ObjetElectronique
{
public:
    ObjetElectronique();
    ObjetElectronique(const QString& reference, const QString& nom, const QString& marque,
                      const QString& modele, const QString& couleur, const QString& numeroSerie,
                      const QString& type, const QString& etat, const QString& technicien, int prix);

    QString getReference() const;
    void setReference(const QString& reference);

    QString getNom() const;
    void setNom(const QString& nom);

    QString getMarque() const;
    void setMarque(const QString& marque);

    QString getModele() const;
    void setModele(const QString& modele);

    QString getCouleur() const;
    void setCouleur(const QString& couleur);

    QString getNumeroSerie() const;
    void setNumeroSerie(const QString& numeroSerie);

    QString getType() const;
    void setType(const QString& type);

    QString getEtat() const;
    void setEtat(const QString& etat);

    QString getTechnicien() const;
    void setTechnicien(const QString& technicien);

    int getPrix() const;
    void setPrix(int prix);

private:
    QString m_reference;
    QString m_nom;
    QString m_marque;
    QString m_modele;
    QString m_couleur;
    QString m_numeroSerie;
    QString m_type;
    QString m_etat;
    QString m_technicien;
    int m_prix;
};

#endif // OBJETELECTRONIQUE_H

