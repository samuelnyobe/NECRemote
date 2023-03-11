#include "remote.h"
#include "ui_remote.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QShortcut>
#include <QFile>

Remote::Remote(QWidget *parent)
    : QDialog(parent),
      greenColor("background-color:rgb(46, 194, 126);"),
      orangeColor("background-color:rgb(255, 120, 0);"),
      grayColor("background-color:gray"),

      currentPortBtn_1(new QPushButton("")),
      currentPortBtn_2(new QPushButton("")),
      currentCheckBox_1(new QCheckBox()),
      currentCheckBox_2(new QCheckBox()),

      tcpSocket_1(new QTcpSocket(this)),
      tcpSocket_2(new QTcpSocket(this))
    , ui(new Ui::Remote)
{
    ui->setupUi(this);

    // On commence par lire les infos du ficher pour récuper
    // mot de passe, ip du vidéoprojecteur 1 et celle du second
    QFile f{":/utils.txt"};
    QString allDataFile = "";
    password = "7777";
    if (f.exists()) {
        if (f.open(QIODevice::ReadOnly)) {
            QTextStream in{&f};
            if(!in.atEnd()){
                allDataFile = in.readLine();
                QStringList allDataFile_split = allDataFile.split('-');
                password = allDataFile_split[0];
                host_1 = allDataFile_split[1];
                host_2 = allDataFile_split[2];
            }
        }
    }

    // On tente une première connexion sur le vidéoprojecteur 1
    tcpSocket_1->connectToHost(host_1, 7142);
    if(tcpSocket_1->waitForConnected(1000)){
        is_connected_1 = true;
        ui->btn_on_1->setStyleSheet("background-color:red;");
        powerState_1 = true;
        ui->lbl_connected_1->setText("Connected");
        ui->lbl_connected_1->setStyleSheet("color:green; font-size:25px;");
    }else{
        is_connected_1 = false;
        ui->btn_on_1->setStyleSheet("background-color:green;");
        powerState_1 = false;
        ui->sl_1->setEnabled(false);
        ui->lbl_connected_1->setText("Not Connected");
        ui->lbl_connected_1->setStyleSheet("color:red; font-size:25px;");
    }
    // Si on est déconnecté soudainement, on bloque tout les bouttons
    connect(tcpSocket_1, &QTcpSocket::disconnected, this, &Remote::disconnected_1);

    // On tente une première connexion sur le vidéoprojecteur 2
    tcpSocket_2->connectToHost(host_2, 7142);
    if(tcpSocket_2->waitForConnected(1000)){
        is_connected_2 = true;
        ui->btn_on_2->setStyleSheet("background-color:red;");
        powerState_2 = true;
        ui->lbl_connected_2->setText("Connected");
        ui->lbl_connected_2->setStyleSheet("color:green; font-size:25px;");
    }else{
        is_connected_2 = false;
        ui->btn_on_2->setStyleSheet("background-color:green;");
        powerState_2 = false;
        ui->sl_2->setEnabled(false);
        ui->lbl_connected_2->setText("Not Connected");
        ui->lbl_connected_2->setStyleSheet("color:red; font-size:25px;");
    }
    connect(tcpSocket_2, &QTcpSocket::disconnected, this, &Remote::disconnected_2);

    ui->frame_7->hide();
    ui->frame_5->hide();

    // Toute donnée reçu à entrée sera redirigé sur la socket
    socketStream_1.setDevice(tcpSocket_1);
    //    socketStream_1.setVersion(QDataStream::Qt_6_4);
    socketStream_2.setDevice(tcpSocket_2);
    //    socketStream_2.setVersion(QDataStream::Qt_6_4);

    // On crée le Thread qui va surveiller les connexions nouvelles si on est déconnecté
    connectThread = new ConnectThread(this, is_connected_1, is_connected_2, host_1, host_2);
    connectThread->start();
    connect(connectThread, &ConnectThread::connexionStatusChanged_1, this, &Remote::onConnexionStatusChanged_1);
    connect(connectThread, &ConnectThread::connexionStatusChanged_2, this, &Remote::onConnexionStatusChanged_2);

    // Les bouttons et checkbox courrants
    currentPortBtn_1 = ui->pushButton_1;
    currentPortBtn_2 = ui->pushButton_10;
    currentCheckBox_1 = ui->checkBox_1;
    currentCheckBox_2 = ui->checkBox_10;

    // On définit l'affichage de la luminausité à 50
    ui->lbl_sl_1->setText("50");
    ui->lbl_sl_2->setText("50");

    // On contraint le mot de passe à être que de taille 4 chiffres
    //    ui->lineEdit->setValidator(new QIntValidator(1000, 9999, this));
    QRegularExpression rx(R"(\d\d\d\d)");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(rx, this));

    // On souhaite que lorsque l'on tape sur le boutton Entrée, que cet évènement soit vu comme un clic
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Return), ui->frame_4);
    QObject::connect(shortcut, &QShortcut::activated, ui->bnt_validate, &QPushButton::click);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &Remote::changeTab);
    connect(ui->bnt_validate, &QPushButton::clicked, this, &Remote::openAdminPage);
    connect(ui->btn_saveConfigs, &QPushButton::clicked, this, &Remote::saveConfigs);

    // Les connect pour les boutton de changement de port
    connect(ui->pushButton_1, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_1, 1, 1);});
    connect(ui->pushButton_2, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_2, 1, 2);});
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_3, 1, 3);});
    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_4, 1, 4);});
    connect(ui->pushButton_5, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_5, 1, 5);});
    connect(ui->pushButton_6, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_6, 1, 6);});
    connect(ui->pushButton_7, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_7, 1, 7);});
    connect(ui->pushButton_8, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_8, 1, 8);});
    connect(ui->pushButton_9, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_9, 1, 9);});


    connect(ui->pushButton_10, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_10, 2, 1);});
    connect(ui->pushButton_11, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_11, 2, 2);});
    connect(ui->pushButton_12, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_12, 2, 3);});
    connect(ui->pushButton_13, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_13, 2, 4);});
    connect(ui->pushButton_14, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_14, 2, 5);});
    connect(ui->pushButton_15, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_15, 2, 6);});
    connect(ui->pushButton_16, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_16, 2, 7);});
    connect(ui->pushButton_17, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_17, 2, 8);});
    connect(ui->pushButton_18, &QPushButton::clicked, this, [=](){Remote::changePort(ui->pushButton_18, 2, 9);});


    connect(ui->sl_1, &QSlider::valueChanged, this, [=](){Remote::changeBrightness(1);});
    connect(ui->sl_2, &QSlider::valueChanged, this, [=](){Remote::changeBrightness(2);});
    connect(ui->btn_on_1, &QPushButton::clicked, this, [=](){Remote::onOff(1);});
    connect(ui->btn_on_2, &QPushButton::clicked, this, [=](){Remote::onOff(2);});

}

