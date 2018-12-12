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

#include "veins/modules/phy/Spectrum80211.h"
#include <array>

namespace Veins {

/**
 * European Channels for ITS-G5 (EN 302 663 - V1.2.1) in the 5GHz spectrum.
 *
 * Only valid for use with Spectrum80211.
 * Independent of Signal class template implementation.
 */
template <template <typename S> class SignalTemplate>
struct ITSG5Channels {
    using Spectrum = Spectrum80211;
    using index_t = Spectrum::index_t;
    using Signal = SignalTemplate<Spectrum>;
    using Powerlevel = typename Signal::Powerlevel;
    using PowerlevelRef = std::reference_wrapper<Powerlevel>;

    struct CCH {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH179>(),
                signal.template at<Spectrum::CH180>(),
                signal.template at<Spectrum::CH181>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH180>::inMHz == 5900, "Central frequency differs from standard!");
    };
    struct SCH2 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH177>(),
                signal.template at<Spectrum::CH178>(),
                signal.template at<Spectrum::CH179>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH178>::inMHz == 5890, "Central frequency differs from standard!");
    };
    struct SCH1 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH175>(),
                signal.template at<Spectrum::CH176>(),
                signal.template at<Spectrum::CH177>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH176>::inMHz == 5880, "Central frequency differs from standard!");
    };
    struct SCH3 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH173>(),
                signal.template at<Spectrum::CH174>(),
                signal.template at<Spectrum::CH175>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH174>::inMHz == 5870, "Central frequency differs from standard!");
    };
    struct SCH4 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH171>(),
                signal.template at<Spectrum::CH172>(),
                signal.template at<Spectrum::CH173>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH172>::inMHz == 5860, "Central frequency differs from standard!");
    };
    struct SCH5 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH181>(),
                signal.template at<Spectrum::CH182>(),
                signal.template at<Spectrum::CH183>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH182>::inMHz == 5910, "Central frequency differs from standard!");
    };
    struct SCH6 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::CH183>(),
                signal.template at<Spectrum::CH184>(),
                signal.template at<Spectrum::CH185>(),
            };
        }
        static_assert(Spectrum::frequency<Spectrum::CH184>::inMHz == 5920, "Central frequency differs from standard!");
    };
};

} // namespace Veins
