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

#include "nonius/nonius.h++"

#include "veins/base/toolbox/Spectrum.h"
#include "veins/base/toolbox/Signal.h"
#include "veins/base/toolbox/STSignal.h"
#include "veins/modules/phy/Spectrum80211.h"
#include "veins/modules/analogueModel/SimplePathloss.h"
#include "veins/modules/analogueModel/SimplePathlossModel.h"
#include "testutils/Simulation.h"

using namespace Veins;

const double alpha = 2.0;
const double carrierFrequency = 5.9e9;
const bool useTorus = false;
const Coord playgoundsize(1, 1);    // not used as useTorus is false
const Coord senderPos(0, 0);
const Coord receiverPos(100, 100);
const std::array<double, Spectrum80211::numFrequencies> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

NONIUS_BENCHMARK("non-templated signal", [](nonius::chronometer meter) {
    DummySimulation ds(new cNullEnvir(0, nullptr, nullptr));     // necessary so simtime_t works
    Spectrum spectrum(std::vector<double>(Spectrum80211::frequenciesInMHz().begin(), Spectrum80211::frequenciesInMHz().end()));
    Signal signalTpl(spectrum);
    for (size_t i = 0; i < values.size(); ++i) signalTpl.at(i) = values[i];
    std::vector<Signal> signals(meter.runs());
    std::fill(signals.begin(), signals.end(), signalTpl);
    SimplePathlossModel spm(alpha, carrierFrequency, useTorus, playgoundsize);
    auto* spmP = &spm;
    meter.measure([spmP, &signals](int i) {
        Signal* usedSignal = signals.data() + i;
        return spmP->filterSignal(usedSignal, senderPos, receiverPos);
    });
    spm.neverIncreasesPower(); // to avoid destruction of spm before the measure ran
});

NONIUS_BENCHMARK("templated signal", [](nonius::chronometer meter) {
    DummySimulation ds(new cNullEnvir(0, nullptr, nullptr));     // necessary so simtime_t works
    STSignal<Spectrum80211> signalTpl(0, 0, values);
    std::vector<STSignal<Spectrum80211>> signals(meter.runs());
    std::fill(signals.begin(), signals.end(), signalTpl);
    AnalogueModels::SimplePathloss<STSignal<Spectrum80211>> spm(alpha);
    auto* spmP = &spm;
    meter.measure([spmP, &signals](int i) {
        STSignal<Spectrum80211>& usedSignal = signals[i];
        return spmP->filterSignal(usedSignal, senderPos, receiverPos);
    });
    spm.neverIncreasesPower(); // to avoid destruction of spm before the measure ran
});
