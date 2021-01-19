#include "client.h"
#include "ui_client.h"

#include <QHostAddress>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
    , _socket(this)
{
    ui->setupUi(this);

    connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(ui->ButtonConnect, SIGNAL(released()), this, SLOT(buttonConnectPressed()));
    connect(ui->ButtonSend, SIGNAL(released()), this, SLOT(buttonSendpressed()));
}

Client::~Client()
{
    delete ui;
}

void Client::onReadyRead()
{
    QByteArray datas = _socket.readAll();
    qDebug() << datas;
}

void Client::buttonConnectPressed(){
      _socket.connectToHost(QHostAddress("127.0.0.1"), 100);
}

void Client::buttonSendpressed(){
    QString textStr = ui->lineSend->text();
    QByteArray bytes = textStr.toUtf8();
    _socket.write(bytes);
}

