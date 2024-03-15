// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include <stdexcept>
#include "SC_PlugIn.hpp"
#include "RipplesLP2.hpp"

static InterfaceTable* ft;

namespace ModeledModules {

    RipplesLP2::RipplesLP2() {
        freq = in0(FREQ);
        rq = in0(RQ);
        calculateCoefficients(freq, 1/rq);
        mCalcFunc = make_calc_function<RipplesLP2, &RipplesLP2::next>();
        next(1);

    }

    void RipplesLP2::next(int nSamples) {
        // Audio rate input
        const float* input = in(IN);
        // Output buffer
        float* outbuf = out(0);

        if (freq != in0(FREQ) || rq != in0(RQ)) {
            freq = in0(FREQ);
            rq = in0(RQ);
            calculateCoefficients(freq, 1/rq);
        }

        for (int i = 0; i < nSamples; ++i) {
            float out = 1 + a1*x1 + a2*x2 + b0*input[i] + b1*y1 + b2*y2;
            x2 = x1;
            x1 = input[i];

            y2 = y1;
            y1 = out;

            outbuf[i] = out;
        }
    }

    inline void RipplesLP2::calculateCoefficients(float FC, float Q){
        float y = tanf((pi_f * FC)/FS);
        float D = Q * sc_squared(y) + y + Q;
        b0 = (Q * sc_squared(y))/D;
        b1 = (2 * (Q * sc_squared(y)))/D;
        b2 = b0;
        a1 = (2 * Q *(sc_squared(y) - 1))/D;
        a2 = (Q * sc_squared(y) - y + Q)/D;
    }

} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::RipplesLP2>(ft, "RipplesLP2", false);
}