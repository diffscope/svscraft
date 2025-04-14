#include "AnchoredCurve.h"
#include "AnchoredCurve_p.h"

#include <optional>
#include <algorithm>
#include <iterator>
#include <array>

#include <boost/math/interpolators/cubic_hermite.hpp>

#include <QList>
#include <QDebug>

namespace SVS {
    AnchoredCurve::AnchoredCurve() : d(new AnchoredCurveData) {
    }
    AnchoredCurve::AnchoredCurve(std::initializer_list<Anchor> anchors) : AnchoredCurve() {
        for (const auto &anchor : anchors) {
            addAnchor(anchor);
        }
    }
    AnchoredCurve &AnchoredCurve::operator=(std::initializer_list<Anchor> anchors) {
        return *this = AnchoredCurve(anchors);
    }
    AnchoredCurve::AnchoredCurve(const AnchoredCurve &anchoredCurve) = default;
    AnchoredCurve &AnchoredCurve::operator=(const AnchoredCurve &anchoredCurve) = default;
    AnchoredCurve::AnchoredCurve(AnchoredCurve &&anchoredCurve) noexcept = default;
    AnchoredCurve &AnchoredCurve::operator=(AnchoredCurve &&anchoredCurve) noexcept = default;

    AnchoredCurve::~AnchoredCurve() = default;

    void AnchoredCurve::addAnchor(const Anchor &anchor) {
        Q_ASSERT(!anchor.isNull());
        auto nextIt = d->anchors.lower_bound(anchor.x);
        if (nextIt != d->anchors.end() && nextIt->first == anchor.x) {
            const auto it = nextIt;
            ++nextIt;
            d->anchors.erase(it);
        }
        std::optional<std::pair<double, AnchoredCurveData::AnchorData>>
            prevKey = d->getPrevExclusive(anchor.x),
            nextKey = d->getNext(anchor.x);
        std::pair<double, AnchoredCurveData::AnchorData> currentKey = {anchor.x, {anchor.y, 0, anchor.interpolationMode}};
        if (prevKey.has_value()) {
            d->updateSlope(d->getPrevExclusive(prevKey->first), prevKey, currentKey);
        }
        if (nextKey.has_value()) {
            d->updateSlope(currentKey, nextKey, d->getNext(nextKey->first));
        }
        d->updateSlope(prevKey, currentKey, nextKey);
    }
    bool AnchoredCurve::removeAnchor(double x) {
        if (d->anchors.erase(x) == 0)
            return false;
        std::optional<std::pair<double, AnchoredCurveData::AnchorData>>
            prevKey = d->getPrevExclusive(x),
            nextKey = d->getNext(x);
        if (prevKey.has_value()) {
            d->updateSlope(d->getPrevExclusive(prevKey->first), prevKey, nextKey);
        }
        if (nextKey.has_value()) {
            d->updateSlope(prevKey, nextKey, d->getNext(nextKey->first));
        }
        return true;
    }
    AnchoredCurve::Anchor AnchoredCurve::anchor(double x) const {
        auto it = d->anchors.find(x);
        if (it == d->anchors.end())
            return {};
        return {it->first, it->second.y, it->second.interpolationMode};
    }
    QList<AnchoredCurve::Anchor> AnchoredCurve::anchors() const {
        QList<Anchor> anchors;
        anchors.reserve(d->anchors.size());
        std::ranges::transform(d->anchors, std::back_inserter(anchors), [](const auto &p) {
            return Anchor(p.first, p.second.y, p.second.interpolationMode);
        });
        return anchors;
    }
    size_t AnchoredCurve::count() const {
        return d->anchors.size();
    }
    static constexpr double linearInterpolation(double x0, double y0, double x1, double y1, double x) {
        return (y1 - y0) / (x1 - x0) * x + (x1 * y0 - x0 * y1) / (x1 - x0);
    }
    static inline double hermite(double x0, double y0, double d0, double x1, double y1, double d1, double x) {
        using Array = std::array<double, 2>;
        return boost::math::interpolators::cubic_hermite(Array{x0, x1}, Array{y0, y1}, Array{d0, d1})(x);
    }
    double AnchoredCurve::value(double x, bool *ok, double *prevX, double *nextX) const {
        auto prevKey = d->getPrevInclusive(x);
        auto nextKey = d->getNext(x);
        if (ok)
            *ok = true;
        if (prevKey.has_value() && prevX)
            *prevX = prevKey->first;
        if (nextKey.has_value() && nextX)
            *nextX = nextKey->first;
        if (!prevKey.has_value() && !nextKey.has_value()) {
            if (ok)
                *ok = false;
            return 0.0;
        }
        if (!prevKey.has_value()) {
            if (ok)
                *ok = false;
            return nextKey->second.y;
        }
        if (!nextKey.has_value()) {
            if (ok)
                *ok = false;
            return prevKey->second.y;
        }
        if (prevKey->second.interpolationMode == Anchor::Linear) {
            return linearInterpolation(prevKey->first, prevKey->second.y, nextKey->first, nextKey->second.y, x);
        }
        if (prevKey->second.interpolationMode == Anchor::Break) {
            if (ok)
                *ok = false;
            return prevKey->second.y;
        }
        if (prevKey->second.interpolationMode == Anchor::Zero) {
            return prevKey->second.y;
        }
        return hermite(prevKey->first, prevKey->second.y, prevKey->second.d, nextKey->first, nextKey->second.y, nextKey->second.d, x);

    }
} // SVS