Remote::~Remote()
{
    if(tcpSocket_1->isOpen())
        tcpSocket_1->close();
    if(tcpSocket_2->isOpen())
        tcpSocket_2->close();

    connectThread->kill = true;

    delete ui;
}

void Remote::changeTab()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit->setText("");
}

void Remote::openAdminPage()
{
    QString pw = ui->lineEdit->text();
    if(pw  == password)
        ui->stackedWidget->setCurrentIndex(1);
    ui->lineEdit->setText("");
}

void Remote::saveConfigs()
{
    QVector<bool> checksPort = {
        ui->checkBox_1->isChecked(),
        ui->checkBox_2->isChecked(),
        ui->checkBox_3->isChecked(),
        ui->checkBox_4->isChecked(),
        ui->checkBox_5->isChecked(),
        ui->checkBox_6->isChecked(),
        ui->checkBox_7->isChecked(),
        ui->checkBox_8->isChecked(),
        ui->checkBox_9->isChecked(),
        ui->checkBox_10->isChecked(),
        ui->checkBox_11->isChecked(),
        ui->checkBox_12->isChecked(),
        ui->checkBox_13->isChecked(),
        ui->checkBox_14->isChecked(),
        ui->checkBox_15->isChecked(),
        ui->checkBox_16->isChecked(),
        ui->checkBox_17->isChecked(),
        ui->checkBox_18->isChecked()
    };

    QVector <QPushButton*> allBtn = {
        ui->pushButton_1,
        ui->pushButton_2,
        ui->pushButton_3,
        ui->pushButton_4,
        ui->pushButton_5,
        ui->pushButton_6,
        ui->pushButton_7,
        ui->pushButton_8,
        ui->pushButton_9,
        ui->pushButton_10,
        ui->pushButton_11,
        ui->pushButton_12,
        ui->pushButton_13,
        ui->pushButton_14,
        ui->pushButton_15,
        ui->pushButton_16,
        ui->pushButton_17,
        ui->pushButton_18,

    };
    for (int i = 0; i < 18; ++i) {
        if(checksPort.at(i)){
            if(allBtn.at(i) != currentPortBtn_1 && allBtn.at(i) != currentPortBtn_2){
                allBtn.at(i)->setStyleSheet(orangeColor);
                allBtn.at(i)->setEnabled(true);
            }
        }else{
            allBtn.at(i)->setStyleSheet(grayColor);
            allBtn.at(i)->setEnabled(false);
        }
    }
    ui->tabWidget->setCurrentIndex(0);
}

