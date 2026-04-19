/********************************************************************************
** Form generated from reading UI file 'settingwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWINDOW_H
#define UI_SETTINGWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWindow
{
public:
    QWidget *centralwidget;
    QPushButton *menuButton;
    QPushButton *botButton;
    QPushButton *personButton;
    QPushButton *statsButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SettingWindow)
    {
        if (SettingWindow->objectName().isEmpty())
            SettingWindow->setObjectName(QString::fromUtf8("SettingWindow"));
        SettingWindow->resize(1920, 1080);
        SettingWindow->setCursor(QCursor(Qt::UpArrowCursor));
        SettingWindow->setAutoFillBackground(false);
        SettingWindow->setStyleSheet(QString::fromUtf8("SettingWindow {background-image: url(:/image/image/settingBackground.jpeg);}"));
        centralwidget = new QWidget(SettingWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        menuButton = new QPushButton(centralwidget);
        menuButton->setObjectName(QString::fromUtf8("menuButton"));
        menuButton->setGeometry(QRect(0, 0, 71, 51));
        menuButton->setCursor(QCursor(Qt::PointingHandCursor));
        menuButton->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/image/menu.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuButton->setIcon(icon);
        menuButton->setIconSize(QSize(45, 45));
        botButton = new QPushButton(centralwidget);
        botButton->setObjectName(QString::fromUtf8("botButton"));
        botButton->setGeometry(QRect(790, 210, 421, 111));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        font.setPointSize(36);
        font.setBold(true);
        font.setItalic(true);
        botButton->setFont(font);
        botButton->setCursor(QCursor(Qt::PointingHandCursor));
        botButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        personButton = new QPushButton(centralwidget);
        personButton->setObjectName(QString::fromUtf8("personButton"));
        personButton->setGeometry(QRect(790, 440, 421, 111));
        QFont font1;
        font1.setPointSize(36);
        font1.setBold(true);
        font1.setItalic(true);
        personButton->setFont(font1);
        personButton->setCursor(QCursor(Qt::PointingHandCursor));
        personButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        statsButton = new QPushButton(centralwidget);
        statsButton->setObjectName(QString::fromUtf8("statsButton"));
        statsButton->setGeometry(QRect(790, 670, 421, 111));
        statsButton->setFont(font1);
        statsButton->setCursor(QCursor(Qt::PointingHandCursor));
        statsButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        SettingWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SettingWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        SettingWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(SettingWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        SettingWindow->setStatusBar(statusbar);

        retranslateUi(SettingWindow);

        QMetaObject::connectSlotsByName(SettingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SettingWindow)
    {
        SettingWindow->setWindowTitle(QCoreApplication::translate("SettingWindow", "MainWindow", nullptr));
        menuButton->setText(QString());
        botButton->setText(QCoreApplication::translate("SettingWindow", "With bot", nullptr));
        personButton->setText(QCoreApplication::translate("SettingWindow", "With person", nullptr));
        statsButton->setText(QCoreApplication::translate("SettingWindow", "Open statistics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingWindow: public Ui_SettingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWINDOW_H
