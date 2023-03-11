#ifndef REMOTE_H
#define REMOTE_H

#include <QDialog>
#include <QTcpSocket>
#include <QCheckBox>
#include <QPushButton>

#include <connectthread.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Remote; }
QT_END_NAMESPACE

class Remote : public QDialog
{
    Q_OBJECT

public:
    Remote(QWidget *parent = nullptr);
    ~Remote();

private:
    QString active_commandData;

    bool is_connected_1;
    QDataStream socketStream_1;
    QByteArray byteArray_1;

    bool is_connected_2;
    QDataStream socketStream_2;
    QByteArray byteArray_2;

    QString greenColor;
    QString orangeColor;
    QString grayColor;

    QPushButton *currentPortBtn_1 = nullptr;
    QPushButton *currentPortBtn_2 = nullptr;
    QCheckBox *currentCheckBox_1 = nullptr;
    QCheckBox *currentCheckBox_2 = nullptr;

    bool powerState_1;
    bool powerState_2;
    QString host_1;
    QString host_2;
    QString password;

    QTcpSocket *tcpSocket_1 = nullptr;
    QTcpSocket *tcpSocket_2 = nullptr;

    ConnectThread *connectThread = nullptr;

public slots:
    void changeTab(); // Pour changer d'onglet
    void openAdminPage(); // Pour gérer les affichages des page sur l'onglet Admin
    void saveConfigs(); // Pour sauvegarder les configs dans la page Admin
    void onOff(int remote); // Pour éteindre ou allumer un des vidéoprojecteurs
    void changeBrightness(int remote); // Pour changer la luminausité sur un des vidéoprojecteurs
    void changePort(QPushButton *btn, int remote, int port); //Pour changer de port sur un des vidéoprojecteurs
    void disconnected_1(); // Traitement à effectuer quand on reçu une déconnexion sur le vidéoprojecteur 1
    void disconnected_2(); // Traitement à effectuer quand on reçu une déconnexion sur le vidéoprojecteur 2
    void onConnexionStatusChanged_1(); // Si le Thread a envoyé un signal de nouvelle connexion détecté sur le vidéprojecteur 1
    void onConnexionStatusChanged_2(); // Si le Thread a envoyé un signal de nouvelle connexion détecté sur le vidéprojecteur 2

private:
    Ui::Remote *ui;
};
#endif // REMOTE_H
