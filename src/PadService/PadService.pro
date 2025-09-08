include($$PWD/../../Public.pri)

QT *= core network sql
QT -= gui

CONFIG += cmdline

TARGET = PadService
TEMPLATE = app

# 编译得到的最终产物输出路径
DESTDIR = $$MGS_BIN_PATH/$$TARGET

include($$THIRD_PARTY_LIBRARY_PATH/EasyQtSql/EasyQtSql.pri)
include($$THIRD_PARTY_LIBRARY_PATH/HttpClient/HttpClient.pri)
include($$THIRD_PARTY_LIBRARY_PATH/utils/Utils.pri)
include($$THIRD_PARTY_LIBRARY_PATH/HttpServer/HttpServer.pri)

SOURCES += \
        bend/bizhandler.cpp \
        bend/dataservice.cpp \
        bend/dtpsender.cpp \
        bend/httphandler.cpp \
        config/baseexception.cpp \
        config/config.cpp \
        global/globalmanager.cpp \
        main.cpp

HEADERS += \
    bean/t_auditpayback.h \
    bean/t_etcout.h \
    bean/t_freetempvehicle.h \
    bean/t_laneinputshift.h \
    bean/t_mtcout.h \
    bean/t_specialcards.h \
    bean/t_splitout.h \
    bean/t_mticketuse.h \
    bend/bizhandler.h \
    bend/dataservice.h \
    bend/dtpsender.h \
    bend/httphandler.h \
    config/baseexception.h \
    config/config.h \
    global/defs.h \
    global/globalmanager.h

# 引入第三方库
unix:!macx|win32: LIBS += \
    -l$$qtLibraryTargetName(CuteLogger) \
    -l$$qtLibraryTargetName(QJson) \

INCLUDEPATH += \
    $$THIRD_PARTY_LIBRARY_PATH/CuteLogger/include \
    $$THIRD_PARTY_LIBRARY_PATH/QJson/include \

copyLibsToDestdir($$qtLibraryTargetName(CuteLogger))
copyLibsToDestdir($$qtLibraryTargetName(QJson))
