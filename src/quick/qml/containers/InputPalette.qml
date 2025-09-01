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

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Popup {
    id: popup

    property string placeholderText: ""
    property alias text: textField.text
    property string promptText: ""
    property int status: SVS.CT_Normal
    property bool acceptable: true

    x: parent ? (parent.width - implicitWidth) / 2 : 0
    implicitWidth: 600
    implicitHeight: Math.min(400, textField.height + (promptLabel.visible ? promptLabel.height + 8 : 0) + topPadding + bottomPadding)

    onAboutToShow: textField.forceActiveFocus()

    signal attemptingAcceptButFailed()
    signal accepted()
    signal rejected()
    signal finished(var text)

    property bool isAccepting: false

    function accept() {
        isAccepting = true
        close();
    }

    onAboutToHide: () => {
        if (isAccepting) {
            accepted()
            finished(text)
            isAccepting = false
        } else {
            rejected()
            finished(undefined)
        }
    }

    ColumnLayout {
        spacing: 8
        anchors.fill: parent
        TextField {
            id: textField
            Layout.fillWidth: true
            placeholderText: popup.placeholderText
            ThemedItem.controlType: popup.status
            Keys.onReturnPressed: () => {
                if (!popup.acceptable) {
                    GlobalHelper.beep()
                    popup.attemptingAcceptButFailed()
                } else {
                    popup.accept()
                }
            }
        }
        Label {
            id: promptLabel
            visible: text !== ""
            text: popup.promptText
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }
    }

}