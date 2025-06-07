QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Sources
SOURCES += \
    main.cpp \
    Project/media.cpp \
    Project/articolo.cpp \
    Project/film.cpp \
    Project/libro.cpp \
    Project/mediarepo.cpp \
    GUI/mainwindow.cpp \
    GUI/MediaManager/mediaviewwidget.cpp \
    GUI/MediaManager/mediaformwidget.cpp \
    GUI/MediaManager/mediamanagerwidget.cpp \
    GUI/LoanManager/loanviewwidget.cpp \
    GUI/LoanManager/loanmanagerwidget.cpp \
    GUI/SearchBar/searchwidget.cpp


# Headers - solo percorsi esistenti
HEADERS += \
    Project/media.h \
    Project/articolo.h \
    Project/film.h \
    Project/libro.h \
    Project/mediarepo.h \
    GUI/mainwindow.h \
    GUI/MediaManager/mediaviewwidget.h \
    GUI/MediaManager/mediaformwidget.h \
    GUI/MediaManager/mediamanagerwidget.h \
    GUI/LoanManager/loanviewwidget.h \
    GUI/LoanManager/loanmanagerwidget.h \
    GUI/SearchBar/searchwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources/resources.qrc
