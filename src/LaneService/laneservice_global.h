#pragma once

#include <QtCore/qglobal.h>

#if defined(Q_OS_WIN)
    #if defined(LANESERVICE_DYNAMIC)
        #define LANESERVICE_EXPORT Q_DECL_EXPORT
    #elif defined(LANESERVICE_STATIC)
        #define LANESERVICE_EXPORT
    #else
        #define LANESERVICE_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define LANESERVICE_EXPORT
#endif
