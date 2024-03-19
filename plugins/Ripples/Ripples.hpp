// PluginRipples.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules {

    class Ripples : public SCUnit {
    public:
        Ripples();
    private:
        void next(int nSamples);


        struct Coefficients {
            float b0 = 0;
            float b1 = 0;
            float b2 = 0;
            float b3 = 0;
            float b4 = 0;
            float a1 = 0;
            float a2 = 0;
            float a3 = 0;
            float a4 = 0;
        };

        struct StoredSamples {
            float x1 = 0;
            float x2 = 0;
            float x3 = 0;
            float x4 = 0;
            float y1 = 0;
            float y2 = 0;
            float y3 = 0;
            float y4 = 0;
        };

        struct Filter {
            float cf;
            float r;
            float bw = 0;
            Coefficients coefficients;
            StoredSamples storedSamples;
        };

        const float FS = static_cast<float>(sampleRate());
        const float trt = sqrt2_f * 2;

        Filter BPF = Filter();
        Filter LPF2 = Filter();
        Filter LPF4 = Filter();

        inline Coefficients secondOrderLowpassCoefficients(Filter &filter) const;

        enum inputs {INPUT, BP_CF, BP_R, BP_BW, LP2_CF, LP2_R, LP4_CF, LP4_R, FM_FREQ, FM_MUL, FM_INDEX};

    };

} // namespace ModeledModules