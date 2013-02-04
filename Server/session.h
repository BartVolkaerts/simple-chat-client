#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class QTcpSocket;
class QString;

class Session : public QObject
{
    Q_OBJECT

public:
    Session(QTcpSocket *socket, QObject *parent = 0);
    virtual ~Session();

signals:
    void messageRecieved(QString message);
    void closed();

public slots:
    void sendMessage(QString message);
    void disconnect();


private:
    QTcpSocket *socket;

private slots:
    void dataRecieved();

};
#endif // SESSION_H
