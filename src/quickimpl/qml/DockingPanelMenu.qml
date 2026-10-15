// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

Menu {
    id: control
    required property DockingPanel panel
    property var groupView: null

    MenuItem {
        text: qsTr("Float")
        onTriggered: control.panel.context.requestFloat(control.panel)
    }

    MenuItem {
        text: control.panel?.shown ? qsTr("Hide") : qsTr("Show")
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
        text: qsTr("Remove")
        onTriggered: control.panel.context.remove(control.panel)
    }

    MenuSeparator {}

    DockingGroupMenu {
        group: control.panel?.group ?? null
        groupView: control.groupView
    }
}
