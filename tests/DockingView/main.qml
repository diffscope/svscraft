// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents

Window {
    id: window
    visible: true
    width: 1200
    height: 800
    title: qsTr("Docking View")

    property DockingContext context: DockingContext {}

    property DockingHost host: DockingHost {
        context: window.context
        window: window
    }
    property list<DockingPane> panels: [
        DockingPane {
            panelKey: "first"
            title: qsTr("First Panel")
            minimumSize: Qt.size(120, 100)
            preferredSize: Qt.size(300, 300)

            Label {
                anchors.centerIn: parent
                text: qsTr("First Panel")
            }
        },

        DockingPane {
            panelKey: "second"
            title: qsTr("Second Panel")
            minimumSize: Qt.size(80, 100)

            Label {
                anchors.centerIn: parent
                text: qsTr("Second Panel")
            }
        },

        DockingPane {
            panelKey: "third"
            title: qsTr("Third Panel")

            Label {
                anchors.centerIn: parent
                text: qsTr("Third Panel")
            }
        }
    ]

    DockingView {
        anchors.fill: parent
        host: window.host
    }
    Component.onCompleted: {
        for (const pane of panels)
            context.registerPanel(pane.panel)
        host.layout.restore({
            kind: "split",
            nodeId: "root",
            orientation: Qt.Horizontal,
            weights: [1, 2],
            children: [
                {
                    kind: "group",
                    nodeId: "left",
                    tabBarEdge: Qt.LeftEdge,
                    widthLocked: true,
                    lockedSize: Qt.size(300, 300),
                    leading: {
                        panelIds: ["first"],
                        selected: "first"
                    }
                },
                {
                    kind: "group",
                    nodeId: "center",
                    tabBarEdge: Qt.TopEdge,
                    leading: {
                        panelIds: ["second"],
                        selected: "second"
                    },
                    trailing: {
                        panelIds: ["third"],
                        selected: "third"
                    }
                }
            ]
        })
    }
}
