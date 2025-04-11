#ifndef SVSCRAFT_ANCHOREDCURVE_P_H
#define SVSCRAFT_ANCHOREDCURVE_P_H

#include <SVSCraftCore/AnchoredCurve.h>

#include <map>

#include <QSharedData>

namespace SVS {

    class AnchoredCurveData : public QSharedData {
    public:
        struct AnchorData {
            double y;
            double d;
            AnchoredCurve::Anchor::InterpolationMode interpolationMode;
        };
        std::map<double, AnchorData> anchors;
        inline std::optional<std::pair<double, AnchoredCurveData::AnchorData>> getPrevExclusive(double x) const {
            auto lower = anchors.lower_bound(x);
            if (lower != anchors.begin()) {
                lower = std::prev(lower);
                return *lower;
            }
            return std::nullopt;
        }
        inline std::optional<std::pair<double, AnchoredCurveData::AnchorData>> getPrevInclusive(double x) const {
            auto lower = anchors.upper_bound(x);
            if (lower != anchors.begin()) {
                lower = std::prev(lower);
                return *lower;
            }
            return std::nullopt;
        }
        inline std::optional<std::pair<double, AnchoredCurveData::AnchorData>> getNext(double x) const {
            auto upper = anchors.upper_bound(x);
            if (upper != anchors.end()) {
                return *upper;
            }
            return std::nullopt;
        }
        inline void updateSlope(
            const std::optional<std::pair<double, AnchorData>> &prev,
            const std::optional<std::pair<double, AnchorData>> &target,
            const std::optional<std::pair<double, AnchorData>> &next
        ) {
            Q_ASSERT(target.has_value());
            double slope = 0;
            if (!prev.has_value() && !next.has_value()) {
                slope = 0;
            } else if (target->second.interpolationMode == AnchoredCurve::Anchor::Zero) {
                slope = 0;
            } else if (prev.has_value() && prev->second.interpolationMode == AnchoredCurve::Anchor::Zero) {
                slope = 0;
            } else if ((!prev.has_value() || prev->second.interpolationMode == AnchoredCurve::Anchor::Break) && next.has_value()) {
                slope = (next->second.y - target->second.y) / (next->first - target->first);
            } else if ((!next.has_value() || target->second.interpolationMode == AnchoredCurve::Anchor::Break) && prev.has_value()) {
                slope = (prev->second.y - target->second.y) / (prev->first - target->first);
            } else if ((next->second.y - target->second.y) * (target->second.y - prev->second.y) < 0) {
                slope = 0;
            }  else {
                slope = ((next->first - prev->first) * (next->second.y - target->second.y) * (target->second.y - prev->second.y)) / ((next->second.y - prev->second.y) * (next->first - target->first) * (target->first - prev->first));
            }
            anchors.erase(target->first);
            anchors.insert({target->first, {target->second.y, slope, target->second.interpolationMode}});
        }
    };

}

#endif //SVSCRAFT_ANCHOREDCURVE_P_H
