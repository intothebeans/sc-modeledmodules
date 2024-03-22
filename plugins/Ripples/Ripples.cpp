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
        Slopes lp2_slopes;
        Slopes lp4_slopes;
        Slopes bpf_slopes;

        if(LPF2.cf != in0(LP2_CF) || LPF2.r != in0(LP2_R)){
            LPF2.cf = in0(LP2_CF);
            LPF2.r = in0(LP2_R);
            Ripples::Coefficients newLPF2 = secondOrderLowpassCoefficients(LPF2);
            lp2_slopes = calcSlopes(LPF2, newLPF2, nSamples);
        }
        if(LPF4.cf != in0(LP4_CF) || LPF4.r != in0(LP4_R)){
            LPF4.cf = in0(LP4_CF);
            LPF4.r = in0(LP4_R);
            Ripples::Coefficients newLPF4 = fourthOrderLowpassCoefficients(LPF4);
            lp4_slopes = calcSlopes(LPF4, newLPF4, nSamples);
        }
        if(BPF.cf != in0(BP_CF) || BPF.r != in0(BP_R) || BPF.rq != in0(BP_RQ)){
            BPF.cf = in0(BP_CF);
            BPF.r = in0(BP_R);
            BPF.rq = in0(BP_RQ);
            Ripples::Coefficients newBPF = fourthOrderBandpassCoefficients(BPF);
            bpf_slopes = calcSlopes(BPF, newBPF, nSamples);
        }
        calcFourthOrder(BPF, bpf_slopes, nSamples, input, outbuf0);
        calcLP2(lp2_slopes, nSamples, input, outbuf1);
        calcFourthOrder(LPF4, lp4_slopes, nSamples, input, outbuf2);

    }

    inline void Ripples::calcLP2(const Slopes& slopes, int nSamples, const float *input, float *outbuf) {
        for (int i = 0; i  < nSamples; i++ ){
            float y0 = LPF2.coefficients.b0 * input[i] + LPF2.coefficients.b1 * LPF2.storedSamples.x1 + LPF2.coefficients.b2 * LPF2.storedSamples.x2 - LPF2.coefficients.a1 * LPF2.storedSamples.y1 - LPF2.coefficients.a2 * LPF2.storedSamples.y2;
            outbuf[i] = zapgremlins(y0);
            LPF2.storedSamples.x2 = LPF2.storedSamples.x1;
            LPF2.storedSamples.x1 = input[i];
            LPF2.storedSamples.y2 = LPF2.storedSamples.y1;
            LPF2.storedSamples.y1 = zapgremlins(y0);
            LPF2.coefficients.b0 += slopes.b0_slope;
            LPF2.coefficients.b1 += slopes.b1_slope;
            LPF2.coefficients.b2 += slopes.b2_slope;
            LPF2.coefficients.a1 += slopes.a1_slope;
            LPF2.coefficients.a2 += slopes.a2_slope;
        }
  }

    inline void Ripples::calcFourthOrder(Filter& filter, const Slopes& slopes, int nSamples, const float *input, float *outbuf){
        for (int i = 0; i < nSamples; i++){
            float y0 = filter.coefficients.b0 * input[i] + filter.coefficients.b1 * filter.storedSamples.x1 + filter.coefficients.b2 *
                    filter.storedSamples.x2 + filter.coefficients.b3 * filter.storedSamples.x3 + filter.coefficients.b4 * filter.storedSamples.x4
                    - filter.coefficients.a1 * filter.storedSamples.y1 - filter.coefficients.a2 * filter.storedSamples.y2 -
                    filter.coefficients.a3 * filter.storedSamples.y3 - filter.coefficients.a4 * filter.storedSamples.y4;
            y0 = zapgremlins(y0);
            outbuf[i] = y0;
            filter.storedSamples.x4 = filter.storedSamples.x3;
            filter.storedSamples.x3 = filter.storedSamples.x2;
            filter.storedSamples.x2 = filter.storedSamples.x1;
            filter.storedSamples.x1 = input[i];
            filter.storedSamples.y4 = filter.storedSamples.y3;
            filter.storedSamples.y3 = filter.storedSamples.y2;
            filter.storedSamples.y2 = filter.storedSamples.y1;
            filter.storedSamples.y1 = y0;

            filter.coefficients.b0 += slopes.b0_slope;
            filter.coefficients.b1 += slopes.b1_slope;
            filter.coefficients.b2 += slopes.b2_slope;
            filter.coefficients.b3 += slopes.b3_slope;
            filter.coefficients.b4 += slopes.b4_slope;
            filter.coefficients.a1 += slopes.a1_slope;
            filter.coefficients.a2 += slopes.a2_slope;
            filter.coefficients.a3 += slopes.a3_slope;
            filter.coefficients.a4 += slopes.a4_slope;
        }

    }
    inline Ripples::Slopes Ripples::calcSlopes(const Filter& filter, const Coefficients& newCoefficients, int nSamples) {
        Slopes slopes;
        slopes.b0_slope = (newCoefficients.b0 - filter.coefficients.b0) / nSamples;
        slopes.b1_slope = (newCoefficients.b1 - filter.coefficients.b1) / nSamples;
        slopes.b2_slope = (newCoefficients.b2 - filter.coefficients.b2) / nSamples;
        slopes.b3_slope = (newCoefficients.b3 - filter.coefficients.b3) / nSamples;
        slopes.b4_slope = (newCoefficients.b4 - filter.coefficients.b4) / nSamples;
        slopes.a1_slope = (newCoefficients.a1 - filter.coefficients.a1) / nSamples;
        slopes.a2_slope = (newCoefficients.a2 - filter.coefficients.a2) / nSamples;
        slopes.a3_slope = (newCoefficients.a3 - filter.coefficients.a3) / nSamples;
        slopes.a4_slope = (newCoefficients.a4 - filter.coefficients.a4) / nSamples;
        return slopes;
    }

    inline Ripples::Coefficients Ripples::secondOrderLowpassCoefficients(const Filter &filter) const {
        Ripples::Coefficients newCoefficients;
        float cf = filter.cf;
        float r = filter.r;

        float v = 0.5f * asinhf(1 / sc_sqrt(powf(10, r / 10) - 1));
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

    inline Ripples::Coefficients Ripples::fourthOrderLowpassCoefficients(const Filter &filter) const {
        Ripples::Coefficients newCoefficients;
        float cf = filter.cf;
        float r = filter.r;
        float v = 0.25f * asinhf(1 /(sc_sqrt(powf(10, r/10) - 1)));
        float k = sinhf(v);
        float l = coshf(v);
        float a = cosf(5 * pi_f / 8);
        float B = cosf(7*pi_f/8);
        float g = tanf(pi_f * cf/FS);

        float term_1 = ((powf(k, 4) + powf(l, 4))/8 + (3*sc_squared(k)*sc_squared(l))/4) * powf(g, 4);
        float term_2 = k * (trt* sc_squared(k)*(a + B) + 2*sc_squared(l)* sc_cubed(g)*(sc_cubed(a) + sc_cubed(B)));
        float term_3 = sc_squared(g)*((1 + sqrt2_f)*sc_squared(k) + sc_squared(l) );
        float term_4 = 2*k*(a+B)*g;
        float D = term_1 - term_2 + term_3 - term_4 + 1;

        newCoefficients.b0 = term_1 / D;
        newCoefficients.b1 = 4 * newCoefficients.b0;
        newCoefficients.b2 = 6 * newCoefficients.b0;
        newCoefficients.b3 = 4 * newCoefficients.b0;
        newCoefficients.b4 = newCoefficients.b0;

        newCoefficients.a1 = 4 * term_1 - 2*term_2 - 2 * term_4 - 4;
        newCoefficients.a2 = 6 * term_1 - 2 * term_3 + 6;
        newCoefficients.a3 = 4 * term_1 + 2 * term_2 - 2 * term_4 - 4;
        newCoefficients.a4 = term_1 + 2 * term_2 + term_3 + term_4 + 1;

        return newCoefficients;
    }

    Ripples::Coefficients Ripples::fourthOrderBandpassCoefficients(const Ripples::Filter &filter) const {
        Ripples::Coefficients newCoefficients;
        float cf = filter.cf;
        float r = filter.r;
        float bw = cf / (1 / filter.rq);
        float v = 0.5f * asinhf(1 / sc_sqrt(powf(10, r / 10) - 1));
        float k = sinhf(v);
        float l = coshf(v);
        float g = tanf(pi_f * cf / FS);

        float term_1 = 2 * sc_squared(cf) * (powf(g, 4) + 1);
        float term_2 = trt * k * bw * cf * g * (sc_squared(g) + 1);
        float term_3 = sc_squared(g) * (sc_squared(bw)*(sc_squared(k) + sc_squared(l)) + 4 * sc_squared(cf));
        float a_term_1 = 8 * sc_squared(cf) * (powf(g, 4) - 1);
        float a_term_2 = 2 * trt * k * bw * cf * g * (sc_squared(g) - 1);
        float D = term_1 + term_2 + term_3;

        newCoefficients.b0 = sc_squared(bw) * sc_squared(g) * (sc_squared(k) + sc_squared(l)) / D;
        newCoefficients.b2 = -2 * newCoefficients.b0;
        newCoefficients.b4 = newCoefficients.b0;

        newCoefficients.a1 =  (a_term_1 + a_term_2) / D;
        newCoefficients.a2 = (6 * term_1 - 2 * term_3) / D;
        newCoefficients.a3 = (a_term_1 - a_term_2) / D;
        newCoefficients.a4 = (term_1 - term_2 + term_3) / D;
        return newCoefficients;
    }


} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::Ripples>(ft, "Ripples", false);
}