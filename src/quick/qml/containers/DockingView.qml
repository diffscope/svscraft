// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick

import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

DockingViewTemplate {
    id: control

    property Component background: Rectangle {
        color: Theme.backgroundPrimaryColor
    }

    property Component groupDelegate: DockingGroupView {}

    property Component separatorDelegate: Rectangle {
        color: parent?.parent?.pressed ? Theme.accentColor : Theme.dividerStrokeColor(control.ThemedItem.dividerStroke, Theme.splitterColor)
    }
    property bool automaticFloatingWindows: true
    readonly property var currentDropTarget: {
        if (host && emptyDrop.containsDrag)
            return {
                hostId: host.hostId,
                kind: Docking.EmptyHost
            }
        for (let i = 0; i < groupRepeater.count; ++i) {
            const target = groupRepeater.itemAt(i)?.item?.currentDropTarget
            if (target?.hostId !== undefined)
                return target
        }
        return ({})
    }

    readonly property Component floatingHostComponent: DockingHost {
        role: DockingHost.Floating
    }
    property Component floatingWindowComponent: floatingDefaults.windowComponent

    QtObject {
        id: floatingDefaults
        readonly property Component windowComponent: Qt.createComponent("SVSCraft.UIComponents", "DockingWindow", Component.PreferSynchronous, floatingDefaults)
    }

    Connections {
        target: control.host?.context ?? null
        enabled: control.automaticFloatingWindows && control.host?.role === DockingHost.Embedded

        function onFloatingRequested(request) {
            if (request.finished)
                return
            const host = control.floatingHostComponent.createObject(control.host.context, {
                context: control.host.context
            })
            const hint = request.panel.effectivePreferredSize
            const properties = {
                host: host,
                transientParent: control.Window.window,
                width: Math.max(400, hint.width),
                height: Math.max(300, hint.height + 64)
            }
            if (control.floatingWindowComponent === floatingDefaults.windowComponent)
                properties.automaticDestruction = true
            const window = control.floatingWindowComponent.createObject(control, properties)
            if (!window) {
                host.destroy()
                request.reject("floatingWindowUnavailable")
                return
            }
            if (request.accept(host).accepted)
                window.show()
            else {
                window.destroy()
                host.destroy()
            }
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: control.background
    }

    DropArea {
        id: emptyDrop
        anchors.fill: parent
        enabled: control.host?.empty ?? false
        keys: ["application/x-svscraft-panel"]
        onEntered: drag => drag.accepted = drag.source?.dockingContext === control.host.context
        onDropped: drop => {
            const p = control.host.context.panel(drop.source.panelKey)
            const request = control.host.context.requestPlaceAt(p, {
                hostId: control.host.hostId,
                kind: Docking.EmptyHost
            })
            drop.accepted = request.result.accepted
        }

        Rectangle {
            anchors.fill: parent
            visible: parent.containsDrag
            color: Theme.accentColor
            opacity: 0.25
        }
    }

    Repeater {
        id: groupRepeater
        model: control.groupFrames
        delegate: DockingDelegate {
            id: groupLoader
            required property var modelData
            x: modelData.rect.x
            y: modelData.rect.y
            width: modelData.rect.width
            height: modelData.rect.height
            sourceComponent: control.groupDelegate
            initialProperties: ({group: modelData.node, dockingView: control})
        }
    }

    Repeater {
        model: control.splitHandles
        delegate: Item {
            id: handle
            required property var modelData
            readonly property bool pressed: handleMouse.pressed
            x: modelData.rect.x
            y: modelData.rect.y
            width: modelData.rect.width
            height: modelData.rect.height
            activeFocusOnTab: true
            Accessible.name: qsTr("Resize Panels")
            Keys.onPressed: event => {
                const negative = modelData.horizontal ? Qt.Key_Left : Qt.Key_Up
                const positive = modelData.horizontal ? Qt.Key_Right : Qt.Key_Down
                if (event.key !== negative && event.key !== positive)
                    return
                control.resizeSplit(modelData.node, modelData.index, (event.key === positive ? 1 : -1) * (event.modifiers & Qt.ShiftModifier ? 10 : 1))
                event.accepted = true
            }

            Loader {
                anchors.fill: parent
                sourceComponent: control.separatorDelegate
            }

            MouseArea {
                id: handleMouse
                anchors.fill: parent
                anchors.margins: -3
                cursorShape: handle.modelData.horizontal ? Qt.SplitHCursor : Qt.SplitVCursor
                enabled: handle.modelData.node.childNodes[handle.modelData.index]?.collapsed !== true && handle.modelData.node.childNodes[handle.modelData.index + 1]?.collapsed !== true
                property point previous
                onPressed: mouse => previous = mapToItem(control, mouse.x, mouse.y)
                onPositionChanged: mouse => {
                    if (!pressed)
                        return
                    const point = mapToItem(control, mouse.x, mouse.y)
                    const delta = handle.modelData.horizontal ? point.x - previous.x : point.y - previous.y
                    control.resizeSplit(handle.modelData.node, handle.modelData.index, delta)
                    previous = point
                }
            }
        }
    }
}
