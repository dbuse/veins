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

#include "veins/base/utils/Coord.h"

#include <algorithm>
#include <memory>

namespace Veins {

template <typename Spec>
class STSignal {
public:
    using Spectrum = Spec;
    using index_t = typename Spectrum::index_t;
    using Powerlevel = double;

    using value_type = Powerlevel;
    using reference = value_type&;
    using const_reference = const value_type&;

    STSignal() = default;
    STSignal(const STSignal<Spectrum>& orig) = default;
    STSignal(simtime_t startTime, simtime_t duration)
        : startTime(startTime)
        , endTime(startTime + duration)
    {}
    STSignal(simtime_t startTime, simtime_t duration, std::array<value_type, Spectrum::numFrequencies> powerLevels)
        : startTime(startTime)
        , endTime(startTime + duration)
        , powerLevels(powerLevels)
    {}

    simtime_t getStartTime() const
    {
        return startTime;
    }

    simtime_t getEndTime() const
    {
        return endTime;
    }

    simtime_t getDuration() const
    {
        return endTime - startTime;
    }

    std::array<value_type, Spectrum::numFrequencies>& getPowerLevels()
    {
        return powerLevels;
    }

    const std::array<value_type, Spectrum::numFrequencies>& getPowerLevels() const
    {
        return powerLevels;
    }

    template <typename Frequency>
    reference at()
    {
        return powerLevels.at(Frequency::index);
    }

    template <typename Frequency>
    const_reference at() const
    {
        return powerLevels.at(Frequency::index);
    }

    // TODO: decide whether to allow (but discourage) or remove this in favor of the function template
    reference at(index_t index)
    {
        return powerLevels.at(index);
    }

    // TODO: decide whether to allow (but discourage) or remove this in favor of the function template
    const_reference at(index_t index) const
    {
        return powerLevels.at(index);
    }

    STSignal<Spectrum>& operator+=(const STSignal<Spectrum>& other);
    STSignal<Spectrum>& operator+=(value_type scalar);

    STSignal<Spectrum>& operator-=(const STSignal<Spectrum>& other);
    STSignal<Spectrum>& operator-=(value_type scalar);

    STSignal<Spectrum>& operator*=(const STSignal<Spectrum>& other);
    STSignal<Spectrum>& operator*=(value_type scalar);

