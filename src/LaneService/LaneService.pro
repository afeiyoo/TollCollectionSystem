include($$PWD/../../Public.pri)

QT += core network

QT -= gui

TARGET = LaneService
TEMPLATE = app

isEqual(TEMPLATE, app) {
    win32 {
        DESTDIR = $$MGS_BIN_PATH/win/$$TARGET
    } else {
        DESTDIR = $$MGS_BIN_PATH/linux/$$TARGET
    }

    CONFIG += cmdline
    DEFINES += LANESERVICE_STATIC

    SOURCES += main.cpp
} else {
    win32 {
        DESTDIR = $$MGS_LIBRARY_PATH/win
    } else {
        DESTDIR = $$MGS_LIBRARY_PATH/linux
    }

    CONFIG += shared dll
    TARGET = $$qtLibraryTargetName($$TARGET)

    shared|dll {
        DEFINES += LANESERVICE_DYNAMIC
    }else{
        DEFINES += LANESERVICE_STATIC
    }
}

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)
include($$THIRD_PARTY_LIBRARY_PATH/EasyQtSql/EasyQtSql.pri)

SOURCES += \
    config/config.cpp \
    global/globalmanager.cpp \
    laneservice.cpp \

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


# 拷贝第三方库
isEqual(TEMPLATE, app) {
    copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
    copyLibsToDestdir($$qtLibraryTargetName(Jcon))
    copyLibsToDestdir($$qtLibraryTargetName(QJson))
}
