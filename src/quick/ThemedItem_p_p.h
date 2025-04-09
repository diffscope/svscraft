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
        SVSCraft::ControlType controlType = SVSCraft::CT_Normal;
        SVSCraft::BackgroundLevel backgroundLevel = SVSCraft::BL_Primary;
        SVSCraft::ForegroundLevel foregroundLevel = SVSCraft::FL_Primary;
        SVSCraft::TabIndicator tabIndicator = SVSCraft::TI_Fill;
        SVSCraft::DividerStroke dividerStroke = SVSCraft::DS_Auto;
        bool foldable = false;
        bool folded = false;
        bool flat = false;
        bool splitHandleVisible = true;
        bool splitHandleEnabled = true;
        bool toolTipFollowsCursor = false;

    };
}

#endif // SVSCRAFT_THEMEDITEM_P_P_H