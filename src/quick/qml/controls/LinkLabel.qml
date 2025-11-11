import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Label {
    id: label
    property string href: ""
    property string linkText: ""
    property bool externalLink: false
    text: `<a href="${GlobalHelper.toHtmlEscaped(href)}" style="text-decoration: ${hoveredLink ? "underline" : "none"}; color: rgba(${Theme.linkColor.r * 255}, ${Theme.linkColor.g * 255}, ${Theme.linkColor.b * 255}, ${Theme.linkColor.a});">${GlobalHelper.toHtmlEscaped(linkText)}</a>`
    textFormat: Text.RichText
    rightPadding: externalLink ? 12 : 0
    ColorImage {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        visible: label.externalLink
        sourceSize.width: 12
        sourceSize.height: 12
        source: "image://fluent-system-icons/arrow_up_right?size=12"
        color: Theme.linkColor
    }
}