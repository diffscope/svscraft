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

#ifndef SVSCRAFT_GLOBALHELPER_P_H
#define SVSCRAFT_GLOBALHELPER_P_H

#include <functional>

#include <QObject>
#include <qqmlintegration.h>

#include <QWindow>

#include <SVSCraftQuickImpl/SVSCraftQuickImplGlobal.h>

class QQuickItem;

namespace SVS {

    class SVSCRAFT_QUICK_IMPL_EXPORT GlobalHelper : public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        explicit GlobalHelper(QObject *parent = nullptr);
        ~GlobalHelper() override;

        using AlertHandler = std::function<void(QObject *)>;
        static AlertHandler alertHandler();
        static void setAlertHandler(const AlertHandler &handler);
        static void defaultAlertHandler(QObject *messageBox);

        Q_INVOKABLE static QPoint cursorPos();
        Q_INVOKABLE static void setProperty(QObject *object, const QString &key, const QVariant &value);
        Q_INVOKABLE static void ungrabMouse(QQuickItem *item);
        Q_INVOKABLE static double getCmyk(const QColor &color, int i);
        Q_INVOKABLE static QColor setCmyk(float c, float m, float y, float k, float a);
        Q_INVOKABLE static bool hasNativeColorChooser();
        Q_INVOKABLE static QColor nativeChooseColor(const QColor &color, QWindow *window, bool hasAlpha);
        Q_INVOKABLE static QObject *createGlobalCursorListener(QObject *parent = nullptr);
        Q_INVOKABLE static void invokeAlertHandler(QObject *o);
        Q_INVOKABLE static inline QString toHtmlEscaped(const QString &text) {
            return text.toHtmlEscaped();
        }
    };
}

#endif // SVSCRAFT_GLOBALHELPER_P_H
