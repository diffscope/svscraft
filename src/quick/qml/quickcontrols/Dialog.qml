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
import QtQuick.Templates as T
import QtQuick.Effects
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

T.Dialog {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
        implicitContentWidth + leftPadding + rightPadding,
        implicitHeaderWidth,
        implicitFooterWidth)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
        implicitContentHeight + topPadding + bottomPadding
        + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0)
        + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0))

    padding: 12

    background: PopupBackground {
        control: control
    }

    header: Label {
        text: control.title
        visible: parent?.parent === T.Overlay.overlay && control.title
        elide: Label.ElideRight
        padding: 12
        Rectangle {
            anchors.bottom: parent.bottom
            x: 1
            width: parent.width - 2
            height: 1
            color: Theme.paneSeparatorColor
        }
        MouseArea {
            anchors.fill: parent
            property point lastMousePos: Qt.point(0, 0)

            onPressed: (mouse) => {
                lastMousePos = Qt.point(mouse.x, mouse.y)
            }

            onPositionChanged: (mouse) => {
                let deltaX = mouse.x - lastMousePos.x
                let deltaY = mouse.y - lastMousePos.y
                control.x += deltaX
                control.y += deltaY
            }
        }
    }

    footer: DialogButtonBox {
        ThemedItem.backgroundLevel: control.ThemedItem.backgroundLevel
        topPadding: 0
        background: Item {
            implicitHeight: 48
        }
        visible: count > 0
    }

    T.Overlay.modal: Rectangle {
        color: Color.transparent(control.palette.shadow, 0.5)
    }

    T.Overlay.modeless: Rectangle {
        color: Color.transparent(control.palette.shadow, 0.12)
    }
}
