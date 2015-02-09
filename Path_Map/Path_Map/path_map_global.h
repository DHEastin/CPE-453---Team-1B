#ifndef PATH_MAP_GLOBAL_H
#define PATH_MAP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PATH_MAP_LIBRARY)
#  define PATH_MAPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PATH_MAPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PATH_MAP_GLOBAL_H
