/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

import QtQml
import QtQuick
import QtQuick.Templates as T

import SVSCraft
import SVSCraft.UIComponents

T.StackView {
    id: control

    popEnter: Transition {
        XAnimator {
            from: (control.mirrored ? -1 : 1) * -control.width * 0.2
            to: 0
            duration: Theme.visualEffectAnimationDuration * 2
            easing.type: Easing.OutCubic
        }
    }

    popExit: Transition {
        XAnimator {
            from: 0
            to: (control.mirrored ? -1 : 1) * control.width
            duration: Theme.visualEffectAnimationDuration * 2
            easing.type: Easing.OutCubic
        }
    }

    pushEnter: Transition {
        XAnimator {
            from: (control.mirrored ? -1 : 1) * control.width
            to: 0
            duration: Theme.visualEffectAnimationDuration * 2
            easing.type: Easing.OutCubic
        }
    }

    pushExit: Transition {
        XAnimator {
            from: 0
            to: (control.mirrored ? -1 : 1) * -control.width * 0.2
            duration: Theme.visualEffectAnimationDuration * 2
            easing.type: Easing.OutCubic
        }
    }

    replaceEnter: Transition {
        XAnimator {
            from: (control.mirrored ? -1 : 1) * control.width
            to: 0
            duration: Theme.visualEffectAnimationDuration * 2
            easing.type: Easing.OutCubic
        }
    }

    replaceExit: Transition {
        XAnimator {
            from: 0
            to: (control.mirrored ? -1 : 1) * -control.width
            duration: Theme.visualEffectAnimationDuration * 2
            easing.type: Easing.OutCubic
        }
    }
}
