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
	BtScanner.cpp

HEADERS  += mainwindow.h \
	CommandHandler.h \
	SpheroManager.h \
	HistoryLineEdit.h \
	BtScanner.h

INCLUDEPATH += 

FORMS    += mainwindow.ui \
	BtScanner.ui

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/btscanner

CONFIG   += c++11

unix:!macx: LIBS += -L$$PWD/../lib/ -lsphero

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

QMAKE_CXXFLAGS += -g
