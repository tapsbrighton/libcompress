CONFIG += debug
HEADERS += library.h
SOURCES += library.cpp

TARGET = "libcompress"

QT += widgets core gui

unix {
    DESTDIR = "/home/mine/taps-libraries"
}

win32 {
    DESTDIR = "c:\\taps-libraries"
}

TEMPLATE = lib
CONFIG += staticlib

