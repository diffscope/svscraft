import QtQuick
import QtQuick.Templates as T
import QtQuick.Effects

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
            color: Theme.backgroundPrimaryColor
            border.color: Theme.borderColor
            radius: 2
        }
        MultiEffect {
            source: backgroundArea
            anchors.fill: parent
            shadowEnabled: true
            shadowColor: Theme.shadowColor
        }
    }
}