QT       += core gui sql printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gestionemploye.cpp \
    menuprincipale.cpp \
    gestionobjetelectronique.cpp \
    gestionclient.cpp \
    client.cpp \
    objetelectronique.cpp \
    database.cpp \
    connection.cpp \
    login.cpp \
    chatbotemploye.cpp \
    callmebot.cpp

HEADERS += \
    mainwindow.h \
    gestionemploye.h \
    menuprincipale.h \
    gestionobjetelectronique.h \
    gestionclient.h \
    client.h \
    objetelectronique.h \
    database.h \
    connection.h \
    login.h \
    chatbotemploye.h \
    callmebot.h

FORMS += \
    gestionemploye.ui \
    menuprincipale.ui \
    gestionobjetelectronique.ui \
    gestionclient.ui \
    login.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
