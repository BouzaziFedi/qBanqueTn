#include "client.h"
#include "comptesbancaire.h"

Client::Client()
{

}

Client::Client(QString nom, QString telephone, QString adresse, bool sexe)
                 :nom(nom), telephone(telephone), adresse(adresse), sexe(sexe){}

QString Client::getNom() const
{
    return nom;
}

QString Client::getTelephone() const
{
    return telephone;
}

QString Client::getAdresse() const
{
    return adresse;
}

bool Client::getSexe() const
{
    return sexe;
}

void Client::InsererCompte(QString num, ComptesBancaire &compte)
{
    comptes.insert(num, compte);
}

void Client::SupprimerCompte(QString num)
{
    comptes[num].~ComptesBancaire();
    comptes.remove(num);
}

QMap<QString, ComptesBancaire> Client::getComptes() const
{
    return comptes;
}

void Client::setData(QString nouveauNom, QString nouveauTelephone, QString nouvelleAdresse, bool nouveauSexe)
{
    nom = nouveauNom;
    telephone = nouveauTelephone;
    adresse = nouvelleAdresse;
    sexe = nouveauSexe;
}

QTextStream &operator <<(QTextStream &outStream, const Client &client)
{
   outStream << client.nom << Qt::endl;
   outStream << client.telephone << Qt::endl;
   outStream << client.adresse << Qt::endl;
   outStream << client.sexe << Qt::endl;
   return outStream;
}
