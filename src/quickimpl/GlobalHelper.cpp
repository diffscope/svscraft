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

#include "GlobalHelper_p.h"

#include <QCursor>
#include <QQuickItem>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformdialoghelper.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformservices.h>
#include <QEventLoop>

#ifdef Q_OS_WIN
#    include <qt_windows.h>
#endif

namespace SVS {
    GlobalHelper::GlobalHelper(QObject *parent) : QObject(parent) {
    }
    GlobalHelper::~GlobalHelper() = default;
    QPoint GlobalHelper::cursorPos() {
        return QCursor::pos();
    }
    void GlobalHelper::setProperty(QObject *object, const QString &key, const QVariant &value) {
        if (!object)
            return;
        object->setProperty(key.toUtf8(), value);
    }
    void GlobalHelper::ungrabMouse(QQuickItem *item) {
        if (!item)
            return;
        item->ungrabMouse();
    }
    double GlobalHelper::getCmyk(const QColor &color, int i) {
        if (i == 0)
            return color.cyanF();
        if (i == 1)
            return color.magentaF();
        if (i == 2)
            return color.yellowF();
        return color.blackF();
    }
    QColor GlobalHelper::setCmyk(float c, float m, float y, float k, float a) {
        return QColor::fromCmykF(c, m, y, k, a);
    }
    bool GlobalHelper::hasNativeColorChooser() {
#ifdef Q_OS_WIN
        return true;
#else
        return QGuiApplicationPrivate::platformTheme()->usePlatformNativeDialog(QPlatformTheme::ColorDialog);
#endif
    }
    QColor GlobalHelper::nativeChooseColor(const QColor &color, QWindow *window, bool hasAlpha) {
#ifdef Q_OS_WIN
        CHOOSECOLOR cc;
        static COLORREF acrCustClr[16];
        DWORD rgbCurrent = RGB(color.red(), color.green(), color.blue());

        ZeroMemory(&cc, sizeof(cc));
        cc.lStructSize = sizeof(cc);
        cc.hwndOwner = reinterpret_cast<HWND>(window->winId());
        cc.lpCustColors = acrCustClr;
        cc.rgbResult = rgbCurrent;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;

        if (ChooseColor(&cc)) {
            rgbCurrent = cc.rgbResult;
            return QColor::fromRgb(GetRValue(rgbCurrent), GetGValue(rgbCurrent), GetBValue(rgbCurrent));
        }

        return {};
#else
        QScopedPointer<QPlatformColorDialogHelper> dlg(qobject_cast<QPlatformColorDialogHelper *>(QGuiApplicationPrivate::platformTheme()->createPlatformDialogHelper(QPlatformTheme::ColorDialog)));
        if (!dlg)
            return color;
        auto options = QColorDialogOptions::create();
        options->setOption(QColorDialogOptions::ShowAlphaChannel, hasAlpha);
        dlg->setOptions(options);
        dlg->setCurrentColor(color);
        QEventLoop eventLoop;
        connect(dlg.get(), &QPlatformDialogHelper::accept, &eventLoop, [&] { eventLoop.exit(QPlatformDialogHelper::Accepted); });
        connect(dlg.get(), &QPlatformDialogHelper::reject, &eventLoop, [&] { eventLoop.exit(QPlatformDialogHelper::Rejected); });
        dlg->show(Qt::Dialog, Qt::ApplicationModal, window);
        if (eventLoop.exec() == QPlatformDialogHelper::Accepted)
            return dlg->currentColor();
        return {};
#endif
    }
}

#include "moc_GlobalHelper_p.cpp"