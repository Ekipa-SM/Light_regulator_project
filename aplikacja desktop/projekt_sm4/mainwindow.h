#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_szukaj_clicked();

    void on_pushButton_polacz_clicked();

    void on_pushButton_rozlacz_clicked();

    void on_pushButton_wyczysc_clicked();

    void ReadData();

    //void WriteData();

    void on_pushButton_odbierz_clicked();

    void on_pushButton_wyslij_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
    void addToLogs(QString message);
    QSerialPort *device;
    void sendMessageToDevice(QString message);


};
#endif // MAINWINDOW_H
