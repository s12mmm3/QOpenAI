#ifndef QOPENAI_GLOBAL_H
#define QOPENAI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BUILD_SHARED_LIBS)
#if defined(QCLOUDMUSICAPI_LIBRARY)
#  define QOPENAI_EXPORT Q_DECL_EXPORT
#else
#  define QOPENAI_EXPORT Q_DECL_IMPORT
#endif
#else
#  define QOPENAI_EXPORT
#endif

#endif // QOPENAI_GLOBAL_H
