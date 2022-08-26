#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QtCore>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>
#include <QInputDialog>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget::setWindowTitle ("banque TN.");
    QPixmap logo(":/assets/assets/login.png");
    ui->logo->setPixmap(logo.scaled(90,90, Qt::KeepAspectRatio));
    QPixmap card(":/assets/assets/card.png");
    ui->cardImage->setPixmap(card.scaled(300,300, Qt::KeepAspectRatio));
    QPixmap logoEnAcceuil(":/assets/assets/logo.png");
    ui->logoAcceuil->setPixmap(logoEnAcceuil.scaled(90,90, Qt::KeepAspectRatio));

    ui->usernameLineEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->passwordLineEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);

    // Démarrez l'application sur la page d'accueil ----------
    ui->stackedWidget->setCurrentIndex(4);
    ui->errorLoginLabel->hide();

    // CONNECTIONS
    connect(ui->nomLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ValiderAjouterClient()));
    connect(ui->telephoneLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ValiderAjouterClient()));
    connect(ui->adresseLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ValiderAjouterClient()));
    connect(ui->hommeRadioButton, SIGNAL(toggled(bool)), this, SLOT(ValiderAjouterClient()));
    connect(ui->femmeRadioButton, SIGNAL(toggled(bool)), this, SLOT(ValiderAjouterClient()));
    connect(ui->numCompteLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ValiderAjouterCompte()));
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(ValiderAjouterCompte()));
    connect(ui->usernameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ValiderChampsLogin()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ValiderChampsLogin()));

    // Mettre à jour les fichiers -------------------------
    MettreAJourListeClient();
    MettreAJourListeCompte();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI(int typeUser)
{
    if(typeUser == 2)
    {
        ui->SupprimerCompteBtn->hide();
        ui->CreerCompteBtn->hide();
        ui->SupprimerTitulaireBtn->hide();
        ui->SupprimerCompteBtn_2->hide();
        ui->EditerSoldeBtn->show();
        ui->dateEdit->hide();
    }
    else
    {
        ui->SupprimerCompteBtn->show();
        ui->CreerCompteBtn->show();
        ui->SupprimerTitulaireBtn->show();
        ui->SupprimerCompteBtn_2->show();
        ui->EditerSoldeBtn->hide();
        ui->dateEdit->show();
    }

    ui->usernameLineEdit->setText("");
    ui->passwordLineEdit->setText("");
    ui->soldeLineEdit->setValidator(new QDoubleValidator(-999.0,999.0, 2, ui->soldeLineEdit));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->soldeWidgetLayout->hide();
}

void MainWindow::SupprimerChampsAjouterClient()
{
    ui->nomLineEdit->setText("");
    ui->telephoneLineEdit->setText("");
    ui->adresseLineEdit->setText("");
    ui->hommeRadioButton->setChecked(false);
    ui->femmeRadioButton->setChecked(false);
}

void MainWindow::MettreAJourClientDBFile()
{
    QFile clientsBDFile("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/BD/Clients.txt");
    if (!clientsBDFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
       qDebug() << "ERROR: le fichier client n'est pas ouvert.";
    }
    else
    {
        QTextStream stream(&clientsBDFile);
        foreach(Client client, clients)
        {
            stream << client;
        }
    }
    clientsBDFile.close();
    MettreAJourListeClient();
}

void MainWindow::MettreAJourListeClient()
{
    QFile clientsBDFile("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/BD/Clients.txt");
    int lineCount = 0;
    int attributeCount = 0;
    QString data;
    QString nom; QString tel; QString adr; bool sex;

    if(!clientsBDFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: le fichier client n'est pas ouvert.";
    }
    else
    {
        QTextStream in(&clientsBDFile);
        while(!in.atEnd())
        {
            attributeCount = lineCount % 4;
            data = in.readLine();

            if(attributeCount == 0)
            {
                nom = data;
            }
            else if(attributeCount == 1)
            {
                tel = data;
            }
            else if(attributeCount == 2)
            {
                adr = data;
            }
            else if(attributeCount == 3)
            {
                sex = data.toInt();
                Client clienteAAajouter(nom, tel, adr, sex);
                clients.insert(nom, clienteAAajouter);
            }
            lineCount++;
        }
    }
    clientsBDFile.close();

    // Mise à jour modèle
    Modelclient = new QStringListModel();
    QStringList clientsList;
    foreach(Client client, clients)
    {
        clientsList << client.getNom();
    }
    Modelclient->setStringList(clientsList);

    ui->clientsComboBox->setModel(Modelclient);
}

