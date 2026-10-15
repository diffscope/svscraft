// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls

import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

DockingGroupViewTemplate {
    id: control
    property DockingViewTemplate dockingView: null

    property Component tabDelegate: DockingTab {
        groupView: control
    }

    property Component panelDelegate: DockingPanelFrame {}

    property Component highlightDelegate: DockingDropGuide {
        region: panelDrop.target
        origin: Qt.point(area.x, area.y)
        highlighted: true
    }

    property Component dropGuideDelegate: DockingDropGuide {}

    property Component ghostDelegate: DockingTabGhost {}
    property bool revealRequested: false
    readonly property var currentDropTarget: tabDrop.containsDrag ? tabDrop.target : panelDrop.target
    readonly property DockingContext context: group?.layout.host.context ?? null
    readonly property bool verticalBar: group && (group.tabBarEdge === Qt.LeftEdge || group.tabBarEdge === Qt.RightEdge)
    mirrored: LayoutMirroring.enabled
    revealBar: revealRequested || Boolean(context?.dragging) || groupOptions.visible
    activeFocusOnTab: true
    Keys.onPressed: event => {
        if (event.key === Qt.Key_Escape) {
            revealRequested = false
            event.accepted = true
        } else if (event.key === Qt.Key_Menu || event.key === Qt.Key_F10) {
            revealRequested = true
            groupOptions.popup()
            event.accepted = true
        }
    }

    component PanelLoader: DockingDelegate {
        id: panelLoader
        required property int side
        readonly property rect area: side ? control.trailingRect : control.leadingRect
        readonly property DockingPanel panel: side ? control.group?.trailingPanel ?? null : control.group?.leadingPanel ?? null
        x: area.x
        y: area.y
        width: area.width
        height: area.height
        visible: area.width > 0 && area.height > 0
        sourceComponent: control.panelDelegate
        initialProperties: ({panel: panel, groupView: control})
    }

    PanelLoader {
        side: 0
    }

    PanelLoader {
        side: 1
    }

    MouseArea {
        id: contentHandle
        activeFocusOnTab: true
        Accessible.name: qsTr("Resize Panel Directions")
        readonly property rect a: control.leadingRect
        readonly property rect b: control.trailingRect
        visible: Boolean(control.group?.leadingPanel && control.group?.trailingPanel)
        x: control.verticalBar ? a.x : control.mirrored ? b.x + b.width - 3 : a.x + a.width - 3
        y: control.verticalBar ? a.y + a.height - 3 : a.y
        width: control.verticalBar ? a.width : 7
        height: control.verticalBar ? 7 : a.height
        cursorShape: control.verticalBar ? Qt.SplitVCursor : Qt.SplitHCursor
        Keys.onPressed: event => {
            const negative = control.verticalBar ? Qt.Key_Up : Qt.Key_Left
            const positive = control.verticalBar ? Qt.Key_Down : Qt.Key_Right
            if (event.key !== negative && event.key !== positive)
                return
            let delta = event.key === positive ? 0.01 : -0.01
            if (!control.verticalBar && control.mirrored)
                delta = -delta
            control.resizeContents(control.group.contentRatio + delta)
            event.accepted = true
        }
        onPositionChanged: mouse => {
            if (!pressed)
                return
            const point = mapToItem(control, mouse.x, mouse.y)
            const total = control.verticalBar ? a.height + b.height : a.width + b.width
            let position = control.verticalBar ? point.y - a.y : point.x - Math.min(a.x, b.x)
            if (!control.verticalBar && control.mirrored)
                position = total - position
            control.resizeContents(position / total)
        }

        Rectangle {
            anchors.centerIn: parent
            width: control.verticalBar ? parent.width : 1
            height: control.verticalBar ? 1 : parent.height
            color: contentHandle.pressed ? Theme.accentColor : Theme.dividerStrokeColor(control.ThemedItem.dividerStroke, Theme.splitterColor)
        }
    }

    DropArea {
        id: panelDrop
        anchors.fill: parent
        keys: ["application/x-svscraft-panel"]
        property var target: ({})

        function update(drag) {
            const next = control.hitTest(drag.x, drag.y)
            drag.accepted = drag.source?.dockingContext === control.context && next.hostId !== undefined
            if (!drag.accepted) {
                target = ({})
                return
            }
            const panel = control.context.panel(drag.source.panelKey)
            if (!control.context.canPlaceAt(panel, next).accepted) {
                drag.accepted = false
                target = ({})
                return
            }
            if (next.kind === Docking.Edge && control.dockingView) {
                const rect = control.dockingView.previewPlacement(panel, next)
                if (rect.width <= 0 || rect.height <= 0) {
                    drag.accepted = false
                    target = ({})
                    return
                }
            }
            const currentRect = target.previewRect
            const nextRect = next.previewRect
            if (target.hostId !== next.hostId || target.groupId !== next.groupId || target.kind !== next.kind || target.side !== next.side || target.anchor !== next.anchor || target.edge !== next.edge || currentRect?.x !== nextRect.x || currentRect?.y !== nextRect.y || currentRect?.width !== nextRect.width || currentRect?.height !== nextRect.height)
                target = next
        }
        onEntered: drag => update(drag)
        onPositionChanged: drag => update(drag)
        onExited: target = ({})
        onDropped: drop => {
            update(drop)
            if (!drop.accepted)
                return
            const request = control.context.requestPlaceAt(control.context.panel(drop.source.panelKey), target)
            drop.accepted = request.result.accepted
            target = ({})
        }

        Repeater {
            model: control.dropRegions
            delegate: DockingDelegate {
                required property var modelData
                readonly property rect panelRect: modelData.panelRect
                readonly property bool current: panelDrop.containsDrag && panelDrop.target.groupId === modelData.groupId && panelDrop.target.kind === modelData.kind && panelDrop.target.side === modelData.side && panelDrop.target.edge === modelData.edge
                visible: {
                    if (!panelDrop.containsDrag || tabDrop.containsDrag || current && control.highlightDelegate)
                        return false
                    const point = panelDrop.drag
                    return point.x >= panelRect.x && point.x < panelRect.x + panelRect.width && point.y >= panelRect.y && point.y < panelRect.y + panelRect.height
                }
                width: panelDrop.width
                height: panelDrop.height
                sourceComponent: control.dropGuideDelegate
                initialProperties: ({region: modelData})
            }
        }

        Loader {
            readonly property rect area: panelDrop.target.previewRect ?? Qt.rect(0, 0, 0, 0)
            x: area.x
            y: area.y
            width: area.width
            height: area.height
            active: panelDrop.containsDrag && panelDrop.target.hostId !== undefined
            sourceComponent: control.highlightDelegate
        }
    }

    HoverHandler {
        onHoveredChanged: if (!hovered)
            control.revealRequested = false
    }

    Item {
        id: bar
        x: control.barRect.x
        y: control.barRect.y
        width: control.barRect.width
        height: control.barRect.height
        visible: width > 0 && height > 0

        Rectangle {
            anchors.fill: parent
            color: Theme.backgroundPrimaryColor
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: mouse => groupOptions.popup(bar, mouse.x, mouse.y)
        }

        DockingGroupMenu {
            id: groupOptions
            group: control.group
            groupView: control
        }

        component SideBar: Item {
            id: sideBar
            required property int side
            readonly property DockingGroup sourceGroup: control.group
            property var panels: []
            readonly property real total: control.verticalBar ? bar.height : bar.width
            readonly property real available: Math.max(0, total - 8)
            readonly property bool hasPanels: panels.length > 0 || hasGhost
            readonly property bool oppositeHasPanels: (side ? control.group?.leadingPanels.length ?? 0 : control.group?.trailingPanels.length ?? 0) > 0 || (tabDrop.containsDrag && tabDrop.target.side === 1 - side)
            readonly property real start: side ? available - length : 0
            readonly property bool hasGhost: tabDrop.containsDrag && tabDrop.target.side === side
            readonly property alias flickable: flickable
            readonly property alias repeater: tabRepeater
            readonly property alias ghost: ghostLoader
            property int layoutRevision: 0
            readonly property real tabsExtent: {
                void (layoutRevision)
                let value = 0
                let count = 0
                for (let i = 0; i < tabRepeater.count; ++i) {
                    const tab = tabRepeater.itemAt(i)
                    if (!tab || tab.sourceHidden)
                        continue
                    value += control.verticalBar ? tab.height : tab.width
                    ++count
                }
                value += Math.max(0, count - 1) * 4
                return value
            }
            readonly property real contentExtent: tabsExtent + (hasGhost ? tabDrop.ghostExtent + (tabsExtent > 0 ? 4 : 0) : 0)
            readonly property real length: {
                if (!hasPanels)
                    return 0
                if (!oppositeHasPanels)
                    return available
                const oppositeExtent = side ? leadingBar.contentExtent : trailingBar.contentExtent
                if (contentExtent + oppositeExtent <= available)
                    return side ? contentExtent : available - oppositeExtent
                return Math.max(Math.min(contentExtent, available / 2), available - oppositeExtent)
            }
            readonly property real leadingPadding: side ? Math.max(0, length - contentExtent) : 0
            width: control.verticalBar ? 24 : length
            height: control.verticalBar ? length : 24
            x: control.verticalBar ? 4 : control.mirrored ? total - 4 - start - length : 4 + start
            y: control.verticalBar ? 4 + start : 4

            function updatePanels() {
                const updated = side ? sourceGroup?.trailingPanels ?? [] : sourceGroup?.leadingPanels ?? []
                if (updated.length === panels.length && updated.every((panel, index) => panel === panels[index]))
                    return
                panels = updated
            }

            onSourceGroupChanged: updatePanels()
            Component.onCompleted: updatePanels()

            Connections {
                target: sideBar.sourceGroup

                function onChanged() {
                    sideBar.updatePanels()
                }
            }

            function offset(index) {
                void (layoutRevision)
                let value = leadingPadding
                for (let i = 0; i < index; ++i) {
                    const tab = tabRepeater.itemAt(i)
                    if (tab && !tab.sourceHidden)
                        value += (control.verticalBar ? tab.height : tab.width) + 4
                }
                return value
            }

            function ghostIndex() {
                const index = panels.findIndex(p => p.key === tabDrop.target.anchor)
                return index < 0 ? panels.length : index + (tabDrop.target.after ? 1 : 0)
            }

            Flickable {
                id: flickable
                anchors.fill: parent
                clip: true
                contentWidth: control.verticalBar ? width : Math.max(width, sideBar.contentExtent)
                contentHeight: control.verticalBar ? Math.max(height, sideBar.contentExtent) : height
                boundsBehavior: Flickable.StopAtBounds
                property bool manuallyScrolled: false
                onMovementStarted: manuallyScrolled = true
                onContentWidthChanged: if (!manuallyScrolled)
                    contentX = control.mirrored && !control.verticalBar ? Math.max(0, contentWidth - width) : 0
                onContentHeightChanged: if (!manuallyScrolled)
                    contentY = 0
                ScrollBar.horizontal: ScrollBar {
                    parent: sideBar
                    x: 0
                    y: sideBar.height
                    width: sideBar.width
                    height: 4
                    padding: 1
                    visible: !control.verticalBar && size < 1 && width > 0
                }
                ScrollBar.vertical: ScrollBar {
                    parent: sideBar
                    x: sideBar.width
                    y: 0
                    width: 4
                    height: sideBar.height
                    padding: 1
                    visible: control.verticalBar && size < 1 && height > 0
                }

                Repeater {
                    id: tabRepeater
                    model: sideBar.panels
                    onItemAdded: ++sideBar.layoutRevision
                    onItemRemoved: ++sideBar.layoutRevision
                    delegate: DockingDelegate {
                        id: tabLoader
                        required property var modelData
                        required property int index
                        readonly property bool sourceHidden: sideBar.hasGhost && modelData.key === control.context?.draggedPanelKey
                        readonly property real logicalPosition: sideBar.offset(index) + (sideBar.hasGhost && index >= sideBar.ghostIndex() ? tabDrop.ghostExtent + 4 : 0)
                        x: control.verticalBar ? 0 : control.mirrored ? flickable.contentWidth - logicalPosition - width : logicalPosition
                        y: control.verticalBar ? logicalPosition : 0
                        width: control.verticalBar ? 24 : sourceHidden ? 0 : item?.implicitWidth ?? 24
                        height: control.verticalBar ? sourceHidden ? 0 : item?.implicitHeight ?? 24 : 24
                        opacity: sourceHidden ? 0 : 1
                        sourceComponent: control.tabDelegate
                        initialProperties: ({panel: modelData})
                        Connections {
                            target: tabLoader.item

                            function onActiveFocusChanged() {
                                if (!tabLoader.item.activeFocus)
                                    return
                                flickable.manuallyScrolled = true
                                if (control.verticalBar && tabLoader.y < flickable.contentY)
                                    flickable.contentY = tabLoader.y
                                else if (control.verticalBar && tabLoader.y + tabLoader.height > flickable.contentY + flickable.height)
                                    flickable.contentY = tabLoader.y + tabLoader.height - flickable.height
                                else if (!control.verticalBar && tabLoader.x < flickable.contentX)
                                    flickable.contentX = tabLoader.x
                                else if (!control.verticalBar && tabLoader.x + tabLoader.width > flickable.contentX + flickable.width)
                                    flickable.contentX = tabLoader.x + tabLoader.width - flickable.width
                            }
                        }
                    }
                }

                DockingDelegate {
                    id: ghostLoader
                    x: control.verticalBar ? 0 : control.mirrored ? flickable.contentWidth - sideBar.offset(sideBar.ghostIndex()) - width : sideBar.offset(sideBar.ghostIndex())
                    y: control.verticalBar ? sideBar.offset(sideBar.ghostIndex()) : 0
                    width: control.verticalBar ? 24 : tabDrop.ghostExtent
                    height: control.verticalBar ? tabDrop.ghostExtent : 24
                    visible: sideBar.hasGhost
                    sourceComponent: sideBar.hasGhost ? control.ghostDelegate : null
                    initialProperties: sideBar.hasGhost ? ({panel: control.context?.panel(control.context.draggedPanelKey) ?? null}) : ({})
                }
            }
        }

        SideBar {
            id: leadingBar
            side: 0
        }

        SideBar {
            id: trailingBar
            side: 1
        }

        DropArea {
            id: tabDrop
            anchors.fill: parent
            keys: ["application/x-svscraft-panel"]
            property var target: ({})
            property real ghostExtent: {
                const ghost = target.side ? trailingBar.ghost.item : leadingBar.ghost.item
                return (control.verticalBar ? ghost?.implicitHeight : ghost?.implicitWidth) ?? 24
            }
            property real scrollDelta: 0
            property var scrollBar: null

            function update(drag) {
                drag.accepted = drag.source?.dockingContext === control.context
                if (!drag.accepted)
                    return
                const coordinate = (control.verticalBar ? drag.y : control.mirrored ? width - drag.x : drag.x) - 4
                const total = Math.max(0, (control.verticalBar ? height : width) - 8)
                let side = coordinate < total / 2 ? 0 : 1
                for (const candidate of [leadingBar, trailingBar]) {
                    const point = candidate.flickable.mapFromItem(bar, drag.x, drag.y)
                    if (point.x < 0 || point.x >= candidate.width || point.y < 0 || point.y >= candidate.height)
                        continue
                    const position = control.verticalBar ? point.y + candidate.flickable.contentY : control.mirrored ? candidate.flickable.contentWidth - point.x - candidate.flickable.contentX : point.x + candidate.flickable.contentX
                    for (let i = 0; i < candidate.repeater.count; ++i) {
                        const tab = candidate.repeater.itemAt(i)
                        const extent = control.verticalBar ? tab?.height ?? 0 : tab?.width ?? 0
                        if (tab && !tab.sourceHidden && position >= tab.logicalPosition && position < tab.logicalPosition + extent)
                            side = candidate.side
                    }
                }
                const container = side ? trailingBar : leadingBar
                const panels = container.panels
                const local = coordinate - container.start + (control.verticalBar ? container.flickable.contentY : control.mirrored ? container.flickable.contentWidth - container.width - container.flickable.contentX : container.flickable.contentX)
                let anchor = ""
                let after = true
                const ghostStart = container.offset(container.ghostIndex())
                if (container.hasGhost && local >= ghostStart && local < ghostStart + ghostExtent) {
                    anchor = target.anchor
                    after = target.after
                } else {
                    for (let i = 0; i < panels.length; ++i) {
                        const tab = container.repeater.itemAt(i)
                        if (!tab || tab.sourceHidden)
                            continue
                        const position = tab.logicalPosition
                        const extent = control.verticalBar ? tab.height : tab.width
                        if (local < position + extent) {
                            anchor = panels[i].key
                            after = local >= position + extent / 2
                            break
                        }
                    }
                }
                if (target.hostId === undefined || target.side !== side || target.anchor !== anchor || target.after !== after) {
                    target = {
                        hostId: control.group.layout.host.hostId,
                        groupId: control.group.nodeId,
                        kind: Docking.Tab,
                        side: side,
                        anchor: anchor,
                        after: after
                    }
                }
                scrollBar = container.flickable
                const viewport = coordinate - container.start
                scrollDelta = (viewport < 24 ? -8 : viewport > container.length - 24 ? 8 : 0) * (control.mirrored && !control.verticalBar ? -1 : 1)
            }
            onEntered: drag => update(drag)
            onPositionChanged: drag => update(drag)
            onExited: {
                target = ({})
                scrollDelta = 0
            }
            onDropped: drop => {
                update(drop)
                if (drop.accepted)
                    drop.accepted = control.context.requestPlaceAt(control.context.panel(drop.source.panelKey), target).result.accepted
                target = ({})
                scrollDelta = 0
            }

            Timer {
                interval: 40
                repeat: true
                running: tabDrop.containsDrag && tabDrop.scrollDelta !== 0
                onTriggered: {
                    const f = tabDrop.scrollBar
                    f.manuallyScrolled = true
                    if (control.verticalBar)
                        f.contentY = Math.max(0, Math.min(f.contentHeight - f.height, f.contentY + tabDrop.scrollDelta))
                    else
                        f.contentX = Math.max(0, Math.min(f.contentWidth - f.width, f.contentX + tabDrop.scrollDelta))
                }
            }
        }

        Rectangle {
            visible: control.group && !control.group.collapsed
            x: control.group?.tabBarEdge === Qt.LeftEdge ? parent.width - 1 : 0
            y: control.group?.tabBarEdge === Qt.TopEdge ? parent.height - 1 : 0
            width: control.verticalBar ? 1 : parent.width
            height: control.verticalBar ? parent.height : 1
            color: Theme.paneSeparatorColor
        }
    }
}
