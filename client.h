#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void buttonConnectPressed();
    void buttonSendpressed();

private:
    Ui::Client *ui;
    QTcpSocket _socket;
};
#endif // CLIENT_H
