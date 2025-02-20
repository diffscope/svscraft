import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import QtQuick.Effects

import SVSCraft.UIComponents

T.Menu {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    margins: 0
    padding: 3

    delegate: MenuItem {

    }

    contentItem: ListView {
        implicitHeight: contentHeight
        model: control.contentModel
        interactive: Window.window ? contentHeight + control.topPadding + control.bottomPadding > control.height : false
        clip: true
        currentIndex: control.currentIndex

        ScrollIndicator.vertical: ScrollIndicator {}
    }

    background: Item {
        implicitWidth: 200
        implicitHeight: 28
        Rectangle {
            id: backgroundArea
            anchors.fill: parent
            color: Theme.buttonColor
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
        color: Color.transparent(control.palette.shadow, 0.5)
    }

    T.Overlay.modeless: Rectangle {
        color: Color.transparent(control.palette.shadow, 0.12)
    }
}
