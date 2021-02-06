#include "client.h"
#include "ui_client.h"

#include <QHostAddress>

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
    , _socket(this)
{
    ui->setupUi(this);
    ui->lineIP->setText("192.168.0.164");
    ui->linePort->setText("7");

    timer1 = new QTimer(this);

    connected = false;

    connect(timer1, SIGNAL(timeout()), this, SLOT(client_sendCallback()));
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
    timer1->start(500);
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
                timer1->stop();
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
        client_displayWheather(temperature, humidity, pressure);
        break;
    }
    default:
        break;
    }
}

void Client::client_displayWheather(QString temperature,QString humidity, QString pressure){
    ui->labelTemperature->setText(temperature);
    ui->labelHumidity->setText(humidity);
    ui->labelPressure->setText(pressure);
}

void Client::client_sendCallback(){
    QString textStr = "GET_PARAMS";
    QByteArray bytes = textStr.toUtf8();
    _socket.write(bytes);
}
