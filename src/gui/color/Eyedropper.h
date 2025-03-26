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

#ifndef SVSCRAFT_EYEDROPPER_H
#define SVSCRAFT_EYEDROPPER_H

#include <QObject>
#include <SVSCraftGui/SVSCraftGuiGlobal.h>

class QWindow;

namespace SVS {

    class EyedropperPrivate;

    class SVSCRAFT_GUI_EXPORT Eyedropper : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Eyedropper)
    public:
        explicit Eyedropper(QObject *parent = nullptr);
        ~Eyedropper() override;

        void pickColor(QWindow *window = nullptr, bool useNativeColorPickerIfAvailable = true);
        static QColor pickColorSync(QWindow *window = nullptr, bool useNativeColorPickerIfAvailable = true);

        bool eventFilter(QObject *watched, QEvent *event) override;

    signals:
        void colorPicked(const QColor &color);

    private:
        QScopedPointer<EyedropperPrivate> d_ptr;
    };

}

#endif //SVSCRAFT_EYEDROPPER_H
