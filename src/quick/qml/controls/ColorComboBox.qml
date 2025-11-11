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
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Control {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, indicator.implicitHeight + topPadding + bottomPadding)

    leftPadding: padding + (!control.mirrored ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored  ? 0 : indicator.width + spacing)

    spacing: 4
    hoverEnabled: true

    Accessible.role: Accessible.ComboBox

    property color color: Theme.accentColor
    property int axis: SVS.CA_Hue
    property int flags: 16383

    Item {
        id: indicator
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: image.width + 8
        ColorImage {
            id: image
            anchors.centerIn: parent
            color: !control.enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) :
                Theme.foregroundPrimaryColor
            sourceSize.width: 12
            sourceSize.height: 12
            source: "image://fluent-system-icons/chevron_down?size=12"
            Behavior on color {
                ColorAnimation {
                    duration: Theme.colorAnimationDuration
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    Item {
        x: !control.mirrored ? control.padding + 8 : control.width - width - control.padding - 8
        y: control.topPadding + (control.availableHeight - height) / 2
        height: control.availableHeight - 16
        width: control.width - 28
        property color color: control.color
        Image {
            anchors.fill: parent
            fillMode: Image.Tile
            horizontalAlignment: Image.AlignLeft
            verticalAlignment: Image.AlignTop
            source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/transparent_tile.svg"
        }
        Rectangle {
            anchors.fill: parent
            color: parent.color
        }
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 28

        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.textFieldColor) :
            Theme.textFieldColor
        border.color: control.editable && control.contentItem.activeFocus ? Theme.accentColor : control.visualFocus ? Theme.navigationColor : Theme.borderColor
        border.width: control.visualFocus || control.editable && control.contentItem.activeFocus ? 2 : 1
        radius: 4
        Behavior on color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
        Behavior on border.color {
            ColorAnimation {
                duration: Theme.colorAnimationDuration
                easing.type: Easing.OutCubic
            }
        }
    }

    TapHandler {
        onTapped: () => {
            popup.visible = !popup.visible
        }
    }

    Popup {
        id: popup
        x: control.mirrored ? control.width - width : 0
        y: control.height
        margins: 6
        onAboutToShow: () => {
            GlobalHelper.setProperty(colorPicker, "color", control.color)
        }
        ColumnLayout {
            spacing: 8
            ColorPicker {
                id: colorPicker
                color: control.color
                currentColor: control.color
                axis: control.axis
                flags: control.flags
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("OK")
                ThemedItem.controlType: SVS.CT_Accent
                icon.source: "image://fluent-system-icons/checkmark"
                Layout.fillWidth: true
                onClicked: () => {
                    GlobalHelper.setProperty(control, "color", colorPicker.color)
                    popup.close()
                }
            }
        }

    }
}