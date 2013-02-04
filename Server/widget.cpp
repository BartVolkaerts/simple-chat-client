// Project
#include "widget.h"
#include "ui_widget.h"
#include "session.h"

// Qt
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");

    ui->startButton->setCheckable(true);
    connect(ui->startButton, SIGNAL(toggled(bool)), this, SLOT(setUpServer(bool)));
    connect(this, SIGNAL(message(QString)), this, SLOT(newMessage(QString)));
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
        emit done();
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
        temp.push_front("<b>");
        temp.append("</b>");
        ui->textBrowser->append(temp);

        // Send the message to all threads
        emit message(temp);
    }
}

void Widget::clientDisconnected()
{
    ui->textBrowser->setText("Client disconnected!");
}

void Widget::handleConnection()
{
    Session *session = new Session(tcp_server->nextPendingConnection(), this);
    QThread *session_thread = new QThread(this);

    connect(this, SIGNAL(message(QString)), session, SLOT(sendMessage(QString)));
    connect(this, SIGNAL(done()), session, SLOT(disconnect()));
    connect(session, SIGNAL(messageRecieved(QString)), this, SIGNAL(message(QString)));
    connect(session, SIGNAL(closed()), this, SLOT(clientDisconnected()));

    session->moveToThread(session_thread);

    ui->textBrowser->setText("Client connected!");
}

void Widget::newMessage(QString message)
{
    ui->textBrowser->append(message);
}

Widget::~Widget()
{
    delete ui;
}
