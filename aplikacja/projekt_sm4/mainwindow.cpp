#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QIODevice>
#include <QtDebug>

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
 buffer=device ->readAll();

 if(!buffer.isEmpty())
 {
     QString odebrane=ui->lineEdit_2->text();
     std::string odebrane1;
     QString odebrane_zadana = ui->lineEdit_3->text();
     std::string odebrane_zadana1;
     QString odebrane_pid = ui->lineEdit_4->text();
     std::string odebrane_pid1;
     odebrane+=tr(buffer);
     odebrane1 = odebrane.toStdString();

     odebrane_zadana+=tr(buffer);
     odebrane_zadana1 = odebrane_zadana.toStdString();

     odebrane_pid+=tr(buffer);
     odebrane_pid1 = odebrane_pid.toStdString();

     ui->lineEdit_2->clear();
     ui->lineEdit_3->clear();
     ui->lineEdit_4->clear();

     int found1 = odebrane1.find(":");
     int found2 = odebrane1.find(",");
     char foundFinall[20];
     odebrane1.copy(foundFinall,found2-found1,found1+1);
     ui->lineEdit_2->insert(foundFinall);

     int found3 = odebrane_zadana1.find("set:");
     int found4 = odebrane_zadana1.find(",");
     char foundFinall1[20];
     odebrane_zadana1.copy(foundFinall1,found4-found3,found1+5);
     ui->lineEdit_3->insert(foundFinall1);

     int found5 = odebrane_pid1.find("pid:");
     int found6 = odebrane_pid1.find(",");
     char foundFinall2[20];
     odebrane_zadana1.copy(foundFinall2,found6-found5,found1+10);
     ui->lineEdit_4->insert(foundFinall2);


//     QRegularExpression re("lux");
//    QString str = odebrane.section(re, 1, 1);
//    qDebug() << str;
//     QJsonObject buffer;
//     QJsonDocument doc(buffer);
//     QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
//     QString strJson = QLatin1String(docByteArray);
//     qDebug() << strJson;
//     odebrane = odebrane.replace("{","");
//     odebrane = odebrane.replace("}", "\t");
//     odebrane = odebrane.replace("set", "\t");
//    // odebrane = odebrane.remove();




     //QJsonArray test = obj["lux"].toArray();

//     if (ui->lineEdit_2->text().isEmpty()){
//         qDebug("puste");
//          ui->lineEdit_2->insert(odebrane);//zmienić na name
//          //ui->lineEdit_2->

//      }
//     odebrane = odebrane.replace("{","");
//     odebrane = odebrane.replace("}", "\t");

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


void MainWindow::on_pushButton_wyslij_v2_clicked()
{
    arg1=ui->lineEdit->text(); //QString
    std::string str=arg1.toStdString();
    std::string poz="setValue=";
    std::string koniec=";";
    //poz=poz.append(str);
    std::string lol = poz + str + koniec;
    //poz=poz.append(koniec);
    dane=lol.c_str();
    this-> device->write(dane); //wysyła 1 znak
}

void MainWindow::on_pushButton_odbierz_v2_clicked()
{

QObject::connect(device, &QSerialPort::readyRead, this, &MainWindow::ReadData);

}

void MainWindow::on_pushButton_clicked()
{


}


void MainWindow::on_pushButton_2_clicked()
{

}

