// PluginRipples.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules {

    class RipplesLP2 : public SCUnit {
    public:
        RipplesLP2();
    private:
        void next(int nSamples);
        inline void calculateCoefficients(float FC, float Q);
        // Member variables
        const float FS = sampleRate();
        float freq;
        float rq;
        enum inputs {IN, FREQ, RQ};
        // coefficients
        float b0 = 0;
        float b1 = 0;
        float b2 = 0;
        float a1 = 0;
        float a2 = 0;
        // stored samples
        float x1 = 0;
        float x2 = 0;
        float y1 = 0 ;
        float y2 = 0;
    };

} // namespace ModeledModules