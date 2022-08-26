#include "comptesbancaire.h"

ComptesBancaire::ComptesBancaire()
{

}

ComptesBancaire::ComptesBancaire(QString num,  int type, double solde, QDate dateOuverture, Client *titulaire):
    num(num), type(type), solde(solde), dateOuverture(dateOuverture){
    this->titulaire = titulaire;
}

void ComptesBancaire::modelPopulaire(QStandardItemModel *model, int ligne)
{
    QModelIndex index = model->index(ligne, 0, QModelIndex());
    model->setData(index, QVariant::fromValue(num));
    index = model->index(ligne, 1, QModelIndex());
    model->setData(index, QVariant::fromValue(titulaire->getNom()));
    index = model->index(ligne, 2, QModelIndex());
    model->setData(index, QVariant::fromValue(type));
    index = model->index(ligne, 3, QModelIndex());
    model->setData(index, QVariant::fromValue(dateOuverture));
    index = model->index(ligne, 4, QModelIndex());
    model->setData(index, QVariant::fromValue(solde));

    QStandardItem *typeItem = model->item(ligne,2);
    QStandardItem *soldeItem = model->item(ligne,4);

    switch(typeItem->text().toInt()){
    case 0:
        typeItem->setText("Crédit");
        soldeItem->setText("----");
        break;
    case 1:
        typeItem->setText("Débit");
        soldeItem->setText("€" + soldeItem->text());
        break;
    case 2:
        typeItem->setText("Cheques");
        soldeItem->setText("€" + soldeItem->text());
        break;
    }
}

QString ComptesBancaire::getNum() const
{
    return num;
}

int ComptesBancaire::getType() const
{
    return type;
}

double ComptesBancaire::getSolde() const
{
    return solde;
}

QDate ComptesBancaire::getDateOuverture() const
{
    return dateOuverture;
}

Client *ComptesBancaire::getTitulaire() const
{
    return titulaire;
}

void ComptesBancaire::setSolde(double value)
{
    solde = value;
}

QTextStream &operator <<(QTextStream &outStream, const ComptesBancaire &compte)
{
   outStream << compte.num << Qt::endl;
   outStream << compte.titulaire->getNom() << Qt::endl;
   outStream << compte.type << Qt::endl;
   outStream << compte.dateOuverture.toString("dd MMMM yyyy") << Qt::endl;
   outStream << compte.solde << Qt::endl;
   return outStream;
}
