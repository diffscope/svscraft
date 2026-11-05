// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DIALOGBUTTONBOXTEMPLATE_P_H
#define SVSCRAFT_DIALOGBUTTONBOXTEMPLATE_P_H

#include <QList>
#include <QQmlListProperty>
#include <QScopedPointer>
#include <qqmlintegration.h>

#include <QtQuickTemplates2/private/qquickdialogbuttonbox_p.h>

namespace SVS {

    class DialogButtonBoxTemplatePrivate;

    class DialogButtonBoxTemplate : public QQuickDialogButtonBox {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DialogButtonBoxTemplate)
        QML_ELEMENT
        QML_ATTACHED(QQuickDialogButtonBoxAttached)
        QML_EXTENDED_NAMESPACE(QPlatformDialogHelper)

        Q_PROPERTY(QQmlListProperty<QQuickItem> leadingContent READ leadingContent NOTIFY leadingContentChanged)
        Q_PROPERTY(QQmlListProperty<QQuickItem> accessoryContent READ accessoryContent NOTIFY accessoryContentChanged)
        Q_PROPERTY(qreal layoutImplicitWidth READ layoutImplicitWidth NOTIFY layoutChanged)
        Q_PROPERTY(qreal layoutImplicitHeight READ layoutImplicitHeight NOTIFY layoutChanged)
        Q_PROPERTY(bool hasContent READ hasContent NOTIFY layoutChanged)
#if QT_VERSION < QT_VERSION_CHECK(6, 11, 0)
        Q_PROPERTY(QQuickAbstractButton *defaultButton READ defaultButton WRITE setDefaultButton NOTIFY defaultButtonChanged)
        Q_PROPERTY(QPlatformDialogHelper::StandardButton defaultStandardButton READ defaultStandardButton WRITE setDefaultStandardButton NOTIFY defaultStandardButtonChanged)
#endif

    public:
        explicit DialogButtonBoxTemplate(QQuickItem *parent = nullptr);
        ~DialogButtonBoxTemplate() override;

        QQmlListProperty<QQuickItem> leadingContent();
        QQmlListProperty<QQuickItem> accessoryContent();

        qreal layoutImplicitWidth() const;
        qreal layoutImplicitHeight() const;
        bool hasContent() const;

        Q_INVOKABLE QPlatformDialogHelper::StandardButton standardButtonId(QQuickAbstractButton *button) const;
        Q_INVOKABLE QList<QQuickAbstractButton *> buttonsInLayoutOrder() const;
        Q_INVOKABLE void selectDefaultButton(QQuickAbstractButton *button);

#if QT_VERSION < QT_VERSION_CHECK(6, 11, 0)
        QQuickAbstractButton *defaultButton() const;
        void setDefaultButton(QQuickAbstractButton *button);
        QPlatformDialogHelper::StandardButton defaultStandardButton() const;
        void setDefaultStandardButton(QPlatformDialogHelper::StandardButton button);
#endif

    Q_SIGNALS:
        void leadingContentChanged();
        void accessoryContentChanged();
        void layoutChanged();
#if QT_VERSION < QT_VERSION_CHECK(6, 11, 0)
        void defaultButtonChanged();
        void defaultStandardButtonChanged();
#endif

    protected:
        void componentComplete() override;
        void updatePolish() override;
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
        void contentItemChange(QQuickItem *newItem, QQuickItem *oldItem) override;
        void itemChange(ItemChange change, const ItemChangeData &data) override;
        bool isContent(QQuickItem *item) const override;
        void itemAdded(int index, QQuickItem *item) override;
        void itemRemoved(int index, QQuickItem *item) override;
        bool eventFilter(QObject *watched, QEvent *event) override;

    private:
        QScopedPointer<DialogButtonBoxTemplatePrivate> d_ptr;
    };

}

#endif
