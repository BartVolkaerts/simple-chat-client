#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");

    ui->startButton->setCheckable(true);
    connect(ui->startButton, SIGNAL(toggled(bool)), this, SLOT(setUpServer(bool)));
}

void Widget::setUpServer(bool status)
{
    if(status)
    {
        teller = 0;
        tcp_server = new QTcpServer(this);
        if(!tcp_server->listen(QHostAddress::Any, ui->lineEdit_2->text().toInt()))
        {
            QMessageBox::critical(this, "Server", "Unable to start the server");
            exit(1);
        }
        else
        {
            ui->label_adres->setText(tcp_server->serverAddress().toString());
            ui->label_port->setText(QString::number(tcp_server->serverPort()));
        }

        connect(tcp_server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
        connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(pressedSend()));
        connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(pressedSend()));
        ui->startButton->setText("Stop server");
    }
    else
    {
        client->close();
        tcp_server->close();
        ui->startButton->setText("Start server");
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
        client->write(array);
        temp.push_front("<b>");
        temp.append("</b>");
        ui->textBrowser->append(temp);
    }
}

void Widget::clientDisconnected()
{
    ui->textBrowser->setText("Client disconnected!");
}

void Widget::handleConnection()
{
        client = tcp_server->nextPendingConnection();
        connect(client, SIGNAL(readyRead()), this, SLOT(read()));
        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        ui->textBrowser->setText("Client connected!");
}

void Widget::read()
{
    QString string = client->readAll();
    ui->textBrowser->append(string);
}

Widget::~Widget()
{
    delete ui;
}
