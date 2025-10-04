include($$PWD/../../Public.pri)

QT       *= core gui network

TARGET = LaneSystem
TEMPLATE = app

win32 {
    DESTDIR = $$MGS_BIN_PATH/win/$$TARGET
} else {
    DESTDIR = $$MGS_BIN_PATH/linux/$$TARGET
}

DEFINES += USE_ELAWIDGETTOOLS

include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)
include($$THIRD_PARTY_LIBRARY_PATH/EasyQtSql/EasyQtSql.pri)
include($$THIRD_PARTY_LIBRARY_PATH/HttpClient/HttpClient.pri)

SOURCES += \
    bend/mtcin/autocardprocess.cpp \
    bend/mtcin/mibizenv.cpp \
    bend/mtcin/mideskprocess.cpp \
    config/config.cpp \
    config/laneenv.cpp \
    dao/dataservice.cpp \
    dao/midataservice.cpp \
    global/globalmanager.cpp \
    global/signalmanager.cpp \
    global/statemanager.cpp \
    gui/component/mgsbasedialog.cpp \
    gui/component/mgsdevicepanel.cpp \
    gui/component/mgsiconbutton.cpp \
    gui/component/mgspagearea.cpp \
    gui/component/mgsplateedit.cpp \
    gui/component/mgsrecenttradepanel.cpp \
    gui/component/mgsscrolltext.cpp \
    gui/component/mgsweightinfopanel.cpp \
    gui/delegate/mgscolordelegate.cpp \
    gui/delegate/mgskeydelegate.cpp \
    gui/delegate/mgsweightinfodelegate.cpp \
    gui/mgsauthdialog.cpp \
    gui/mgsbasepage.cpp \
    gui/mgsetcpage.cpp \
    gui/mgsmainwindow.cpp \
    gui/mgsmenudialog.cpp \
    gui/mgsmtcinpage.cpp \
    gui/mgsmtcoutpage.cpp \
    gui/mgsplateeditdialog.cpp \
    gui/mgsoptionsdialog.cpp \
    main.cpp\
    # claneform.cpp \
    # cbizenv.cpp \
    bend/mtcout/cmodeskprocess.cpp \
    # bend/mtcout/csptprocess.cpp \
    # bend/etc/cetcprocess.cpp \
 \    # bend/etc/cetcdeskprocess.cpp
    tools/laneauth.cpp

HEADERS += \
    bend/mtcin/autocardprocess.h \
    bend/mtcin/mibizenv.h \
    bend/mtcin/mideskprocess.h \
    # claneform.h \
    config/config.h \
    config/laneenv.h \
    dao/dataservice.h \
    dao/midataservice.h \
    global/constant.h \
    global/globalmanager.h \
    global/signalmanager.h \
    global/statemanager.h \
    gui/component/mgsbasedialog.h \
    gui/component/mgsdevicepanel.h \
    gui/component/mgsiconbutton.h \
    gui/component/mgspagearea.h \
    gui/component/mgsplateedit.h \
    gui/component/mgsrecenttradepanel.h \
    gui/component/mgsscrolltext.h \
    gui/component/mgsweightinfopanel.h \
    gui/delegate/mgscolordelegate.h \
    gui/delegate/mgskeydelegate.h \
    gui/delegate/mgsweightinfodelegate.h \
    gui/mgsauthdialog.h \
    gui/mgsbasepage.h \
    gui/mgsetcpage.h \
    gui/mgsmainwindow.h \
    gui/mgsmenudialog.h \
    gui/mgsmtcinpage.h \
    gui/mgsmtcoutpage.h \
    gui/mgsplateeditdialog.h \
    gui/mgsoptionsdialog.h \
    # cbizenv.h \
    bend/mtcout/cmodeskprocess.h \
    # bend/mtcout/csptprocess.h \
    # bend/etc/cetcprocess.h \
    # bend/etc/cetcdeskprocess.h \
 \    # ILaneDeviceCtrl.h
    tools/laneauth.h

FORMS += claneform.ui

# 引入第三方库
unix:!macx|win32: LIBS += \
    -l$$qtLibraryTargetName(ElaWidgetTools) -l$$qtLibraryTargetName(CuteLogger) \
    -l$$qtLibraryTargetName(LaneService) -l$$qtLibraryTargetName(Jcon) \
    -l$$qtLibraryTargetName(QSimpleUpdater) -l$$qtLibraryTargetName(QJson) \

INCLUDEPATH += \
    $$THIRD_PARTY_LIBRARY_PATH/ElaWidgetTools \
    $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include \
    $$THIRD_PARTY_LIBRARY_PATH/Jcon \
    $$PWD/../LaneService \
    $$THIRD_PARTY_LIBRARY_PATH/QSimpleUpdater/include \
    $$THIRD_PARTY_LIBRARY_PATH/QJson/include

RESOURCES += \
    resource.qrc

RC_ICONS += mgskj.ico

copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
copyLibsToDestdir($$qtLibraryTargetName(ElaWidgetTools))
copyLibsToDestdir($$qtLibraryTargetName(LaneService))
copyLibsToDestdir($$qtLibraryTargetName(Jcon))
copyLibsToDestdir($$qtLibraryTargetName(QSimpleUpdater))
copyLibsToDestdir($$qtLibraryTargetName(QJson))

