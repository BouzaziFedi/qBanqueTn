#ifndef COMPTESBANCAIRE_H
#define COMPTESBANCAIRE_H

#include <QtCore>
#include <QStandardItemModel>
#include "client.h"


class ComptesBancaire
{
private:
    QString num;
    int type;
    double solde;
    QDate dateOuverture;
    Client *titulaire;

public:
    ComptesBancaire();
    ComptesBancaire(QString, int, double, QDate, Client*);
    void modelPopulaire(QStandardItemModel* , int);
    friend QTextStream &operator <<(QTextStream &outStream, const ComptesBancaire  &compte);

    QString getNum() const;
    int getType() const;
    double getSolde() const;
    QDate getDateOuverture() const;
    Client *getTitulaire() const;
    void setSolde(double value);
};

#endif // COMPTESBANCAIRE_H
