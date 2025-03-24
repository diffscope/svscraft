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

#ifndef SVSCRAFT_THEMEDITEM_P_H
#define SVSCRAFT_THEMEDITEM_P_H

#include <QObject>

#include <SVSCraftQuick/Theme.h>
#include <SVSCraftQml/private/SVSQmlNamespace_p.h>

namespace SVS {

    class ThemedItemAttachedType;

    class ThemedItemPrivate;

    class ThemedItem : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ThemedItem)
        QML_ANONYMOUS

        Q_PROPERTY(SVS::SVSCraft::ControlType controlType READ controlType WRITE setControlType NOTIFY controlTypeChanged)
        Q_PROPERTY(SVS::SVSCraft::BackgroundLevel backgroundLevel READ backgroundLevel WRITE setBackgroundLevel NOTIFY backgroundLevelChanged)
        Q_PROPERTY(SVS::SVSCraft::ForegroundLevel foregroundLevel READ foregroundLevel WRITE setForegroundLevel NOTIFY foregroundLevelChanged)
        Q_PROPERTY(bool foldable READ foldable WRITE setFoldable NOTIFY foldableChanged)
        Q_PROPERTY(bool folded READ folded WRITE setFolded NOTIFY foldedChanged)
        Q_PROPERTY(double trackStart READ trackStart WRITE setTrackStart NOTIFY trackStartChanged)
        Q_PROPERTY(bool flat READ flat WRITE setFlat NOTIFY flatChanged)
        Q_PROPERTY(SVS::SVSCraft::TabIndicator tabIndicator READ tabIndicator WRITE setTabIndicator NOTIFY tabIndicatorChanged)
        Q_PROPERTY(SVS::SVSCraft::DividerStroke dividerStroke READ dividerStroke WRITE setDividerStroke NOTIFY dividerStrokeChanged)
        Q_PROPERTY(bool splitHandleVisible READ splitHandleVisible WRITE setSplitHandleVisible NOTIFY splitHandleVisibleChanged)
        Q_PROPERTY(bool splitHandleEnabled READ splitHandleEnabled WRITE setSplitHandleEnabled NOTIFY splitHandleEnabledChanged)
    public:
        ~ThemedItem() override;

        SVSCraft::ControlType controlType() const;
        void setControlType(SVSCraft::ControlType value);

        SVS::SVSCraft::BackgroundLevel backgroundLevel() const;
        void setBackgroundLevel(SVS::SVSCraft::BackgroundLevel value);

        SVS::SVSCraft::ForegroundLevel foregroundLevel() const;
        void setForegroundLevel(SVS::SVSCraft::ForegroundLevel value);

        bool foldable() const;
        void setFoldable(bool value);

        bool folded() const;
        void setFolded(bool value);

        double trackStart() const;
        void setTrackStart(double value);

        bool flat() const;
        void setFlat(bool value);

        SVSCraft::TabIndicator tabIndicator() const;
        void setTabIndicator(SVSCraft::TabIndicator value);

        SVSCraft::DividerStroke dividerStroke() const;
        void setDividerStroke(SVSCraft::DividerStroke value);

        bool splitHandleVisible() const;
        void setSplitHandleVisible(bool value);

        bool splitHandleEnabled() const;
        void setSplitHandleEnabled(bool value);

    signals:
        void controlTypeChanged();
        void backgroundLevelChanged();
        void foregroundLevelChanged();
        void foldableChanged();
        void foldedChanged();
        void trackStartChanged();
        void flatChanged();
        void tabIndicatorChanged();
        void dividerStrokeChanged();
        void splitHandleVisibleChanged();
        void splitHandleEnabledChanged();

    private:
        friend class ThemedItemAttachedType;
        explicit ThemedItem(QObject *parent = nullptr);
        QScopedPointer<ThemedItemPrivate> d_ptr;
    };

}

#endif // SVSCRAFT_THEMEDITEM_P_H
