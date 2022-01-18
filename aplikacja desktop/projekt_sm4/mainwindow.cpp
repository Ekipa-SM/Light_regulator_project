#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QSerialPortInfo>
#include <QMessageBox>

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

void MainWindow::sendMessageToDevice(QString message)
{
        //this->device->write(message.
}

void MainWindow::on_pushButton_odbierz_clicked()
{
    QObject::connect(device, &QSerialPort::readyRead, this, &MainWindow::ReadData);

}

void MainWindow::on_pushButton_wyslij_clicked()
{
 device->write(ui->textEdit->toPlainText().toLatin1());

 //QObject::connect(device, &QSerialPort::readyRead, this, &MainWindow::WriteData);
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
 buffer=device -> readAll();

 if(!buffer.isEmpty())
 {

     QString string=ui->textEdit->toPlainText();
     string+=tr(buffer);
     ui->textEdit->clear();
     ui->textEdit->append(string);
 }
 buffer.clear();

    while(this->device->canReadLine()){

      QString line=this->device->readLine();
      QString endline = "\r";//usuwamy niepotrzebny koniec linii
      int pozycja=line.lastIndexOf(endline);

      this->addToLogs(line.left(pozycja)); //dodanie linii bez końcówki do terminala
        QByteArray buf;
        buf = this->device->readAll();

    }
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
        device->setBaudRate(QSerialPort::Baud38400);
        device->setDataBits(QSerialPort::Data8);
        device->setParity(QSerialPort::NoParity);
        device->setStopBits(QSerialPort::OneStop);
        device->setFlowControl(QSerialPort::NoFlowControl);

        this->addToLogs("Połączono z portem szeregowym");
        connect(this->device, SIGNAL(readyRead()), this, SLOT(ReadData));
        //QObject::connect(device, &QSerialPort::readyRead, this, &MainWindow::ReadData);
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

