QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriend.cpp \
    chatwindow.cpp \
    dragevent.cpp \
    frienditemwidget.cpp \
    friendmanagement.cpp \
    index.cpp \
    information.cpp \
    main.cpp \
    mainwindow.cpp \
    messagebubblewidget.cpp \
    registerwindow.cpp \

HEADERS += \
    addfriend.h \
    chatwindow.h \
    dragevent.h \
    frienditemwidget.h \
    friendmanagement.h \
    index.h \
    information.h \
    mainwindow.h \
    messagebubblewidget.h \
    registerwindow.h \
    registerwindow.h

FORMS += \
    addfriend.ui \
    chatwindow.ui \
    friendmanagement.ui \
    index.ui \
    information.ui \
    mainwindow.ui \
    registerwindow.ui \

TRANSLATIONS += \
    Client_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
