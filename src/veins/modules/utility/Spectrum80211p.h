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

#include "veins/veins.h"

namespace Veins {

class Spectrum80211p {
public:
    using Frequency = double;
    using Frequencies = std::vector<Frequency>;
    using FrequencyIndex = size_t;
    using FrequencyIndices = std::vector<FrequencyIndex>;
    using Powerlevel = double;

    enum class Channel {
        CRIT_SOL = 172,
        SCH1 = 174,
        SCH2 = 176,
        CCH = 178,
        SCH3 = 180,
        SCH4 = 182,
        HPPS = 184
    };

    static Frequency centerFrequency(Channel channel) {
        // have to be ordered by frequency
        static std::map<Channel, Frequency> mapping = {
            {Channel::CRIT_SOL, 5.86e9},
            {Channel::SCH1, 5.87e9},
            {Channel::SCH2, 5.88e9},
            {Channel::CCH, 5.89e9},
            {Channel::SCH3, 5.90e9},
            {Channel::SCH4, 5.91e9},
            {Channel::HPPS, 5.92e9},
        };
        return mapping.at(channel);
    }

    static Frequencies channelFrequencies(Channel channel) {
        Frequency cf = centerFrequency(channel);
        return {cf - 5e6, cf, cf + 5e6};
    }

    static std::array<Channel, 7> channels()
    {
        std::array<Channel, 7> result = { Channel::CRIT_SOL, Channel::SCH1, Channel::SCH2, Channel::CCH, Channel::SCH3, Channel::SCH4, Channel::HPPS };
        return result;
    }

    static constexpr size_t numFrequencies = 7 * 2 + 1;
};


} // namespace Veins
