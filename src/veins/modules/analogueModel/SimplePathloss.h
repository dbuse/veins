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

#include "veins/veins.h"

#include "veins/base/utils/Coord.h"
#include "veins/base/phyLayer/AnalogueModel.h"

#pragma once

namespace Veins {
namespace AnalogueModels {

// FIXME: inherit from AnalogueModel<SignalImplementation>
template <typename SignalImplementation>
class SimplePathloss {
public:
    using Signal = SignalImplementation;
    static constexpr double speedOfLightMpS = 299792458.0;
    static constexpr double twoPiSquared = 16.0 * 3.14159265358979323846 * 3.14159265358979323846;

    SimplePathloss(double alpha)
        : alphaHalf(alpha * 0.5)
    {}

    bool neverIncreasesPower() const
    {
        return true;
    }

    void filterSignal(Signal& signal, const Coord& senderPos, const Coord& receiverPos) // override
    {
        double sqrDistance = receiverPos.sqrdist(senderPos);
        if (sqrDistance <= 1) {
            return; // negligible attenuation
        }

        Signal attenuation = Signal(signal.getStartTime(), signal.getDuration(), getWavelengthsSquared());
        attenuation *= pow(sqrDistance, -alphaHalf) / twoPiSquared; // apply distFactor
        // perform attenuation
        signal *= attenuation; // FIXME: test and compare this!
    }
private:
    double alphaHalf;

    static std::array<double, Signal::Spectrum::numFrequencies> getWavelengthsSquared()
    {
        static std::array<double, Signal::Spectrum::numFrequencies> result = makeWavelengthsSquared();
        return result;
    }

    static std::array<double, Signal::Spectrum::numFrequencies> makeWavelengthsSquared()
    {
        std::array<double, Signal::Spectrum::numFrequencies> result;
        for (size_t i = 0; i < Signal::Spectrum::numFrequencies; ++i) {
            double wavelength = speedOfLightMpS / (Signal::Spectrum::frequenciesInMHz()[i] * 1e6);
            result[i] = wavelength * wavelength;
        }
        return result;
    }
};
} // namespace AnalogueModels
} // namespace Veins
