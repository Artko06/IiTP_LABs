/********************************************************************************
** Form generated from reading UI file 'fieldfirstplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIELDFIRSTPLAYER_H
#define UI_FIELDFIRSTPLAYER_H

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

class Ui_FieldFirstPlayer
{
public:
    QWidget *centralwidget;
    QPushButton *backButton;
    QLineEdit *nicknameEdit;
    QPushButton *autoButton;
    QPushButton *startButton;
    QGraphicsView *graphicsView;
    QPushButton *firstShipButton;
    QPushButton *secondShipButton;
    QPushButton *thirdShipButton;
    QPushButton *forthShipButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *FieldFirstPlayer)
    {
        if (FieldFirstPlayer->objectName().isEmpty())
            FieldFirstPlayer->setObjectName(QString::fromUtf8("FieldFirstPlayer"));
        FieldFirstPlayer->resize(1920, 1080);
        FieldFirstPlayer->setCursor(QCursor(Qt::ArrowCursor));
        FieldFirstPlayer->setStyleSheet(QString::fromUtf8("FieldFirstPlayer {background-image: url(:/image/image/sea.jpg)}"));
        centralwidget = new QWidget(FieldFirstPlayer);
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
        nicknameEdit = new QLineEdit(centralwidget);
        nicknameEdit->setObjectName(QString::fromUtf8("nicknameEdit"));
        nicknameEdit->setGeometry(QRect(310, 20, 341, 91));
        QFont font;
        font.setPointSize(32);
        font.setBold(true);
        font.setItalic(true);
        nicknameEdit->setFont(font);
        nicknameEdit->setCursor(QCursor(Qt::PointingHandCursor));
        nicknameEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44);\n"
""));
        autoButton = new QPushButton(centralwidget);
        autoButton->setObjectName(QString::fromUtf8("autoButton"));
        autoButton->setGeometry(QRect(1250, 730, 191, 81));
        QFont font1;
        font1.setPointSize(28);
        font1.setBold(true);
        font1.setItalic(true);
        autoButton->setFont(font1);
        autoButton->setCursor(QCursor(Qt::PointingHandCursor));
        autoButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        autoButton->setIconSize(QSize(16, 27));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(1520, 730, 191, 81));
        startButton->setFont(font1);
        startButton->setCursor(QCursor(Qt::PointingHandCursor));
        startButton->setStyleSheet(QString::fromUtf8("background-color: rgb(99, 69, 44)"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(240, 200, 503, 503));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
        FieldFirstPlayer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FieldFirstPlayer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        FieldFirstPlayer->setMenuBar(menubar);
        statusbar = new QStatusBar(FieldFirstPlayer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        FieldFirstPlayer->setStatusBar(statusbar);

        retranslateUi(FieldFirstPlayer);

        QMetaObject::connectSlotsByName(FieldFirstPlayer);
    } // setupUi

    void retranslateUi(QMainWindow *FieldFirstPlayer)
    {
        FieldFirstPlayer->setWindowTitle(QCoreApplication::translate("FieldFirstPlayer", "MainWindow", nullptr));
        backButton->setText(QString());
        nicknameEdit->setText(QCoreApplication::translate("FieldFirstPlayer", "Player 1", nullptr));
        autoButton->setText(QCoreApplication::translate("FieldFirstPlayer", "AUTO", nullptr));
        startButton->setText(QCoreApplication::translate("FieldFirstPlayer", "START", nullptr));
        firstShipButton->setText(QCoreApplication::translate("FieldFirstPlayer", " - 4", nullptr));
        secondShipButton->setText(QCoreApplication::translate("FieldFirstPlayer", " - 3", nullptr));
        thirdShipButton->setText(QCoreApplication::translate("FieldFirstPlayer", " - 2", nullptr));
        forthShipButton->setText(QCoreApplication::translate("FieldFirstPlayer", " - 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FieldFirstPlayer: public Ui_FieldFirstPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIELDFIRSTPLAYER_H
