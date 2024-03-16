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

        struct Filter {
            int type = 0;
            float cf = 0;
            float r = 0;
            float bw = 0;

            float b0 = 0;
            float b1 = 0;
            float b2 = 0;
            float b3 = 0;
            float b4 = 0;
            float a1 = 0;
            float a2 = 0;
            float a3 = 0;
            float a4 = 0;

            float x1 = 0;
            float x2 = 0;
            float x3 = 0;
            float x4 = 0;
            float y1 = 0;
            float y2 = 0;
            float y3 = 0;
            float y4 = 0;

        };

        inline void initializeFilter(Filter& filter, int cfIndex, int rIndex, int type, int bwIndex = -1) const;
        inline void updateFilter(float& cf, float& r, int cfIndex, int rIndex, Ripples::Filter &filter) const;

        inline void calculateCoefficients(const float& FC, const float& R, Filter& filter) const;
        [[nodiscard]] static inline float sampleCalculation(const float& in, Filter& filter) ;
        [[nodiscard]] inline float FM(const float& in) const;

        inline void bpCoefficients(const float& FC, const float& R, Filter& filter) const;
        inline void lp2Coefficients(const float& FC, const float& R, Filter &filter) const;
        inline void lp4Coefficients(const float& FC, const float& R, Filter &filter) const;


        const float FS = static_cast<float>(sampleRate());
        const float trt = sqrt2_f * 2;

        Filter Bandpass = Filter();
        Filter Lowpass2 = Filter();
        Filter Lowpass4 = Filter();

        enum filterType {BP, LP2, LP4};
        enum inputs {IN, BP_CF, BP_R, BW_in, LP2_CF, LP2_R, LP4_CF,
                LP4_R, FM_FREQ, FM_MUL, FM_INDEX};
    };

} // namespace ModeledModules