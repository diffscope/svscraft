// SPDX-FileCopyrightText: OpenVPI
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DockingTypes.h"

#include <cmath>
#include <functional>

#include <QMetaType>

namespace SVS {
    QVariantMap DockingPlacement::toVariant() const {
        return {
            {"hostId", hostId},
            {"groupId", groupId},
            {"kind", int(kind)},
            {"side", side},
            {"anchor", anchor},
            {"after", after},
            {"edge", int(edge)}
        };
    }

    DockingPlacement DockingPlacement::fromVariant(const QVariantMap &v) {
        return {
            v.value("hostId").toString(),
            v.value("groupId").toString(),
            Kind(v.value("kind").toInt()),
            v.value("side").toInt(),
            v.value("anchor").toString(),
            v.value("after", true).toBool(),
            Qt::Edge(v.value("edge", int(Qt::LeftEdge)).toInt())
        };
    }

    QVariantMap DockingNodeState::toVariant() const {
        if (kind == "split") {
            QVariantList c, w;
            for (const auto &child : children)
                c.append(child.toVariant());
            for (auto weight : weights)
                w.append(weight);
            return {
                {"kind", kind},
                {"nodeId", nodeId},
                {"orientation", int(orientation)},
                {"children", c},
                {"weights", w}
            };
        }

        QVariantMap v{
            {"kind", kind},
            {"nodeId", nodeId},
            {"tabBarEdge", int(tabBarEdge)},
            {"autoHide", autoHide},
            {"widthLocked", widthLocked},
            {"heightLocked", heightLocked},
            {"expandedSize", expandedSize},
            {"lockedSize", lockedSize},
            {"contentRatio", contentRatio}
        };
        for (int i = 0; i < 2; ++i)
            v.insert(i ? "trailing" : "leading", QVariantMap{{"panelIds", panelIds[i]}, {"selected", selected[i]}, {"lastSelected", lastSelected[i]}});
        return v;
    }

    DockingNodeState DockingNodeState::fromVariant(const QVariantMap &v) {
        int count = 0;
        std::function<DockingNodeState(const QVariantMap &, int)> read = [&](const QVariantMap &node, int depth) {
            DockingNodeState s;
            auto invalid = [&] {
                s.kind = "invalid";
                return s;
            };
            if (++count > 4096 || depth > 64 || node.value("children").toList().size() > 4096 || node.value("weights").toList().size() > 4096)
                return invalid();
            auto validNumber = [&](const char *key, bool integral) {
                if (!node.contains(key))
                    return true;
                bool converted = false;
                auto value = node.value(key).toDouble(&converted);
                return converted && std::isfinite(value) && (!integral || value == std::trunc(value));
            };
            for (auto key : {"children", "weights"})
                if (node.contains(key) && !node.value(key).canConvert<QVariantList>())
                    return invalid();
            for (auto key : {"autoHide", "widthLocked", "heightLocked"})
                if (node.contains(key) && node.value(key).metaType().id() != QMetaType::Bool)
                    return invalid();
            if (!validNumber("orientation", true) || !validNumber("tabBarEdge", true) || !validNumber("contentRatio", false))
                return invalid();
            s.kind = node.value("kind").toString();
            s.nodeId = node.value("nodeId").toString();
            s.orientation = Qt::Orientation(node.value("orientation").toInt());
            for (const auto &child : node.value("children").toList()) {
                if (count > 4096)
                    return invalid();
                s.children.append(read(child.toMap(), depth + 1));
            }
            for (const auto &weight : node.value("weights").toList()) {
                bool converted = false;
                auto value = weight.toDouble(&converted);
                if (!converted)
                    return invalid();
                s.weights.append(value);
            }
            s.tabBarEdge = Qt::Edge(node.value("tabBarEdge", int(Qt::TopEdge)).toInt());
            s.autoHide = node.value("autoHide").toBool();
            s.widthLocked = node.value("widthLocked").toBool();
            s.heightLocked = node.value("heightLocked").toBool();
            s.expandedSize = node.value("expandedSize", QSizeF(400, 400)).toSizeF();
            s.lockedSize = node.value("lockedSize", QSizeF(400, 400)).toSizeF();
            s.contentRatio = node.value("contentRatio", 0.5).toDouble();
            for (int i = 0; i < 2; ++i) {
                auto key = i ? "trailing" : "leading";
                if (node.contains(key) && !node.value(key).canConvert<QVariantMap>())
                    return invalid();
                auto side = node.value(key).toMap();
                if (side.contains("panelIds") && !side.value("panelIds").canConvert<QStringList>())
                    return invalid();
                s.panelIds[i] = side.value("panelIds").toStringList();
                if (s.panelIds[i].size() > 4096)
                    return invalid();
                s.selected[i] = side.value("selected").toString();
                s.lastSelected[i] = side.value("lastSelected").toString();
            }
            return s;
        };
        return read(v, 0);
    }

    QVariantMap DockingLayoutState::toVariant() const {
        return hasRoot ? root.toVariant() : QVariantMap{};
    }

    DockingLayoutState DockingLayoutState::fromVariant(const QVariantMap &v) {
        return {!v.isEmpty(), DockingNodeState::fromVariant(v)};
    }
}
