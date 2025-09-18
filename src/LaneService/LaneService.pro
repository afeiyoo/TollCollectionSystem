include($$PWD/../../Public.pri)

QT += core network

QT -= gui

TARGET = LaneService
TEMPLATE = lib

MGS_LANE_GANTRY = $$PWD/MgsLaneGantry


isEqual(TEMPLATE, app) {
    win32 {
        DESTDIR = $$MGS_BIN_PATH/win/$$TARGET
    } else {
        DESTDIR = $$MGS_BIN_PATH/linux/$$TARGET
    }

    CONFIG += cmdline
    DEFINES += LANESERVICE_STATIC
    # 在线式
    DEFINES += LANESERVICE_NETWORK

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
    # 网络版
    DEFINES += LANESERVICE_LOCAL
}

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)
include($$THIRD_PARTY_LIBRARY_PATH/EasyQtSql/EasyQtSql.pri)
include($$THIRD_PARTY_LIBRARY_PATH/HttpClient/HttpClient.pri)

SOURCES += \
    config/config.cpp \
    global/globalmanager.cpp \
    laneservice.cpp \

HEADERS += \
    config/config.h \
    global/constant.h \
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
    $$MGS_LANE_GANTRY

# 拷贝第三方库
isEqual(TEMPLATE, app) {
    copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
    copyLibsToDestdir($$qtLibraryTargetName(Jcon))
    copyLibsToDestdir($$qtLibraryTargetName(QJson))
}

RESOURCES += \
    resource.qrc
