QT       += widgets
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5
LIBS += -L/usr/lib/x86_64-linux-gnu -lQt5Widgets
CONFIG   += c++11

TARGET = Minesweeper
TEMPLATE = app

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/cellbutton.cpp

HEADERS += include/mainwindow.h \
           include/cellbutton.h

INCLUDEPATH += include
