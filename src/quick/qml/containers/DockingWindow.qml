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
    title: d.titlePanel?.title ?? qsTr("Panels")
    width: 400
    height: 300
    StatusTextContext.statusContext: transientParent?.StatusTextContext.statusContext ?? null
    StatusTextContext.contextHelpContext: transientParent?.StatusTextContext.contextHelpContext ?? null

    QtObject {
        id: d
        readonly property DockingHost host: window.host
        property DockingPanel lastActivePanel: null
        readonly property DockingPanel titlePanel: {
            if (!host)
                return null
            const activePanel = host.context?.activePanel
            if (activePanel?.group?.layout.host === host)
                return activePanel
            const previousPanel = lastActivePanel?.group?.layout.host === host ? lastActivePanel : null
            if (previousPanel?.shown)
                return previousPanel
            const groups = host.layout.groups
            for (const group of groups) {
                const selectedPanel = group.leadingPanel ?? group.trailingPanel
                if (selectedPanel)
                    return selectedPanel
            }
            if (previousPanel)
                return previousPanel
            for (const group of groups) {
                const panel = group.leadingPanels[0] ?? group.trailingPanels[0]
                if (panel)
                    return panel
            }
            return null
        }
        onHostChanged: rememberActivePanel()

        function rememberActivePanel() {
            const activePanel = host?.context?.activePanel
            if (activePanel && activePanel.group?.layout.host === host)
                lastActivePanel = activePanel
        }
    }

    Connections {
        target: d.host?.context ?? null

        function onChanged() {
            d.rememberActivePanel()
        }
    }

    DockingView {
        id: view
        anchors.fill: parent
        host: window.host
        enabled: window.interactionEnabled
    }
    Component.onCompleted: {
        host.window = window
        d.rememberActivePanel()
    }
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
