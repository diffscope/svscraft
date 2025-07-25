import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.impl

import SVSCraft
import SVSCraft.UIComponents
import SVSCraft.UIComponents.impl

Window {
    id: dialog
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | (escapeButton !== 0 && buttonRepeater.model.some(v => v.id === escapeButton) ? Qt.WindowCloseButtonHint : 0)
    width: Math.max(400, buttonLayout.implicitWidth + 24)
    minimumHeight: mainLayout.implicitHeight
    maximumHeight: mainLayout.implicitHeight
    height: mainLayout.implicitHeight
    modality: Qt.ApplicationModal
    property var icon: SVS.NoIcon
    property var buttons: SVS.Ok
    property var primaryButton: SVS.Ok
    property var escapeButton: {
        const a = buttonRepeater.model
        for (const o of a) {
            if (o.id === SVS.Cancel) {
                return SVS.Cancel
            }
        }
        if (a.length === 1) {
            return a[0].id
        }
        let rejectButton = 0
        for (const o of a) {
            if (o.id === SVS.Close || o.id === SVS.Abort) {
                if (!rejectButton) {
                    rejectButton = o.id
                } else {
                    rejectButton = 0
                }
            }
        }
        if (rejectButton) {
            return rejectButton
        }
        return 0
    }
    property bool hasContextHelp: false
    property string text: ""
    property string detailedText: ""
    property string informativeText: ""
    property bool detailsVisible: false
    property int textFormat: Text.AutoText
    default property Item content: null

    signal done(var id)
    signal contextHelpClicked()
    signal linkActivated(string link)

    onClosing: () => {
        if (buttonLayout.doneByButton) {
            buttonLayout.doneByButton = false
            return
        }
        if (escapeButton !== 0 && buttonRepeater.model.some(v => v.id === escapeButton)) {
            done(escapeButton)
        }
    }

    onVisibleChanged: () => {
        if (visible) {
            GlobalHelper.invokeAlertHandler(this)
        }
    }

    ColumnLayout {
        id: mainLayout
        Accessible.role: Accessible.AlertMessage
        width: dialog.width
        spacing: 0
        Keys.onEscapePressed: () => {
            if (escapeButton !== 0 && buttonRepeater.model.some(v => v.id === escapeButton)) {
                dialog.close()
            }
        }
        Rectangle {
            color: Theme.backgroundQuaternaryColor
            Layout.fillWidth: true
            implicitHeight: textLayout.implicitHeight + 24
            RowLayout {
                id: textLayout
                spacing: 12
                anchors.fill: parent
                anchors.margins: 12
                ColorImage {
                    visible: dialog.icon === SVS.Information || dialog.icon === SVS.Warning || dialog.icon === SVS.Critical || dialog.icon === SVS.Question || dialog.icon === SVS.Success || dialog.icon === SVS.Tip
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 48
                    color: dialog.icon === SVS.Warning ? Theme.warningColor : dialog.icon === SVS.Critical ? Theme.errorColor : dialog.icon === SVS.Success || dialog.icon === SVS.Tip ? Theme.accentColor : Theme.foregroundPrimaryColor
                    source: `qrc:/qt/qml/SVSCraft/UIComponents/assets/${dialog.icon === SVS.Information ? "Info48Regular.svg" : dialog.icon === SVS.Warning ? "Warning48Regular.svg" : dialog.icon === SVS.Critical ? "DismissCircle48Regular.svg" : dialog.icon === SVS.Question ? "QuestionCircle48Regular.svg" : dialog.icon === SVS.Success ? "CheckmarkCircle48Regular.svg" : "InfoSparkle48Regular.svg"}`
                }
                ColorImage {
                    visible: typeof(dialog.icon) !== "number"
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 48
                    color: dialog.icon.color ?? "transparent"
                    source: dialog.icon.source ?? ""
                    cache: dialog.icon.cache ?? false
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.horizontalStretchFactor: 1
                    Layout.alignment: Qt.AlignLeft
                    spacing: 8
                    Label {
                        id: textLabel
                        Layout.fillWidth: true
                        text: dialog.text
                        font.pixelSize: informativeTextLabel.font.pixelSize * 1.5
                        wrapMode: Text.Wrap
                        textFormat: dialog.textFormat
                        onLinkActivated: (link) => dialog.linkActivated(link)

                    }
                    Label {
                        id: informativeTextLabel
                        Layout.fillWidth: true
                        text: dialog.informativeText
                        wrapMode: Text.Wrap
                        textFormat: dialog.textFormat
                        onLinkActivated: (link) => dialog.linkActivated(link)
                    }
                    Item {
                        Layout.fillWidth: true
                        onWidthChanged: () => {
                            if (data[0])
                                data[0].width = width
                        }
                        implicitHeight: dialog.content?.implicitHeight ?? 0
                        visible: dialog.content
                        data: [dialog.content]
                    }
                }
            }
        }
        Rectangle {
            color: Theme.backgroundSecondaryColor
            Layout.fillWidth: true
            height: 52
            RowLayout {
                id: buttonLayout
                property bool doneByButton: false
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12
                Button {
                    id: showDetailsButton
                    visible: dialog.detailedText.length !== 0
                    checkable: true
                    flat: true
                    display: AbstractButton.IconOnly
                    text: checked ? qsTr("Hide Details...") : qsTr("Show Details...")
                    DescriptiveText.toolTip: text
                    DescriptiveText.activated: hovered
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/MoreCircle24Filled.svg"
                    icon.width: 24
                    icon.height: 24
                    padding: 2
                    horizontalPadding: 2
                    checked: dialog.detailsVisible
                    Binding {
                        dialog.detailsVisible: showDetailsButton.checked
                    }
                }
                Button {
                    visible: dialog.hasContextHelp
                    flat: true
                    display: AbstractButton.IconOnly
                    text: "Help"
                    DescriptiveText.toolTip: text
                    DescriptiveText.activated: hovered
                    icon.source: "qrc:/qt/qml/SVSCraft/UIComponents/assets/QuestionCircle24Filled.svg"
                    icon.width: 24
                    icon.height: 24
                    padding: 2
                    horizontalPadding: 2
                    onClicked: dialog.contextHelpClicked()
                }
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                Repeater {
                    id: buttonRepeater
                    model: {
                        let a = []
                        if (typeof(dialog.buttons) === "number") {
                            if (dialog.buttons & SVS.Reset) {
                                a.push({id: SVS.Reset, text: qsTranslate("QPlatformTheme", "Reset")})
                            }
                            if (dialog.buttons & SVS.RestoreDefaults) {
                                a.push({id: SVS.RestoreDefaults, text: qsTranslate("QPlatformTheme", "Restore Defaults")})
                            }
                            if (dialog.buttons & SVS.Yes) {
                                a.push({id: SVS.Yes, text: qsTranslate("QPlatformTheme", "&Yes")})
                            }
                            if (dialog.buttons & SVS.YesToAll) {
                                a.push({id: SVS.YesToAll, text: qsTranslate("QPlatformTheme", "Yes to &All")})
                            }
                            if (dialog.buttons & SVS.Ok) {
                                a.push({id: SVS.Ok, text: qsTranslate("QPlatformTheme", "OK")})
                            }
                            if (dialog.buttons & SVS.Save) {
                                a.push({id: SVS.Save, text: qsTranslate("QPlatformTheme", "Save")})
                            }
                            if (dialog.buttons & SVS.SaveAll) {
                                a.push({id: SVS.SaveAll, text: qsTranslate("QPlatformTheme", "Save All")})
                            }
                            if (dialog.buttons & SVS.Open) {
                                a.push({id: SVS.Open, text: qsTranslate("QPlatformTheme", "Open")})
                            }
                            if (dialog.buttons & SVS.Retry) {
                                a.push({id: SVS.Retry, text: qsTranslate("QPlatformTheme", "Retry")})
                            }
                            if (dialog.buttons & SVS.Ignore) {
                                a.push({id: SVS.Ignore, text: qsTranslate("QPlatformTheme", "Ignore")})
                            }
                            if (dialog.buttons & SVS.Discard) {
                                a.push({id: SVS.Discard, text: qsTranslate("QPlatformTheme", "Discard")})
                            }
                            if (dialog.buttons & SVS.No) {
                                a.push({id: SVS.No, text: qsTranslate("QPlatformTheme", "&No")})
                            }
                            if (dialog.buttons & SVS.NoToAll) {
                                a.push({id: SVS.NoToAll, text: qsTranslate("QPlatformTheme", "N&o to All")})
                            }
                            if (dialog.buttons & SVS.Abort) {
                                a.push({id: SVS.Abort, text: qsTranslate("QPlatformTheme", "Abort")})
                            }
                            if (dialog.buttons & SVS.Close) {
                                a.push({id: SVS.Close, text: qsTranslate("QPlatformTheme", "Close")})
                            }
                            if (dialog.buttons & SVS.Cancel) {
                                a.push({id: SVS.Cancel, text: qsTranslate("QPlatformTheme", "Cancel")})
                            }
                            if (dialog.buttons & SVS.Apply) {
                                a.push({id: SVS.Apply, text: qsTranslate("QPlatformTheme", "Apply")})
                            }
                            if (dialog.buttons & SVS.Help) {
                                a.push({id: SVS.Help, text: qsTranslate("QPlatformTheme", "Help")})
                            }
                        } else {
                            for (const o of dialog.buttons) {
                                if (typeof(o) === "number") {
                                    if (o === SVS.Reset) {
                                        a.push({id: SVS.Reset, text: qsTranslate("QPlatformTheme", "Reset")})
                                    } else if (o === SVS.RestoreDefaults) {
                                        a.push({id: SVS.RestoreDefaults, text: qsTranslate("QPlatformTheme", "Restore Defaults")})
                                    } else if (o === SVS.Yes) {
                                        a.push({id: SVS.Yes, text: qsTranslate("QPlatformTheme", "&Yes")})
                                    } else if (o === SVS.YesToAll) {
                                        a.push({id: SVS.YesToAll, text: qsTranslate("QPlatformTheme", "Yes to &All")})
                                    } else if (o === SVS.Ok) {
                                        a.push({id: SVS.Ok, text: qsTranslate("QPlatformTheme", "OK")})
                                    } else if (o === SVS.Save) {
                                        a.push({id: SVS.Save, text: qsTranslate("QPlatformTheme", "Save")})
                                    } else if (o === SVS.SaveAll) {
                                        a.push({id: SVS.SaveAll, text: qsTranslate("QPlatformTheme", "Save All")})
                                    } else if (o === SVS.Open) {
                                        a.push({id: SVS.Open, text: qsTranslate("QPlatformTheme", "Open")})
                                    } else if (o === SVS.Retry) {
                                        a.push({id: SVS.Retry, text: qsTranslate("QPlatformTheme", "Retry")})
                                    } else if (o === SVS.Ignore) {
                                        a.push({id: SVS.Ignore, text: qsTranslate("QPlatformTheme", "Ignore")})
                                    } else if (o === SVS.Discard) {
                                        a.push({id: SVS.Discard, text: qsTranslate("QPlatformTheme", "Discard")})
                                    } else if (o === SVS.No) {
                                        a.push({id: SVS.No, text: qsTranslate("QPlatformTheme", "&No")})
                                    } else if (o === SVS.NoToAll) {
                                        a.push({id: SVS.NoToAll, text: qsTranslate("QPlatformTheme", "N&o to All")})
                                    } else if (o === SVS.Abort) {
                                        a.push({id: SVS.Abort, text: qsTranslate("QPlatformTheme", "Abort")})
                                    } else if (o === SVS.Close) {
                                        a.push({id: SVS.Close, text: qsTranslate("QPlatformTheme", "Close")})
                                    } else if (o === SVS.Cancel) {
                                        a.push({id: SVS.Cancel, text: qsTranslate("QPlatformTheme", "Cancel")})
                                    } else if (o === SVS.Apply) {
                                        a.push({id: SVS.Apply, text: qsTranslate("QPlatformTheme", "Apply")})
                                    } else if (o === SVS.Help) {
                                        a.push({id: SVS.Help, text: qsTranslate("QPlatformTheme", "Help")})
                                    }
                                } else {
                                    a.push(o)
                                }
                            }
                        }
                        return a
                    }
                    delegate: Button {
                        required property var modelData
                        ThemedItem.controlType: dialog.primaryButton === modelData.id ? SVS.CT_Accent : SVS.CT_Normal
                        Layout.alignment: Qt.AlignVCenter
                        text: modelData.text
                        Connections {
                            target: dialog
                            function onVisibleChanged() {
                                if (dialog.visible && dialog.primaryButton === modelData.id)
                                    forceActiveFocus()
                            }
                        }
                        onClicked: () => {
                            dialog.done(modelData.id)
                            buttonLayout.doneByButton = true
                            dialog.close()
                        }
                        Keys.onReturnPressed: animateClick()
                    }
                }
            }
        }
        Rectangle {
            visible: dialog.detailsVisible
            color: Theme.backgroundSecondaryColor
            Layout.fillWidth: true
            implicitHeight: detailedTextFrame.implicitHeight + 12
            Frame {
                id: detailedTextFrame
                anchors.fill: parent
                anchors.margins: 12
                anchors.topMargin: 0
                padding: 4
                Label {
                    width: parent.width
                    text: dialog.detailedText
                    wrapMode: Text.Wrap
                    textFormat: dialog.textFormat
                    onLinkActivated: (link) => dialog.linkActivated(link)
                }
            }
        }
    }

}