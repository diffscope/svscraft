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

#ifndef SVSCRAFT_THEMEDITEM_P_P_H
#define SVSCRAFT_THEMEDITEM_P_P_H

#include <SVSCraftQuick/private/ThemedItem_p.h>

namespace SVS {

    class ThemedItemAttachedType : public QObject {
        Q_OBJECT
        QML_NAMED_ELEMENT(ThemedItem)
        QML_ATTACHED(ThemedItem)
    public:
        static ThemedItem *qmlAttachedProperties(QObject *object);
    };

    class ThemedItemPrivate {
        Q_DECLARE_PUBLIC(ThemedItem)
    public:
        ThemedItem *q_ptr;
        SVSQmlNamespace::ControlType controlType = SVSQmlNamespace::CT_Normal;
        SVSQmlNamespace::BackgroundLevel backgroundLevel = SVSQmlNamespace::BL_Primary;
        SVSQmlNamespace::ForegroundLevel foregroundLevel = SVSQmlNamespace::FL_Primary;
        SVSQmlNamespace::TabIndicator tabIndicator = SVSQmlNamespace::TI_Fill;
        SVSQmlNamespace::DividerStroke dividerStroke = SVSQmlNamespace::DS_Auto;
        double trackStart = 0.0;
        bool foldable = false;
        bool folded = false;
        bool flat = false;
        bool splitHandleVisible = true;
        bool splitHandleEnabled = true;

    };
}

#endif // SVSCRAFT_THEMEDITEM_P_P_H