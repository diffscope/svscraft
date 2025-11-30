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

#ifndef SVSCRAFT_MUSICTIMEVALIDATOR_P_H
#define SVSCRAFT_MUSICTIMEVALIDATOR_P_H

#include <SVSCraftGui/MusicTimeValidator.h>

#include <limits>

namespace SVS {

    class MusicTimeValidatorPrivate {
        Q_DECLARE_PUBLIC(MusicTimeValidator)
    public:
        MusicTimeValidator *q_ptr;
        
        int top{std::numeric_limits<int>::max()};
        int bottom{0};
        int measureWidth{1};
        int beatWidth{1};
        int tickWidth{3};
        MusicTimeline *timeline{};
    };

}

#endif // SVSCRAFT_MUSICTIMEVALIDATOR_P_H
