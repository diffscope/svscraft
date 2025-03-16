#ifndef SVSCRAFT_DECIBELLINEARIZER_H
#define SVSCRAFT_DECIBELLINEARIZER_H

#include <cmath>

#include <SVSCraftCore/svscraftcoreglobal.h>

namespace SVS {

    class SVSCRAFT_CORE_EXPORT DecibelLinearizer {
    public:
        inline static double decibelToLinearValue(double decibel, double factor = -24) {
            return std::exp((decibel - factor) / - factor) - std::exp(1);
        }

        inline static double linearValueToDecibel(double linearValue, double factor = -24) {
            return -factor * std::log(linearValue + std::exp(1)) + factor;
        }
    };

} // SVS

#endif // SVSCRAFT_DECIBELLINEARIZER_H
