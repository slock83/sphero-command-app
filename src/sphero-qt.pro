#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T10:44:42
#
#-------------------------------------------------

QT       += core bluetooth gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sphero-qt
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	CommandHandler.cpp \
	SpheroManager.cpp \
	HistoryLineEdit.cpp \
	BtScanner.cpp \
    MapDiscovery/Coord.cpp \
    MapDiscovery/MapDiscoverer.cpp \
    MapDiscovery/PointStruct.cpp \
    MapDiscovery/WorldMap.cpp \
    moc_BtScanner.cpp \
    moc_CommandHandler.cpp \
    moc_HistoryLineEdit.cpp \
    moc_mainwindow.cpp

HEADERS  += mainwindow.h \
	CommandHandler.h \
	SpheroManager.h \
	HistoryLineEdit.h \
	BtScanner.h \
    MapDiscovery/Coord.hpp \
    MapDiscovery/MapDiscoverer.hpp \
    MapDiscovery/PointStruct.hpp \
    MapDiscovery/WorldMap.hpp \
    ui_BtScanner.h \
    ui_mainwindow.h

INCLUDEPATH +=

FORMS    += mainwindow.ui \
	BtScanner.ui

CONFIG   += c++11

unix:!macx: LIBS += -lsphero

#INCLUDEPATH += $$PWD/../
#DEPENDPATH += $$PWD/../

QMAKE_CXXFLAGS += -g
