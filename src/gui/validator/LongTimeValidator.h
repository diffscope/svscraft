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

#ifndef SVSCRAFT_LONGTIMEVALIDATOR_H
#define SVSCRAFT_LONGTIMEVALIDATOR_H

#include <QIntValidator>

#include <SVSCraftGui/SVSCraftGuiGlobal.h>

namespace SVS {

    class SVSCRAFT_GUI_EXPORT LongTimeValidator : public QIntValidator {
        Q_OBJECT

    public:
        explicit LongTimeValidator(QObject *parent = nullptr);
        LongTimeValidator(int minimum, int maximum, QObject *parent = nullptr);

        State validate(QString &, int &) const override;
        void fixup(QString &) const override;

    };

}

#endif // SVSCRAFT_LONGTIMEVALIDATOR_H
