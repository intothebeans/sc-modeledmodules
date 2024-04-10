// PluginRipples.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"
#include "../../external/iir1/Iir.h"

namespace ModeledModules {

    class Ripples : public SCUnit {
    public:
        Ripples();
    private:
        void next(int nSamples);

        Iir::ChebyshevI::LowPass<2> LPF2;
        Iir::ChebyshevI::LowPass<4> LPF4;
        Iir::ChebyshevI::BandPass<4> BPF;

        [[nodiscard]] float frequencyModulation(float s) const;
        enum inputs {INPUT, BP_CF, BP_R, BP_RQ, LP2_CF, LP2_R, LP4_CF, LP4_R, FM_FREQ, FM_MUL, FM_INDEX};

    };

} // namespace ModeledModules