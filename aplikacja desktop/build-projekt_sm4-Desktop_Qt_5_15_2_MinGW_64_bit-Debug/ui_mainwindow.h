/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_szukaj;
    QComboBox *comboBox_urzadzenia;
    QPushButton *pushButton_polacz;
    QPushButton *pushButton_rozlacz;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_wyczysc;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textEdit;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_odbierz;
    QPushButton *pushButton_wyslij;
    QPushButton *pushButton_stop;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(483, 292);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_szukaj = new QPushButton(groupBox);
        pushButton_szukaj->setObjectName(QString::fromUtf8("pushButton_szukaj"));

        horizontalLayout->addWidget(pushButton_szukaj);

        comboBox_urzadzenia = new QComboBox(groupBox);
        comboBox_urzadzenia->setObjectName(QString::fromUtf8("comboBox_urzadzenia"));

        horizontalLayout->addWidget(comboBox_urzadzenia);

        pushButton_polacz = new QPushButton(groupBox);
        pushButton_polacz->setObjectName(QString::fromUtf8("pushButton_polacz"));

        horizontalLayout->addWidget(pushButton_polacz);

        pushButton_rozlacz = new QPushButton(groupBox);
        pushButton_rozlacz->setObjectName(QString::fromUtf8("pushButton_rozlacz"));

        horizontalLayout->addWidget(pushButton_rozlacz);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_wyczysc = new QPushButton(groupBox_3);
        pushButton_wyczysc->setObjectName(QString::fromUtf8("pushButton_wyczysc"));

        verticalLayout_2->addWidget(pushButton_wyczysc);


        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textEdit = new QTextEdit(groupBox_2);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        horizontalLayout_2->addWidget(textEdit);

        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton_odbierz = new QPushButton(groupBox_4);
        pushButton_odbierz->setObjectName(QString::fromUtf8("pushButton_odbierz"));

        verticalLayout_3->addWidget(pushButton_odbierz);

        pushButton_wyslij = new QPushButton(groupBox_4);
        pushButton_wyslij->setObjectName(QString::fromUtf8("pushButton_wyslij"));

        verticalLayout_3->addWidget(pushButton_wyslij);

        pushButton_stop = new QPushButton(groupBox_4);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));

        verticalLayout_3->addWidget(pushButton_stop);


        horizontalLayout_2->addWidget(groupBox_4);


        verticalLayout->addWidget(groupBox_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 483, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Komunikacja", nullptr));
        pushButton_szukaj->setText(QCoreApplication::translate("MainWindow", "Szukaj", nullptr));
        pushButton_polacz->setText(QCoreApplication::translate("MainWindow", "Po\305\202\304\205cz", nullptr));
        pushButton_rozlacz->setText(QCoreApplication::translate("MainWindow", "Roz\305\202\304\205cz", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Polecenia", nullptr));
        pushButton_wyczysc->setText(QCoreApplication::translate("MainWindow", "Wyczy\305\233\304\207 terminal", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "terminal", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Dane", nullptr));
        pushButton_odbierz->setText(QCoreApplication::translate("MainWindow", "Odbierz dane", nullptr));
        pushButton_wyslij->setText(QCoreApplication::translate("MainWindow", "Wy\305\233lij dane", nullptr));
        pushButton_stop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
