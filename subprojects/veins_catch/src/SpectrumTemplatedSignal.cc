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

#include "veins/base/toolbox/STSignal.h"
#include "veins/modules/phy/Spectrum80211.h"
#include "veins/modules/analogueModel/SimplePathloss.h"
#include "testutils/Simulation.h"

using namespace Veins;

class DummySpectrum {
public:
    using index_t = std::size_t;
    using frequency_t = std::uint64_t;
    using Powerlevel = double;

    struct F1 {
        static constexpr index_t index = 0;
    };
    struct F2 {
        static constexpr index_t index = 1;
    };
    struct F3 {
        static constexpr index_t index = 2;
    };
    struct F4 {
        static constexpr index_t index = 3;
    };
    struct F5 {
        static constexpr index_t index = 4;
    };
    struct F6 {
        static constexpr index_t index = 5;
    };

    static constexpr index_t numFrequencies = 6;
    static constexpr std::array<frequency_t, numFrequencies> frequenciesInMHz() {
        return {1000, 2000, 3000, 4000, 5000, 6000};
    }
};

template <template <typename S> class SignalTemplate>
class DummyChannelSet {
public:
    using Spectrum = DummySpectrum;
    using index_t = Spectrum::index_t;
    using Signal = SignalTemplate<Spectrum>;
    using Powerlevel = typename Signal::Powerlevel;
    using PowerlevelRef = std::reference_wrapper<Powerlevel>;

    struct CH1 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::F1>(),
                signal.template at<Spectrum::F2>(),
                signal.template at<Spectrum::F3>(),
            };
        }
    };
    struct CH2 {
        static constexpr index_t size = 3;
        static inline std::array<PowerlevelRef, size> access(Signal& signal)
        {
            return {
                signal.template at<Spectrum::F4>(),
                signal.template at<Spectrum::F5>(),
                signal.template at<Spectrum::F6>(),
            };
        }
    };
};

using index_t = DummySpectrum::index_t;
using Powerlevel = DummySpectrum::Powerlevel;

SCENARIO("Operators on two ST Signals", "[STSignals]")
{
    DummySimulation ds(new cNullEnvir(0, nullptr, nullptr)); // necessary so simtime_t works
    GIVEN("Two identical signals on a dummy 6-channel spectrum")
    {
        STSignal<DummySpectrum> sig_a(0, 5, {1, 1, 1, 0, 0, 0});
        STSignal<DummySpectrum> sig_b(0, 5, {1, 1, 1, 0, 0, 0});

        THEN("Comparing them should yield true")
        {
            REQUIRE(sig_a == sig_b);
        }

        WHEN("They are added up")
        {
            auto sum = sig_a + sig_b;
            THEN("The power level should be twice of the first for all frequencies")
            {
                for (size_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(sum.at(i) == Approx(sig_a.at(i) * 2));
                }
            }
        }

        WHEN("The second is substracted from the first")
        {
            auto diff = sig_a - sig_b;
            THEN("The power level should be zero for all frequencies")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(diff.at(i) == Approx(0));
                }
            }
        }

        WHEN("They are multiplied")
        {
            auto prod = sig_a * sig_b;
            THEN("The power level should be the square of the first for all frequencies")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(prod.at(i) == Approx(sig_a.at(i) * sig_a.at(i)));
                }
            }
        }

        WHEN("The first one is divided by the second")
        {
            auto quot = sig_a / sig_b;
            THEN("The power level should be one or not defined")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    if (sig_a.at(i) > 0)
                        REQUIRE(quot.at(i) == Approx(1));
                    else
                        REQUIRE(std::isnan(quot.at(i)));
                }
            }
        }
    }

}

SCENARIO("Operators on a ST Signal and a scalar", "[STSignals]")
{
    DummySimulation ds(new cNullEnvir(0, nullptr, nullptr)); // necessary so simtime_t works
    GIVEN("A signal with some power levels set and a scalar > 0")
    {
        STSignal<DummySpectrum> sig(0, 5, {2, 4, 6, 8, 10, 12});
        STSignal<DummySpectrum>::value_type scalar = 2;

        WHEN("The scalar is added to the signal")
        {
            auto sum = sig + scalar;
            THEN("The power level should be the sum for all frequencies")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(sum.at(i) == Approx(sig.at(i) + scalar));
                }
            }
        }

        WHEN("The scalar is substracted from the signal")
        {
            auto diff = sig - scalar;
            THEN("The power level should be the sum for all frequencies")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(diff.at(i) == Approx(sig.at(i) - scalar));
                }
            }
        }

        WHEN("The signal is multiplied by the scalar")
        {
            auto prod = sig * scalar;
            THEN("The power level should be the sum for all frequencies")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(prod.at(i) == Approx(sig.at(i) * scalar));
                }
            }
        }

        WHEN("The signal is divided by the scalar")
        {
            auto quot = sig / scalar;
            THEN("The power level should be the sum for all frequencies")
            {
                for (index_t i = 0; i < DummySpectrum::numFrequencies; ++i) {
                    REQUIRE(quot.at(i) == Approx(sig.at(i) / scalar));
                }
            }
        }
    }
}

