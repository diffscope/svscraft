// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

Menu {
    id: control
    required property DockingGroup group
    title: qsTr("Panel Group")
    enabled: Boolean(group)

    MenuItem {
        text: qsTr("Show Panel Names")
        checkable: true
        checked: control.group?.showPanelNames ?? false
        onTriggered: control.group.showPanelNames = checked
    }

    MenuItem {
        text: qsTr("Compact Layout")
        checkable: true
        checked: control.group?.compactLayout ?? false
        onTriggered: control.group.compactLayout = checked
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
        icon.source: "image://fluent-system-icons/dismiss"
        onTriggered: control.group.layout.removeGroup(control.group)
    }
}
