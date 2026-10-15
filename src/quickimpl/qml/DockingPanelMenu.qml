// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

Menu {
    id: control
    required property DockingPanel panel

    MenuItem {
        text: qsTr("Float")
        icon.source: "image://fluent-system-icons/panel_separate_window"
        onTriggered: control.panel.context.requestFloat(control.panel)
    }

    MenuItem {
        text: control.panel?.shown ? qsTr("Hide") : qsTr("Show")
        icon.source: control.panel?.shown ? "image://fluent-system-icons/subtract" : "image://fluent-system-icons/eye"
        enabled: !control.panel?.shown || (control.panel?.group?.canHide(control.panel) ?? false)
        onTriggered: {
            if (control.panel.shown)
                control.panel.context.hide(control.panel)
            else
                control.panel.context.show(control.panel)
        }
    }

    DockingMoveMenu {
        panel: control.panel
    }

    MenuItem {
        text: qsTr("Close")
        icon.source: "image://fluent-system-icons/dismiss"
        onTriggered: control.panel.context.remove(control.panel)
    }

    MenuSeparator {}

    DockingGroupMenu {
        group: control.panel?.group ?? null
    }
}
