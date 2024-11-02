QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addships.cpp \
    battlefield.cpp \
    battlescene.cpp \
    bot.cpp \
    buildbattlefield.cpp \
    correctposship.cpp \
    fieldfirstplayer.cpp \
    fieldsecondplayer.cpp \
    lettersonwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    qmessagequestion.cpp \
    settingwindow.cpp \
    ship.cpp \
    ship1.cpp \
    ship2.cpp \
    ship3.cpp \
    ship4.cpp \
    statswindow.cpp

HEADERS += \
    ConstAutoPlacing.h \
    ConstBot.h \
    ConstField.h \
    ConstShips.h \
    addships.h \
    battlefield.h \
    battlescene.h \
    bot.h \
    buildbattlefield.h \
    correctposship.h \
    fieldfirstplayer.h \
    fieldsecondplayer.h \
    lettersonwindow.h \
    mainwindow.h \
    qmessagequestion.h \
    selectTypeShip.h \
    settingwindow.h \
    ship.h \
    ship1.h \
    ship2.h \
    ship3.h \
    ship4.h \
    statswindow.h

FORMS += \
    battlefield.ui \
    fieldfirstplayer.ui \
    fieldsecondplayer.ui \
    mainwindow.ui \
    settingwindow.ui \
    statswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