void Remote::onOff(int remote)
{
    if(remote == 1){
        if(is_connected_1){
            if(tcpSocket_1->isOpen())
            {
                active_commandData = powerState_1?"0x02 0x01 0x00 0x00 0x00 0x03":"0x02 0x00 0x00 0x00 0x00 0x02";
                (powerState_1)?ui->btn_on_1->setStyleSheet("background-color:green;"):ui->btn_on_1->setStyleSheet("background-color:red;");
                powerState_1 = !powerState_1;
                ui->sl_1->setEnabled(powerState_1);
                if(powerState_1)
                    ui->frame_7->hide();
                byteArray_1 = active_commandData.toUtf8();
                socketStream_1 << byteArray_1;
            }
        }
    }else{
        if(is_connected_2){
            if(tcpSocket_2->isOpen())
            {
                active_commandData = powerState_2?"0x02 0x01 0x00 0x00 0x00 0x03":"0x02 0x00 0x00 0x00 0x00 0x02";
                (powerState_2)?ui->btn_on_2->setStyleSheet("background-color:green;"):ui->btn_on_2->setStyleSheet("background-color:red;");
                powerState_2= !powerState_2;
                ui->sl_2->setEnabled(powerState_2);
                if(powerState_2)
                    ui->frame_5->hide();
                byteArray_2 = active_commandData.toUtf8();
                socketStream_2 << byteArray_2;
            }
        }
    }
}

void Remote::changeBrightness(int remote)
{
    QChar fillChar = u'0';
    int val = (remote==1)?ui->sl_1->value():ui->sl_2->value();
    QString hexvalue = tr("%1").arg(val, 4, 16, fillChar).toUpper();
    bool ok;
    QString lowDigits = hexvalue.last(2);
    QString highDigits = hexvalue.first(2);

    auto cks_int = 279; // La somme entière des premiers bits
    cks_int = cks_int + lowDigits.toInt(&ok, 16) + highDigits.toInt(&ok, 16);

    QString cks = tr("%1").arg(cks_int, 2, 16, fillChar).toUpper();
    cks = cks.last(2);
    // On fait la requete pour changer la luminosité
    active_commandData = "0x03 0x10 0x00 0x00 0x05 0x00 0xFF 0x00 0x" + lowDigits + " 0x" + highDigits + " 0x" + cks;
    if(remote == 1){
        if(powerState_1){
            ui->lbl_sl_1->setText(QString::number(val));
            if(tcpSocket_1->isOpen())
            {
                // On envoie la commande
                byteArray_1 = active_commandData.toUtf8();
                socketStream_1 << byteArray_1;
            }
        }
    }else{
        if(powerState_2){
            ui->lbl_sl_2->setText(QString::number(val));
            if(tcpSocket_2->isOpen())
            {
                // On envoie la commande
                byteArray_2 = active_commandData.toUtf8();
                socketStream_2 << byteArray_2;
            }
        }
    }
}


