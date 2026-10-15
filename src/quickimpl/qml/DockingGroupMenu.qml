// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

Menu {
    id: control
    required property DockingGroup group
    property var groupView: null
    title: qsTr("Panel Group")
    enabled: Boolean(group)

    MenuItem {
        text: qsTr("Show Tab Bar")
        enabled: Boolean(control.groupView)
        onTriggered: control.groupView.revealRequested = true
    }

    MenuItem {
        text: qsTr("Auto Hide Tab Bar")
        checkable: true
        checked: control.group?.autoHide ?? false
        onTriggered: control.group.autoHide = checked
    }

    MenuItem {
        text: qsTr("Lock Width")
        checkable: true
        checked: control.group?.widthLocked ?? false
        onTriggered: control.group.widthLocked = checked
    }

    MenuItem {
        text: qsTr("Lock Height")
        checkable: true
        checked: control.group?.heightLocked ?? false
        onTriggered: control.group.heightLocked = checked
    }

    Menu {
        title: qsTr("Tab Bar Edge")

        MenuItem {
            text: qsTr("Top")
            checkable: true
            autoExclusive: true
            checked: control.group?.tabBarEdge === Qt.TopEdge
            onTriggered: control.group.tabBarEdge = Qt.TopEdge
        }

        MenuItem {
            text: qsTr("Right")
            checkable: true
            autoExclusive: true
            checked: control.group?.tabBarEdge === Qt.RightEdge
            onTriggered: control.group.tabBarEdge = Qt.RightEdge
        }

        MenuItem {
            text: qsTr("Bottom")
            checkable: true
            autoExclusive: true
            checked: control.group?.tabBarEdge === Qt.BottomEdge
            onTriggered: control.group.tabBarEdge = Qt.BottomEdge
        }

        MenuItem {
            text: qsTr("Left")
            checkable: true
            autoExclusive: true
            checked: control.group?.tabBarEdge === Qt.LeftEdge
            onTriggered: control.group.tabBarEdge = Qt.LeftEdge
        }
    }

    MenuSeparator {}

    MenuItem {
        text: qsTr("Close Panel Group")
        onTriggered: control.group.layout.removeGroup(control.group)
    }
}
