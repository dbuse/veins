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

#include "veins/base/toolbox/STSignal.h"
#include "veins/base/phyLayer/AnalogueModel.h"

#include <memory>
#include <vector>

namespace Veins {

/**
 * @brief Owning Wrapper around a Signal that manages lazy attenuation.
 *
 * STSignal is just a representation of what a Signal *could be*.
 * This class represents a conctete Signal that can be subject to attenuation and may be associated with a transmission.
 *
 * For performance reasons, attenuation is performed lazily.
 * Instead of working with the completely attenuated Signal, predicates can attenuate a Signal just as far as necessary.
 * This may avoid computing expensive attenuation patterns of AnalogueModels for many Signals.
 * E.g., Signals could be know to be too weak for reception before computing the most expensive AnalogueModels on it.
 *
 * TODO: (maybe) implement operator< for Signal and/or Powerlevel
 */
template <typename Spec>
class LazyAttenuatedSignal {
public:
    using Signal = STSignal<Spec>;
    using Powerlevel = typename Signal::Powerlevel;
    using AnalogueModelInstance = std::shared_ptr<const AnalogueModel>;
    using AnalogueModelInstances = std::vector<AnalogueModelInstance>;

    /**
     * @brief Create a new LazyAttenuatedSignal by copying a Signal and a collection of AnalogueModels.
     */
    LazyAttenuatedSignal(STSignal<Spec> signal, AnalogueModelInstances analogueModels)
        : signal(std::move(signal))
        , analogueModels(std::move(analogueModels))
    {}

    bool belowThresholdEverywhere(Powerlevel threshold)
    {
        while(!analogueModels.empty() && !signalBelowPowerlevelEverywhere<Signal>(signal, threshold)) {
            applyNextAnalogueModel();
        }
        return signalBelowThresholdAtIndices(signal, threshold);
    }

    template <typename Channel>
    bool belowThresholdAtChannel(Powerlevel threshold)
    {
        while(!analogueModels.empty() && !signalBelowPowerlevelAtChannel<Signal, Channel>(signal, threshold)) {
            applyNextAnalogueModel();
        }
        return signalBelowPowerlevelAtChannel<Signal, Channel>(signal, threshold);
    }

    template <typename Frequency>
    bool belowThresholdAtFrequency(Powerlevel threshold)
    {
        while(!analogueModels.empty() && !signalBelowPowerlevelAtFrequency<Signal, Frequency>(signal, threshold)) {
            applyNextAnalogueModel();
        }
        return signalBelowPowerlevelAtFrequency<Signal, Frequency>(signal, threshold);
    }

    STSignal<Spec> attenuatedSignal() {
        while(!analogueModels.empty()) {
            applyNextAnalogueModel();
        }
        return signal;
    }

    // TODO: check if we really need this (or could just create a new/different instance of this class)
    // void push_front(AnalogueModelInstance analogueModel);
    // void push_back(AnalogueModelInstance analogueModel);

private:
    Signal signal;
    AnalogueModelInstances analogueModels;

    void applyNextAnalogueModel()
    {
        ASSERT(!analogueModels.empty());
        // FIXME: implement
    }
};

} // namespace Veins
