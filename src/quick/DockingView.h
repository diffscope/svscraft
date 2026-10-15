// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DOCKINGVIEW_H
#define SVSCRAFT_DOCKINGVIEW_H

#include <QQuickItem>
#include <QScopedPointer>
#include <qqmlintegration.h>

#include <SVSCraftQuick/DockingTypes.h>

Q_MOC_INCLUDE(<SVSCraftQuick / DockingHost.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingSplit.h>)
Q_MOC_INCLUDE(<SVSCraftQuick / DockingPanel.h>)

namespace SVS {
    class DockingHost;
    class DockingSplit;
    class DockingPanel;
    class DockingViewPrivate;

    class SVSCRAFT_QUICK_EXPORT DockingView : public QQuickItem {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DockingView)
        QML_NAMED_ELEMENT(DockingViewTemplate)
        Q_PROPERTY(DockingHost *host READ host WRITE setHost NOTIFY hostChanged)
        Q_PROPERTY(QVariantList groupFrames READ groupFrames NOTIFY framesChanged)
        Q_PROPERTY(QVariantList splitHandles READ splitHandles NOTIFY framesChanged)
        Q_PROPERTY(qreal padding READ padding WRITE setPadding NOTIFY paddingChanged)
    public:
        explicit DockingView(QQuickItem *parent = nullptr);
        ~DockingView() override;
        DockingHost *host() const;
        void setHost(DockingHost *host);
        QVariantList groupFrames() const;
        QVariantList splitHandles() const;
        qreal padding() const;
        void setPadding(qreal padding);
        Q_INVOKABLE void resizeSplit(DockingSplit *split, int index, qreal delta);
        Q_INVOKABLE QRectF previewPlacement(DockingPanel *panel, const QVariantMap &placement) const;
        Q_INVOKABLE QRectF groupGeometry(const QString &groupId);
    Q_SIGNALS:
        void hostChanged();
        void framesChanged();
        void paddingChanged();

    protected:
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
        void updatePolish() override;

    private:
        QScopedPointer<DockingViewPrivate> d_ptr;
    };
}

#endif
