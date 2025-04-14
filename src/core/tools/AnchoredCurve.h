#ifndef SVSCRAFT_ANCHOREDCURVE_H
#define SVSCRAFT_ANCHOREDCURVE_H

#include <QSharedDataPointer>

#include <SVSCraftCore/SVSCraftCoreGlobal.h>

namespace SVS {

    class AnchoredCurveData;

    class SVSCRAFT_CORE_EXPORT AnchoredCurve {
    public:
        struct Anchor {
            double x;
            double y;
            enum InterpolationMode {
                Null, Break, Linear, Zero, Pchip,
            } interpolationMode;

            constexpr Anchor() : x(0), y(0), interpolationMode(Null) {
            }
            constexpr Anchor(double x, double y, InterpolationMode interpolationMode) : x(x), y(y), interpolationMode(interpolationMode) {
            }
            constexpr bool isNull() const {
                return interpolationMode == Null;
            }
        };
        AnchoredCurve();

        AnchoredCurve(std::initializer_list<Anchor> anchors);
        AnchoredCurve &operator=(std::initializer_list<Anchor> anchors);

        AnchoredCurve(const AnchoredCurve &anchoredCurve);
        AnchoredCurve &operator=(const AnchoredCurve &anchoredCurve);
        AnchoredCurve(AnchoredCurve &&anchoredCurve) noexcept;
        AnchoredCurve &operator=(AnchoredCurve &&anchoredCurve) noexcept;

        ~AnchoredCurve();

        void addAnchor(const Anchor &anchor);
        bool removeAnchor(double x);
        Anchor anchor(double x) const;
        QList<Anchor> anchors() const;
        size_t count() const;

        double value(double x, bool *ok = nullptr, double *prevX = nullptr, double *nextX = nullptr) const;

    private:
        QSharedDataPointer<AnchoredCurveData> d;
    };

}

#endif //SVSCRAFT_ANCHOREDCURVE_H
