// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGGROUPVIEW_H
#define SVSCRAFT_DOCKINGGROUPVIEW_H

#include <QQuickItem>
#include <QScopedPointer>

#include <SVSCraftQuick/DockingTypes.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingGroup.h>)

namespace SVS {
    class DockingGroup;
    class DockingGroupViewPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingGroupView : public QQuickItem {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingGroupView)
        QML_NAMED_ELEMENT(DockingGroupViewTemplate)
        Q_PROPERTY(DockingGroup *group READ group WRITE setGroup NOTIFY groupChanged)
        Q_PROPERTY(QRectF barRect READ barRect NOTIFY geometryChanged)
        Q_PROPERTY(qreal barThickness READ barThickness CONSTANT)
        Q_PROPERTY(QRectF leadingRect READ leadingRect NOTIFY geometryChanged)
        Q_PROPERTY(QRectF trailingRect READ trailingRect NOTIFY geometryChanged)
        Q_PROPERTY(QVariantList dropRegions READ dropRegions NOTIFY geometryChanged)
        Q_PROPERTY(bool revealBar READ revealBar WRITE setRevealBar NOTIFY geometryChanged)
        Q_PROPERTY(bool mirrored READ mirrored WRITE setMirrored NOTIFY geometryChanged)
    public:
        explicit DockingGroupView(QQuickItem *parent = nullptr);
        ~DockingGroupView() override;
        DockingGroup *group() const;
        void setGroup(DockingGroup *group);
        QRectF barRect() const;
        qreal barThickness() const;
        QRectF leadingRect() const;
        QRectF trailingRect() const;
        QVariantList dropRegions() const;
        bool revealBar() const;
        void setRevealBar(bool reveal);
        bool mirrored() const;
        void setMirrored(bool mirrored);
        Q_INVOKABLE QVariantMap hitTest(qreal x, qreal y) const;
        Q_INVOKABLE void resizeContents(qreal ratio);
    Q_SIGNALS:
        void groupChanged();
        void geometryChanged();

    protected:
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    private:
        QScopedPointer<DockingGroupViewPrivate> d_ptr;
    };
}

#endif
