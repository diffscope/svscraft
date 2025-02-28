#ifndef SVSCRAFTQMLGLOBAL_H
#define SVSCRAFTQMLGLOBAL_H

#include <QtGlobal>

#ifndef SVSCRAFT_QML_EXPORT
#  ifdef SVSCRAFT_QML_STATIC
#    define SVSCRAFT_QML_EXPORT
#  else
#    ifdef SVSCRAFT_QML_LIBRARY
#      define SVSCRAFT_QML_EXPORT Q_DECL_EXPORT
#    else
#      define SVSCRAFT_QML_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif //SVSCRAFTQMLGLOBAL_H
