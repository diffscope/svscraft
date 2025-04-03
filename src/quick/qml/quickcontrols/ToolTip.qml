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

import QtQml
import QtQuick
import QtQuick.Templates as T
import QtQuick.Effects

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

T.ToolTip {
    id: control

    x: parent ? (parent.width - implicitWidth) / 2 : 0
    y: -implicitHeight - 3

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    margins: 8
    padding: 4

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent

    QtObject {
        id: cursorListenerHelper
        readonly property QtObject listener: GlobalHelper.createGlobalCursorListener(this)
        readonly property bool enabled: (control.ThemedItem.toolTipFollowsCursor || control.parent && control.parent.ThemedItem.toolTipFollowsCursor) && control.visible
        onEnabledChanged: listener.enabled = enabled
        readonly property Connections connections: Connections {
            target: cursorListenerHelper.listener
            enabled: cursorListenerHelper.enabled
            function onPositionChanged() {
                let p = control.parent.mapFromGlobal(GlobalHelper.cursorPos())
                control.x = p.x
                control.y = p.y + 24
            }
        }
    }


    enter: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: Theme.visualEffectAnimationDuration
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: Theme.visualEffectAnimationDuration
        }
    }

    onParentChanged: () => {
        if (!parent)
            return
        let p = parent.mapFromGlobal(GlobalHelper.cursorPos())
        x = p.x
        y = p.y + 24
    }

    onAboutToShow: () => {
        if (!parent)
            return
        let p = parent.mapFromGlobal(GlobalHelper.cursorPos())
        x = p.x
        y = p.y + 24
    }

    contentItem: Text {
        text: control.text
        font: control.font
        wrapMode: Text.Wrap
        color: Theme.foregroundPrimaryColor
    }

    background: Item {
        Rectangle {
            id: backgroundArea
            anchors.fill: parent
            color: Theme.backgroundQuaternaryColor
            border.color: Theme.borderColor
            radius: 2
        }
    }
}