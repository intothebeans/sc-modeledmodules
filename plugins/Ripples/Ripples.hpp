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

        inline void calculateCoefficients_bp(const float& FC, const float& R);
        inline void calculateCoefficients_lp1(const float& FC, const float& R);
        inline void calculateCoefficients_lp2(const float& FC, const float& R);

        [[nodiscard]] inline float FM(const float& in) const;

        const float FS = static_cast<float>(sampleRate());
        const float trt = sqrt2_f * 2;

        // BP
        float bp_cf;
        float bp_rq;
        // coefficients
        float bp_b0 = 0;
        float bp_b1 = 0;
        float bp_b2 = 0;
        float bp_b3 = 0;
        float bp_b4 = 0;
        float bp_a1 = 0;
        float bp_a2 = 0;
        float bp_a3 = 0;
        float bp_a4 = 0;

        // stored samples
        float bp_x1 = 0;
        float bp_x2 = 0;
        float bp_x3 = 0;
        float bp_x4 = 0;
        float bp_y1 = 0;
        float bp_y2 = 0;
        float bp_y3 = 0;
        float bp_y4 = 0;

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

        // coefficients
        float lp2_b0 = 0;
        float lp2_b1 = 0;
        float lp2_b2 = 0;
        float lp2_b3 = 0;
        float lp2_b4 = 0;
        float lp2_a1 = 0;
        float lp2_a2 = 0;
        float lp2_a3 = 0;
        float lp2_a4 = 0;

        // stored samples
        float lp2_x1 = 0;
        float lp2_x2 = 0;
        float lp2_x3 = 0;
        float lp2_x4 = 0;
        float lp2_y1 = 0;
        float lp2_y2 = 0;
        float lp2_y3 = 0;
        float lp2_y4 = 0;

        enum inputs {IN, FM_FREQ, FM_MUL, FM_INDEX, BW_in,
                BP_CF, BP_RQ, LP1_CF, LP1_RQ, LP2_CF, LP2_RQ};
    };

} // namespace ModeledModules