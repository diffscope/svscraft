#ifndef SVSCRAFT_MUSICTIMESIGNATURE_H
#define SVSCRAFT_MUSICTIMESIGNATURE_H

#include <QMetaType>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT MusicTimeSignature {
        Q_GADGET
        Q_PROPERTY(int numerator READ numerator CONSTANT)
        Q_PROPERTY(int denominator READ denominator CONSTANT)
        Q_PROPERTY(bool isValid READ isValid CONSTANT)
    public:
        constexpr inline MusicTimeSignature() : MusicTimeSignature(4, 4) {
        }
        constexpr inline MusicTimeSignature(int numerator, int denominator) : m_numerator(numerator), m_denominator(denominator) {
        }

        constexpr inline int numerator() const {
            return m_numerator;
        }
        constexpr inline int denominator() const {
            return m_denominator;
        }
        constexpr inline void setNumerator(int numerator) {
            m_numerator = numerator;
        }
        constexpr inline void setDenominator(int denominator) {
            m_denominator = denominator;
        }

        constexpr bool isValid() const {
            return m_numerator > 0 && m_denominator > 0 && !(m_denominator & (m_denominator - 1));
        }

        Q_INVOKABLE constexpr int ticksPerBar(int resolution) const {
            return resolution * m_numerator * 4 / m_denominator;
        }

        Q_INVOKABLE constexpr int ticksPerBeat(int resolution) const {
            return resolution * 4 / denominator();
        }

        Q_INVOKABLE QString toString() const;

        constexpr inline bool operator==(const MusicTimeSignature &t) const = default;

        friend SVSCRAFT_CORE_EXPORT QDebug operator<<(QDebug debug, const MusicTimeSignature &t);

    private:
        int m_numerator;
        int m_denominator;
    };

}

#endif //MUSICTIMESIGNATURE_H
