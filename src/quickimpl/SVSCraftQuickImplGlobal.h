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

#ifndef SVSCRAFT_SVSCRAFTQUICKIMPLGLOBAL_H
#define SVSCRAFT_SVSCRAFTQUICKIMPLGLOBAL_H

#include <QtGlobal>

#ifndef SVSCRAFT_QUICK_IMPL_EXPORT
#  ifdef SVSCRAFT_QUICK_IMPL_STATIC
#    define SVSCRAFT_QUICK_IMPL_EXPORT
#  else
#    ifdef SVSCRAFT_QUICK_IMPL_LIBRARY
#      define SVSCRAFT_QUICK_IMPL_EXPORT Q_DECL_EXPORT
#    else
#      define SVSCRAFT_QUICK_IMPL_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // SVSCRAFT_SVSCRAFTQUICKIMPLGLOBAL_H

