#ifndef ECHO_GLOBAL_H
#define ECHO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ECHO_LIBRARY)
#  define ECHO_EXPORT Q_DECL_EXPORT
#else
#  define ECHO_EXPORT Q_DECL_IMPORT
#endif

#endif // ECHO_GLOBAL_H
