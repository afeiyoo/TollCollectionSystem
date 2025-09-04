include($$PWD/../../Public.pri)

QT -= gui

TARGET = CuteLogger

DESTDIR = $$MGS_LIBRARY_PATH

TEMPLATE = lib
CONFIG += shared dll
TARGET = $$qtLibraryTargetName($$TARGET)

include($$PWD/CuteLogger.pri)

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
