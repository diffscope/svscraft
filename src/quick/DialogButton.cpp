// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DialogButton_p.h"
#include "DialogButton_p_p.h"

namespace SVS {

    DialogButton::DialogButton(QObject *parent) : QObject(parent), d_ptr(new DialogButtonPrivate) {
        Q_D(DialogButton);
        d->q_ptr = this;
    }

    DialogButton::~DialogButton() = default;

    DialogButton *DialogButton::qmlAttachedProperties(QObject *object) {
        return new DialogButton(object);
    }

    DialogButton::LayoutRole DialogButton::layoutRole() const {
        Q_D(const DialogButton);
        return d->layoutRole;
    }

    void DialogButton::setLayoutRole(LayoutRole role) {
        Q_D(DialogButton);
        if (d->layoutRole == role)
            return;
        d->layoutRole = role;
        Q_EMIT layoutRoleChanged();
    }

    DialogButton::Emphasis DialogButton::emphasis() const {
        Q_D(const DialogButton);
        return d->emphasis;
    }

    void DialogButton::setEmphasis(Emphasis emphasis) {
        Q_D(DialogButton);
        if (d->emphasis == emphasis)
            return;
        const auto previous = effectiveEmphasis();
        d->emphasis = emphasis;
        Q_EMIT emphasisChanged();
        if (previous != effectiveEmphasis())
            Q_EMIT effectiveEmphasisChanged();
    }

    DialogButton::Emphasis DialogButton::effectiveEmphasis() const {
        Q_D(const DialogButton);
        if (!d->managed)
            return d->emphasis;
        if (d->emphasis == Auto)
            return d->primary ? Primary : Auto;
        if (d->emphasis == Primary && !d->primary)
            return Normal;
        return d->emphasis;
    }

    void DialogButton::setManaged(bool managed, bool primary) {
        Q_D(DialogButton);
        const auto previous = effectiveEmphasis();
        d->managed = managed;
        d->primary = primary;
        if (previous != effectiveEmphasis())
            Q_EMIT effectiveEmphasisChanged();
    }

}
