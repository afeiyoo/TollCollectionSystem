include($$PWD/../../../Public.pri)

QT       *= core gui network

TARGET = SmartLaneControllerDemo
TEMPLATE = app

win32 {
    DESTDIR = $$MGS_BIN_PATH/win/$$TARGET
} else {
    DESTDIR = $$MGS_BIN_PATH/linux/$$TARGET
}

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)

SOURCES += \
    main.cpp \
    relaycontroller.cpp \
    smartlanecontroller.cpp \
    widget.cpp

HEADERS += \
    relaycontroller.h \
    smartlanecontroller.h \
    widget.h

FORMS += \
    widget.ui

# 引入第三方库
unix:!macx|win32: LIBS += \
    -l$$qtLibraryTargetName(CuteLogger) \

INCLUDEPATH += \
    $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include \

copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
