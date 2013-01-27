#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Client");

    tcp_socket = new QTcpSocket(this);
    ui->connectButton->setCheckable(true);

    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(pressedSend()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(pressedSend()));
    connect(tcp_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcp_socket, SIGNAL(connected()), this, SLOT(connectedToServer()));
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(ui->connectButton, SIGNAL(toggled(bool)), this, SLOT(connectToServer(bool)));
    connect(tcp_socket, SIGNAL(disconnected()), this, SLOT(disconnectedFromServer()));

    ui->lineEdit->setDisabled(true);
    ui->sendButton->setDisabled(true);
}

void Widget::connectedToServer()
{

    ui->label_status->setText("Connected!");
    ui->lineEdit->setDisabled(false);
    ui->sendButton->setDisabled(false);
    ui->connectButton->setText("Disconnect");
}

void Widget::disconnectedFromServer()
{

    ui->label_status->setText("Not connected!");
    ui->lineEdit->setDisabled(true);
    ui->sendButton->setDisabled(true);
    ui->connectButton->setText("Connect");
}

void Widget::read()
{
    QString string = tcp_socket->readAll();
    ui->textBrowser->append(string);
}

void Widget::connectToServer(bool state)
{
    if(state)
    {
        tcp_socket->connectToHost(ui->lineEdit_hostname->text(), ui->lineEdit_port->text().toInt());
    }
    else
    {
        ui->textBrowser->clear();
        tcp_socket->abort();
        disconnectedFromServer();
    }
}

void Widget::pressedSend()
{
    if(ui->lineEdit->text() != "")
    {
        QString text, temp;
        QTime time;
        text = ui->lineEdit->text();
        ui->lineEdit->clear();
        time = time.currentTime();
        temp = time.toString("HH:mm:ss") + ":  " + text;
        QByteArray array = temp.toAscii();
        tcp_socket->write(array);
        temp.push_front("<b>");
        temp.append("</b>");
        ui->textBrowser->append(temp);
    }
}

void Widget::displayError(QAbstractSocket::SocketError socketError)
 {
     switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
         QMessageBox::information(this, tr("Client"),
                                  tr("The host was not found. Please check the "
                                     "host name and port settings."));
         break;
     case QAbstractSocket::ConnectionRefusedError:
         QMessageBox::information(this, tr("Client"),
                                  tr("The connection was refused by the peer. "
                                     "Make sure the server is running, "
                                     "and check that the host name and port "
                                     "settings are correct."));
         break;
     default:
         QMessageBox::information(this, tr("Client"),
                                  tr("The following error occurred: %1.")
                                  .arg(tcp_socket->errorString()));


     }
     ui->connectButton->setChecked(false);
 }

Widget::~Widget()
{
    delete ui;
}
