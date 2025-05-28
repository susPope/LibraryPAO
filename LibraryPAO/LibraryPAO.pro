QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Sources - usa solo percorsi corretti e rimuovi duplicati
SOURCES += \
    GUI/loanmanagerwidget.cpp \
    GUI/mediamanagerwidget.cpp \
    Project/mediarepo.cpp \
    main.cpp \
    GUI/mainwindow.cpp \
    GUI/searchwidget.cpp \
    Project/articolo.cpp \
    Project/film.cpp \
    Project/libro.cpp \
    Project/media.cpp

# Headers - solo percorsi esistenti
HEADERS += \
    GUI/loanmanagerwidget.h \
    GUI/mainwindow.h \
    GUI/mediamanagerwidget.h \
    GUI/searchwidget.h \
    Project/articolo.h \
    Project/film.h \
    Project/libro.h \
    Project/media.h \
    Project/mediarepo.h

FORMS += \
    GUI/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources.qrc
