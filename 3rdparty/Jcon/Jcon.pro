include($$PWD/../../Public.pri)

QT += testlib websockets

QT -= gui

TARGET = Jcon

win32 {
    DLLDESTDIR = $$MGS_BIN_PATH
}
DESTDIR = $$MGS_LIBRARY_PATH

TEMPLATE = lib
CONFIG += shared dll
TARGET = $$qtLibraryTargetName($$TARGET)

shared|dll {
    DEFINES += JCON_DYNAMIC
}else{
    DEFINES += JCON_STATIC
}

HEADERS = jcon*.h \
    json*.h \
    string_util.h
SOURCES = json*.cpp \
    string_util.cpp
