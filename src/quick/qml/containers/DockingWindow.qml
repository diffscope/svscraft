// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick

import SVSCraft.UIComponents

Window {
    id: window
    required property DockingHost host
    property bool automaticDestruction: false
    property bool interactionEnabled: true
    property bool disposing: false
    property rect savedNormalGeometry: Qt.rect(x, y, width, height)

    function saveNormalGeometry() {
        if (visibility === Window.Windowed)
            savedNormalGeometry = Qt.rect(x, y, width, height)
    }
    onXChanged: saveNormalGeometry()
    onYChanged: saveNormalGeometry()
    onWidthChanged: saveNormalGeometry()
    onHeightChanged: saveNormalGeometry()
    title: host.context?.activePanel?.group?.layout.host === host ? host.context.activePanel.title : qsTr("Panels")
    width: 400
    height: 300
    StatusTextContext.statusContext: transientParent?.StatusTextContext.statusContext ?? null
    StatusTextContext.contextHelpContext: transientParent?.StatusTextContext.contextHelpContext ?? null

    DockingView {
        id: view
        anchors.fill: parent
        host: window.host
        enabled: window.interactionEnabled
    }
    Component.onCompleted: host.window = window
    onClosing: close => {
        close.accepted = interactionEnabled && host.close().accepted
    }

    Connections {
        target: window.host

        function onCloseRequested() {
            if (window.disposing)
                return
            window.visible = false
            if (window.automaticDestruction) {
                window.disposing = true
                Qt.callLater(() => {
                    if (window.host) {
                        window.host.context?.unregisterHost(window.host, true)
                        window.host.destroy()
                    }
                    window.destroy()
                })
            }
        }
    }
}
