import QtQml
import QtQuick

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Window {
    id: window
    default property DockingPane pane
    Component.onCompleted: () => {
        pane.DockingImpl.dockingView = null
    }
    DockingPanel {
        id: panel
        anchors.fill: parent
        pane: window.pane
        dockingWindow: true
        onRemoved: () => {
            window.close()
            window.pane.DockingImpl.window = null
        }
    }
    onVisibleChanged: () => {
        if (visible) {
            pane.DockingImpl.dockingView = null
            pane.DockingImpl.window = window
            panel.set()
        }
    }
}