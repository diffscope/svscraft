import QtQuick
import QtQuick.Templates as T

T.ScrollView {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    ScrollBar.vertical: ScrollBar {
        parent: control
        x: control.mirrored ? 0 : control.width - width
        y: control.topPadding
        height: control.availableHeight - (control.ScrollBar.horizontal.size < 1.0 ? control.ScrollBar.horizontal.height : 0)
        active: control.ScrollBar.horizontal.active
    }

    ScrollBar.horizontal: ScrollBar {
        parent: control
        x: control.leftPadding
        y: control.height - height
        width: control.availableWidth - (control.ScrollBar.vertical.size < 1.0 ? control.ScrollBar.vertical.width : 0)
        active: control.ScrollBar.vertical.active
    }
}