// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft
import SVSCraft.UIComponents

ToolButton {
    id: control
    required property DockingPanel panel
    property var groupView: null
    text: panel?.title ?? ""
    icon.source: panel?.iconSource ?? ""
    display: AbstractButton.IconOnly
    highlighted: panel?.shown ?? false

    Binding {
        target: control
        property: "icon.color"
        value: control.panel?.iconColor
        when: control.panel?.iconColor.valid ?? false
    }
    Accessible.role: Accessible.PageTab
    Accessible.checkable: true
    Accessible.checked: highlighted
    Accessible.description: panel?.description ?? ""
    DescriptiveText.statusTip: panel?.description ?? ""

    Rectangle {
        id: badge
        visible: (control.panel?.badgeNumber ?? 0) !== 0
        width: control.panel?.badgeNumber > 0 ? Math.max(height, badgeText.implicitWidth) : 8
        height: control.panel?.badgeNumber > 0 ? Math.max(16, badgeText.implicitHeight) : 8
        radius: height / 2
        anchors.right: control.mirrored ? undefined : parent.right
        anchors.left: control.mirrored ? parent.left : undefined
        anchors.leftMargin: control.padding
        anchors.rightMargin: control.padding
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.controlColor(control.panel?.badgeType ?? SVS.CT_Accent)

        Label {
            id: badgeText
            anchors.centerIn: parent
            visible: control.panel?.badgeNumber > 0
            padding: 2
            text: (control.panel?.badgeNumber ?? 0).toLocaleString()
        }
    }
    onClicked: {
        if (!panel)
            return
        if (panel.shown)
            panel.context.hide(panel)
        else
            panel.context.show(panel)
    }
    Keys.onPressed: event => {
        if (event.key === Qt.Key_Menu || event.key === Qt.Key_F10 && (event.modifiers & Qt.ShiftModifier)) {
            options.popup()
            event.accepted = true
        }
    }

    TapHandler {
        acceptedButtons: Qt.RightButton
        onSingleTapped: options.popup()
    }

    DockingPanelMenu {
        id: options
        panel: control.panel
        groupView: control.groupView
    }

    DragHandler {
        target: null
        onActiveChanged: if (active && control.panel)
            control.panel.context.startDrag(control.panel)
    }
}
