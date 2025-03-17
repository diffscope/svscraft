/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

#ifndef SVSCRAFT_SVSCRAFTQMLGLOBAL_H
#define SVSCRAFT_SVSCRAFTQMLGLOBAL_H

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

#endif // SVSCRAFT_SVSCRAFTQMLGLOBAL_H
