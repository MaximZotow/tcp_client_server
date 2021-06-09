#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
        connect(socket,SIGNAL(connected()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1",9999);
    //socket->waitForConnected();
    //socket->write("writing smth");
    ui->plainTextEdit->appendPlainText(Data);
    socket->flush();
}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}

void MainWindow::sockReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        //socket->flush();
        Data = socket->readAll();
        qDebug()<<Data;
        ui->plainTextEdit->appendPlainText(Data);
        socket->write("client received");
        //socket->readAll();
        socket->flush();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    socket->write(ui->lineEdit->text().toUtf8());
    socket->flush();

    socket->waitForReadyRead(300);

    ui->plainTextEdit->appendPlainText("Sending string to server: "+ui->lineEdit->text());
//    QMessageBox msgBox;
//    msgBox.setText(socket->readAll());
//    msgBox.exec();
    QByteArray newData = socket->readAll();
    ui->plainTextEdit_2->appendPlainText("Got from server: "+QString(newData));

}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file((QFileDialog::getOpenFileName()));
//     QFile file("C:/Users/dinno/Desktop/1.txt");
     file.open(QIODevice::ReadOnly);
     socket->flush();
    QFileInfo inf(file);
    QByteArray data;
    QDataStream dStream(&data, QIODevice::ReadWrite);
    dStream.device()->seek(0);
    dStream.setVersion(QDataStream::Qt_4_8);
//    char* st;

    dStream << inf.fileName()  << file.size();
    qDebug() << "file size: " << file.size();
    qDebug() << data;
    socket->write(data);
    socket->waitForReadyRead(300);
    while (!file.atEnd()) {
        QByteArray buff = file.read(65536);
         socket->write(buff);

    }
    file.close();
//    while (!dStream.atEnd()) {
//        dStream.readRawData(st,65536);
//        socket->write(st);
//    }
     ui->plainTextEdit->appendPlainText("Sending file...Size of file:" + QString::number(file.size()));
}
