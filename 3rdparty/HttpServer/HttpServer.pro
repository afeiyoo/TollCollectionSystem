include($$PWD/../../Public.pri)

QT -= gui

TARGET = HttpServer

win32 {
   DLLDESTDIR = $$MGS_BIN_PATH
}
DESTDIR = $$MGS_LIBRARY_PATH

TEMPLATE = lib
CONFIG += shared dll
TARGET = $$qtLibraryTargetName($$TARGET)

win32 {
   DEFINES += QTWEBAPPLIB_EXPORT
}

include($$PWD/httpserver.pri)