void MainWindow::MettreAJourCompteDBFile()
{
    QFile comptesBDFile("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/BD/Comptes.txt");
    if (!comptesBDFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
       qDebug() << "ERROR: le fichier des comptes n'est pas ouvert.";
    }
    else
    {
        QTextStream stream(&comptesBDFile);
        foreach(ComptesBancaire compte, comptes)
        {
            stream << compte;
        }
    }
    comptesBDFile.close();
    MettreAJourListeCompte();
}

void MainWindow::MettreAJourListeCompte()
{
    {
        QFile comptesBDFile("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/BD/Comptes.txt");
        int lineCount = 0;
        int attributeCount = 0;
        QString data;
        QString num; int type; QDate date; double solde;
        Client *client = new Client;

        if(!comptesBDFile.open(QIODevice::ReadOnly)){
            qDebug() << "ERROR:le fichier des comptes n'est pas ouvert";
        } else{
            QTextStream in(&comptesBDFile);
            while(!in.atEnd())
            {
                attributeCount = lineCount % 5;
                data = in.readLine();

                if(attributeCount == 0)
                {
                    num = data;
                }
                else if(attributeCount == 1)
                {
                    client = &clients[data];
                }
                else if(attributeCount == 2)
                {
                    type = data.toInt();
                }
                else if(attributeCount == 3)
                {
                    date = QDate::fromString(data,"dd MMMM yyyy");
                }
                else if(attributeCount == 4)
                {
                    solde = data.toDouble();
                    ComptesBancaire compteAAjouter(num, type, solde, date, client);
                    comptes.insert(num, compteAAjouter);
                }
                lineCount++;
            }
        }
        comptesBDFile.close();

        // Creer modèle populaire
        Modelcompte = new QStandardItemModel(this);
        Modelcompte->setColumnCount(5);
        Modelcompte->setRowCount(lineCount/5);
        Modelcompte->setHeaderData(0, Qt::Horizontal, QObject::tr("Numéro du compte"));
        Modelcompte->setHeaderData(1, Qt::Horizontal, QObject::tr("Titulaire"));
        Modelcompte->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
        Modelcompte->setHeaderData(3, Qt::Horizontal, QObject::tr("Date d'ouverture"));
        Modelcompte->setHeaderData(4, Qt::Horizontal, QObject::tr("Solde"));

        int index = 0;
        foreach(ComptesBancaire compte, comptes)
        {
            compte.modelPopulaire(Modelcompte, index);
            index++;
        }

        ui->TableDesComptes->setModel(Modelcompte);
    }
}


void MainWindow::on_Login_clicked()
{
    int validerEntree(QString, QString);
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    ui->errorLoginLabel->hide();

    int success = validerEntree(username, password);
    QString acceuil = (success == 1) ? "GESTIONNAIRE. Bienvenue, Fadi.": "";
    acceuil = (success == 2) ? "EMPLOYÉ. Bienvenue, Chadi.": acceuil;


    if(!success)
    {
        ui->errorLoginLabel->show();
    }
    else
    {
        ui->bienvenueLabel->setText(acceuil);
        ui->stackedWidget->setCurrentIndex(0);
        setupUI(success);
    }
}


void MainWindow::on_AnnulerModification_clicked()
{
    int ret =  QMessageBox::warning(this, tr("Quitter sans Sauvegarder"),
                       tr(" Voulez-vous vraiment quitter sans enregistrer?\n"),
                            QMessageBox::Cancel|QMessageBox::Yes);

    if(ret ==  QMessageBox::Yes)
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
}


void MainWindow::on_EnregistrerModification_clicked()
{
    int ligne = ui->TableDesComptes->currentIndex().row();
    QString titularireNom = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(ligne,1)).toString();
    QString compteNum = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(ligne,0)).toString();

    Client clientAEditer = clients[titularireNom];
    ComptesBancaire compteAEditer = comptes[compteNum];

    bool isHomme = (ui->hommeEdition->isChecked()) ?  true : false;

    clients[titularireNom].setData(ui->numeroEdition->text(),ui->telephoneEdition->text(),ui->directionEdition->text(), isHomme);

    ComptesBancaire *compte = new ComptesBancaire(comptes[compteNum].getNum(), comptes[compteNum].getType(), ui->soldeEdition->text().toDouble(), ui->DateEdition->date(), &clients[ui->numeroEdition->text()]);

    comptes.insert(comptes[compteNum].getNum(), *compte);

    MettreAJourClientDBFile();
    MettreAJourClientDBFile();
    MettreAJourListeClient();
    MettreAJourListeCompte();

    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_RetourBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}


