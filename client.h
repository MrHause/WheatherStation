#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void onReadyRead();
    void client_connected();
    void buttonConnectPressed();
    void buttonSendpressed();
    void buttonClearpressed();
    void client_sendCallback();

private:
    Ui::Client *ui;
    QTcpSocket _socket;
    bool connected;
    QTimer *timer1;

    typedef enum{
        LED2_ON,
        LED2_OFF,
        LED2_TOG,
        LED3_ON,
        LED3_OFF,
        LED3_TOG,
        GET_TEMP,
        GET_HUM,
        GET_PRESS,
        GET_WEATHER_PARAM,
        SET_ALTITUDE,
        COMMAND_UNKNOWN
    }MC_Commands;

    //MC_Commands command;

    void client_parseResponse(QString response);
    void client_displayWheather(QString temperature,QString humidity, QString pressure);

};
#endif // CLIENT_H
