/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *name;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *infoButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1920, 1080);
        MainWindow->setCursor(QCursor(Qt::UpArrowCursor));
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        MainWindow->setAcceptDrops(true);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("MainWindow {background-image: url(:/image/image/background2.jpg)}"));
        MainWindow->setAnimated(true);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        name = new QLabel(centralwidget);
        name->setObjectName(QString::fromUtf8("name"));
        name->setEnabled(true);
        name->setGeometry(QRect(670, 5, 633, 200));
        QFont font;
        font.setFamily(QString::fromUtf8("Uroob"));
        font.setPointSize(150);
        font.setBold(false);
        font.setItalic(false);
        name->setFont(font);
        name->setCursor(QCursor(Qt::UpArrowCursor));
        name->setContextMenuPolicy(Qt::DefaultContextMenu);
        name->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255)"));
        name->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(750, 370, 391, 161));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu"));
        font1.setPointSize(120);
        font1.setBold(false);
        font1.setItalic(true);
        startButton->setFont(font1);
        startButton->setCursor(QCursor(Qt::PointingHandCursor));
        startButton->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;\n"
"color: rgb(3, 11, 187)"));
        quitButton = new QPushButton(centralwidget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(790, 520, 291, 181));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Z003"));
        font2.setPointSize(128);
        font2.setBold(true);
        font2.setItalic(true);
        quitButton->setFont(font2);
        quitButton->setCursor(QCursor(Qt::PointingHandCursor));
        quitButton->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;\n"
"color: rgb(3, 11, 187)"));
        infoButton = new QPushButton(centralwidget);
        infoButton->setObjectName(QString::fromUtf8("infoButton"));
        infoButton->setGeometry(QRect(0, 1010, 71, 51));
        infoButton->setCursor(QCursor(Qt::PointingHandCursor));
        infoButton->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/image/info.png"), QSize(), QIcon::Normal, QIcon::Off);
        infoButton->setIcon(icon);
        infoButton->setIconSize(QSize(50, 50));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        name->setText(QCoreApplication::translate("MainWindow", "Battleship", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        quitButton->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        infoButton->setText(QString());
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
