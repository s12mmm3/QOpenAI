#ifndef QOPENAI_GLOBAL_H
#define QOPENAI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QOPENAI_LIBRARY)
#define QOPENAI_EXPORT Q_DECL_EXPORT
#else
#define QOPENAI_EXPORT Q_DECL_IMPORT
#endif

#endif // QOPENAI_GLOBAL_H