SCENARIO("Comparing ST Signals against powerlevel thresholds", "[STSignals]")
{
    DummySimulation ds(new cNullEnvir(0, nullptr, nullptr)); // necessary so simtime_t works
    using SignalType = STSignal<DummySpectrum>;
    using activeFreq = DummySpectrum::F1;
    using activeChannel = DummyChannelSet<STSignal>::CH1;
    using passiveFreq = DummySpectrum::F4;
    using passiveChannel = DummyChannelSet<STSignal>::CH2;
    GIVEN("A signal with an active channel 1 (powerlevel 100) and passive channel 2")
    {
        SignalType sig(0, 5, {100, 100, 100, 0, 0, 0});
        WHEN("Comparing to a scalar powerlevel of 120 (above the powerlevel of the active frequencies)")
        {
            Powerlevel scalar = 120;
            THEN("The signal should be lower (true) for the active Frequency and Channel.")
            {
                REQUIRE(signalBelowPowerlevelAtFrequency<SignalType, activeFreq>(sig, scalar));
                REQUIRE(signalBelowPowerlevelAtChannel<SignalType, activeChannel>(sig, scalar));
            }
            THEN("The signal should be lower (true) for the passive Frequency and Channel.")
            {
                REQUIRE(signalBelowPowerlevelAtFrequency<SignalType, passiveFreq>(sig, scalar));
                REQUIRE(signalBelowPowerlevelAtChannel<SignalType, passiveChannel>(sig, scalar));
            }
            THEN("The whole signal should be lower (true).")
            {
                REQUIRE(signalBelowPowerlevelEverywhere<SignalType>(sig, scalar));
            }
        }
        GIVEN("A scalar powerlevel of 80 (below the powerlevel of the active frequencies)")
        {
            Powerlevel scalar = 80;
            THEN("The signal should be higher (false) for the active Frequency and Channel.")
            {
                REQUIRE(!signalBelowPowerlevelAtFrequency<SignalType, activeFreq>(sig, scalar));
                REQUIRE(!signalBelowPowerlevelAtChannel<SignalType, activeChannel>(sig, scalar));
            }
            THEN("The signal should be lower (true) for the passive Frequency and Channel.")
            {
                REQUIRE(signalBelowPowerlevelAtFrequency<SignalType, passiveFreq>(sig, scalar));
                REQUIRE(signalBelowPowerlevelAtChannel<SignalType, passiveChannel>(sig, scalar));
            }
            THEN("The whole signal should be higher (false).")
            {
                REQUIRE(!signalBelowPowerlevelEverywhere<SignalType>(sig, scalar));
            }
        }
    }

    GIVEN("A signal with a triangle-shaped powerlevel on channel one (50, 150, 50, 0, 0, 0)")
    {
        SignalType sig(0, 5, {50, 100, 50, 0, 0, 0});
        using highFreq = DummySpectrum::F2;
        using otherActiveFreq = DummySpectrum::F3;
        WHEN("Comparing to a threshold above the highest point of the spike (160)")
        {
            Powerlevel scalar = 120;
            THEN("The whole signal should be lower")
            {
                REQUIRE(signalBelowPowerlevelEverywhere<SignalType>(sig, scalar));
            }
        }
        WHEN("Comparing to a threshold below the hightest powerlevel of the signal but below the mittle ones (75)")
        {
            Powerlevel scalar = 75;
            THEN("The whole signal, the active channel, and the center of the active channel should be higher")
            {
                REQUIRE(!signalBelowPowerlevelEverywhere<SignalType>(sig, scalar));
                REQUIRE(!signalBelowPowerlevelAtChannel<SignalType, activeChannel>(sig, scalar));
                REQUIRE(!signalBelowPowerlevelAtFrequency<SignalType, highFreq>(sig, scalar));
            }
            THEN("The passive channel and the two side frequencies of the active channel should be lower")
            {
                REQUIRE(signalBelowPowerlevelAtChannel<SignalType, passiveChannel>(sig, scalar));
                REQUIRE(signalBelowPowerlevelAtFrequency<SignalType, activeFreq>(sig, scalar));
                REQUIRE(signalBelowPowerlevelAtFrequency<SignalType, otherActiveFreq>(sig, scalar));
            }
        }
    }
}

SCENARIO("Template SimplePathloss with alpha = 2", "[analogueModel]")
{
    DummySimulation ds(new cNullEnvir(0, nullptr, nullptr));
    using Spectrum = Spectrum80211;
    using Signal = STSignal<Spectrum>;
    AnalogueModels::SimplePathloss<Signal> spm(2.0);

    GIVEN("A signal in the 80211-Spectrum sent from (0, 0) with powerlevel 1")
    {
        STSignal<Spectrum> s(0, 0, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
        const Coord senderPos(0, 0, 2);
        WHEN("the receiver is at (2, 0)")
        {
            const Coord receiverPos(2, 0, 2);
            THEN("SimplePathlossModel drops power from 1 to 4.0874e-6 at 5.9 GHz")
            {
                spm.filterSignal(s, senderPos, receiverPos);
                REQUIRE(s.at<Spectrum::CH180>() == Approx(4.0874e-6).epsilon(0.001));
            }
        }

        WHEN("the receiver is at (5, 0)")
        {
            const Coord receiverPos(5, 0, 2);
            THEN("SimplePathlossModel drops power from 1 to 6.539e-7")
            {
                spm.filterSignal(s, senderPos, receiverPos);
                REQUIRE(s.at<Spectrum::CH180>()  == Approx(6.539e-7).epsilon(0.001));
            }
        }

        WHEN("the receiver is at (10, 0)")
        {
            const Coord receiverPos(10, 0, 2);
            THEN("SimplePathlossModel drops power from 1 to 1.634e-7")
            {
                spm.filterSignal(s, senderPos, receiverPos);
                REQUIRE(s.at<Spectrum::CH180>()  == Approx(1.634e-7).epsilon(0.001));
            }
        }

        WHEN("the receiver is at (100, 0)")
        {
            const Coord receiverPos(100, 0, 2);
            THEN("SimplePathlossModel drops power from 1 to 1.634e-9")
            {
                spm.filterSignal(s, senderPos, receiverPos);
                REQUIRE(s.at<Spectrum::CH180>() == Approx(1.634e-9).epsilon(0.001));
            }
        }
    }
}
