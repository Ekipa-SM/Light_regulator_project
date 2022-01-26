#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QIODevice>
#include <QtDebug>
#include <regex>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this-> device=new QSerialPort(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEdit->append(currentDateTime + "\t" + message);
}

void MainWindow::on_pushButton_szukaj_clicked()
{
    this->addToLogs("Szukam urządzeń...");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();

    for(int i = 0; i < devices.count(); i++) {
      this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
      ui->comboBox_urzadzenia->addItem(devices.at(i).portName() + "\t" + devices.at(i).description());
    }
}

void MainWindow::ReadData()
{
 QByteArray buffer;
 buffer=device ->readLine();

 if(!buffer.isEmpty())
 {
     //QString data = QString(buffer);
     const char* data = buffer.constData();
     float zmienna1;
     float zmienna2;
     int arg;
     sscanf(data,"{\"lux\":%f,\"set\":%d,\"pid\":%f}\r\n",&zmienna1, &arg, &zmienna2);
     //sscanf(buffer,"{\"lux\":%f}\r\n",&zmienna1);

     QString wyjscie = QString::number(zmienna1);
     QString wyjscie2 = QString::number(zmienna2);
     QString wyjscie3 = QString::number(arg);
     if(zmienna1, zmienna2, arg != 0){
     qDebug()<<buffer << zmienna1;
     ui->lineEdit_2->setText(wyjscie);
     ui->lineEdit_3->setText(wyjscie3);
     ui->lineEdit_4->setText(wyjscie2);
     }

 }
 buffer.clear();
}

void MainWindow::on_pushButton_polacz_clicked()
{
    //srawdzanie czy jakieś urządzenie jest już podlączone, jeżeli nie to wysyłamy info i lecimy dalej
    if(ui->comboBox_urzadzenia->count() == 0) {
      this->addToLogs("Nie wykryto żadnych urządzeń!");
      return;
    }
    //obiekt zawierajacy nazwe portu z formatu ComboBoxa
    QString nazwa_portu = ui-> comboBox_urzadzenia -> currentText().split("\t").first();
    this->device->setPortName(nazwa_portu);

    //otwarcie i konfiguracja portu
    //zwracamy true gdy otwarcie portu się udało
    if(device->open(QSerialPort::ReadWrite)) //ustawiamy parametry połączenia
    {
        device->setBaudRate(QSerialPort::Baud115200);// zmienić na 38400!!!!!!!!!!!
        device->setDataBits(QSerialPort::Data8);
        device->setParity(QSerialPort::NoParity);
        device->setStopBits(QSerialPort::OneStop);
        device->setFlowControl(QSerialPort::NoFlowControl);

        this->addToLogs("Połączono z portem szeregowym");
        connect(this->device, SIGNAL(readyRead()), this, SLOT(ReadData));
    } else this->addToLogs("Otwarcie portu szeregowego się nie udało");
     return;
}


void MainWindow::on_pushButton_rozlacz_clicked()
{
    if(this->device->isOpen()){
        this->device->close();
        this->addToLogs("Rozłączono połączenie z portem szeregowym");
    }else{
        this->addToLogs("Port szeregowy nie był połączony");
    }
}


void MainWindow::on_pushButton_wyczysc_clicked()
{
    ui->textEdit->clear();
}


void MainWindow::on_pushButton_stop_clicked()
{
    QObject::disconnect(device, &QSerialPort::readyRead, this, &MainWindow::ReadData);
}


void MainWindow::on_pushButton_wyslij_v2_clicked()
{
    arg1=ui->lineEdit->text(); //QString
    std::string str=arg1.toStdString();
    std::string poz="setValue=";
    std::string koniec=";";
    std::string wyj = poz + str + koniec;
    dane=wyj.c_str();
    this-> device->write(dane); //wysyła 1 znak
}

void MainWindow::on_pushButton_odbierz_v2_clicked()
{

QObject::connect(device, &QSerialPort::readyRead, this, &MainWindow::ReadData);

}

