import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.TabBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    contentItem: ListView {
        model: control.contentModel
        currentIndex: control.currentIndex

        spacing: control.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickIfNeeded
        snapMode: ListView.SnapToItem

        highlightMoveDuration: 0
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: 40
        preferredHighlightEnd: width - 40

        Rectangle {
            width: parent.width
            height: parent.height
            visible: !control.ThemedItem.flat
            color: "transparent"
            z: 10
            radius: 4
            border.width: 1
            border.color: Theme.borderColor
        }
    }

    background: Rectangle {
        opacity: control.ThemedItem.flat ? 0 : 1
        color: !control.enabled ? Theme.controlDisabledColorChange.apply(Theme.buttonColor) : Theme.buttonColor
        radius: 4
    }

}