void MainWindow::on_EditerSoldeBtn_clicked()
{
    bool ok;
    QString nouveauSolde = QInputDialog::getText(this, tr("Modifier le solde du compte"),
                                             "Solde Actuel: " + ui->soldeDetails->text() + "\nNouveau solde:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !nouveauSolde.isEmpty())
    {
        comptes[ui->numDetails->text()].setSolde(nouveauSolde.toDouble());
        ui->soldeDetails->setText(nouveauSolde);
        MettreAJourCompteDBFile();
        MettreAJourListeCompte();
    }
}


void MainWindow::on_EditerDonneesBtn_clicked()
{
    int fila = ui->TableDesComptes->currentIndex().row();
    QString titulaireNom = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(fila,1)).toString();
    QString compteNum = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(fila,0)).toString();

    Client clientAEditer = clients[titulaireNom];
    ComptesBancaire compteAEditer = comptes[compteNum];

    QString typeString;

    switch(compteAEditer.getType()){
    case 0:
        typeString = "Crédit";
        ui->soldeEdition->hide();
        ui->soldeLabelEdit->hide();
        break;
    case 1:
        typeString = "Débit";
        ui->soldeEdition->show();
        ui->soldeLabelEdit->show();
        break;
    case 2:
        typeString = "Cheques";
        ui->soldeEdition->hide();
        ui->soldeLabelEdit->hide();
        break;
    }

    ui->numeroEdition->setText(compteAEditer.getNum());
    ui->typeEdition->setText(typeString);
    ui->soldeEdition->setText(QString::number(compteAEditer.getSolde()));
    ui->DateEdition->setDate(compteAEditer.getDateOuverture());
    ui->numeroEdition->setText(clientAEditer.getNom());
    ui->telephoneEdition->setText(clientAEditer.getTelephone());
    ui->directionEdition->setText(clientAEditer.getAdresse());
    ui->hommeEdition->setChecked(clientAEditer.getSexe());
    ui->femmeEdition->setChecked(!clientAEditer.getSexe());

    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_SupprimerTitulaireBtn_clicked()
{
    QString nomClient = ui->nomDetails->text();
    QString comptesString;
    QList<QString> comptesASupprimer;

    foreach(ComptesBancaire compte, comptes){
        if(compte.getTitulaire()->getNom() == nomClient){
            comptesString += ("<br>" + compte.getNum());
            comptesASupprimer.append(compte.getNum());
        }
    }


    QMessageBox msgBox;
    msgBox.setText("Autoriser la suppression du client.");
    msgBox.setInformativeText(tr("<p> La suppression de ce client supprimera tous les comptes qui lui sont associés. <br>")
                         .append(   "Êtes-vous sûr de vouloir continuer? </p>")
                         .append("<b> Nom:  </b>" + nomClient + "<br>")
                         .append("<b> Compte: </b>" + comptesString)
                                 );

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes){

        clients.remove(nomClient);
        foreach(QString nomDuCompte, comptesASupprimer){
            comptes.remove(nomDuCompte);
        }

        MettreAJourClientDBFile();
        MettreAJourCompteDBFile();
        MettreAJourListeClient();
        MettreAJourListeCompte();
        ui->stackedWidget->setCurrentIndex(0);
    }
}


void MainWindow::on_SupprimerCompteBtn_2_clicked()
{
    QString num = ui->numDetails->text();
    QString client = ui->nomDetails->text();
    QString type = ui->typeDetails->text();
    double solde = ui->soldeDetails->text().toDouble();
    QString date = ui->dateDetails->text();


    QMessageBox msgBox;
    msgBox.setText("Autoriser la suppression du compte.");
    msgBox.setInformativeText(tr("<p> Vous voulez bien sûr supprimer le compte avec les informations suivantes? </p>")
                                 .append("<br><b>Num:</b> " + num)
                                 .append("<br><b>Titulaire:</b> " + client)
                                 .append("<br><b>Type:</b> " + type)
                                 .append("<br><b>Solde:</b> " + QString::number(solde))
                                 .append("<br><b>Date d'ouverture:</b> " + date));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes){
        comptes.remove(num);
        clients[client].SupprimerCompte(num);
        MettreAJourCompteDBFile();
        MettreAJourListeCompte();
        ui->stackedWidget->setCurrentIndex(0);
    }
}


