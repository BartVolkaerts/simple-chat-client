#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
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
    QTcpServer *tcp_server;
    QTcpSocket *client;
    int teller;
    Ui::Widget *ui;

private slots:
    void clientDisconnected();
    void setUpServer(bool);
    void pressedSend();
    void handleConnection();
    void read();
};

#endif // WIDGET_H
