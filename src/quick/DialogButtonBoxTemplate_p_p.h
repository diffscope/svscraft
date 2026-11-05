// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DIALOGBUTTONBOXTEMPLATE_P_P_H
#define SVSCRAFT_DIALOGBUTTONBOXTEMPLATE_P_P_H

#include <SVSCraftQuick/private/DialogButtonBoxTemplate_p.h>

#include <QHash>
#include <QList>
#include <QPointer>

namespace SVS {

    class DialogButtonBoxTemplatePrivate {
        Q_DECLARE_PUBLIC(DialogButtonBoxTemplate)
    public:
        static void appendContent(QQmlListProperty<QQuickItem> *property, QQuickItem *item);
        static qsizetype contentCount(QQmlListProperty<QQuickItem> *property);
        static QQuickItem *contentAt(QQmlListProperty<QQuickItem> *property, qsizetype index);
        static void clearContent(QQmlListProperty<QQuickItem> *property);

        void watchItem(QQuickItem *item);
        void unwatchItem(QQuickItem *item);
        void contentChanged(QList<QQuickItem *> *items);
        void scheduleLayout();
        QList<QQuickAbstractButton *> orderedButtons() const;
        void updateLayout();
        void updateKeyTarget();
        QQuickAbstractButton *effectiveDefaultButton() const;

        DialogButtonBoxTemplate *q_ptr = nullptr;
        QList<QQuickAbstractButton *> buttons;
        QList<QQuickItem *> leadingItems;
        QList<QQuickItem *> accessoryItems;
        QHash<QQuickItem *, QList<QMetaObject::Connection>> connections;
        QPointer<QQuickItem> keyTarget;
        qreal implicitWidth = 0;
        qreal implicitHeight = 0;
        bool hasContent = false;
        bool updatingLayout = false;
#if QT_VERSION < QT_VERSION_CHECK(6, 11, 0)
        QPointer<QQuickAbstractButton> defaultButton;
        QPlatformDialogHelper::StandardButton defaultStandardButton = QPlatformDialogHelper::NoButton;
#endif
    };

}

#endif
