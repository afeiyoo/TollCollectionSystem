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

SOURCES += \
    # bend/mtcin/cardrobothandler.cpp \
    # bend/mtcin/mtcindeskhandler.cpp \
    global/globalmanager.cpp \
    global/modemanager.cpp \
    global/signalmanager.cpp \
    gui/component/mgscontentdialog.cpp \
    gui/component/mgsdevicepanel.cpp \
    gui/component/mgsiconbutton.cpp \
    gui/component/mgsmenu.cpp \
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
    gui/mgsmtcinpage.cpp \
    gui/mgsmtcoutpage.cpp \
    gui/mgsplateeditdialog.cpp \
    main.cpp\
    # claneform.cpp \
    cbizenv.cpp \
    # bend/mtcout/cmodeskprocess.cpp \
    # bend/mtcout/csptprocess.cpp \
    # bend/etc/cetcprocess.cpp \
    # bend/etc/cetcdeskprocess.cpp

HEADERS += \
    # bend/mtcin/cardrobothandler.h \
    # bend/mtcin/mtcindeskhandler.h \
    # claneform.h \
    global/constant.h \
    global/globalmanager.h \
    global/modemanager.h \
    global/signalmanager.h \
    gui/component/mgscontentdialog.h \
    global/tlaneconfig.h \
    gui/component/mgsdevicepanel.h \
    gui/component/mgsiconbutton.h \
    gui/component/mgsmenu.h \
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
    gui/mgsmtcinpage.h \
    gui/mgsmtcoutpage.h \
    gui/mgsplateeditdialog.h \
    ibizservice.h \
    cbizenv.h \
    # bend/mtcout/cmodeskprocess.h \
    # bend/mtcout/csptprocess.h \
    # bend/etc/cetcprocess.h \
    # bend/etc/cetcdeskprocess.h \
    # ILaneDeviceCtrl.h

FORMS += claneform.ui

# 引入第三方库
unix:!macx|win32: LIBS += \
    -l$$qtLibraryTargetName(ElaWidgetTools) -l$$qtLibraryTargetName(CuteLogger) \
    -l$$qtLibraryTargetName(LaneService) -l$$qtLibraryTargetName(Jcon) \
    -l$$qtLibraryTargetName(QSimpleUpdater)

INCLUDEPATH += \
    $$THIRD_PARTY_LIBRARY_PATH/ElaWidgetTools \
    $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include \
    $$THIRD_PARTY_LIBRARY_PATH/Jcon \
    $$PWD/../LaneService \
    $$THIRD_PARTY_LIBRARY_PATH/QSimpleUpdater/include

RESOURCES += \
    resource.qrc

copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
copyLibsToDestdir($$qtLibraryTargetName(ElaWidgetTools))
copyLibsToDestdir($$qtLibraryTargetName(LaneService))
copyLibsToDestdir($$qtLibraryTargetName(Jcon))
copyLibsToDestdir($$qtLibraryTargetName(QSimpleUpdater))
copyLibsToDestdir($$qtLibraryTargetName(QJson))

