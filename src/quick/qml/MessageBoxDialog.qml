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
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | (escapeButton !== 0 && buttonIds.includes(escapeButton) ? Qt.WindowCloseButtonHint : 0)
    width: Math.max(400, buttonBox.implicitWidth)
    minimumHeight: mainLayout.implicitHeight
    maximumHeight: mainLayout.implicitHeight
    height: mainLayout.implicitHeight
    modality: Qt.ApplicationModal
    property var icon: SVS.NoIcon
    property var buttons: SVS.Ok
    property var primaryButton: SVS.Ok
    property bool doneByButton: false
    readonly property var buttonIds: {
        const ids = []
        const flags = typeof(buttons) === "number" ? buttons
            : buttons.reduce((flags, button) => typeof(button) === "number" ? flags | button : flags, SVS.NoButton)
        for (let id = SVS.Ok; id <= SVS.RestoreDefaults; id *= 2) {
            if (flags & id)
                ids.push(id)
        }
        if (typeof(buttons) !== "number") {
            for (const button of buttons) {
                if (typeof(button) !== "number")
                    ids.push(button.id)
            }
        }
        return ids
    }
    property var escapeButton: {
        const a = buttonIds
        for (const o of a) {
            if (o === SVS.Cancel) {
                return SVS.Cancel
            }
        }
        if (a.length === 1) {
            return a[0]
        }
        let rejectButton = 0
        for (const o of a) {
            if (o === SVS.Close || o === SVS.Abort) {
                if (!rejectButton) {
                    rejectButton = o
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
        if (dialog.doneByButton) {
            dialog.doneByButton = false
            return
        }
        if (escapeButton !== 0 && buttonIds.includes(escapeButton)) {
            done(escapeButton)
        }
    }

    onVisibleChanged: () => {
        if (visible) {
            updateDefaultButton(true)
            GlobalHelper.invokeAlertHandler(this)
        }
    }

    Component.onCompleted: updateDefaultButton()
    onPrimaryButtonChanged: Qt.callLater(updateDefaultButton)

    function updateDefaultButton(focusButton = false) {
        const button = buttonBox.buttonsInLayoutOrder().find(button => button.messageButtonId === dialog.primaryButton) ?? null
        buttonBox.selectDefaultButton(button)
        if (focusButton && button && button.enabled)
            button.forceActiveFocus()
    }

    function finish(id) {
        doneByButton = true
        done(id)
        close()
    }

    Shortcut {
        sequences: [StandardKey.Copy]
        onActivated: dialog.copyMessageContent()
    }

    function copyMessageContent() {
        GlobalHelper.setClipboardText(
`---------------------------
${dialog.title}
---------------------------
${dialog.text}
---------------------------
${dialog.informativeText.length !== 0 ? `${dialog.informativeText}\n---------------------------` : ""}
${buttonBox.buttonsInLayoutOrder().map(button => button.text).join("   ")}
---------------------------
${dialog.detailedText.length !== 0 ? `${dialog.detailedText}\n---------------------------` : ""}
`)
    }

    ColumnLayout {
        id: mainLayout
        focus: true
        Accessible.role: Accessible.AlertMessage
        width: dialog.width
        spacing: 0
        Keys.onEscapePressed: () => {
            if (escapeButton !== 0 && buttonIds.includes(escapeButton)) {
                dialog.close()
            }
        }
        Rectangle {
            color: Theme.backgroundQuaternaryColor
            Layout.fillWidth: true
            implicitHeight: textLayout.implicitHeight + buttonBox.leftPadding + buttonBox.topPadding
            RowLayout {
                id: textLayout
                spacing: 12
                anchors.fill: parent
                anchors.leftMargin: buttonBox.leftPadding
                anchors.rightMargin: buttonBox.rightPadding
                anchors.topMargin: buttonBox.leftPadding
                anchors.bottomMargin: buttonBox.topPadding
                ColorImage {
                    visible: dialog.icon === SVS.Information || dialog.icon === SVS.Warning || dialog.icon === SVS.Critical || dialog.icon === SVS.Question || dialog.icon === SVS.Success || dialog.icon === SVS.Tip
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 48
                    color: dialog.icon === SVS.Warning ? Theme.warningColor : dialog.icon === SVS.Critical ? Theme.errorColor : dialog.icon === SVS.Success || dialog.icon === SVS.Tip ? Theme.accentColor : Theme.foregroundPrimaryColor
                    sourceSize.width: 48
                    sourceSize.height: 48
                    source: `image://fluent-system-icons/${dialog.icon === SVS.Information ? "info" : dialog.icon === SVS.Warning ? "warning" : dialog.icon === SVS.Critical ? "dismiss_circle" : dialog.icon === SVS.Question ? "question_circle" : dialog.icon === SVS.Success ? "checkmark_circle" : "info_sparkle"}?size=48&style=regular`
                }
                ColorImage {
                    visible: typeof(dialog.icon) !== "number"
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredWidth: visible && dialog.icon.width !== 0 ? dialog.icon.width : implicitWidth
                    Layout.preferredHeight: visible && dialog.icon.height !== 0 ? dialog.icon.height : implicitHeight
                    color: dialog.icon.color ?? "transparent"
                    source: dialog.icon.source ?? ""
                    cache: dialog.icon.cache ?? false
                    sourceSize.width: Layout.preferredWidth
                    sourceSize.height: Layout.preferredHeight
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
        WindowDialogButtonBox {
            id: buttonBox
            Layout.fillWidth: true
            hostWindow: dialog
            separatorVisible: false
            standardButtons: typeof(dialog.buttons) === "number" ? dialog.buttons
                : dialog.buttons.reduce((flags, button) => typeof(button) === "number" ? flags | button : flags, DialogButtonBox.NoButton)
            onCountChanged: Qt.callLater(dialog.updateDefaultButton)
            onClicked: button => dialog.finish(button.messageButtonId)

            leadingContent: [
                Button {
                    id: showDetailsButton
                    visible: dialog.detailedText.length !== 0
                    checkable: true
                    flat: true
                    display: AbstractButton.IconOnly
                    text: checked ? qsTr("Hide Details...") : qsTr("Show Details...")
                    icon.source: "image://fluent-system-icons/more_circle"
                    icon.width: 24
                    icon.height: 24
                    padding: 2
                    horizontalPadding: 2
                    checked: dialog.detailsVisible
                    Binding {
                        dialog.detailsVisible: showDetailsButton.checked
                    }
                },
                DialogHelpButton {
                    visible: dialog.hasContextHelp
                    onClicked: dialog.contextHelpClicked()
                }
            ]

            delegate: Button {
                id: standardMessageButton
                readonly property int messageButtonId: buttonBox.standardButtonId(standardMessageButton)
                DialogButton.emphasis: dialog.primaryButton === messageButtonId ? DialogButton.Primary : DialogButton.Normal
            }

            Repeater {
                model: typeof(dialog.buttons) === "number" ? [] : dialog.buttons.filter(button => typeof(button) !== "number")
                delegate: Button {
                    required property var modelData
                    readonly property var messageButtonId: modelData.id
                    text: modelData.text
                    DialogButtonBox.buttonRole: modelData.role ?? DialogButtonBox.ActionRole
                    DialogButton.emphasis: dialog.primaryButton === messageButtonId ? DialogButton.Primary : DialogButton.Normal
                }
            }
        }
        Rectangle {
            visible: dialog.detailsVisible
            color: Theme.backgroundSecondaryColor
            Layout.fillWidth: true
            implicitHeight: detailedTextFrame.implicitHeight + buttonBox.bottomPadding
            Frame {
                id: detailedTextFrame
                anchors.fill: parent
                anchors.leftMargin: buttonBox.leftPadding
                anchors.rightMargin: buttonBox.rightPadding
                anchors.topMargin: 0
                anchors.bottomMargin: buttonBox.bottomPadding
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
