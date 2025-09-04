shared|dll {
    DEFINES += LANESERVICE_DYNAMIC
}else{
    DEFINES += LANESERVICE_STATIC
}

SOURCES += \
    $$PWD/global/globalmanager.cpp \
    $$PWD/laneservice.cpp

HEADERS += \
    $$PWD/global/globalmanager.h \
    $$PWD/laneservice.h \
    $$PWD/laneservice_global.h

INCLUDEPATH += $$PWD/include
