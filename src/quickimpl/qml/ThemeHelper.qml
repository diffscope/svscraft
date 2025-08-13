import QtQml
import QtQuick

import SVSCraft

QtObject {
    id: d
    function controlColor(theme) {
        return (controlType) => {
            switch (controlType) {
                case SVS.CT_Normal:
                    return theme.buttonColor
                case SVS.CT_Accent:
                    return theme.accentColor
                case SVS.CT_Warning:
                    return theme.warningColor
                case SVS.CT_Error:
                    return theme.errorColor
            }
            return undefined
        }
    }

    function backgroundColor(theme) {
        return (backgroundLevel) => {
            switch (backgroundLevel) {
                case SVS.BL_Primary:
                    return theme.backgroundPrimaryColor
                case SVS.BL_Secondary:
                    return theme.backgroundSecondaryColor
                case SVS.BL_Tertiary:
                    return theme.backgroundTertiaryColor
                case SVS.BL_Quaternary:
                    return theme.backgroundQuaternaryColor
            }
            return undefined
        }
    }

    function foregroundColor(theme) {
        return (foregroundLevel) => {
            switch (foregroundLevel) {
                case SVS.FL_Primary:
                    return theme.foregroundPrimaryColor
                case SVS.FL_Secondary:
                    return theme.foregroundSecondaryColor
            }
            return undefined
        }
    }

    function dividerStrokeColor(theme) {
        return (dividerStroke, autoColor = Qt.rgba(0, 0, 0, 0)) => {
            switch (dividerStroke) {
                case SVS.DS_Auto:
                    return autoColor
                case SVS.DS_None:
                    return Qt.rgba(0, 0, 0, 0)
                case SVS.DS_Border:
                    return theme.borderColor
                case SVS.DS_Splitter:
                    return theme.splitterColor
            }
            return undefined
        }
    }
}