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

#include "catch/catch.hpp"

#include "veins/modules/phy/Spectrum80211.h"
#include "veins/modules/phy/ITSG5Channels.h"

using Veins::Spectrum80211;
using Veins::ITSG5Channels;

template <typename Spect>
class TypedSignal {
public:
    using Powerlevel = int;
    TypedSignal(std::array<Powerlevel, Spect::numFrequencies> powerlevels)
        : powerlevels(powerlevels)
    {}

    template<typename Freq>
    Powerlevel& at()
    {
        return powerlevels[Freq::index];
    }

    template<typename Freq>
    const Powerlevel& at() const
    {
        return powerlevels[Freq::index];
    }
private:
    std::array<Powerlevel, Spect::numFrequencies> powerlevels;
};

SCENARIO("Templated access to powerlevels")
{
    GIVEN("A TypedSignal in Spectrum80211 with Powerlevels (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)")
    {
        TypedSignal<Spectrum80211> sig({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
        THEN("The powerlevel at channel 180 (5900 MHz) should be 9")
        {
            REQUIRE(sig.at<Spectrum80211::CH180>() == 9);
        }
    }
}

SCENARIO("Accessing power levels via a channel")
{
    GIVEN("A TypedSignal in Spectrum80211 with Powerlevels (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)")
    {
        TypedSignal<Spectrum80211> sig({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
        using Channel = ITSG5Channels<TypedSignal>::CCH;

        WHEN("Accessing the signal (readonly) via ITSG5Channels::CCH")
        {
            const auto res = Channel::access(sig);
            THEN("The size of the returned powerlevel array should be 3")
            {
                REQUIRE(res.size() == 3);
            }
            THEN("The result should contain the power levels (8, 9, 10)")
            {
                REQUIRE(res.at(0) == 8);
                REQUIRE(res.at(1) == 9);
                REQUIRE(res.at(2) == 10);
            }
        }

        WHEN("Overriding the frequencies of the channel on that signal to (20, 21, 22)")
        {
            auto res = Channel::access(sig);
            // write using the .get member
            res[0].get() = 20;
            // write by converting to a plain reference
            TypedSignal<Spectrum80211>::Powerlevel& powerlevel_four_mhz = res[1];
            powerlevel_four_mhz = 21;
            // write by casting to plain reference
            static_cast<TypedSignal<Spectrum80211>::Powerlevel&>(res[2]) = 22;
            THEN("The signal's powerlevel at (CH179, CH180, CH181) should be (20, 21, 22)")
            {
                REQUIRE(sig.at<Spectrum80211::CH179>() == 20);
                REQUIRE(sig.at<Spectrum80211::CH180>() == 21);
                REQUIRE(sig.at<Spectrum80211::CH181>() == 22);
            }
        }
    }
}
