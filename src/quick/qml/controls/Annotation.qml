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
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents

Frame {
    id: annotation
    property QtObject icon: QtObject {
        property string name: ""
        property url source: annotation.ThemedItem.controlType === SVS.CT_Normal ?
                "image://fluent-system-icons/info?style=regular" :
            annotation.ThemedItem.controlType === SVS.CT_Accent ?
                "image://fluent-system-icons/checkmark_circle?style=regular" :
            annotation.ThemedItem.controlType === SVS.CT_Warning ?
                "image://fluent-system-icons/error_circle?style=regular" :
            annotation.ThemedItem.controlType === SVS.CT_Error ?
                "image://fluent-system-icons/dismiss_circle?style=regular" : ""
        property int width: 16
        property int height: 16
        property color color: rowLayout._color
        property bool cache: true
    }
    property var label: ""
    property bool closable: false
    property Action action: null

    spacing: 4
    padding: 4
    implicitWidth: Math.max(200, implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(32, label.height + annotation.topPadding + annotation.bottomPadding)

    background: Rectangle {
        color: "transparent"
        border.color: rowLayout._color
        radius: 2
    }
    RowLayout {
        id: rowLayout
        property color _color: annotation.ThemedItem.controlType === SVS.CT_Normal ? Theme.foregroundSecondaryColor : Theme.controlColor(annotation.ThemedItem.controlType)
        spacing: annotation.spacing
        anchors.fill: parent
        IconLabel {
            width: 16
            height: 16
            icon.name: annotation.icon.name
            icon.source: annotation.icon.source
            icon.color: annotation.icon.color
            icon.width: annotation.icon.width
            icon.height: annotation.icon.height
            icon.cache: annotation.icon.cache
            Layout.alignment: Qt.AlignVCenter
        }
        RowLayout {
            id: labelContainer
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            property Label labelItem: Label {
                id: label
                text: typeof(annotation.label) === "string" ? annotation.label : ""
                horizontalAlignment: Qt.AlignLeft
                wrapMode: Text.Wrap
            }
            property Label realLabelItem: typeof(annotation.label) === "string" ? labelItem : annotation.label instanceof Label ? annotation.label : null
            data: [realLabelItem]
            onRealLabelItemChanged: () => {
                if (realLabelItem) {
                    realLabelItem.color = Qt.binding(() => rowLayout._color)
                    realLabelItem.Layout.alignment = Qt.AlignVCenter
                    realLabelItem.Layout.fillWidth = true
                }
            }
        }
        ToolButton {
            visible: annotation.action !== null
            action: annotation.action
            display: AbstractButton.IconOnly
            ThemedItem.foregroundLevel: SVS.FL_Secondary
            Layout.alignment: Qt.AlignVCenter
        }
        ToolButton {
            visible: annotation.closable
            icon.source: "image://fluent-system-icons/dismiss?size=12"
            icon.width: 12
            icon.height: 12
            ThemedItem.foregroundLevel: SVS.FL_Secondary
            display: AbstractButton.IconOnly
            text: qsTr("Close")
            Layout.alignment: Qt.AlignVCenter
            onClicked: annotation.visible = false
        }
    }

}