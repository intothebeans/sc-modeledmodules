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

        inline void calculateCoefficients_bp(float FC, float Q);
        inline void calculateCoefficients_lp1(float FC, float R);
        inline void calculateCoefficients_lp2(float FC, float Q);

        inline float FM(float in);
        // Member variables
        const float FS = static_cast<float>(sampleRate());
        // BP
        float bp_cf;
        float bp_rq;

        //LP1
        float lp1_cf;
        float lp1_r;
        // coefficients
        float lp1_b0 = 0;
        float lp1_b1 = 0;
        float lp1_b2 = 0;
        float lp1_a1 = 0;
        float lp1_a2 = 0;

        // stored samples
        float lp1_x1 = 0;
        float lp1_x2 = 0;
        float lp1_y1 = 0;
        float lp1_y2 = 0;

        //LP2
        float lp2_cf;
        float lp2_rq;

        enum inputs {IN, FM_FREQ, FM_MUL, FM_INDEX,
                BP_CF, BP_RQ, LP1_CF, LP1_RQ, LP2_CF, LP2_RQ};
    };

} // namespace ModeledModules