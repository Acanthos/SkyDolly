#ifndef SKYCONNECTLIB_H
#define SKYCONNECTLIB_H

#include <QtGlobal>

#ifdef SKYCONNECT_EXPORT
# define SKYCONNECT_API Q_DECL_EXPORT
#else
# define SKYCONNECT_API Q_DECL_IMPORT
#endif

#endif // SKYCONNECTLIB_H
