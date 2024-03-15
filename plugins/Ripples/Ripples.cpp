// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "Ripples.hpp"
#include <cmath>

static InterfaceTable* ft;

namespace ModeledModules {

    Ripples::Ripples() {
        bp_cf = in0(BP_CF);
        bp_rq = in0(BP_RQ);
        lp1_cf = in0(LP1_CF);
        lp1_r = in0(LP1_RQ);
        lp2_cf = in0(LP2_CF);
        lp2_rq = in0(LP2_RQ);

        mCalcFunc = make_calc_function<Ripples, &Ripples::next>();
        next(1);
    }

    void Ripples::next(int nSamples) {
        const float* input = in(IN);
        float * bp_outbuf = out(0);
        float * lp1_outbuf = out(1);
        float * lp2_outbuf = out(2);


        if(bp_cf != in0(BP_CF) || bp_rq != in0(BP_RQ)){
            bp_cf = in0(BP_CF);
            bp_rq = in0(BP_RQ);
            calculateCoefficients_bp(bp_cf, bp_rq);
        }

        if(lp1_cf != in0(LP1_CF) || lp1_r != in0(LP1_RQ)){
            lp1_cf = in0(LP1_CF);
            lp1_r = in0(LP1_RQ);
            calculateCoefficients_lp1(lp1_cf, lp1_r);
        }

        if(lp2_cf != in0(LP2_CF) || lp2_rq != in0(LP2_RQ)){
            lp2_cf = in0(LP2_CF);
            lp2_rq = in0(LP2_RQ);
            calculateCoefficients_lp2(lp2_cf, lp2_rq);
        }


        for (int i = 0; i < nSamples; ++i) {
            float in = input[i];

            // bp

            // lp1
            float lp1_out = 1 + lp1_a1 * lp1_x1 + lp1_a2 * lp1_x2 +
                    lp1_b0 * in + lp1_b1 * lp1_y1 + lp1_b2 * lp1_y2;
            lp1_x2 = lp1_x1;
            lp1_x1 = in;
            lp1_y2 = lp1_y1;
            lp1_y1 = lp1_out;

            lp1_outbuf[i] = FM(lp1_out);

            // lp2
        }
    }

    inline void Ripples::calculateCoefficients_lp1(float FC, float R){
        auto v = static_cast<float>(0.5 * asinh(1 / (sc_sqrt(pow(10, (R/10))))-1));
        float k = sinh(v);
        float k2 = k * k;
        float l = cosh(v);
        float l2 = l * l;
        float y = tanf((pi_f * FC) / FS);
        float y2 = y * y;
        float D = (k2 + l2) * y2 + 2 * sqrt2_f*k*y + 2;

        lp1_b0 = (k2 + l2) * y2 / D;
        lp1_b1 = 2 * lp1_b0;
        lp1_b2 = lp1_b0;

        lp1_a1 = 2 *(k2 + l2) * y2 - 4/ D;
        lp1_a2 = (k2 + l2) * y2 - 2 * sqrt2_f * k * y + 2 / D;
    }

    inline float Ripples::FM(float in){
        float fm_index = in0(FM_INDEX);
        float fm_mul = in0(FM_MUL);
        float fm_freq = in0(FM_FREQ);

        return sinf(in * twopi_f + (fm_index *
        (fm_mul * sinf(fm_freq * twopi_f))));
    }


} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::Ripples>(ft, "Ripples", false);
}