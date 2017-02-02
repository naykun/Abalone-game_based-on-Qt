#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T23:32:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chessboard.cpp \
    chess_board_tmp.cpp

HEADERS  += mainwindow.h \
    chessboard.h \
    commdef.h \
    chess_board_tmp.h

FORMS    += mainwindow.ui

RESOURCES += \
    Image/image.qrc
RC_ICONS = hexagon.net.ico
