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

#include "SoftwarePainterNode.h"
#include "SoftwarePainterNode_p.h"

#include <QPainter>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSGRendererInterface>

namespace SVS {

    SoftwarePainterNode::SoftwarePainterNode(QQuickItem *item)
        : d_ptr(new SoftwarePainterNodePrivate) {
        Q_D(SoftwarePainterNode);
        d->q_ptr = this;
        d->item = item;
    }

    SoftwarePainterNode::~SoftwarePainterNode() = default;

    QSGRenderNode::StateFlags SoftwarePainterNode::changedStates() const {
        return {};
    }

    QSGRenderNode::RenderingFlags SoftwarePainterNode::flags() const {
        return BoundedRectRendering;
    }

    QRectF SoftwarePainterNode::rect() const {
        Q_D(const SoftwarePainterNode);
        return d->boundingRect;
    }

    void SoftwarePainterNode::render(const RenderState *state) {
        Q_D(SoftwarePainterNode);
        if (!d->item || !d->item->window()) {
            return;
        }
        auto *rendererInterface = d->item->window()->rendererInterface();
        auto *painter = static_cast<QPainter *>(
            rendererInterface->getResource(d->item->window(), QSGRendererInterface::PainterResource));
        if (!painter) {
            return;
        }
        const auto *clipRegion = state->clipRegion();
        if (clipRegion) {
            painter->setClipRegion(*clipRegion, Qt::ReplaceClip);
        }
        painter->setTransform(matrix()->toTransform());
        painter->setOpacity(inheritedOpacity());
        paint(painter);
    }

    QQuickItem *SoftwarePainterNode::item() const {
        Q_D(const SoftwarePainterNode);
        return d->item;
    }

    void SoftwarePainterNode::setBoundingRect(const QRectF &rect) {
        Q_D(SoftwarePainterNode);
        if (d->boundingRect == rect) {
            return;
        }
        d->boundingRect = rect;
        markDirty(QSGNode::DirtyGeometry);
    }

}
