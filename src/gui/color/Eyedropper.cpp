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

#include "Eyedropper.h"
#include "Eyedropper_p.h"

#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include <QBackingStore>
#include <QKeyEvent>
#include <QPainter>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformservices.h>

namespace SVS {


    class ScreenMirrorWindow : public QWindow {
    public:
        explicit ScreenMirrorWindow(QScreen *screen, EyedropperPrivate *d) : QWindow(screen), d(d), m_backingStore(this) {
            setSurfaceType(RasterSurface);
            setScreen(screen);
            setGeometry(screen->geometry());
            setFlag(Qt::FramelessWindowHint, true);
            setFlag(Qt::WindowStaysOnTopHint, true);
            setFlag(Qt::Popup, true);
            m_screenshot = screen->grabWindow(0);
            showFullScreen();
        }

        void update() {
            m_backingStore.resize(size());
            QRegion region(QRect(QPoint(0, 0), size()));
            m_backingStore.beginPaint(region);

            QPainter painter(m_backingStore.paintDevice());
            painter.setRenderHint(QPainter::Antialiasing);
            painter.drawPixmap(0, 0, m_screenshot);

            int regionSize = 9;
            int factor = 10;

            QPoint pos = mapFromGlobal(QCursor::pos());
            if (geometry().contains(QCursor::pos())) {
                auto ratio = screen()->devicePixelRatio();
                auto pixmapPos = pos * ratio;
                QRect sourceRect(pixmapPos.x() - regionSize / 2, pixmapPos.y() - regionSize / 2, regionSize, regionSize);
                QRectF targetRect(pos.x() - regionSize * 0.5 * factor, pos.y() - regionSize * 0.5 * factor, regionSize * factor, regionSize * factor);
                painter.drawPixmap(targetRect, m_screenshot, sourceRect);
                painter.setPen(0x7f7f7f);
                painter.drawRect(targetRect);

                for (int i = 1; i < regionSize; i++) {
                    auto lineX = targetRect.x() + i * targetRect.width() / regionSize;
                    auto lineY = targetRect.y() + i * targetRect.height() / regionSize;
                    painter.drawLine(QLineF(lineX, targetRect.top(), lineX, targetRect.bottom()));
                    painter.drawLine(QLineF(targetRect.left(), lineY, targetRect.right(), lineY));
                }

                QRectF labelRectTop(targetRect.x(), targetRect.top() - 32, targetRect.width(), 24);
                QRectF labelRectBottom(targetRect.x(), targetRect.bottom() + 8, targetRect.width(), 24);
                QRectF &labelRect = labelRectBottom.bottom() > size().height() ? labelRectTop : labelRectBottom;
                painter.setPen(0xbfbfbf);
                painter.setBrush(QColor(0x3f3f3f));
                painter.drawRect(labelRect);

                QColor pickedColor = m_screenshot.copy(pixmapPos.x(), pixmapPos.y(), 1, 1).toImage().pixel(0, 0);
                painter.setPen(Qt::white);
                painter.drawText(labelRect, Qt::AlignCenter, pickedColor.name());
                d->color = pickedColor;
            }
            painter.end();

            m_backingStore.endPaint();
            m_backingStore.flush(region);

        }

    protected:
        void paintEvent(QPaintEvent *) override {
            update();
        }

    private:
        EyedropperPrivate *d;
        QBackingStore m_backingStore;
        QPixmap m_screenshot;
    };

    void EyedropperPrivate::createWindows() {
        for (auto screen : QGuiApplication::screens()) {
            auto window = new ScreenMirrorWindow(screen, this);
            windows.append(window);
        }
    }
    void EyedropperPrivate::destroyWindows() {
        for (auto window : windows) {
            window->hide();
            window->deleteLater();
        }
        windows.clear();
    }

    Eyedropper::Eyedropper(QObject *parent) : QObject(parent), d_ptr(new EyedropperPrivate) {
        Q_D(Eyedropper);
        d->q_ptr = this;
    }
    Eyedropper::~Eyedropper() {
        Q_D(Eyedropper);
        d->destroyWindows();
    }

    void Eyedropper::pickColor(QWindow *window, bool useNativeColorPickerIfAvailable) {
        Q_D(Eyedropper);
        if (useNativeColorPickerIfAvailable && QGuiApplicationPrivate::platformIntegration()->services()->hasCapability(QPlatformServices::ColorPicking)) {
            auto colorPicker = QGuiApplicationPrivate::platformIntegration()->services()->colorPicker(window);
            colorPicker->setParent(this);
            connect(colorPicker, &QPlatformServiceColorPicker::colorPicked, this, &Eyedropper::colorPicked);
            colorPicker->pickColor();
        } else {
            qGuiApp->installEventFilter(this);
            d->createWindows();
        }
    }

    QColor Eyedropper::pickColorSync(QWindow *window, bool useNativeColorPickerIfAvailable) {
        QColor color;
        QEventLoop eventLoop;
        Eyedropper eyedropper;
        connect(&eyedropper, &Eyedropper::colorPicked, &eventLoop, [&](const QColor &c) {
            color = c;
            eventLoop.exit();
        });
        eyedropper.pickColor(window, useNativeColorPickerIfAvailable);
        eventLoop.exec();
        return color;
    }
    bool Eyedropper::eventFilter(QObject *watched, QEvent *event) {
        Q_D(Eyedropper);
        if (event->type() == QEvent::MouseMove) {
            for (auto window : d->windows) {
                window->update();
            }
        } else if (event->type() == QEvent::KeyPress) {
            auto keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Escape) {
                d->destroyWindows();
                emit colorPicked({});
            } else if (keyEvent->key() == Qt::Key_Return) {
                d->destroyWindows();
                emit colorPicked(d->color);
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            emit colorPicked(d->color);
        }
        return QObject::eventFilter(watched, event);
    }
}