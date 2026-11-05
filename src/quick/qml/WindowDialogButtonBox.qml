// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick

import SVSCraft
import SVSCraft.UIComponents

DialogButtonBox {
    id: control

    property Window hostWindow: Window.window
    property bool separatorVisible: true
    readonly property bool windowModalMargins: (Qt.platform.os === "osx" || Qt.platform.os === "macos")
                                             && hostWindow !== null && hostWindow.modality === Qt.WindowModal

    ThemedItem.backgroundLevel: SVS.BL_Secondary
    horizontalPadding: windowModalMargins ? 24 : 12
    topPadding: 12
    bottomPadding: windowModalMargins ? 24 : 12

    background: Rectangle {
        color: Theme.backgroundSecondaryColor

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: Theme.paneSeparatorColor
            visible: control.separatorVisible
        }
    }
}