void MainWindow::on_Annuler_Btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    SupprimerChampsAjouterClient();
}


void MainWindow::on_AjouterClientBtn_2_clicked()
{
    QString nom = ui->nomLineEdit->text();
    QString telephone = ui->telephoneLineEdit->text();
    QString adresse = ui->adresseLineEdit->text();
    bool sexe = (ui->hommeRadioButton->isChecked()? 1 : 0);
    QString sexeString = (sexe? "Homme" : "Femme");

    QMessageBox msgBox;
    msgBox.setText("Autoriser l'ajout d'un nouveau client.");
    msgBox.setInformativeText(tr("<p> Vous voulez bien sûr ajouter le nouveau client avec les informations suivantes? </p>")
                                 .append("<br><b>Nom:</b> " + nom)
                                 .append("<br><b>Téléphone:</b> " + telephone)
                                 .append("<br><b>Adresse:</b> " + adresse)
                                 .append("<br><b>Sexe:</b> " + sexeString));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes){
        auto clientAAjouter = new Client(nom, telephone, adresse, sexe);
        clients.insert(nom, *clientAAjouter);
        MettreAJourClientDBFile();
        ui->clientsComboBox->setCurrentText(clients[nom].getNom());
        ui->stackedWidget->setCurrentIndex(1);
        SupprimerChampsAjouterClient();
    }
}


void MainWindow::on_typeCompteComboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Crédit")
    {
        ui->soldeWidgetLayout->hide();
    } else
    {
        ui->soldeWidgetLayout->show();
    }
}


void MainWindow::on_Recherche_textChanged(const QString &inputText)
{
    if(inputText == "")
    {
        ui->TableDesComptes->setModel(Modelcompte);
        return;
    }

    auto Modelfilter = new QStandardItemModel(this);
    Modelfilter->setColumnCount(Modelcompte->columnCount());
    Modelfilter->setRowCount(Modelcompte->rowCount());
    Modelfilter->setHeaderData(0, Qt::Horizontal, QObject::tr("Num. du compte"));
    Modelfilter->setHeaderData(1, Qt::Horizontal, QObject::tr("Titulaire"));
    Modelfilter->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    Modelfilter->setHeaderData(3, Qt::Horizontal, QObject::tr("date d'ouverture"));
    Modelfilter->setHeaderData(4, Qt::Horizontal, QObject::tr("Solde"));

    int numLigne = 0;
    int numColAtribut = ui->filtreComboBox->currentIndex();
    int numElemFiltred = 0;

    foreach(ComptesBancaire compte, comptes)
    {
        QString contenido = Modelcompte->index(numLigne, numColAtribut).data().toString();
        if(contenido.contains(inputText))
        {
            compte.modelPopulaire(Modelfilter, numElemFiltred);
            numElemFiltred++;
        }
        numLigne++;
    }

    ui->TableDesComptes->setModel(Modelfilter);
}


void MainWindow::on_TableDesComptes_doubleClicked()
{
    on_SelectionnerCompteBtn_clicked();
}


void MainWindow::on_TableDesComptes_clicked()
{
    ui->SelectionnerCompteBtn->setEnabled(true);
    ui->SupprimerCompteBtn->setEnabled(true);
}

void MainWindow::ValiderAjouterClient()
{
    if (   !ui->nomLineEdit->text().isEmpty() &&
           !ui->telephoneLineEdit->text().isEmpty() &&
           !(ui->telephoneLineEdit->text().length() < 15) &&
           !ui->adresseLineEdit->text().isEmpty() &&
           (ui->hommeRadioButton->isChecked() ||
            ui->femmeRadioButton->isChecked()))
    {
        ui->AjouterClientBtn_2->setEnabled(true);
    }
    else
    {
        ui->AjouterClientBtn_2->setEnabled(false);
    }
}

void MainWindow::ValiderAjouterCompte()
{
    if(ui->numCompteLineEdit->text().length() < 19 || ui->clientsComboBox->currentText() == "")
    {
        ui->AjouterCompteBtn->setEnabled(false);
    }
    else
    {
        ui->AjouterCompteBtn->setEnabled(true);
    }
}

