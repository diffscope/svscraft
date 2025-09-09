import QtQml
import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.Action {
    icon.width: 16
    icon.height: 16
    icon.color: !enabled ? Theme.foregroundDisabledColorChange.apply(Theme.foregroundPrimaryColor) : Theme.foregroundPrimaryColor
}