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

#include "SVSQmlNamespace_p.h"

#include <SVSCraftCore/DecibelLinearizer.h>

namespace SVS {

    double SVSQmlNamespace::decibelToLinearValue(double decibel, double factor) {
        return DecibelLinearizer::decibelToLinearValue(decibel, factor);
    }
    double SVSQmlNamespace::linearValueToDecibel(double linearValue, double factor) {
        return DecibelLinearizer::linearValueToDecibel(linearValue, factor);
    }
    
}