void MainWindow::ValiderChampsLogin()
{
    if(ui->usernameLineEdit->text() != "")
    {
        ui->usernameLineEdit->setStyleSheet("#usernameLineEdit{ border-bottom : 2px solid #BAD296; }");
    }
    else
    {
        ui->usernameLineEdit->setStyleSheet("#usernameLineEdit{ border-bottom : 2px solid #E3EBF5; }");
    }

    if(ui->passwordLineEdit->text() != "")
    {
        ui->passwordLineEdit->setStyleSheet("#passwordLineEdit{ border-bottom : 2px solid #BAD296; }");
    }
    else
    {
        ui->passwordLineEdit->setStyleSheet("#passwordLineEdit{ border-bottom : 2px solid #E3EBF5; }");
    }

    if(ui->usernameLineEdit->text() == "" || ui->passwordLineEdit->text() == "")
    {
        ui->Login->setEnabled(false);
    }
    else
    {
        ui->Login->setEnabled(true);
    }
}


void MainWindow::on_SupprimerCompteBtn_clicked()
{
    QModelIndex index = ui->TableDesComptes->currentIndex();
    int currentRow = index.row();
    QString titulaireNomString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,1)).toString();
    QString compteNomString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,0)).toString();
    QString typeString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,2)).toString();
    QString soldeString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,4)).toString();
    QString dateString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,3)).toString();

    QMessageBox msgBox;
    msgBox.setText("Autoriser la suppression du compte.");
    msgBox.setInformativeText(tr("<p> Vous voulez bien sûr supprimer le compte avec les informations suivantes? </p>")
                                 .append("<br><b>Num:</b> " + compteNomString)
                                 .append("<br><b>Titulaire:</b> " + titulaireNomString)
                                 .append("<br><b>Type:</b> " + typeString)
                                 .append("<br><b>Solde:</b> " + soldeString)
                                 .append("<br><b>Date d'ouverture:</b> " + dateString));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        comptes.remove(compteNomString);
        clients[titulaireNomString].SupprimerCompte(compteNomString);
        MettreAJourCompteDBFile();
        MettreAJourListeCompte();
    }
}


void MainWindow::on_SelectionnerCompteBtn_clicked()
{
    QModelIndex index = ui->TableDesComptes->currentIndex();
    int currentRow = index.row();
    QString titulaireNomString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,1)).toString();
    QString compteNumString = ui->TableDesComptes->model()->data(ui->TableDesComptes->model()->index(currentRow,0)).toString();

    ComptesBancaire *compteSelectione = &comptes[compteNumString];
    Client *clientSelectionne = &clients[titulaireNomString];

    if (clientSelectionne->getSexe())
    {
        ui->sexeDetails->setText("Homme");
    }
    else
    {
        ui->sexeDetails->setText("Femme");
    }

    if(compteSelectione->getType() == 0)
    {
        ui->soldeDetails->hide();
        ui->soldeLabelDetails->hide();
        ui->typeDetails->setText("Crédit");
        ui->EditerSoldeBtn->hide();
    }
    else
    {
        if(compteSelectione->getType() == 1)
            ui->typeDetails->setText("Débit");
        else
            ui->typeDetails->setText("Cheques");

        ui->soldeDetails->show();
        ui->soldeLabelDetails->show();
        ui->EditerSoldeBtn->show();
    }

    ui->numDetails->setText(compteSelectione->getNum());
    ui->numDetails_2->setText(compteSelectione->getNum());
    ui->soldeDetails->setText("$" + QString::number(compteSelectione->getSolde()));
    ui->dateDetails->setText(compteSelectione->getDateOuverture().toString("dd MMMM yyyy"));
    ui->nomDetails->setText(clientSelectionne->getNom());
    ui->nomDetails_2->setText(clientSelectionne->getNom());
    ui->telephoneDetails->setText(clientSelectionne->getTelephone());
    ui->adresseDetails->setText(clientSelectionne->getAdresse());

    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_ImprimerRapportBtn_clicked()
{
    QMessageBox::information(this, "Opération réussie", "Un fichier de rapport au format pdf a été généré avec succès dans le dossier du projet.",
                                QMessageBox::Ok);

    QString comptesString, clientsString;

    foreach(ComptesBancaire compte, comptes)
    {

        QString typeString;
        QString soldeString;

        switch(compte.getType()){
        case 0:
            typeString = "Crédit";
            soldeString = "--";
            break;
        case 1:
            typeString = "Débit";
            soldeString = "$" + QString::number(compte.getSolde());
            break;
        case 2:
            typeString = "Cheques";
            soldeString = "--";
            break;
        }

        comptesString += ""
                + compte.getNum() + " | "
                "<span style=\"color:grey\">"
                + compte.getTitulaire()->getNom().remove(10,50) + "...  | "
                + typeString + " |  "
                + compte.getDateOuverture().toString("dd/MM/yy") + "  | "
                + soldeString +
                "</span>"
                + "<br> <br>";
    }

    foreach(Client client, clients)
    {

        QString sexeString = (client.getSexe() ? "Homme" : "Femme");

        clientsString += ""
                "<h3>" + client.getNom() + "</h3>"
                "Téléphone:    " + client.getTelephone() + "<br>"
                "Adresse:   " + client.getAdresse() + "<br>"
                "Sexe:           " + sexeString + "<br>"
                "<hr>";
    }

    QString html = ""
            "<div align=right>"
                "Rapport généré le " + QDate::currentDate().toString("dd MMMM yyyy") +
            "</div>"
            "<div>"
                "<img width=100 align=left src=\":/assets/assets/logo.png\" alt=\"COIN bank\"></img><br>"
                "<h2 align=center>RAPPORT DES COMPTES BANCAIRES</h2>"
                "<hr>"
                "<p style=\"white-space:pre\">"
                    "<span style=\"background:'#E8E8E8'\">NUM. DE COMPTE     |     TITULAIRE    |   TYPE   |   DATE   |   SOLDE     </span><br><br>"
                    + comptesString +
                "</p>"
                "<hr><br>"
                "Date du reporte: " + QDate::currentDate().toString("dd MMMM yyyy") +
            "</div>"

            "<div style=\"page-break-before:always\">"
                "<img width=100  align=left src=\":/assets/assets/logo.png\" alt=\"COIN bank\"></img><br>"
            "</div>"
            "<div>"
                "<h2 align=center>RAPPORT DES CLIENTS</h2>"
                "<hr>"
                "<p style=\"white-space:pre\">"
                    + clientsString +
                "</p>"
                "<hr><br>"
                "<div align=right>"
                    "Rapport généré le " + QDate::currentDate().toString("dd MMMM yyyy") +
                "</div>"
            "</div>"
                   "";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer. setPageSize(QPageSize::A4);
    printer.setOutputFileName("C:/Users/f.bouzazi/Desktop/Qt_Training/BanqueTN/BD/Report.pdf");
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
    document.print(&printer);
    printer.newPage();
    document.print(&printer);
}


