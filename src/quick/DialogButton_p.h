// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DIALOGBUTTON_P_H
#define SVSCRAFT_DIALOGBUTTON_P_H

#include <QObject>
#include <QScopedPointer>
#include <qqmlintegration.h>

namespace SVS {

    class DialogButtonPrivate;
    class DialogButtonBoxTemplate;
    class DialogButtonBoxTemplatePrivate;

    class DialogButton : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(DialogButton)
        QML_ELEMENT
        QML_ATTACHED(DialogButton)
        QML_UNCREATABLE("")

        Q_PROPERTY(LayoutRole layoutRole READ layoutRole WRITE setLayoutRole NOTIFY layoutRoleChanged)
        Q_PROPERTY(Emphasis emphasis READ emphasis WRITE setEmphasis NOTIFY emphasisChanged)
        Q_PROPERTY(Emphasis effectiveEmphasis READ effectiveEmphasis NOTIFY effectiveEmphasisChanged)

    public:
        enum LayoutRole {
            AutoRole = -2,
            InvalidRole = -1,
            AcceptRole,
            RejectRole,
            DestructiveRole,
            ActionRole,
            HelpRole,
            YesRole,
            NoRole,
            ResetRole,
            ApplyRole,
        };
        Q_ENUM(LayoutRole)

        enum Emphasis {
            Auto,
            Normal,
            Primary,
            Warning,
            Error,
        };
        Q_ENUM(Emphasis)

        ~DialogButton() override;

        static DialogButton *qmlAttachedProperties(QObject *object);

        LayoutRole layoutRole() const;
        void setLayoutRole(LayoutRole role);

        Emphasis emphasis() const;
        void setEmphasis(Emphasis emphasis);
        Emphasis effectiveEmphasis() const;

    Q_SIGNALS:
        void layoutRoleChanged();
        void emphasisChanged();
        void effectiveEmphasisChanged();

    private:
        friend class DialogButtonBoxTemplate;
        friend class DialogButtonBoxTemplatePrivate;
        explicit DialogButton(QObject *parent = nullptr);
        void setManaged(bool managed, bool primary = false);

        QScopedPointer<DialogButtonPrivate> d_ptr;
    };

}

#endif
