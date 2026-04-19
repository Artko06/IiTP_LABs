/********************************************************************************
** Form generated from reading UI file 'statswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATSWINDOW_H
#define UI_STATSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatsWindow
{
public:
    QWidget *centralwidget;
    QPushButton *backButton;
    QProgressBar *progressBarWithBot;
    QLCDNumber *lcdWinWithBot;
    QLCDNumber *lcdAllWithBot;
    QLCDNumber *lcdDefeatWithBot;
    QLabel *label;
    QLabel *statsLabel;
    QLCDNumber *lcdWinWithPerson;
    QLCDNumber *lcdDefeatWithPerson;
    QLCDNumber *lcdAllWithPerson;
    QProgressBar *progressBarWithPerson;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *resetStatsBotButton;
    QPushButton *resetStatsPersonButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *StatsWindow)
    {
        if (StatsWindow->objectName().isEmpty())
            StatsWindow->setObjectName(QString::fromUtf8("StatsWindow"));
        StatsWindow->resize(1920, 1080);
        StatsWindow->setStyleSheet(QString::fromUtf8("StatsWindow {background-image: url(:/image/image/stats.png);}"));
        centralwidget = new QWidget(StatsWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        backButton = new QPushButton(centralwidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setGeometry(QRect(0, 5, 71, 51));
        backButton->setCursor(QCursor(Qt::PointingHandCursor));
        backButton->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/image/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        backButton->setIcon(icon);
        backButton->setIconSize(QSize(45, 45));
        progressBarWithBot = new QProgressBar(centralwidget);
        progressBarWithBot->setObjectName(QString::fromUtf8("progressBarWithBot"));
        progressBarWithBot->setGeometry(QRect(1420, 250, 201, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("URW Bookman"));
        font.setPointSize(18);
        font.setBold(true);
        progressBarWithBot->setFont(font);
        progressBarWithBot->setStyleSheet(QString::fromUtf8("selection-background-color: rgb(46, 194, 126);\n"
"background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        progressBarWithBot->setValue(0);
        lcdWinWithBot = new QLCDNumber(centralwidget);
        lcdWinWithBot->setObjectName(QString::fromUtf8("lcdWinWithBot"));
        lcdWinWithBot->setGeometry(QRect(750, 240, 141, 71));
        lcdWinWithBot->setStyleSheet(QString::fromUtf8("color: rgb(52, 221, 57);\n"
"border-color: rgb(38, 162, 105);\n"
"background-color: rgb(18, 141, 212);"));
        lcdWinWithBot->setFrameShadow(QFrame::Raised);
        lcdWinWithBot->setLineWidth(4);
        lcdWinWithBot->setMidLineWidth(0);
        lcdWinWithBot->setSmallDecimalPoint(false);
        lcdWinWithBot->setDigitCount(5);
        lcdWinWithBot->setSegmentStyle(QLCDNumber::Flat);
        lcdAllWithBot = new QLCDNumber(centralwidget);
        lcdAllWithBot->setObjectName(QString::fromUtf8("lcdAllWithBot"));
        lcdAllWithBot->setGeometry(QRect(1230, 240, 141, 71));
        lcdAllWithBot->setStyleSheet(QString::fromUtf8("background-color: rgb(224, 173, 20);\n"
"border-color: rgb(38, 162, 105);\n"
"color: rgb(61, 56, 70);"));
        lcdAllWithBot->setFrameShape(QFrame::Box);
        lcdAllWithBot->setFrameShadow(QFrame::Raised);
        lcdAllWithBot->setLineWidth(4);
        lcdAllWithBot->setDigitCount(5);
        lcdDefeatWithBot = new QLCDNumber(centralwidget);
        lcdDefeatWithBot->setObjectName(QString::fromUtf8("lcdDefeatWithBot"));
        lcdDefeatWithBot->setGeometry(QRect(990, 240, 141, 71));
        lcdDefeatWithBot->setStyleSheet(QString::fromUtf8("color: rgb(243, 41, 41);\n"
"color: rgb(161, 4, 4);\n"
"border-color: rgb(38, 162, 105);\n"
"background-color: rgb(18, 141, 212);"));
        lcdDefeatWithBot->setFrameShadow(QFrame::Raised);
        lcdDefeatWithBot->setLineWidth(4);
        lcdDefeatWithBot->setMidLineWidth(0);
        lcdDefeatWithBot->setSmallDecimalPoint(false);
        lcdDefeatWithBot->setDigitCount(5);
        lcdDefeatWithBot->setSegmentStyle(QLCDNumber::Flat);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(1200, 140, 201, 61));
        QFont font1;
        font1.setFamily(QString::fromUtf8("URW Bookman"));
        font1.setPointSize(16);
        font1.setBold(true);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        statsLabel = new QLabel(centralwidget);
        statsLabel->setObjectName(QString::fromUtf8("statsLabel"));
        statsLabel->setGeometry(QRect(560, -40, 761, 191));
        QFont font2;
        font2.setFamily(QString::fromUtf8("URW Bookman"));
        font2.setPointSize(48);
        font2.setBold(true);
        statsLabel->setFont(font2);
        statsLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lcdWinWithPerson = new QLCDNumber(centralwidget);
        lcdWinWithPerson->setObjectName(QString::fromUtf8("lcdWinWithPerson"));
        lcdWinWithPerson->setGeometry(QRect(750, 370, 141, 71));
        lcdWinWithPerson->setStyleSheet(QString::fromUtf8("color: rgb(52, 221, 57);\n"
"border-color: rgb(38, 162, 105);\n"
"background-color: rgb(18, 141, 212);"));
        lcdWinWithPerson->setFrameShadow(QFrame::Raised);
        lcdWinWithPerson->setLineWidth(4);
        lcdWinWithPerson->setMidLineWidth(0);
        lcdWinWithPerson->setSmallDecimalPoint(false);
        lcdWinWithPerson->setDigitCount(5);
        lcdWinWithPerson->setSegmentStyle(QLCDNumber::Flat);
        lcdDefeatWithPerson = new QLCDNumber(centralwidget);
        lcdDefeatWithPerson->setObjectName(QString::fromUtf8("lcdDefeatWithPerson"));
        lcdDefeatWithPerson->setGeometry(QRect(990, 370, 141, 71));
        lcdDefeatWithPerson->setStyleSheet(QString::fromUtf8("color: rgb(243, 41, 41);\n"
"color: rgb(161, 4, 4);\n"
"border-color: rgb(38, 162, 105);\n"
"background-color: rgb(18, 141, 212);"));
        lcdDefeatWithPerson->setFrameShadow(QFrame::Raised);
        lcdDefeatWithPerson->setLineWidth(4);
        lcdDefeatWithPerson->setMidLineWidth(0);
        lcdDefeatWithPerson->setSmallDecimalPoint(false);
        lcdDefeatWithPerson->setDigitCount(5);
        lcdDefeatWithPerson->setSegmentStyle(QLCDNumber::Flat);
        lcdAllWithPerson = new QLCDNumber(centralwidget);
        lcdAllWithPerson->setObjectName(QString::fromUtf8("lcdAllWithPerson"));
        lcdAllWithPerson->setGeometry(QRect(1230, 370, 141, 71));
        lcdAllWithPerson->setStyleSheet(QString::fromUtf8("background-color: rgb(224, 173, 20);\n"
"border-color: rgb(38, 162, 105);\n"
"color: rgb(61, 56, 70);"));
        lcdAllWithPerson->setFrameShape(QFrame::Box);
        lcdAllWithPerson->setFrameShadow(QFrame::Raised);
        lcdAllWithPerson->setLineWidth(4);
        lcdAllWithPerson->setDigitCount(5);
        progressBarWithPerson = new QProgressBar(centralwidget);
        progressBarWithPerson->setObjectName(QString::fromUtf8("progressBarWithPerson"));
        progressBarWithPerson->setGeometry(QRect(1420, 380, 201, 51));
        progressBarWithPerson->setFont(font);
        progressBarWithPerson->setStyleSheet(QString::fromUtf8("selection-background-color: rgb(46, 194, 126);\n"
"background-color: rgb(36, 31, 49);\n"
"color: rgb(255, 255, 255);"));
        progressBarWithPerson->setValue(0);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(740, 150, 171, 41));
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(940, 150, 241, 41));
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(550, 240, 171, 41));
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(435, 380, 231, 41));
        label_6->setFont(font1);
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        resetStatsBotButton = new QPushButton(centralwidget);
        resetStatsBotButton->setObjectName(QString::fromUtf8("resetStatsBotButton"));
        resetStatsBotButton->setGeometry(QRect(650, 540, 341, 71));
        QFont font3;
        font3.setPointSize(18);
        font3.setBold(true);
        font3.setItalic(true);
        resetStatsBotButton->setFont(font3);
        resetStatsBotButton->setCursor(QCursor(Qt::PointingHandCursor));
        resetStatsBotButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        resetStatsPersonButton = new QPushButton(centralwidget);
        resetStatsPersonButton->setObjectName(QString::fromUtf8("resetStatsPersonButton"));
        resetStatsPersonButton->setGeometry(QRect(1100, 540, 421, 71));
        resetStatsPersonButton->setFont(font3);
        resetStatsPersonButton->setCursor(QCursor(Qt::PointingHandCursor));
        resetStatsPersonButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        StatsWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(StatsWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        StatsWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(StatsWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        StatsWindow->setStatusBar(statusbar);

        retranslateUi(StatsWindow);

        QMetaObject::connectSlotsByName(StatsWindow);
    } // setupUi

    void retranslateUi(QMainWindow *StatsWindow)
    {
        StatsWindow->setWindowTitle(QCoreApplication::translate("StatsWindow", "MainWindow", nullptr));
        backButton->setText(QString());
        label->setText(QCoreApplication::translate("StatsWindow", "\320\236\320\261\321\211\320\265\320\265 \321\207\320\270\321\201\320\273\320\276 \320\270\320\263\321\200", nullptr));
        statsLabel->setText(QCoreApplication::translate("StatsWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260 \320\270\320\263\321\200\320\276\320\272\320\260 1:", nullptr));
        label_3->setText(QCoreApplication::translate("StatsWindow", "\320\247\320\270\321\201\320\273\320\276 \320\277\320\276\320\261\320\265\320\264", nullptr));
        label_4->setText(QCoreApplication::translate("StatsWindow", "\320\247\320\270\321\201\320\273\320\276 \320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\320\271", nullptr));
        label_5->setText(QCoreApplication::translate("StatsWindow", " \320\241 \320\261\320\276\321\202\320\276\320\274", nullptr));
        label_6->setText(QCoreApplication::translate("StatsWindow", " \320\241\320\276 \320\262\321\202\320\276\321\200\320\276\320\274 \320\270\320\263\321\200\320\276\320\272\320\276\320\274", nullptr));
        resetStatsBotButton->setText(QCoreApplication::translate("StatsWindow", "\320\241\320\261\321\200\320\276\321\201 \321\201\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\270 \321\201 \320\261\320\276\321\202\320\276\320\274", nullptr));
        resetStatsPersonButton->setText(QCoreApplication::translate("StatsWindow", "\320\241\320\261\321\200\320\276\321\201 \321\201\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\270 \321\201\320\276 2-\320\274 \320\270\320\263\321\200\320\276\320\272\320\276\320\274", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatsWindow: public Ui_StatsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATSWINDOW_H
