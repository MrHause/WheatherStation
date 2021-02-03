#include "client.h"
#include "ui_client.h"

#include <QHostAddress>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
    , _socket(this)
{
    ui->setupUi(this);
    ui->lineIP->setText("127.0.0.1");
    ui->linePort->setText("100");

    connected = false;

    connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&_socket, SIGNAL(connected()), this, SLOT(client_connected()));
    connect(ui->ButtonConnect, SIGNAL(released()), this, SLOT(buttonConnectPressed()));
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendpressed()));
    connect(ui->ButtonClear, SIGNAL(released()), this, SLOT(buttonClearpressed()));
}

Client::~Client()
{
    delete ui;
}

void Client::onReadyRead()
{
    QByteArray datas = _socket.readAll();  
    QString msg(datas);
    ui->textReceive->append(msg);
    client_parseResponse(msg);
    qDebug() << datas;
}

void Client::client_connected(){
    ui->lineStatus->setText("Connected");
    ui->ButtonConnect->setText("disconnect");
    connected = true;
}

void Client::buttonClearpressed(){
    ui->textReceive->clear();
}

void Client::buttonConnectPressed(){
    if(!connected){
        QString Ip_str = ui->lineIP->text();
        QString Port_str = ui->linePort->text();
          _socket.connectToHost(QHostAddress(Ip_str), Port_str.toInt());
    }else{
        _socket.disconnectFromHost();
        if (_socket.state() == QAbstractSocket::UnconnectedState
            || _socket.waitForDisconnected(1000)) {
                ui->lineStatus->setText("Disconnected");
                ui->ButtonConnect->setText("connect");
                connected = false;
        }
    }
}

void Client::buttonSendpressed(){
    QString textStr = ui->lineSend->text();
    QByteArray bytes = textStr.toUtf8();
    _socket.write(bytes);
}

void Client::client_parseResponse(QString response){
    qsizetype idx = response.indexOf(":");
    QString command_str = response.left(idx);
    int command = command_str.toInt();
    switch(command){
    case GET_WEATHER_PARAM:{
        qsizetype idx2 = response.indexOf(":",idx+1);
        qsizetype idx3 = response.indexOf(":",idx2+1);
        QString temperature = response.mid(idx+1, idx2-idx-1);
        QString humidity = response.mid(idx2+1, idx3-idx2-1);
        QString pressure = response.mid(idx3+1, response.length());
        break;
    }
    default:
        break;
    }
}

