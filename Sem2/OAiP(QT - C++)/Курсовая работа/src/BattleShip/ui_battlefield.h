/********************************************************************************
** Form generated from reading UI file 'battlefield.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATTLEFIELD_H
#define UI_BATTLEFIELD_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Battlefield
{
public:
    QWidget *centralwidget;
    QLabel *firstPlayerLabel;
    QLabel *secondPlayerLabel;
    QPushButton *menuButton;
    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView_2;
    QLabel *imageHand;
    QLCDNumber *lcdShip_2;
    QLCDNumber *lcdShip;
    QLabel *label;
    QLabel *label_2;
    QLCDNumber *lcdStep;
    QLCDNumber *lcdStep_2;
    QLabel *redWinLabel;
    QLabel *greenWinLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Battlefield)
    {
        if (Battlefield->objectName().isEmpty())
            Battlefield->setObjectName(QString::fromUtf8("Battlefield"));
        Battlefield->resize(1920, 1080);
        Battlefield->setCursor(QCursor(Qt::ArrowCursor));
        Battlefield->setStyleSheet(QString::fromUtf8("Battlefield {background-image: url(:/image/image/sea.jpg)}"));
        centralwidget = new QWidget(Battlefield);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        firstPlayerLabel = new QLabel(centralwidget);
        firstPlayerLabel->setObjectName(QString::fromUtf8("firstPlayerLabel"));
        firstPlayerLabel->setGeometry(QRect(140, 670, 501, 91));
        QFont font;
        font.setFamily(QString::fromUtf8("URW Bookman"));
        font.setPointSize(36);
        font.setBold(true);
        font.setItalic(false);
        firstPlayerLabel->setFont(font);
        firstPlayerLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        firstPlayerLabel->setAlignment(Qt::AlignCenter);
        secondPlayerLabel = new QLabel(centralwidget);
        secondPlayerLabel->setObjectName(QString::fromUtf8("secondPlayerLabel"));
        secondPlayerLabel->setGeometry(QRect(1280, 670, 501, 91));
        QFont font1;
        font1.setFamily(QString::fromUtf8("URW Bookman"));
        font1.setPointSize(36);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setStyleStrategy(QFont::PreferDefault);
        secondPlayerLabel->setFont(font1);
        secondPlayerLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        secondPlayerLabel->setAlignment(Qt::AlignCenter);
        menuButton = new QPushButton(centralwidget);
        menuButton->setObjectName(QString::fromUtf8("menuButton"));
        menuButton->setGeometry(QRect(0, 5, 71, 51));
        menuButton->setCursor(QCursor(Qt::PointingHandCursor));
        menuButton->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/image/menu.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuButton->setIcon(icon);
        menuButton->setIconSize(QSize(45, 45));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(140, 140, 503, 503));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::UpArrowCursor)));
        graphicsView->setMouseTracking(false);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView_2 = new QGraphicsView(centralwidget);
        graphicsView_2->setObjectName(QString::fromUtf8("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(1280, 140, 503, 503));
        graphicsView_2->viewport()->setProperty("cursor", QVariant(QCursor(Qt::UpArrowCursor)));
        graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        imageHand = new QLabel(centralwidget);
        imageHand->setObjectName(QString::fromUtf8("imageHand"));
        imageHand->setGeometry(QRect(790, 240, 341, 281));
        imageHand->setStyleSheet(QString::fromUtf8("image: url(:/image/image/rightHand.png);"));
        lcdShip_2 = new QLCDNumber(centralwidget);
        lcdShip_2->setObjectName(QString::fromUtf8("lcdShip_2"));
        lcdShip_2->setGeometry(QRect(1050, 750, 121, 71));
        lcdShip_2->setStyleSheet(QString::fromUtf8("color: rgb(192, 28, 40);\n"
"background-color: rgb(18, 43, 182);"));
        lcdShip_2->setFrameShape(QFrame::Box);
        lcdShip_2->setFrameShadow(QFrame::Plain);
        lcdShip_2->setLineWidth(2);
        lcdShip_2->setMidLineWidth(0);
        lcdShip_2->setSmallDecimalPoint(false);
        lcdShip_2->setDigitCount(2);
        lcdShip_2->setSegmentStyle(QLCDNumber::Flat);
        lcdShip_2->setProperty("intValue", QVariant(20));
        lcdShip = new QLCDNumber(centralwidget);
        lcdShip->setObjectName(QString::fromUtf8("lcdShip"));
        lcdShip->setGeometry(QRect(760, 750, 121, 71));
        lcdShip->setStyleSheet(QString::fromUtf8("color: rgb(46, 194, 126);\n"
"background-color: rgb(18, 43, 182);\n"
"border-color: rgb(38, 162, 105);"));
        lcdShip->setFrameShadow(QFrame::Plain);
        lcdShip->setLineWidth(2);
        lcdShip->setDigitCount(2);
        lcdShip->setSegmentStyle(QLCDNumber::Flat);
        lcdShip->setProperty("intValue", QVariant(20));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(760, 670, 411, 61));
        QFont font2;
        font2.setFamily(QString::fromUtf8("URW Bookman"));
        font2.setPointSize(28);
        font2.setBold(true);
        font2.setItalic(false);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(840, 830, 261, 61));
        label_2->setFont(font2);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lcdStep = new QLCDNumber(centralwidget);
        lcdStep->setObjectName(QString::fromUtf8("lcdStep"));
        lcdStep->setGeometry(QRect(760, 910, 121, 71));
        lcdStep->setStyleSheet(QString::fromUtf8("color: rgb(46, 194, 126);\n"
"background-color: rgb(18, 43, 182);"));
        lcdStep->setFrameShadow(QFrame::Plain);
        lcdStep->setLineWidth(2);
        lcdStep->setDigitCount(2);
        lcdStep->setSegmentStyle(QLCDNumber::Flat);
        lcdStep->setProperty("intValue", QVariant(0));
        lcdStep_2 = new QLCDNumber(centralwidget);
        lcdStep_2->setObjectName(QString::fromUtf8("lcdStep_2"));
        lcdStep_2->setGeometry(QRect(1050, 910, 121, 71));
        lcdStep_2->setStyleSheet(QString::fromUtf8("color: rgb(192, 28, 40);\n"
"background-color: rgb(18, 43, 182);"));
        lcdStep_2->setFrameShadow(QFrame::Plain);
        lcdStep_2->setLineWidth(2);
        lcdStep_2->setDigitCount(2);
        lcdStep_2->setSegmentStyle(QLCDNumber::Flat);
        lcdStep_2->setProperty("intValue", QVariant(0));
        redWinLabel = new QLabel(centralwidget);
        redWinLabel->setObjectName(QString::fromUtf8("redWinLabel"));
        redWinLabel->setGeometry(QRect(730, 90, 571, 191));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Ubuntu"));
        font3.setPointSize(80);
        font3.setBold(true);
        font3.setItalic(false);
        redWinLabel->setFont(font3);
        redWinLabel->setStyleSheet(QString::fromUtf8("color: rgb(224, 27, 36);"));
        redWinLabel->setWordWrap(false);
        greenWinLabel = new QLabel(centralwidget);
        greenWinLabel->setObjectName(QString::fromUtf8("greenWinLabel"));
        greenWinLabel->setGeometry(QRect(730, 90, 571, 191));
        greenWinLabel->setFont(font3);
        greenWinLabel->setStyleSheet(QString::fromUtf8("color: rgb(45, 230, 30);"));
        greenWinLabel->setWordWrap(false);
        Battlefield->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Battlefield);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        Battlefield->setMenuBar(menubar);
        statusbar = new QStatusBar(Battlefield);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Battlefield->setStatusBar(statusbar);

        retranslateUi(Battlefield);

        QMetaObject::connectSlotsByName(Battlefield);
    } // setupUi

    void retranslateUi(QMainWindow *Battlefield)
    {
        Battlefield->setWindowTitle(QCoreApplication::translate("Battlefield", "MainWindow", nullptr));
        firstPlayerLabel->setText(QCoreApplication::translate("Battlefield", "Player 1", nullptr));
        secondPlayerLabel->setText(QCoreApplication::translate("Battlefield", "Player 2", nullptr));
        menuButton->setText(QString());
        imageHand->setText(QString());
        label->setText(QCoreApplication::translate("Battlefield", "\320\247\320\270\321\201\320\273\320\276 \321\206\320\265\320\273\321\213\321\205 \320\277\320\260\320\273\321\203\320\261", nullptr));
        label_2->setText(QCoreApplication::translate("Battlefield", "\320\247\320\270\321\201\320\273\320\276 \321\205\320\276\320\264\320\276\320\262", nullptr));
        redWinLabel->setText(QCoreApplication::translate("Battlefield", "\320\237\320\236\320\221\320\225\320\224\320\220", nullptr));
        greenWinLabel->setText(QCoreApplication::translate("Battlefield", "\320\237\320\236\320\221\320\225\320\224\320\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Battlefield: public Ui_Battlefield {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATTLEFIELD_H
