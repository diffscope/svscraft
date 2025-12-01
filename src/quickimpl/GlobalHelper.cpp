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

#include <cstring>

#include <QCursor>
#include <QQuickItem>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformdialoghelper.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformservices.h>
#include <QEventLoop>
#include <QtQuickTemplates2/private/qquickicon_p.h>

#ifdef Q_OS_WIN
#    include <qt_windows.h>
#endif

#include <SVSCraftCore/MusicTime.h>
#include <SVSCraftCore/MusicTimeline.h>
#include <SVSCraftGui/ColorBlender.h>

namespace SVS {
    GlobalHelper::GlobalHelper(QObject *parent) : QObject(parent) {
    }
    GlobalHelper::~GlobalHelper() = default;

    GlobalHelper::AlertHandler m_alertHandler = GlobalHelper::defaultAlertHandler;

    GlobalHelper::AlertHandler GlobalHelper::alertHandler() {
        return m_alertHandler;
    }
    void GlobalHelper::setAlertHandler(const AlertHandler &handler) {
        m_alertHandler = handler;
    }

    class GlobalHandlerPseudoQMessageBox : public QObject {
        void *qt_metacast(const char *s) override {
            if (std::strcmp(s, "QMessageBox") == 0)
                return this;
            return nullptr;
        }
    };

    void GlobalHelper::defaultAlertHandler(QObject *messageBox) {
        bool ok;
        auto icon = messageBox->property("icon").toInt(&ok);
        if (!ok) {
            return;
        }
        auto o = new GlobalHandlerPseudoQMessageBox;
        o->setProperty("icon", icon);
        QAccessibleEvent event(o, QAccessible::Alert);
        QAccessible::updateAccessibility(&event);
    }

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
            return QColor::fromRgb(GetRValue(rgbCurrent), GetGValue(rgbCurrent),
                                   GetBValue(rgbCurrent));
        }

        return {};
#else
        QScopedPointer<QPlatformColorDialogHelper> dlg(qobject_cast<QPlatformColorDialogHelper *>(
            QGuiApplicationPrivate::platformTheme()->createPlatformDialogHelper(
                QPlatformTheme::ColorDialog)));
        if (!dlg)
            return color;
        auto options = QColorDialogOptions::create();
        options->setOption(QColorDialogOptions::ShowAlphaChannel, hasAlpha);
        dlg->setOptions(options);
        dlg->setCurrentColor(color);
        QEventLoop eventLoop;
        connect(dlg.get(), &QPlatformDialogHelper::accept, &eventLoop,
                [&] { eventLoop.exit(QPlatformDialogHelper::Accepted); });
        connect(dlg.get(), &QPlatformDialogHelper::reject, &eventLoop,
                [&] { eventLoop.exit(QPlatformDialogHelper::Rejected); });
        dlg->show(Qt::Dialog, Qt::ApplicationModal, window);
        if (eventLoop.exec() == QPlatformDialogHelper::Accepted)
            return dlg->currentColor();
        return {};
#endif
    }

    class MouseMoveListener : public QObject {
        Q_OBJECT
        Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    public:
        bool m_enabled = false;
        explicit MouseMoveListener(QObject *parent = nullptr) : QObject(parent) {
            qGuiApp->installEventFilter(this);
        }
        bool enabled() const {
            return m_enabled;
        }
        void setEnabled(bool enabled) {
            if (enabled == m_enabled)
                return;
            if (enabled) {
                qGuiApp->installEventFilter(this);
            } else {
                qGuiApp->removeEventFilter(this);
            }
            m_enabled = enabled;
        }
        bool eventFilter(QObject *watched, QEvent *event) override {
            if (event->type() == QEvent::MouseMove) {
                emit positionChanged();
            }
            return QObject::eventFilter(watched, event);
        }

    signals:
        void positionChanged();
        void enabledChanged();
    };

    QObject *GlobalHelper::createGlobalCursorListener(QObject *parent) {
        auto o = new MouseMoveListener(parent);
        return o;
    }
    void GlobalHelper::invokeAlertHandler(QObject *o) {
        m_alertHandler(o);
    }
    QQuickIcon GlobalHelper::defaultIcon() {
        return {};
    }
    QColor GlobalHelper::dockingPanelHeaderColor(const QColor &accentColor,
                                                 const QColor &backgroundColor, bool active) {
        if (!active)
            return backgroundColor;
        QColor accentColorTransparent = accentColor;
        accentColorTransparent.setAlphaF(0.15);
        return ColorBlender::blend<ColorBlender::Normal>(accentColorTransparent.rgba(),
                                                         backgroundColor.rgba());
    }
    QWindow *GlobalHelper::focusWindow() {
        return QGuiApplication::focusWindow();
    }
    void GlobalHelper::beep() {
        QGuiApplicationPrivate::platformIntegration()->beep();
    }

    int GlobalHelper::musicTimelineValueFromText(const MusicTimeline *timeline, const QString &text) {
        if (!timeline)
            return 0;
        return timeline->create(text).totalTick();
    }

    QString GlobalHelper::musicTimelineTextFromValue(const MusicTimeline *timeline, int value, int mw, int bw, int tw) {
        if (!timeline)
            return {};
        return timeline->create(0, 0, value).toString(mw, bw, tw);
    }
}

#include "GlobalHelper.moc"
#include "moc_GlobalHelper_p.cpp"
