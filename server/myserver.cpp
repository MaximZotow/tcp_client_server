#include "myserver.h"
int ch1 = 0;
int ch2 = 0;
QString fileName;
qint64 fileSize;
QFile file;

myserver::myserver(){}

myserver::~myserver(){}

void myserver::startServer()
{

    if (this->listen(QHostAddress::Any,9999))
    {
        qDebug()<<"Listening";
    }
    else
    {
        qDebug()<<"Not listening";
    }
}

void myserver::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

    qDebug()<<socketDescriptor<<" Client connected";

    socket->write("You are connect");
    qDebug()<<"Send client connect status - YES";
}

void myserver::sockReady()
{
//    //socket->waitForReadyRead(300);
//    //socket->fbuff = socket->readAll();lush();
//    Data = socket->readAll();
//    qDebug() << Data;
//    //Data = "123";
//    qDebug() << "recieved: " << Data;
//    if (connected_ == true) {
//    socket->write("Welcome, "+Data);
//    qDebug() << "sent: " <<Data;
//    }
//    socket->flush();
//    connected_ = true;
       if (connected_ == true) {
            //start...
            ch1++;
            //qDebug() << "ch1 = " << ch1;

           if (!recieveMode_)
           {
                QByteArray data;
                ch2++;
                //qDebug() << "ch2 = " << ch2;
                data = socket->readAll();
                 //qDebug() << data;
                QDataStream dStream(&data, QIODevice::ReadOnly);
                dStream.setVersion(QDataStream::Qt_4_8);

                dStream >> fileName;
                dStream >> fileSize;

                qDebug()<< "File name: " << fileName
                        << "File size: " << QString::number(fileSize);

                file.setFileName(QDir::currentPath()+"/Received_file/" + fileName);
                file.open(QIODevice::WriteOnly);



                recieveMode_ = true;
           }
           else
           {

               QByteArray received_data;

                // recieve
                static quint64 received_size = 0;
                //qDebug() << "Filesize file size: " << fileSize;
                int i = 0;
                for (i=0;fileSize/65536;++i){
                    //i++;
                    socket->waitForReadyRead(300);
                    received_data = socket->readAll();
                    file.seek(file.size());
                    file.write(received_data);
                    qDebug() << "Current file size: " << file.size() << "i = " << i;
//                    socket->readAll();
                    received_size+=received_data.size();
                    if (received_size>=fileSize-1)
                        break;
                }

                recieveMode_ = false;
                //qDebug() << file.size();
                qDebug() << "File had receieved. Size of file: "+QString::number(fileSize)+"; recieved bytes: "+QString::number(fileSize);
                file.close();
           }
           socket->readAll();
           socket->flush();
        }
       socket->readAll();
       socket->flush();
        connected_=true;
}

void myserver::sockDisc()
{
    qDebug()<<socket->socketDescriptor()<< "Client disconnected";
    connected_=false;
    socket->deleteLater();
}


