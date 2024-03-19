// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "Ripples.hpp"
#include <cmath>
#include <stdexcept>

static InterfaceTable* ft;

namespace ModeledModules {
    Ripples::Ripples() {
        mCalcFunc = make_calc_function<Ripples, &Ripples::next>();
        next(1);
    }



    void Ripples::next(int nSamples) {
        const float *input = in(INPUT);
        float* outbuf0 = out(0);
        float* outbuf1 = out(1);
        float* outbuf2 = out(2);
        float b0_slope = 0;
        float b1_slope = 0;
        float b2_slope = 0;
        float a1_slope = 0;
        float a2_slope = 0;
        if(LPF2.cf != in0(LP2_CF) || LPF2.r != in0(LP2_R)) {
            LPF2.cf = in0(LP2_CF);
            LPF2.r = in0(LP2_R);
            Ripples::Coefficients newCoefficients = secondOrderLowpassCoefficients(LPF2);
            b0_slope = (newCoefficients.b0 - LPF2.coefficients.b0) / nSamples;
            b1_slope = (newCoefficients.b1 - LPF2.coefficients.b1) / nSamples;
            b2_slope = (newCoefficients.b2 - LPF2.coefficients.b2) / nSamples;
            a1_slope = (newCoefficients.a1 - LPF2.coefficients.a1) / nSamples;
            a2_slope = (newCoefficients.a2 - LPF2.coefficients.a2) / nSamples;

        }
        for (int i = 0; i  < nSamples; i++ ){
            float y0 = LPF2.coefficients.b0 * input[i] + LPF2.coefficients.b1 * LPF2.storedSamples.x1 + LPF2.coefficients.b2 * LPF2.storedSamples.x2 - LPF2.coefficients.a1 * LPF2.storedSamples.y1 - LPF2.coefficients.a2 * LPF2.storedSamples.y2;
            outbuf1[i] = zapgremlins(y0);
            LPF2.storedSamples.x2 = LPF2.storedSamples.x1;
            LPF2.storedSamples.x1 = input[i];
            LPF2.storedSamples.y2 = zapgremlins(LPF2.storedSamples.y1);
            LPF2.storedSamples.y1 = zapgremlins(y0);
            LPF2.coefficients.b0 += b0_slope;
            LPF2.coefficients.b1 += b1_slope;
            LPF2.coefficients.b2 += b2_slope;
            LPF2.coefficients.a1 += a1_slope;
            LPF2.coefficients.a2 += a2_slope;

        }


    }

    inline Ripples::Coefficients Ripples::secondOrderLowpassCoefficients(Filter &filter) const {
        Ripples::Coefficients newCoefficients;
        float cf = filter.cf;
        float r = filter.r;

        auto v = static_cast<float>(0.5 * asinh(1 / sc_sqrt(pow(10, r / 10) - 1)));
        float g = tanf((pi_f * cf) / FS);
        float k = sinh(v);
        float l = cosh(v);
        float term_1 = (sc_squared(k) + sc_squared(l)) * sc_squared(g);
        float term_2 = trt * k * g + 2;
        float D = term_1 + term_2;

        newCoefficients.b0 = term_1 / D;
        newCoefficients.b1 = 2 * newCoefficients.b0;
        newCoefficients.b2 = newCoefficients.b0;

        newCoefficients.a1 = 2 * term_1 - 4;
        newCoefficients.a2 = term_1 - term_2 + 2;
        return newCoefficients;
    }


} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::Ripples>(ft, "Ripples", false);
}