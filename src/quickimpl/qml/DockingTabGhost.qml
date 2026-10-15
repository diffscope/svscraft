// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

ToolButton {
    id: control
    required property DockingPanel panel
    opacity: 0.45
    enabled: false
    Accessible.ignored: true
    text: panel?.title ?? ""
    icon.source: panel?.iconSource ?? ""
    display: AbstractButton.IconOnly
    highlighted: true

    Binding {
        target: control
        property: "icon.color"
        value: control.panel?.iconColor
        when: control.panel?.iconColor.valid ?? false
    }
}
