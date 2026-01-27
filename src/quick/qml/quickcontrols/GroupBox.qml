/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

import QtQuick
import QtQuick.Layouts
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.GroupBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding, implicitLabelWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    spacing: 6
    topPadding: padding + (implicitLabelWidth > 0 ? implicitLabelHeight + spacing : 0)

    font: Theme.font

    label: RowLayout {
        spacing: 4
        width: control.availableWidth
        Text {
            Layout.leftMargin: control.leftPadding
            text: control.title
            font: control.font
            color: Theme.foregroundPrimaryColor
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Rectangle {
            implicitHeight: 1
            color: Theme.borderColor
            Layout.fillWidth: true
        }
        Button {
            id: foldButton
            property bool _folded: control.ThemedItem.folded
            property double _rotation: { _rotation = control.ThemedItem.folded ? 0 : 180 }
            Layout.rightMargin: control.leftPadding
            visible: control.ThemedItem.foldable
            width: 20
            height: 20
            padding: 2
            horizontalPadding: 2
            icon.source: "image://fluent-system-icons/chevron_down?size=12"
            icon.name: control.ThemedItem.folded ? qsTr("Expand") : qsTr("Collapse")
            Accessible.name: icon.name
            flat: true
            onClicked: () => {
                _folded = !_folded
            }
            Binding {
                control.ThemedItem.folded: foldButton._folded
                foldButton._folded: control.ThemedItem.folded
                foldButton.contentItem.rotation: foldButton._rotation
            }
            NumberAnimation on _rotation {
                id: animation
                duration: Theme.visualEffectAnimationDuration
                easing.type: Easing.OutCubic
            }
            on_FoldedChanged: () => {
                if (_folded) {
                    animation.from = 180
                    animation.to = 360
                    animation.start()
                } else {
                    animation.from = 0
                    animation.to = 180
                    animation.start()
                }
            }
        }
    }

    background: Rectangle {
        y: control.topPadding - control.bottomPadding
        width: parent.width
        height: parent.height - control.topPadding + control.bottomPadding

        color: "transparent"
    }
}
