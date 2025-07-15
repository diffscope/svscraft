import QtQml
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import SVSCraft
import SVSCraft.UIComponents

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    background: Rectangle {
        color: Theme.backgroundPrimaryColor
    }
    MessageBoxDialog {
        id: dialog
        icon: ""
        text: ""
        informativeText: ""
        detailedText: ""
        onDone: (id) => console.log("Done", id)
        onContextHelpClicked: console.log("Context help")
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        GroupBox {
            title: "Icon"
            RowLayout {
                RadioButton {
                    text: "No Icon"
                    checked: true
                    onCheckedChanged: if (checked) dialog.icon = SVS.NoIcon
                }
                RadioButton {
                    text: "Information"
                    onCheckedChanged: if (checked) dialog.icon = SVS.Information
                }
                RadioButton {
                    text: "Warning"
                    onCheckedChanged: if (checked) dialog.icon = SVS.Warning
                }
                RadioButton {
                    text: "Critical"
                    onCheckedChanged: if (checked) dialog.icon = SVS.Critical
                }
                RadioButton {
                    text: "Question"
                    onCheckedChanged: if (checked) dialog.icon = SVS.Question
                }
                RadioButton {
                    text: "Success"
                    onCheckedChanged: if (checked) dialog.icon = SVS.Success
                }
                RadioButton {
                    text: "Tip"
                    onCheckedChanged: if (checked) dialog.icon = SVS.Tip
                }
                RadioButton {
                    text: "(Custom)"
                    onCheckedChanged: if (checked) dialog.icon = { source: "qrc:/qt/qml/SVSCraft/Test/MessageBoxDialog/crs.png"}
                }
            }
        }
        GroupBox {
            title: "Buttons"
            ColumnLayout {
                RadioButton {
                    id: standardButtonsRadioButton
                    checked: true
                    property int standardButtons: SVS.Ok
                    text: "Standard buttons"
                    onCheckedChanged: if (checked) dialog.buttons = standardButtons
                    onStandardButtonsChanged: if (checked) dialog.buttons = standardButtons
                }
                GridLayout {
                    enabled: standardButtonsRadioButton.checked
                    columns: 6
                    Repeater {
                        model: [
                            {t: "Ok", v: SVS.Ok},
                            {t: "Open", v: SVS.Open},
                            {t: "Save", v: SVS.Save},
                            {t: "Cancel", v: SVS.Cancel},
                            {t: "Close", v: SVS.Close},
                            {t: "Discard", v: SVS.Discard},
                            {t: "Apply", v: SVS.Apply},
                            {t: "Reset", v: SVS.Reset},
                            {t: "Restore Defaults", v: SVS.RestoreDefaults},
                            {t: "Help", v: SVS.Help},
                            {t: "Save All", v: SVS.SaveAll},
                            {t: "Yes", v: SVS.Yes},
                            {t: "Yes To All", v: SVS.YesToAll},
                            {t: "No", v: SVS.No},
                            {t: "No To All", v: SVS.NoToAll},
                            {t: "Abort", v: SVS.Abort},
                            {t: "Retry", v: SVS.Retry},
                            {t: "Ignore", v: SVS.Ignore},
                        ]
                        delegate: CheckBox {
                            required property var modelData
                            text: modelData.t
                            checked: Boolean(standardButtonsRadioButton.standardButtons & modelData.v)
                            onCheckedChanged: {
                                if (checked) {
                                    standardButtonsRadioButton.standardButtons |= modelData.v
                                    dialog.primaryButton = modelData.v
                                } else {
                                    standardButtonsRadioButton.standardButtons &= ~modelData.v
                                }
                            }
                        }
                    }
                }
                RadioButton {
                    id: customButtonsRadioButton
                    property var customButtons: [
                        SVS.Ok,
                        {id: "custom1", text: "Custom 1"},
                        {id: "custom2", text: "Custom 2"},
                        SVS.Cancel,
                    ]
                    text: "Custom buttons"
                    onCheckedChanged: if (checked) dialog.buttons = customButtons
                }
            }
        }
        GroupBox {
            title: "Texts"
            RowLayout {
                TextField {
                    placeholderText: "Text"
                    onTextChanged: dialog.text = text
                }
                TextArea {
                    placeholderText: "Informative text"
                    onTextChanged: dialog.informativeText = text
                }
                TextArea {
                    placeholderText: "Detailed text"
                    onTextChanged: dialog.detailedText = text
                }
            }
        }
        GroupBox {
            title: "Context Help"
            CheckBox {
                text: "Has context help"
                onCheckedChanged: dialog.hasContextHelp = checked
            }
        }
        RowLayout {
            Button {
                text: "Show Message Box with the Config Above"
                onClicked: dialog.show()
            }
            Button {
                text: "Show Message Box Using Attached Type"
                onClicked: MessageBox.information("Test MessageBox attached type", "This is a test.")
            }
            Button {
                text: "Show Message Box with Content"
                MessageBoxDialog {
                    id: messageBoxWithContent
                    icon: SVS.Information
                    text: "Message Box with Content"
                    informativeText: "This is a test."
                    detailedText: "This is a test."
                    Rectangle {
                        implicitHeight: 200
                        color: Theme.accentColor
                        Label {
                            text: "content"
                            anchors.centerIn: parent
                        }
                    }
                }
                onClicked: messageBoxWithContent.show()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}