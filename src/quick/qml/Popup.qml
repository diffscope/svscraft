import QtQuick
import QtQuick.Templates as T
import QtQuick.Effects
import QtQuick.Controls.impl

import SVSCraft.UIComponents

T.Popup {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    padding: 8

    background: Item {
        Rectangle {
            id: backgroundArea
            anchors.fill: parent
            color: Theme.backgroundColor(control.ThemedItem.backgroundLevel)
            border.color: Theme.borderColor
            radius: 4
        }
        MultiEffect {
            source: backgroundArea
            anchors.fill: parent
            shadowEnabled: true
            shadowColor: Theme.shadowColor
        }
    }

    T.Overlay.modal: Rectangle {
        color: Color.transparent(Theme.shadowColor, 0.5)
    }

    T.Overlay.modeless: Rectangle {
        color: Color.transparent(Theme.shadowColor, 0.12)
    }
}