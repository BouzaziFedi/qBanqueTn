#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QMap>

class ComptesBancaire;
class Client
{
    QString nom;
    QString telephone;
    QString adresse;
    bool sexe;
    QMap<QString, ComptesBancaire> comptes;
public:
    Client();
    Client(QString, QString, QString, bool);
    friend QTextStream &operator <<(QTextStream &outStream, const Client  &client);
    QString getNom() const;
    QString getTelephone() const;
    QString getAdresse() const;
    bool getSexe() const;
    void InsererCompte(QString, ComptesBancaire&);
    void SupprimerCompte(QString);
    QMap<QString, ComptesBancaire> getComptes() const;
    void setData(QString, QString, QString, bool);
};

#endif // CLIENT_H
