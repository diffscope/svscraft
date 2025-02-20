import QtQml
import QtQuick
import QtQuick.Controls
import SVSCraft.UIComponents

Item {
    id: wrapper
    required property QtObject descriptiveText
    required property Item attachee

    Binding {
        wrapper.attachee.ToolTip.visible: wrapper.descriptiveText.activated
        wrapper.attachee.ToolTip.text: wrapper.descriptiveText.toolTip
        wrapper.attachee.ToolTip.delay: Theme.toolTipDelay
        wrapper.attachee.ToolTip.timeout: Theme.toolTipTimeout
        wrapper.attachee.Accessible.description: wrapper.descriptiveText.toolTip && wrapper.descriptiveText.statusTip ? wrapper.descriptiveText.toolTip + "\n" + wrapper.descriptiveText.statusTip : (wrapper.descriptiveText.toolTip || wrapper.descriptiveText.statusTip)
    }
}