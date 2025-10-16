QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    menuprincipale.cpp \
<<<<<<< HEAD
    gestionobjetelectronique.cpp \
    gestionclient.cpp
=======
    gestionobjetelectronique.cpp
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

HEADERS += \
    mainwindow.h \
    menuprincipale.h \
<<<<<<< HEAD
    gestionobjetelectronique.h \
    gestionclient.h
=======
    gestionobjetelectronique.h
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

FORMS += \
    mainwindow.ui \
    menuprincipale.ui \
<<<<<<< HEAD
    gestionobjetelectronique.ui \
    gestionclient.ui
=======
    ../gestionobjetelectronique.ui
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
