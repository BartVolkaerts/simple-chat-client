#include "session.h"

// Qt
#include <QTcpSocket>
#include <QString>
#include <QThread>

Session::Session(QTcpSocket *socket, QObject *parent)
    : QObject(parent)
{
    this->socket = socket;
    
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(dataRecieved()));
    connect(this->socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

Session::~Session()
{
}

void Session::dataRecieved()
{
    QString message(this->socket->readAll());
    emit messageRecieved(message);
}

void Session::sendMessage(QString message)
{
    this->socket->write(message.toAscii());
}

void Session::disconnect()
{
    this->socket->close();
    this->thread()->quit();
}
