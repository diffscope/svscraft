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
    property var destinations: []
    title: qsTr("Move Panel")

    function refreshDestinations() {
        const hosts = Array.from(panel?.context?.hosts ?? [])
        if (hosts.length === destinations.length && hosts.every((host, index) => {
            const destination = destinations[index]
            return destination.host === host && destination.revision === host.layout.revision
        }))
            return

        destinations = hosts.map(host => ({
            host: host,
            revision: host.layout.revision,
            groups: Array.from(host.layout.groups, group => ({
                group: group,
                leadingPanels: Array.from(group.leadingPanels),
                trailingPanels: Array.from(group.trailingPanels)
            }))
        }))
    }

    function place(host, group, kind, side, anchor, after, edge) {
        if (!host || (kind !== Docking.EmptyHost && !group))
            return
        panel?.context?.requestPlaceAt(panel, {
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
        required property var panels
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
                title: modelData?.title ?? ""

                MenuItem {
                    text: qsTr("Before")
                    onTriggered: anchorMenu.placeByAnchor(false)
                }

                MenuItem {
                    text: qsTr("After")
                    onTriggered: anchorMenu.placeByAnchor(true)
                }

                function placeByAnchor(after) {
                    if (modelData)
                        control.place(directionMenu.host, directionMenu.group, Docking.Tab, directionMenu.side, modelData.key, after, Qt.LeftEdge)
                }
            }
        }
    }

    Instantiator {
        model: control.destinations
        onObjectAdded: (index, object) => control.insertMenu(index, object)
        onObjectRemoved: (index, object) => control.removeMenu(object)
        delegate: Menu {
            id: hostMenu
            required property var modelData
            readonly property DockingHost host: modelData.host
            title: host?.window?.title || qsTr("Workspace")

            Instantiator {
                model: hostMenu.modelData.groups.length === 0 ? 1 : 0
                delegate: MenuItem {
                    text: qsTr("Fill Window")
                    onTriggered: control.place(hostMenu.host, null, Docking.EmptyHost, 0, "", true, Qt.LeftEdge)
                }
                onObjectAdded: (index, object) => hostMenu.insertItem(0, object)
                onObjectRemoved: (index, object) => hostMenu.removeItem(object)
            }

            Instantiator {
                model: hostMenu.modelData.groups
                onObjectAdded: (index, object) => hostMenu.insertMenu(index, object)
                onObjectRemoved: (index, object) => hostMenu.removeMenu(object)
                delegate: Menu {
                    id: groupMenu
                    required property var modelData
                    readonly property DockingGroup group: modelData.group
                    title: [modelData.leadingPanels[0]?.title, modelData.trailingPanels[0]?.title].filter(t => t).join(" / ")

                    DirectionMenu {
                        host: hostMenu.host
                        group: groupMenu.group
                        side: 0
                        panels: groupMenu.modelData.leadingPanels
                    }

                    DirectionMenu {
                        host: hostMenu.host
                        group: groupMenu.group
                        side: 1
                        panels: groupMenu.modelData.trailingPanels
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
