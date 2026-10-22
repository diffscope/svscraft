// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

DockingTabButton {
    id: control
    highlighted: panel?.shown ?? false
    Accessible.role: Accessible.PageTab
    Accessible.checkable: true
    Accessible.checked: highlighted
    Accessible.description: panel?.description ?? ""
    DescriptiveText.statusTip: panel?.description ?? ""

    Rectangle {
        id: badge
        readonly property bool showsNumber: !control.compactLayout && (control.panel?.badgeNumber ?? 0) > 0

        visible: (control.panel?.badgeNumber ?? 0) !== 0
        width: showsNumber ? Math.max(height + 2, Math.ceil(badgeText.implicitWidth) + 4) : 4
        height: showsNumber ? Math.max(10, Math.ceil(badgeText.implicitHeight)) : 4
        radius: height / 2
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 1
        LayoutMirroring.enabled: false
        color: Theme.controlColor(control.panel?.badgeType ?? SVS.CT_Accent)

        Label {
            id: badgeText
            anchors.centerIn: parent
            visible: badge.showsNumber
            font: GlobalHelper.fontWithPixelSize(control.font, 8)
            textFormat: Text.PlainText
            text: (control.panel?.badgeNumber ?? 0).toLocaleString()
            Accessible.ignored: true
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
    }

    DragHandler {
        target: null
        onActiveChanged: if (active && control.panel)
            control.panel.context.startDrag(control.panel)
    }
}