void Remote::changePort(QPushButton *btn, int remote, int port)
{
    QCheckBox *tmpCheckBox = nullptr;
    switch (port) {
    case 1:
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xA1 0xA9";
        tmpCheckBox = (remote == 1)?ui->checkBox_1:ui->checkBox_10;
        break;
    case 2:
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xA2 0xAA";
        tmpCheckBox = (remote == 1)?ui->checkBox_2:ui->checkBox_11;
        break;
    case 3:
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xA6 0xAE";
        tmpCheckBox = (remote == 1)?ui->checkBox_3:ui->checkBox_12;
        break;
    case 4:
        tmpCheckBox = (remote == 1)?ui->checkBox_4:ui->checkBox_13;
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xA7 0xAF";
        break;
    case 5:
        tmpCheckBox = (remote == 1)?ui->checkBox_5:ui->checkBox_14;
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xC4 0xCC";
        break;
    case 6:
        tmpCheckBox = (remote == 1)?ui->checkBox_6:ui->checkBox_15;
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xC5 0xCD";
        break;
    case 7:
        tmpCheckBox = (remote == 1)?ui->checkBox_7:ui->checkBox_16;
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xC6 0xCE";
        break;
    case 8:
        tmpCheckBox = (remote == 1)?ui->checkBox_8:ui->checkBox_17;
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xC7 0xCF";
        break;
    case 9:
        tmpCheckBox = (remote == 1)?ui->checkBox_9:ui->checkBox_18;
        active_commandData = "0x02 0x03 0x00 0x00 0x02 0x01 0xBF 0xC7";
        break;
    default:
        break;
    }
    if(remote == 1){
        if(powerState_1){
            if(tcpSocket_1->isOpen())
            {
                if(btn != currentPortBtn_1){
                    currentPortBtn_1->setStyleSheet(orangeColor);
                    currentCheckBox_1->setEnabled(true);
                    btn->setStyleSheet(greenColor);
                    currentPortBtn_1 = btn;
                    // On envoie la commande
                    byteArray_1 = active_commandData.toUtf8();
                    socketStream_1 << byteArray_1;

                    currentCheckBox_1 = tmpCheckBox;
                    currentCheckBox_1->setEnabled(false);
                }
            }
        }else
            ui->frame_7->show();
    }else{
        if(powerState_2){
            if(tcpSocket_2->isOpen())
            {
                if(btn != currentPortBtn_2){
                    currentPortBtn_2->setStyleSheet(orangeColor);
                    currentCheckBox_2->setEnabled(true);
                    btn->setStyleSheet(greenColor);
                    currentPortBtn_2 = btn;
                    // On envoie la commande
                    byteArray_2 = active_commandData.toUtf8();
                    socketStream_2 << byteArray_2;

                    currentCheckBox_2 = tmpCheckBox;
                    currentCheckBox_2->setEnabled(false);
                }
            }
        }else {
            ui->frame_5->show();
        }
    }
}

void Remote::disconnected_1()
{
    ui->lbl_connected_1->setText("Not Connected");
    ui->lbl_connected_1->setStyleSheet("color:red; font-size:25px;");
    is_connected_1 = false;
    powerState_1 = false;
    ui->frame_7->hide();
    ui->btn_on_1->setStyleSheet("background-color:green;");
    ui->sl_1->setEnabled(false);
    connectThread->is_connected_1 = false;
}

void Remote::disconnected_2()
{
    ui->lbl_connected_2->setText("Not Connected");
    ui->lbl_connected_2->setStyleSheet("color:red; font-size:25px;");
    is_connected_2 = false;
    powerState_2 = false;
    ui->frame_5->hide();
    ui->btn_on_2->setStyleSheet("background-color:green;");
    ui->sl_2->setEnabled(false);
    connectThread->is_connected_2 = false;
}

void Remote::onConnexionStatusChanged_1()
{
    tcpSocket_1->abort();
    tcpSocket_1->connectToHost(host_1, 7142);
    if(tcpSocket_1->waitForConnected()){
        is_connected_1 = true;
        ui->btn_on_1->setStyleSheet("background-color:red;");
        powerState_1 = true;
        ui->sl_1->setEnabled(true);
        ui->frame_7->hide();
        ui->lbl_connected_1->setText("Connected");
        ui->lbl_connected_1->setStyleSheet("color:green; font-size:25px;");
    }
    connect(tcpSocket_1, &QTcpSocket::disconnected, this, &Remote::disconnected_1);
}

void Remote::onConnexionStatusChanged_2()
{
    tcpSocket_2->abort();
    tcpSocket_2->connectToHost(host_2, 7142);
    if(tcpSocket_2->waitForConnected()){
        is_connected_2 = true;
        ui->btn_on_2->setStyleSheet("background-color:red;");
        powerState_2 = true;
        ui->sl_2->setEnabled(true);
        ui->frame_5->hide();
        ui->lbl_connected_2->setText("Connected");
        ui->lbl_connected_2->setStyleSheet("color:green; font-size:25px;");
    }
    connect(tcpSocket_2, &QTcpSocket::disconnected, this, &Remote::disconnected_2);
}

