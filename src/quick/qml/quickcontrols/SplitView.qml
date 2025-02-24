import QtQuick
import QtQuick.Templates as T

import SVSCraft.UIComponents

T.SplitView {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)

    handle: Rectangle {
        id: handleDelegate
        implicitWidth: 1
        implicitHeight: 1
        color: control.enabled && T.SplitHandle.pressed ? Theme.accentColor : Theme.splitterColor
        containmentMask: Item {
            x: (handleDelegate.width - width) / 2
            property double _size: control.ThemedItem.splitHandleEnabled ? 5 : 0
            implicitWidth: control.orientation === Qt.Horizontal ? _size : control.width
            implicitHeight: control.orientation === Qt.Horizontal ? control.height : _size
        }
        visible: control.ThemedItem.splitHandleVisible
    }
}