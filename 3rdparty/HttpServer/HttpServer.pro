include($$PWD/../../Public.pri)

QT -= gui

TARGET = HttpServer

DESTDIR = $$MGS_LIBRARY_PATH

TEMPLATE = lib
CONFIG += shared dll
TARGET = $$qtLibraryTargetName($$TARGET)

win32 {
   DEFINES += QTWEBAPPLIB_EXPORT
}

include($$PWD/HttpServer.pri)
