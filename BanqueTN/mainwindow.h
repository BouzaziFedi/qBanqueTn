#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtCore>
#include "comptesbancaire.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Méthode utiles
    void setupUI(int);
    void SupprimerChampsAjouterClient();
    // "Base de Données"
    void MettreAJourClientDBFile();
    void MettreAJourListeClient();
    void MettreAJourCompteDBFile();
    void MettreAJourListeCompte();

private slots:
    void on_Login_clicked();

    void on_AnnulerModification_clicked();

    void on_EnregistrerModification_clicked();

    void on_RetourBtn_clicked();

    void on_EditerSoldeBtn_clicked();

    void on_EditerDonneesBtn_clicked();

    void on_SupprimerTitulaireBtn_clicked();

    void on_SupprimerCompteBtn_2_clicked();

    void on_Annuler_Btn_2_clicked();

    void on_AjouterClientBtn_2_clicked();

    void on_typeCompteComboBox_currentTextChanged(const QString &arg1);

    void on_Recherche_textChanged(const QString &arg1);

    void on_TableDesComptes_doubleClicked();

    void on_TableDesComptes_clicked();


    void ValiderAjouterClient();

    void ValiderAjouterCompte();

    void ValiderChampsLogin();

    void on_SupprimerCompteBtn_clicked();

    void on_SelectionnerCompteBtn_clicked();

    void on_ImprimerRapportBtn_clicked();

    void on_AjouterClientBtn_clicked();

    void on_AnnulerBtn_clicked();

    void on_AjouterCompteBtn_clicked();


    void on_FermerSessionBtn_clicked();

    void on_CreerCompteBtn_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *Modelcompte;
    QStringListModel *Modelclient;
    QMap<QString, ComptesBancaire> comptes;
    QMap<QString, Client> clients;
};
#endif // MAINWINDOW_H
