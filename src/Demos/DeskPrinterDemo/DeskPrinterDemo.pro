include($$PWD/../../../Public.pri)

QT       *= core gui network

TARGET = DeskPrinterDemo
TEMPLATE = app

win32 {
    DESTDIR = $$MGS_BIN_PATH/win/$$TARGET
} else {
    DESTDIR = $$MGS_BIN_PATH/linux/$$TARGET
}

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)
include($$THIRD_PARTY_LIBRARY_PATH/HttpClient/HttpClient.pri)
include($$THIRD_PARTY_LIBRARY_PATH/CuteLogger/Cute)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui


# 引入第三方库
unix:!macx|win32: LIBS += \
    -l$$qtLibraryTargetName(CuteLogger) \

INCLUDEPATH += \
    $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
