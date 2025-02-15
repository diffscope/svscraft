#ifndef SVSCRAFTGUIGLOBAL_H
#define SVSCRAFTGUIGLOBAL_H

#include <QtGlobal>

#ifndef SVSCRAFT_GUI_EXPORT
#  ifdef SVSCRAFT_GUI_STATIC
#    define SVSCRAFT_GUI_EXPORT
#  else
#    ifdef SVSCRAFT_GUI_LIBRARY
#      define SVSCRAFT_GUI_EXPORT Q_DECL_EXPORT
#    else
#      define SVSCRAFT_GUI_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // SVSCRAFTGUIGLOBAL_H
