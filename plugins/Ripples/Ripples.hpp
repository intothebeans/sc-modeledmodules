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
            float rq = 0;
            Coefficients coefficients;
            StoredSamples storedSamples;
        };

        struct Slopes {
            float b0_slope = 0;
            float b1_slope = 0;
            float b2_slope = 0;
            float b3_slope = 0;
            float b4_slope = 0;
            float a1_slope = 0;
            float a2_slope = 0;
            float a3_slope = 0;
            float a4_slope = 0;
        };

        const float FS = static_cast<float>(sampleRate());
        const float trt = sqrt2_f * 2;

        Filter BPF = Filter();
        Filter LPF2 = Filter();
        Filter LPF4 = Filter();

        [[nodiscard]] inline Coefficients secondOrderLowpassCoefficients(const Filter &filter) const;
        [[nodiscard]] inline Coefficients fourthOrderLowpassCoefficients(const Filter& filter) const;
        [[nodiscard]] inline Coefficients fourthOrderBandpassCoefficients(const Filter& filter) const;
        inline void calcLP2(const Slopes& slopes, int nSamples, const float* input, float* outbuf);
        static inline void calcFourthOrder(Filter& filter, const Slopes& slopes, int nSamples, const float* input, float* outbuf);
        static inline Slopes calcSlopes(const Filter& filter, const Coefficients& newCoefficients, int nSamples) ;
        enum inputs {INPUT, BP_CF, BP_R, BP_RQ, LP2_CF, LP2_R, LP4_CF, LP4_R, FM_FREQ, FM_MUL, FM_INDEX};

    };

} // namespace ModeledModules