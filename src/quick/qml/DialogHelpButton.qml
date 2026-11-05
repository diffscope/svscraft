// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Templates as T

import SVSCraft.UIComponents

Button {
    text: qsTranslate("QPlatformTheme", "Help")
    display: T.AbstractButton.IconOnly
    icon.source: "image://fluent-system-icons/question_circle"
    icon.width: 24
    icon.height: 24
    flat: true
    padding: 2
    horizontalPadding: 2
}
