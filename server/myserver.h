#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include<QDataStream>
#include<QDir>
class myserver: public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

    QTcpSocket* socket;
    QTcpServer* server;
    QByteArray Data;
    bool connected_ = false;
    bool recieveMode_ = false;

public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // MYSERVER_H
