//
// Copyright (C) 2018 Dominik S. Buse <buse@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#pragma once

#include <cstdint>
#include <array>

namespace Veins {

/**
 * Selection of channels / frequencies in the 5GHz spectrum as defined in IEEE Std 802.11-2012
 *
 * Only channels / frequencies relevant to typical V2X applications implemented here.
 * (As used in WAVE and ITS 5G).
 */
struct Spectrum80211 {
    using index_t = std::size_t;
    using frequency_t = std::uint64_t;

    struct CH171 {
        static constexpr index_t index = 0;
    };
    struct CH172 {
        static constexpr index_t index = 1;
    };
    struct CH173 {
        static constexpr index_t index = 2;
    };
    struct CH174 {
        static constexpr index_t index = 3;
    };
    struct CH175 {
        static constexpr index_t index = 4;
    };
    struct CH176 {
        static constexpr index_t index = 5;
    };
    struct CH177 {
        static constexpr index_t index = 6;
    };
    struct CH178 {
        static constexpr index_t index = 7;
    };
    struct CH179 {
        static constexpr index_t index = 8;
    };
    struct CH180 {
        static constexpr index_t index = 9;
    };
    struct CH181 {
        static constexpr index_t index = 10;
    };
    struct CH182 {
        static constexpr index_t index = 11;
    };
    struct CH183 {
        static constexpr index_t index = 12;
    };
    struct CH184 {
        static constexpr index_t index = 13;
    };
    struct CH185 {
        static constexpr index_t index = 14;
    };

    static constexpr index_t numFrequencies = 15;

    template <typename FrequencyT>
    struct frequency {
        static constexpr frequency_t inMHz = 5000 + 5 * (171 + FrequencyT::index);
    };

    static std::array<frequency_t, numFrequencies> frequenciesInMHz()
    {
        static std::array<frequency_t, numFrequencies> freqInMHz = {
            frequency<CH171>::inMHz,
            frequency<CH172>::inMHz,
            frequency<CH173>::inMHz,
            frequency<CH174>::inMHz,
            frequency<CH175>::inMHz,
            frequency<CH176>::inMHz,
            frequency<CH177>::inMHz,
            frequency<CH178>::inMHz,
            frequency<CH179>::inMHz,
            frequency<CH180>::inMHz,
            frequency<CH181>::inMHz,
            frequency<CH182>::inMHz,
            frequency<CH183>::inMHz,
            frequency<CH184>::inMHz,
            frequency<CH185>::inMHz,
        };
        return freqInMHz;
    }
    static_assert(frequency<CH171>::inMHz == 5855, "channel nr <-> Frequency relation wrong.");
    static_assert(frequency<CH185>::inMHz == 5925, "channel nr <-> Frequency relation wrong.");
};

} // namespace Veins
