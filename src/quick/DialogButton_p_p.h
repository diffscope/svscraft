// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SVSCRAFT_DIALOGBUTTON_P_P_H
#define SVSCRAFT_DIALOGBUTTON_P_P_H

#include <SVSCraftQuick/private/DialogButton_p.h>

namespace SVS {

    class DialogButtonPrivate {
        Q_DECLARE_PUBLIC(DialogButton)
    public:
        DialogButton *q_ptr = nullptr;
        DialogButton::LayoutRole layoutRole = DialogButton::AutoRole;
        DialogButton::Emphasis emphasis = DialogButton::Auto;
        bool managed = false;
        bool primary = false;
    };

}

#endif
