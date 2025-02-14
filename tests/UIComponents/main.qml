import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SVSCraft.UIComponents as SVS

Window {
    visible: true
    width: 800
    height: 600

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        GroupBox {
            title: "按钮"
            RowLayout {
                anchors.fill: parent
                spacing: 8
                Button {
                    SVS.ThemedItem.controlType: SVS.Theme.CT_Normal
                    text: "Normal"
                    icon.source: "icon.svg"
                    enabled: !disabledCheckbox.checked
                }
                Button {
                    SVS.ThemedItem.controlType: SVS.Theme.CT_Accent
                    text: "Accent"
                    icon.source: "icon.svg"
                    enabled: !disabledCheckbox.checked
                }
                Button {
                    SVS.ThemedItem.controlType: SVS.Theme.CT_Error
                    text: "Error"
                    icon.source: "icon.svg"
                    enabled: !disabledCheckbox.checked
                }
                CheckBox {
                    id: disabledCheckbox
                    text: "Disabled"
                }
            }
        }

    }



}