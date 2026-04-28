QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    brick.cpp \
    castle.cpp \
    dynamiccoin.cpp \
    dynamicobject.cpp \
    enemy.cpp \
    fireball.cpp \
    flag.cpp \
    flagpole.cpp \
    flower.cpp \
    gameobject.cpp \
    helpscene.cpp \
    main.cpp \
    mainscene.cpp \
    mushroom.cpp \
    mypushbutton.cpp \
    pipe.cpp \
    piranhaplant.cpp \
    player.cpp \
    playscene.cpp \
    soundmanager.cpp \
    staticcoin.cpp \
    staticobject.cpp

HEADERS += \
    brick.h \
    castle.h \
    dynamiccoin.h \
    dynamicobject.h \
    enemy.h \
    fireball.h \
    flag.h \
    flagpole.h \
    flower.h \
    gameobject.h \
    helpscene.h \
    mainscene.h \
    mushroom.h \
    mypushbutton.h \
    pipe.h \
    piranhaplant.h \
    player.h \
    playscene.h \
    soundmanager.h \
    staticcoin.h \
    staticobject.h

FORMS += \
    mainscene.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
