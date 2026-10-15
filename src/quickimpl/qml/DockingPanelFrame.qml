// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents

Pane {
    id: control
    required property DockingPanel panel
    property DockingGroupViewTemplate groupView: null
    background: Item {}
    Accessible.name: panel?.title ?? ""

    Rectangle {
        id: header
        Accessible.role: Accessible.ToolBar
        Accessible.name: control.panel?.title ?? ""
        width: parent.width
        height: control.groupView?.barThickness ?? 32
        color: control.panel?.active ? Theme.dockingPanelHeaderActiveColorChange.apply(Theme.backgroundPrimaryColor) : Theme.backgroundPrimaryColor

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            spacing: 4

            ComboBox {
                id: selector
                readonly property var panels: {
                    const group = control.panel?.group
                    if (!group)
                        return []
                    return group.sideOf(control.panel) === DockingGroup.Leading ? group.leadingPanels : group.trailingPanels
                }
                model: panels.map(panel => ({
                            text: panel.title,
                            data: panel
                        }))
                textRole: "text"
                valueRole: "data"
                currentIndex: Math.max(0, panels.indexOf(control.panel))
                padding: 4
                indicator.width: 12
                Layout.alignment: Qt.AlignVCenter
                Layout.minimumHeight: 24
                Layout.maximumHeight: 24
                Accessible.name: control.panel?.title ?? ""
                background: Item {}

                contentItem: IconLabel {
                    id: titleLabel
                    text: control.panel?.title ?? ""
                    icon.source: control.panel?.iconSource ?? ""
                    mirrored: selector.mirrored

                    Binding {
                        target: titleLabel
                        property: "icon.color"
                        value: control.panel?.iconColor
                        when: control.panel?.iconColor.valid ?? false
                    }
                }

                popup.popupType: Popup.Item
                popup.width: popup.contentItem.implicitWidth
                popup.contentItem.implicitWidth: {
                    let width = 100
                    const view = selector.popup.contentItem
                    for (let i = 0; i < view.count; ++i)
                        width = Math.max(width, view.itemAtIndex(i)?.implicitWidth ?? 0)
                    return width
                }

                delegate: ItemDelegate {
                    id: entry
                    required property var model
                    required property int index
                    width: ListView.view.width
                    height: 32
                    text: model[selector.textRole]
                    icon.source: model[selector.valueRole].iconSource
                    highlighted: selector.currentIndex === index
                    hoverEnabled: selector.hoverEnabled

                    Binding {
                        target: entry
                        property: "icon.color"
                        value: entry.model[selector.valueRole].iconColor
                        when: entry.model[selector.valueRole].iconColor.valid
                    }
                }
                onActivated: index => control.panel.context.show(valueAt(index))
            }

            Rectangle {
                width: 1
                Layout.fillHeight: true
                Layout.topMargin: 4
                Layout.bottomMargin: 4
                color: Theme.borderColor
            }

            Flickable {
                id: toolbarViewport
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                clip: true
                contentWidth: Math.max(width, control.panel?.headerItem?.implicitWidth ?? 0)
                contentHeight: height
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.AutoFlickIfNeeded
                property bool userScrolled: false

                function adjustPosition() {
                    const maximum = Math.max(0, contentWidth - width)
                    contentX = userScrolled ? Math.max(0, Math.min(maximum, contentX)) : control.mirrored ? maximum : 0
                    Qt.callLater(ensureFocusVisible)
                }

                function ensureFocusVisible() {
                    const focused = control.Window.window?.activeFocusItem
                    let ancestor = focused
                    while (ancestor && ancestor !== toolbarContent)
                        ancestor = ancestor.parent
                    if (!ancestor)
                        return
                    userScrolled = true
                    const point = focused.mapToItem(toolbarContent, 0, 0)
                    let position = contentX
                    if (point.x < contentX)
                        position = point.x
                    else if (point.x + focused.width > contentX + width)
                        position = point.x + focused.width - width
                    contentX = Math.max(0, Math.min(contentWidth - width, position))
                }

                onMovementStarted: userScrolled = true
                onWidthChanged: adjustPosition()
                onContentWidthChanged: adjustPosition()
                Component.onCompleted: adjustPosition()

                Connections {
                    target: control.Window.window

                    function onActiveFocusItemChanged() {
                        toolbarViewport.ensureFocusVisible()
                    }
                }

                WheelHandler {
                    target: null
                    onWheel: event => {
                        if (event.angleDelta.x !== 0 || event.pixelDelta.x !== 0) {
                            event.accepted = false
                            return
                        }
                        const delta = event.pixelDelta.y || event.angleDelta.y / 120 * Qt.styleHints.wheelScrollLines * toolbarViewport.height
                        const position = Math.max(0, Math.min(toolbarViewport.contentWidth - toolbarViewport.width, toolbarViewport.contentX - delta))
                        event.accepted = position !== toolbarViewport.contentX
                        if (event.accepted) {
                            toolbarViewport.userScrolled = true
                            toolbarViewport.cancelFlick()
                            toolbarViewport.contentX = position
                        }
                    }
                }

                DockingContentTemplate {
                    id: toolbarContent
                    panel: control.panel
                    header: true
                    width: toolbarViewport.contentWidth
                    height: toolbarViewport.height
                }
            }

            Rectangle {
                width: 1
                Layout.fillHeight: true
                Layout.topMargin: 4
                Layout.bottomMargin: 4
                color: Theme.borderColor
            }

            ToolButton {
                visible: Boolean(control.panel?.contentItem?.menu)
                text: qsTr("Panel Menu")
                icon.source: "image://fluent-system-icons/more_vertical"
                display: AbstractButton.IconOnly
                Layout.alignment: Qt.AlignVCenter
                onClicked: control.panel.contentItem.menu.popup()
            }

            ToolButton {
                text: qsTr("Panel Options")
                icon.source: "image://fluent-system-icons/settings"
                display: AbstractButton.IconOnly
                Layout.alignment: Qt.AlignVCenter
                onClicked: options.popup()

                DockingPanelMenu {
                    id: options
                    panel: control.panel
                    groupView: control.groupView
                }
            }
        }
    }

    DockingContentTemplate {
        panel: control.panel
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Rectangle {
        anchors.bottom: header.bottom
        width: header.width
        height: 1
        color: Theme.paneSeparatorColor
    }

    MouseArea {
        anchors.fill: parent
        z: 1
        acceptedButtons: Qt.AllButtons
        cursorShape: undefined
        onPressed: mouse => {
            if (control.panel)
                control.panel.context.activate(control.panel, false)
            mouse.accepted = false
        }
    }
}
