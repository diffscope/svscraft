// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

ToolButton {
    id: control
    required property DockingPanel panel
    property bool showPanelNames: false
    property bool compactLayout: false
    property bool vertical: false
    text: panel?.title ?? ""
    icon.source: panel?.iconSource ?? ""
    icon.width: compactLayout ? 14 : 16
    icon.height: compactLayout ? 14 : 16
    padding: compactLayout ? 5 : 4
    spacing: compactLayout ? 2 : 4
    display: !showPanelNames ? AbstractButton.IconOnly : vertical ? AbstractButton.TextUnderIcon : AbstractButton.TextBesideIcon
    background: ButtonRectangle {
        control: control
        implicitWidth: 24
        implicitHeight: 24
        radius: control.compactLayout ? 0 : 4
    }

    readonly property color labelColor: !enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundColor(ThemedItem.foregroundLevel)) : down ? Theme.foregroundPressedColorChange.apply(Theme.foregroundColor(ThemedItem.foregroundLevel)) : hovered ? Theme.foregroundHoveredColorChange.apply(Theme.foregroundColor(ThemedItem.foregroundLevel)) : Theme.foregroundColor(ThemedItem.foregroundLevel)
    readonly property font labelFont: compactLayout ? GlobalHelper.fontWithPixelSize(font, 10) : font

    Binding {
        target: control
        property: "icon.color"
        value: control.panel?.iconColor
        when: control.panel?.iconColor.valid ?? false
    }

    FontMetrics {
        id: verticalMetrics
        font: GlobalHelper.fontWithVerticalAlternates(control.labelFont)
    }

    component TabLabel: Label {
        font: control.labelFont
        color: control.labelColor
        textFormat: Text.PlainText
        Accessible.ignored: true

        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }

    contentItem: Item {
        implicitWidth: contentLayout.implicitWidth
        implicitHeight: contentLayout.implicitHeight

        GridLayout {
            id: contentLayout
            anchors.centerIn: parent
            width: implicitWidth
            height: implicitHeight
            columns: control.vertical ? 1 : 2
            columnSpacing: control.spacing
            rowSpacing: control.spacing
            layoutDirection: control.mirrored ? Qt.RightToLeft : Qt.LeftToRight

            IconLabel {
                icon: control.icon
                display: AbstractButton.IconOnly
                visible: implicitWidth > 0 && implicitHeight > 0
                Layout.alignment: Qt.AlignCenter
                Accessible.ignored: true
            }

            TabLabel {
                visible: control.showPanelNames && !control.vertical && text.length > 0
                text: control.text
                Layout.alignment: Qt.AlignCenter
            }

            ColumnLayout {
                visible: control.showPanelNames && control.vertical && control.text.length > 0
                spacing: 0
                Layout.alignment: Qt.AlignCenter

                Repeater {
                    model: control.showPanelNames && control.vertical ? GlobalHelper.graphemeClusters(control.text) : []

                    Item {
                        required property string modelData
                        implicitWidth: glyphLabel.implicitWidth
                        implicitHeight: Math.max(verticalMetrics.height, glyphLabel.implicitHeight)
                        Layout.alignment: Qt.AlignHCenter

                        TabLabel {
                            id: glyphLabel
                            anchors.centerIn: parent
                            text: parent.modelData
                            font: verticalMetrics.font
                        }
                    }
                }
            }
        }
    }
}
