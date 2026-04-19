/********************************************************************************
** Form generated from reading UI file 'fieldsecondplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIELDSECONDPLAYER_H
#define UI_FIELDSECONDPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FieldSecondPlayer
{
public:
    QWidget *centralwidget;
    QPushButton *autoButton;
    QPushButton *startButton;
    QLineEdit *nicknameEdit;
    QGraphicsView *graphicsView;
    QPushButton *menuButton;
    QPushButton *firstShipButton;
    QPushButton *secondShipButton;
    QPushButton *thirdShipButton;
    QPushButton *forthShipButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *FieldSecondPlayer)
    {
        if (FieldSecondPlayer->objectName().isEmpty())
            FieldSecondPlayer->setObjectName(QString::fromUtf8("FieldSecondPlayer"));
        FieldSecondPlayer->resize(1920, 1080);
        FieldSecondPlayer->setCursor(QCursor(Qt::ArrowCursor));
        FieldSecondPlayer->setStyleSheet(QString::fromUtf8("FieldSecondPlayer {background-image: url(:/image/image/sea.jpg)}"));
        centralwidget = new QWidget(FieldSecondPlayer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        autoButton = new QPushButton(centralwidget);
        autoButton->setObjectName(QString::fromUtf8("autoButton"));
        autoButton->setGeometry(QRect(1250, 730, 191, 81));
        QFont font;
        font.setPointSize(28);
        font.setBold(true);
        font.setItalic(true);
        autoButton->setFont(font);
        autoButton->setCursor(QCursor(Qt::PointingHandCursor));
        autoButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44);\n"
""));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(1520, 730, 191, 81));
        startButton->setFont(font);
        startButton->setCursor(QCursor(Qt::PointingHandCursor));
        startButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44);\n"
""));
        nicknameEdit = new QLineEdit(centralwidget);
        nicknameEdit->setObjectName(QString::fromUtf8("nicknameEdit"));
        nicknameEdit->setGeometry(QRect(310, 20, 341, 91));
        QFont font1;
        font1.setPointSize(32);
        font1.setBold(true);
        font1.setItalic(true);
        nicknameEdit->setFont(font1);
        nicknameEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44);\n"
""));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(240, 200, 503, 503));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
        firstShipButton = new QPushButton(centralwidget);
        firstShipButton->setObjectName(QString::fromUtf8("firstShipButton"));
        firstShipButton->setGeometry(QRect(1380, 200, 191, 81));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Waree"));
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setItalic(false);
        firstShipButton->setFont(font2);
        firstShipButton->setCursor(QCursor(Qt::PointingHandCursor));
        firstShipButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/image/firstShip.png"), QSize(), QIcon::Normal, QIcon::Off);
        firstShipButton->setIcon(icon1);
        firstShipButton->setIconSize(QSize(50, 50));
        secondShipButton = new QPushButton(centralwidget);
        secondShipButton->setObjectName(QString::fromUtf8("secondShipButton"));
        secondShipButton->setGeometry(QRect(1380, 320, 191, 81));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Waree"));
        font3.setPointSize(16);
        font3.setBold(true);
        secondShipButton->setFont(font3);
        secondShipButton->setCursor(QCursor(Qt::PointingHandCursor));
        secondShipButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/image/image/secondShip.png"), QSize(), QIcon::Normal, QIcon::Off);
        secondShipButton->setIcon(icon2);
        secondShipButton->setIconSize(QSize(95, 95));
        thirdShipButton = new QPushButton(centralwidget);
        thirdShipButton->setObjectName(QString::fromUtf8("thirdShipButton"));
        thirdShipButton->setGeometry(QRect(1380, 440, 191, 81));
        thirdShipButton->setFont(font3);
        thirdShipButton->setCursor(QCursor(Qt::PointingHandCursor));
        thirdShipButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/image/image/thirdShip.png"), QSize(), QIcon::Normal, QIcon::Off);
        thirdShipButton->setIcon(icon3);
        thirdShipButton->setIconSize(QSize(100, 100));
        forthShipButton = new QPushButton(centralwidget);
        forthShipButton->setObjectName(QString::fromUtf8("forthShipButton"));
        forthShipButton->setGeometry(QRect(1380, 560, 191, 81));
        forthShipButton->setFont(font3);
        forthShipButton->setCursor(QCursor(Qt::PointingHandCursor));
        forthShipButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/image/image/forthShip.png"), QSize(), QIcon::Normal, QIcon::Off);
        forthShipButton->setIcon(icon4);
        forthShipButton->setIconSize(QSize(130, 130));
        FieldSecondPlayer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FieldSecondPlayer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        FieldSecondPlayer->setMenuBar(menubar);
        statusbar = new QStatusBar(FieldSecondPlayer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        FieldSecondPlayer->setStatusBar(statusbar);

        retranslateUi(FieldSecondPlayer);

        QMetaObject::connectSlotsByName(FieldSecondPlayer);
    } // setupUi

    void retranslateUi(QMainWindow *FieldSecondPlayer)
    {
        FieldSecondPlayer->setWindowTitle(QCoreApplication::translate("FieldSecondPlayer", "MainWindow", nullptr));
        autoButton->setText(QCoreApplication::translate("FieldSecondPlayer", "AUTO", nullptr));
        startButton->setText(QCoreApplication::translate("FieldSecondPlayer", "START", nullptr));
        nicknameEdit->setText(QCoreApplication::translate("FieldSecondPlayer", "Player 2", nullptr));
        menuButton->setText(QString());
        firstShipButton->setText(QCoreApplication::translate("FieldSecondPlayer", " - 4", nullptr));
        secondShipButton->setText(QCoreApplication::translate("FieldSecondPlayer", " - 3", nullptr));
        thirdShipButton->setText(QCoreApplication::translate("FieldSecondPlayer", " - 2", nullptr));
        forthShipButton->setText(QCoreApplication::translate("FieldSecondPlayer", " - 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FieldSecondPlayer: public Ui_FieldSecondPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIELDSECONDPLAYER_H
