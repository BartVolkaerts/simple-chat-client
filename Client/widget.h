#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QMessageBox>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    QTcpSocket *tcp_socket;
    Ui::Widget *ui;

public slots:
    void read();
    void connectedToServer();
    void connectToServer(bool);
    void disconnectedFromServer();
    void pressedSend();
    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // WIDGET_H
