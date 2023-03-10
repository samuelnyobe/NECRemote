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
    void changeTab();
    void openAdminPage();
    void saveConfigs();
    void onOff(int remote);
    void changeBrightness(int remote);
    void changePort(QPushButton *btn, int remote, int port);
    void connected_1();
    void disconnected_1();
    void connected_2();
    void disconnected_2();
    void onConnexionStatusChanged_1();
    void onConnexionStatusChanged_2();


private:
    Ui::Remote *ui;
};
#endif // REMOTE_H