void MainWindow::on_AjouterClientBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_AnnulerBtn_clicked()
{
    ui->clientsComboBox->setCurrentIndex(0);
    ui->typeCompteComboBox->setCurrentIndex(0);
    ui->soldeLineEdit->text() = "";
    ui->dateEdit->setDate(QDate::currentDate());
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_AjouterCompteBtn_clicked()
{
    QString num = ui->numCompteLineEdit->text();
    Client *client = &clients[ui->clientsComboBox->currentText()];
    int type = ui->typeCompteComboBox->currentIndex();
    double solde = ui->soldeLineEdit->text().toDouble();
    QDate date = ui->dateEdit->date();
    QString soldeString = QString::number(solde);

    QString typeString;
    switch(type){
    case 0:
        typeString = "Crédit";
        soldeString = "-";
        break;
    case 1:
        typeString = "Débit";
        break;
    case 2:
        typeString = "Cheques";
        break;
    }


    QMessageBox msgBox;
    msgBox.setText("Autoriser l'ajout d'un nouveau compte.");
    msgBox.setInformativeText(tr("<p> Vous voulez bien sûr ajouter le nouveau compte avec les informations suivantes? </p>")
                                 .append("<br><b>Num:</b> " + num)
                                 .append("<br><b>Titulaire:</b> " + client->getNom())
                                 .append("<br><b>Type:</b> " + typeString)
                                 .append("<br><b>Solde:</b> " + soldeString)
                                 .append("<br><b>Date d'ouverture:</b> " + date.toString("dd MMMM yyyy")));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes){
        auto compteAjouter = new ComptesBancaire(num, type, solde, date, client);
        client->InsererCompte(num, *compteAjouter);
        comptes.insert(num, *compteAjouter);

        MettreAJourCompteDBFile();
        on_AnnulerBtn_clicked();
    }
}

int validerEntree(QString username, QString password)
{
    if(username == "fadibouzazi")
    {
        if(password == "azerty")
        {
            return 1;
        }
    }
    else if(username == "chadi.bouzazi")
    {
        if(password == "chadi123")
        {
            return 2;
        }
    }
    return 0;
}


void MainWindow::on_FermerSessionBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_CreerCompteBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QString randomCompteNum;
    for(int i = 0; i < 16; i++)
        randomCompteNum += QString::number(rand()%10);
    ui->numCompteLineEdit->setText(randomCompteNum);
}

