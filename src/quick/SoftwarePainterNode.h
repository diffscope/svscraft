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

#ifndef SVSCRAFT_SOFTWAREPAINTERNODE_H
#define SVSCRAFT_SOFTWAREPAINTERNODE_H

#include <QScopedPointer>
#include <QSGRenderNode>

#include <SVSCraftQuick/SVSCraftQuickGlobal.h>

class QPainter;
class QQuickItem;

namespace SVS {

    class SoftwarePainterNodePrivate;

    class SVSCRAFT_QUICK_EXPORT SoftwarePainterNode : public QSGRenderNode {
        Q_DECLARE_PRIVATE(SoftwarePainterNode)

    public:
        explicit SoftwarePainterNode(QQuickItem *item);
        ~SoftwarePainterNode() override;

        StateFlags changedStates() const override;
        RenderingFlags flags() const override;
        QRectF rect() const override;
        void render(const RenderState *state) final;

    protected:
        QQuickItem *item() const;
        void setBoundingRect(const QRectF &rect);
        virtual void paint(QPainter *painter) = 0;

    private:
        QScopedPointer<SoftwarePainterNodePrivate> d_ptr;
    };

}

#endif // SVSCRAFT_SOFTWAREPAINTERNODE_H
