// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQml.Models
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

Menu {
    id: control
    required property DockingPanel panel
    title: qsTr("Move Panel")

    function place(host, group, kind, side, anchor, after, edge) {
        panel.context.requestPlaceAt(panel, {
            hostId: host.hostId,
            groupId: group?.nodeId ?? "",
            kind: kind,
            side: side,
            anchor: anchor,
            after: after,
            edge: edge
        })
    }

    component DirectionMenu: Menu {
        id: directionMenu
        required property DockingHost host
        required property DockingGroup group
        required property int side
        readonly property var panels: side ? group.trailingPanels : group.leadingPanels
        title: side ? qsTr("Trailing Direction") : qsTr("Leading Direction")

        MenuItem {
            text: qsTr("Start")
            onTriggered: control.place(directionMenu.host, directionMenu.group, Docking.Tab, directionMenu.side, directionMenu.panels[0]?.key ?? "", false, Qt.LeftEdge)
        }

        MenuItem {
            text: qsTr("End")
            onTriggered: control.place(directionMenu.host, directionMenu.group, Docking.Tab, directionMenu.side, "", true, Qt.LeftEdge)
        }

        Instantiator {
            model: directionMenu.panels.length > 0 ? 1 : 0
            delegate: MenuSeparator {}
            onObjectAdded: (index, object) => directionMenu.insertItem(2, object)
            onObjectRemoved: (index, object) => directionMenu.removeItem(object)
        }

        Instantiator {
            model: directionMenu.panels
            onObjectAdded: (index, object) => directionMenu.insertMenu(index + 3, object)
            onObjectRemoved: (index, object) => directionMenu.removeMenu(object)
            delegate: Menu {
                id: anchorMenu
                required property var modelData
                title: modelData.title

                MenuItem {
                    text: qsTr("Before")
                    onTriggered: control.place(directionMenu.host, directionMenu.group, Docking.Tab, directionMenu.side, anchorMenu.modelData.key, false, Qt.LeftEdge)
                }

                MenuItem {
                    text: qsTr("After")
                    onTriggered: control.place(directionMenu.host, directionMenu.group, Docking.Tab, directionMenu.side, anchorMenu.modelData.key, true, Qt.LeftEdge)
                }
            }
        }
    }

    Instantiator {
        model: control.panel?.context?.hosts ?? []
        onObjectAdded: (index, object) => control.insertMenu(index, object)
        onObjectRemoved: (index, object) => control.removeMenu(object)
        delegate: Menu {
            id: hostMenu
            required property var modelData
            readonly property DockingHost host: modelData
            title: host.window?.title || qsTr("Workspace")

            Instantiator {
                model: hostMenu.host.empty ? 1 : 0
                delegate: MenuItem {
                    text: qsTr("Fill Window")
                    onTriggered: control.place(hostMenu.host, null, Docking.EmptyHost, 0, "", true, Qt.LeftEdge)
                }
                onObjectAdded: (index, object) => hostMenu.insertItem(0, object)
                onObjectRemoved: (index, object) => hostMenu.removeItem(object)
            }

            Instantiator {
                model: hostMenu.host.layout.groups
                onObjectAdded: (index, object) => hostMenu.insertMenu(index, object)
                onObjectRemoved: (index, object) => hostMenu.removeMenu(object)
                delegate: Menu {
                    id: groupMenu
                    required property var modelData
                    readonly property DockingGroup group: modelData
                    title: [group.leadingPanels[0]?.title, group.trailingPanels[0]?.title].filter(t => t).join(" / ")

                    DirectionMenu {
                        host: hostMenu.host
                        group: groupMenu.group
                        side: 0
                    }

                    DirectionMenu {
                        host: hostMenu.host
                        group: groupMenu.group
                        side: 1
                    }

                    MenuSeparator {}

                    MenuItem {
                        text: qsTr("Left")
                        onTriggered: control.place(hostMenu.host, groupMenu.group, Docking.Edge, 0, "", true, Qt.LeftEdge)
                    }

                    MenuItem {
                        text: qsTr("Right")
                        onTriggered: control.place(hostMenu.host, groupMenu.group, Docking.Edge, 0, "", true, Qt.RightEdge)
                    }

                    MenuItem {
                        text: qsTr("Top")
                        onTriggered: control.place(hostMenu.host, groupMenu.group, Docking.Edge, 0, "", true, Qt.TopEdge)
                    }

                    MenuItem {
                        text: qsTr("Bottom")
                        onTriggered: control.place(hostMenu.host, groupMenu.group, Docking.Edge, 0, "", true, Qt.BottomEdge)
                    }
                }
            }
        }
    }
}