    STSignal<Spectrum>& operator/=(const STSignal<Spectrum>& other);
    STSignal<Spectrum>& operator/=(value_type scalar);

private:
    /** Point in time where the signal begins. */
    simtime_t startTime;
    /** Point in time where the signal has ended (no longer exists). */
    simtime_t endTime;
    /** Power levels of channels of this signal. */
    std::array<value_type, Spectrum::numFrequencies> powerLevels;
};

//
// In-Place Operators
//

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator+=(const STSignal<Spectrum>& other)
{
    ASSERT(getStartTime() == other.getStartTime());
    ASSERT(getEndTime() == other.getEndTime());
    std::transform(powerLevels.begin(), powerLevels.end(), other.powerLevels.begin(), powerLevels.begin(), std::plus<value_type>());
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator+=(value_type scalar)
{
    std::transform(powerLevels.begin(), powerLevels.end(), powerLevels.begin(), [scalar](value_type other) { return other + scalar; });
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator-=(const STSignal<Spectrum>& other)
{
    ASSERT(getStartTime() == other.getStartTime());
    ASSERT(getEndTime() == other.getEndTime());
    std::transform(powerLevels.begin(), powerLevels.end(), other.powerLevels.begin(), powerLevels.begin(), std::minus<value_type>());
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator-=(value_type scalar)
{
    std::transform(powerLevels.begin(), powerLevels.end(), powerLevels.begin(), [scalar](value_type other) { return other - scalar; });
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator*=(const STSignal<Spectrum>& other)
{
    ASSERT(getStartTime() == other.getStartTime());
    ASSERT(getEndTime() == other.getEndTime());
    std::transform(powerLevels.begin(), powerLevels.end(), other.powerLevels.begin(), powerLevels.begin(), std::multiplies<value_type>());
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator*=(value_type scalar)
{
    std::transform(powerLevels.begin(), powerLevels.end(), powerLevels.begin(), [scalar](value_type other) { return other * scalar; });
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator/=(const STSignal<Spectrum>& other)
{
    ASSERT(getStartTime() == other.getStartTime());
    ASSERT(getEndTime() == other.getEndTime());
    std::transform(powerLevels.begin(), powerLevels.end(), other.powerLevels.begin(), powerLevels.begin(), std::divides<value_type>());
    return *this;
}

template <typename Spectrum>
STSignal<Spectrum>& STSignal<Spectrum>::operator/=(value_type scalar)
{
    std::transform(powerLevels.begin(), powerLevels.end(), powerLevels.begin(), [scalar](value_type other) { return other / scalar; });
    return *this;
}

//
// Standalone Operators
//

template <typename Spectrum>
bool operator==(const STSignal<Spectrum>& lhs, const STSignal<Spectrum>& rhs)
{
    // TODO: optimize comparison of power levels if necessary
    return lhs.getStartTime() == rhs.getStartTime() && lhs.getEndTime() == rhs.getEndTime() && lhs.getPowerLevels() == rhs.getPowerLevels();
}

template <typename Spec>
STSignal<Spec> operator+(const STSignal<Spec>& lhs, const STSignal<Spec>& rhs)
{
    STSignal<Spec> res(lhs);
    res += rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator+(const STSignal<Spec>& lhs, typename STSignal<Spec>::value_type rhs)
{
    STSignal<Spec> res(lhs);
    res += rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator-(const STSignal<Spec>& lhs, const STSignal<Spec>& rhs)
{
    STSignal<Spec> res(lhs);
    res -= rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator-(const STSignal<Spec>& lhs, typename STSignal<Spec>::value_type rhs)
{
    STSignal<Spec> res(lhs);
    res -= rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator*(const STSignal<Spec>& lhs, const STSignal<Spec>& rhs)
{
    STSignal<Spec> res(lhs);
    res *= rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator*(const STSignal<Spec>& lhs, typename STSignal<Spec>::value_type rhs)
{
    STSignal<Spec> res(lhs);
    res *= rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator/(const STSignal<Spec>& lhs, const STSignal<Spec>& rhs)
{
    STSignal<Spec> res(lhs);
    res /= rhs;
    return res;
}

template <typename Spec>
STSignal<Spec> operator/(const STSignal<Spec>& lhs, typename STSignal<Spec>::value_type rhs)
{
    STSignal<Spec> res(lhs);
    res /= rhs;
    return res;
}

template <typename Signal>
Signal offsetByTime(const Signal& signal, simtime_t offset)
{
    return Signal(signal.getStartTime() + offset, signal.getDuration(), signal.getPowerLevels);
}

//
// Helper Functions for Thresholding
// (Implementation is here though they are mostly helpers to be called by LazyAttenuatedSignal)

// TODO: support const STSignal&
template <typename Signal, typename Frequency>
bool signalBelowPowerlevelAtFrequency(const Signal& signal, typename Signal::Powerlevel threshold)
{
    return signal.template at<Frequency>() < threshold;
}

// TODO: support const STSignal&
template <typename Signal, typename Channel>
bool signalBelowPowerlevelAtChannel(Signal signal, typename Signal::Powerlevel threshold)
{
    using Powerlevel = typename Signal::Powerlevel;
    auto powerlevels = Channel::access(signal);
    // TODO: replace with std::any_of or so
    for (const Powerlevel& value : powerlevels) {
        if (value >= threshold) return false;
    }
    return true;
}

template <typename Signal>
bool signalBelowPowerlevelEverywhere(Signal signal, typename Signal::Powerlevel threshold)
{
    auto& powerlevels = signal.getPowerLevels();
    // TODO: replace with std::any_of or so
    for (size_t i = 0; i < Signal::Spectrum::numFrequencies; ++i) {
        if (powerlevels[i] > threshold) return false;
    }
    return true;
}

// TODO: move to its own file (probably near AirFrame.msg)
template <typename Spec>
struct AirFrameMetadataContainer : cObject {
    // Attach me to AirFrames (wrapped in a std::shared_ptr
public:
    STSignal<Spec> signal;
};

} // namespace Veins
