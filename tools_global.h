#ifndef TOOLS_GLOBAL_H
#define TOOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLS_LIBRARY)
#  define TOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLS_GLOBAL_H
