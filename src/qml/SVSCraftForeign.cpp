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

#include "SVSCraftForeign_p.h"

#include <algorithm>
#include <ranges>

#include <QQmlEngine>

#include <SVSCraftGui/Eyedropper.h>
#include <SVSCraftGui/DesktopServices.h>

namespace SVS {
    void MusicTimelineExtended::setTimeSignature(int bar, const QJSValue &signature) {
        if (bar < 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Bar number should be positive or zero");
            return;
        }
        auto s = MusicTimeSignature(signature.property("numerator").toInt(), signature.property("denominator").toInt());
        if (!s.isValid()) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Invalid signature");
            return;
        }
        timeline->setTimeSignature(bar, s);
    }
    void MusicTimelineExtended::setTimeSignatures(const QJSValueList &value) {
        QList<QPair<int, MusicTimeSignature>> list;
        list.reserve(value.size());
        for (const auto &pair : value) {
            auto bar = pair.property(0).isNumber() ? pair.property(0).toInt() : -1;
            auto signature = pair.property(1);
            auto s = MusicTimeSignature(signature.property("numerator").toInt(), signature.property("denominator").toInt());
            if (bar < 0) {
                qjsEngine(this)->throwError(QJSValue::TypeError, "Bar number should be positive or zero");
                return;
            }
            if (!s.isValid()) {
                qjsEngine(this)->throwError(QJSValue::TypeError, "Invalid signature");
                return;
            }
            list.emplace_back(bar, s);
        }
        timeline->setTimeSignatures(list);
    }
    void MusicTimelineExtended::removeTimeSignatures(const QVariantList &value) {
        QList<int> list;
        list.reserve(value.size());
        for (const auto &v : value) {
            auto bar = v.canConvert(QMetaType(QMetaType::Int)) ? v.toInt() : -1;
            if (bar <= 0) {
                qjsEngine(this)->throwError(QJSValue::TypeError, "Bar number should be positive");
                return;
            }
            list.emplace_back(bar);
        }
        timeline->removeTimeSignatures(list);
    }
    QVariantMap MusicTimelineExtended::timeSignatures() const {
        auto m = timeline->timeSignatures();
        QVariantMap ret;
        std::for_each(m.constKeyValueBegin(), m.constKeyValueEnd(), [&ret](const auto &item) {
            ret.insert(QString::number(item.first), QVariant::fromValue(item.second));
        });
        return ret;
    }
    SVS::MusicTimeSignature MusicTimelineExtended::timeSignatureAt(int bar) const {
        if (bar < 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Bar number should be positive or zero");
            return {};
        }
        return timeline->timeSignatureAt(bar);
    }
    int MusicTimelineExtended::nearestBarWithTimeSignatureTo(int bar) const {
        if (bar < 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Bar number should be positive or zero");
            return {};
        }
        return timeline->nearestBarWithTimeSignatureTo(bar);
    }

    void MusicTimelineExtended::setTempo(int tick, double tempo) {
        if (tick < 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Tick number should be positive or zero");
            return;
        }
        if (tempo <= 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Tempo should be positive");
            return;
        }
        timeline->setTempo(tick, tempo);
    }
    void MusicTimelineExtended::setTempi(const QJSValueList &value) {
        QList<QPair<int, double>> list;
        list.reserve(value.size());
        for (const auto &pair : value) {
            auto tick = pair.property(0).isNumber() ? pair.property(0).toInt() : -1;
            auto tempo = pair.property(1).isNumber() ? pair.property(1).toNumber() : -1;
            if (tick < 0) {
                qjsEngine(this)->throwError(QJSValue::TypeError, "Bar number should be positive or zero");
                return;
            }
            if (tempo < 0) {
                qjsEngine(this)->throwError(QJSValue::TypeError, "Tempo should be positive");
                return;
            }
            list.emplace_back(tick, tempo);
        }
        timeline->setTempi(list);
    }
    void MusicTimelineExtended::removeTempi(const QVariantList &value) {
        QList<int> list;
        list.reserve(value.size());
        for (const auto &v : value) {
            auto tick = v.canConvert(QMetaType(QMetaType::Int)) ? v.toInt() : -1;
            if (tick <= 0) {
                qjsEngine(this)->throwError(QJSValue::TypeError, "Tick number should be positive");
                return;
            }
            list.emplace_back(tick);
        }
        timeline->removeTempi(list);
    }
    QVariantMap MusicTimelineExtended::tempi() const {
        auto m = timeline->tempi();
        QVariantMap ret;
        std::for_each(m.constKeyValueBegin(), m.constKeyValueEnd(), [&ret](const auto &item) {
            ret.insert(QString::number(item.first), item.second);
        });
        return ret;
    }
    double MusicTimelineExtended::tempoAt(int tick) const {
        if (tick < 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Tick number should be positive or zero");
            return {};
        }
        return timeline->tempoAt(tick);
    }
    int MusicTimelineExtended::nearestTickWithTempoTo(int tick) const {
        if (tick < 0) {
            qjsEngine(this)->throwError(QJSValue::TypeError, "Tick number should be positive or zero");
            return {};
        }
        return timeline->nearestTickWithTempoTo(tick);
    }



    EyedropperForeign::EyedropperForeign(QObject *parent) : QObject(parent) {
    }
    EyedropperForeign::~EyedropperForeign() = default;
    QColor EyedropperForeign::pickColor(QWindow *window, bool useNativeColorPickerIfAvailable) {
        return Eyedropper::pickColorSync(window, useNativeColorPickerIfAvailable);
    }



    DesktopServicesForeign::DesktopServicesForeign(QObject *parent) : QObject(parent) {
    }
    DesktopServicesForeign::~DesktopServicesForeign() = default;
    bool DesktopServicesForeign::reveal(const QString &filename) {
        return DesktopServices::reveal(filename);
    }
}
