include($$PWD/../../Public.pri)

QT       *= core gui network

TARGET = InfoboardManager
TEMPLATE = app

DESTDIR = $$MGS_BIN_PATH

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)

SOURCES += \
    CHXInfoBoard.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    CHXInfoBoard.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 引入第三方库 CuteLogger
LIBS *= -l$$qtLibraryTargetName(CuteLogger)

INCLUDEPATH += $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include
