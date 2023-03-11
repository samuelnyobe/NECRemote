/*
  Cette classe permet de créer un Thread qui va simplement tenter de se connecter
  au vidéoprojecteur chaque fois qu'on après une certaine durée.
  Si on est déconnecté, et qu'un jour on a établit une nouvelle connexion,
  elle envoie un signal à la classe principale lui signalant une connexion
  et alors cette classe peut se connecter à nouveau
*/

#ifndef CONNECTTHREAD_H
#define CONNECTTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ConnectThread : public QThread
{
    Q_OBJECT
public:
    explicit ConnectThread(QObject *parent = nullptr);
    // Ce constructeur utilise les variables de la classe principale tels que les hosts
    explicit ConnectThread(QObject *parent = nullptr, bool is_connected_1 = false, bool is_connected_2 = false,
                           QString host_1 = "", QString host_2 = "");
    void run();
    ~ConnectThread();

public:
    QString host_1;
    QString host_2;
    bool is_connected_1;
    bool is_connected_2;
    QTcpSocket *socket_1 = nullptr;
    QTcpSocket *socket_2 = nullptr;
    bool kill;

signals:
    void connexionStatusChanged_1(); // Signal émit à la première télécommande
    void connexionStatusChanged_2(); // Signal émit à la deuxième télécommande

};

#endif // CONNECTTHREAD_H
