#ifndef SVSCRAFTCOREGLOBAL_H
#define SVSCRAFTCOREGLOBAL_H

#include <QtGlobal>

#ifndef SVSCRAFT_CORE_EXPORT
#  ifdef SVSCRAFT_CORE_STATIC
#    define SVSCRAFT_CORE_EXPORT
#  else
#    ifdef SVSCRAFT_CORE_LIBRARY
#      define SVSCRAFT_CORE_EXPORT Q_DECL_EXPORT
#    else
#      define SVSCRAFT_CORE_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // SVSCRAFTCOREGLOBAL_H
