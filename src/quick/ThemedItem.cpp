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

#include "ThemedItem_p.h"
#include "ThemedItem_p_p.h"

namespace SVS {

    ThemedItem *ThemedItemAttachedType::qmlAttachedProperties(QObject *object) {
        return new ThemedItem(object);
    }

    ThemedItem::ThemedItem(QObject *parent) : QObject(parent), d_ptr(new ThemedItemPrivate) {
        Q_D(ThemedItem);
        d->q_ptr = this;
    }
    ThemedItem::~ThemedItem() = default;
    SVSCraft::ControlType ThemedItem::controlType() const {
        Q_D(const ThemedItem);
        return d->controlType;
    }
    void ThemedItem::setControlType(SVSCraft::ControlType value) {
        Q_D(ThemedItem);
        if (d->controlType != value) {
            d->controlType = value;
            emit controlTypeChanged();
        }
    }
    SVS::SVSCraft::BackgroundLevel ThemedItem::backgroundLevel() const {
        Q_D(const ThemedItem);
        return d->backgroundLevel;
    }
    void ThemedItem::setBackgroundLevel(SVS::SVSCraft::BackgroundLevel value) {
        Q_D(ThemedItem);
        if (d->backgroundLevel != value) {
            d->backgroundLevel = value;
            emit backgroundLevelChanged();
        }
    }
    SVS::SVSCraft::ForegroundLevel ThemedItem::foregroundLevel() const {
        Q_D(const ThemedItem);
        return d->foregroundLevel;
    }
    void ThemedItem::setForegroundLevel(SVS::SVSCraft::ForegroundLevel value) {
        Q_D(ThemedItem);
        if (d->foregroundLevel != value) {
            d->foregroundLevel = value;
            emit foregroundLevelChanged();
        }
    }
    bool ThemedItem::foldable() const {
        Q_D(const ThemedItem);
        return d->foldable;
    }
    void ThemedItem::setFoldable(bool value) {
        Q_D(ThemedItem);
        if (d->foldable != value) {
            d->foldable = value;
            emit foldableChanged();
        }
    }
    bool ThemedItem::folded() const {
        Q_D(const ThemedItem);
        return d->folded;
    }
    void ThemedItem::setFolded(bool value) {
        Q_D(ThemedItem);
        if (d->folded != value) {
            d->folded = value;
            emit foldedChanged();
        }
    }
    double ThemedItem::trackStart() const {
        Q_D(const ThemedItem);
        return d->trackStart;
    }
    void ThemedItem::setTrackStart(double value) {
        Q_D(ThemedItem);
        if (d->trackStart != value) {
            d->trackStart = value;
            emit trackStartChanged();
        }
    }
    bool ThemedItem::flat() const {
        Q_D(const ThemedItem);
        return d->flat;
    }
    void ThemedItem::setFlat(bool value) {
        Q_D(ThemedItem);
        if (d->flat != value) {
            d->flat = value;
            emit flatChanged();
        }
    }
    SVSCraft::TabIndicator ThemedItem::tabIndicator() const {
        Q_D(const ThemedItem);
        return d->tabIndicator;
    }
    void ThemedItem::setTabIndicator(SVSCraft::TabIndicator value) {
        Q_D(ThemedItem);
        if (d->tabIndicator != value) {
            d->tabIndicator = value;
            emit tabIndicatorChanged();
        }
    }
    SVSCraft::DividerStroke ThemedItem::dividerStroke() const {
        Q_D(const ThemedItem);
        return d->dividerStroke;
    }
    void ThemedItem::setDividerStroke(SVSCraft::DividerStroke value) {
        Q_D(ThemedItem);
        if (d->dividerStroke != value) {
            d->dividerStroke = value;
            emit dividerStrokeChanged();
        }
    }
    bool ThemedItem::splitHandleVisible() const {
        Q_D(const ThemedItem);
        return d->splitHandleVisible;
    }
    void ThemedItem::setSplitHandleVisible(bool value) {
        Q_D(ThemedItem);
        if (d->splitHandleVisible != value) {
            d->splitHandleVisible = value;
            emit splitHandleVisibleChanged();
        }
    }
    bool ThemedItem::splitHandleEnabled() const {
        Q_D(const ThemedItem);
        return d->splitHandleEnabled;
    }
    void ThemedItem::setSplitHandleEnabled(bool value) {
        Q_D(ThemedItem);
        if (d->splitHandleEnabled != value) {
            d->splitHandleEnabled = value;
            emit splitHandleEnabledChanged();
        }
    }
    bool ThemedItem::toolTipFollowsCursor() const {
        Q_D(const ThemedItem);
        return d->toolTipFollowsCursor;
    }
    void ThemedItem::setToolTipFollowsCursor(bool value) {
        Q_D(ThemedItem);
        if (d->toolTipFollowsCursor != value) {
            d->toolTipFollowsCursor = value;
            emit toolTipFollowsCursorChanged();
        }
    }

}