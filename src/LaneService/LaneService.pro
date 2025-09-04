include($$PWD/../../Public.pri)

QT += core network

QT -= gui

TARGET = LaneService

DESTDIR = $$MGS_LIBRARY_PATH

TEMPLATE = lib
CONFIG += shared dll
TARGET = $$qtLibraryTargetName($$TARGET)

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)
include($$THIRD_PARTY_LIBRARY_PATH/EasyQtSql/EasyQtSql.pri)

shared|dll {
    DEFINES += LANESERVICE_DYNAMIC
}else{
    DEFINES += LANESERVICE_STATIC
}

SOURCES += \
    config/config.cpp \
    global/globalmanager.cpp \
    laneservice.cpp

HEADERS += \
    config/config.h \
    global/constant.h \
    global/defs.h \
    global/globalmanager.h \
    laneservice.h \
    laneservice_global.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

# 引入第三方库
unix:!macx|win32: LIBS += \
    -l$$qtLibraryTargetName(CuteLogger) -l$$qtLibraryTargetName(Jcon) \
    -l$$qtLibraryTargetName(QJson)


INCLUDEPATH += \
    $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include \
    $$THIRD_PARTY_LIBRARY_PATH/Jcon \
    $$THIRD_PARTY_LIBRARY_PATH/QJson/include
