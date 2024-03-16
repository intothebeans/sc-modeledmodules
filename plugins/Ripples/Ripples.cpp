// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "Ripples.hpp"
#include <cmath>
#include <stdexcept>

static InterfaceTable* ft;

namespace ModeledModules {

    Ripples::Ripples() {
        bp_cf = in0(BP_CF);
        bp_rq = in0(BP_RQ);
        lp1_cf = in0(LP1_CF);
        lp1_r = in0(LP1_RQ);
        lp2_cf = in0(LP2_CF);
        lp2_rq = in0(LP2_RQ);
        if(bp_cf != 0){
            calculateCoefficients_bp(bp_cf, bp_rq);
        }
        else if(lp1_cf != 0){
            calculateCoefficients_lp1(lp1_cf, lp1_r);
        }
        else if(lp2_cf != 0){
            calculateCoefficients_lp2(lp2_cf, lp2_rq);
        }else{
            throw std::invalid_argument("No filter type specified");
        }

        mCalcFunc = make_calc_function<Ripples, &Ripples::next>();
        next(1);
    }

    void Ripples::next(int nSamples) {
        const float* input = in(IN);
        float * bp_outbuf = out(0);
        float * lp1_outbuf = out(1);
        float * lp2_outbuf = out(2);


        if(bp_cf != in0(BP_CF) || bp_rq != in0(BP_RQ) && bp_cf != 0){
            bp_cf = in0(BP_CF);
            bp_rq = in0(BP_RQ);
            calculateCoefficients_bp(bp_cf, bp_rq);
        }

        if(lp1_cf != in0(LP1_CF) || lp1_r != in0(LP1_RQ) && lp1_cf != 0){
            lp1_cf = in0(LP1_CF);
            lp1_r = in0(LP1_RQ);
            calculateCoefficients_lp1(lp1_cf, lp1_r);
        }

        if(lp2_cf != in0(LP2_CF) || lp2_rq != in0(LP2_RQ) && lp2_cf != 0){
            lp2_cf = in0(LP2_CF);
            lp2_rq = in0(LP2_RQ);
            calculateCoefficients_lp2( lp2_cf, lp2_rq);
        }


        for (int i = 0; i < nSamples; ++i) {
            float in = input[i];
            // bp
            if (bp_cf != 0) {
                float bp_out = bp_b0 * in + bp_b1 * bp_x1 + bp_b2 * bp_x2 + bp_b3 * bp_x3 + bp_b4 * bp_x4
                               - bp_a1 * bp_y1 - bp_a2 * bp_y2 - bp_a3 * bp_y3 - bp_a4 * bp_y4;
                bp_x4 = bp_x3;
                bp_x3 = bp_x2;
                bp_x2 = bp_x1;
                bp_x1 = in;

                bp_y4 = bp_y3;
                bp_y3 = bp_y2;
                bp_y2 = bp_y1;
                bp_y1 = bp_out;

                bp_outbuf[i] = FM(bp_out);
            } else {
                bp_outbuf[i] = 0;
            }

            // lp1
            if(lp1_cf != 0){
                float lp1_out = lp1_b0 * in + lp1_b1 * lp1_x1 + lp1_b2 * lp1_x2
                        - lp1_a1 * lp1_y1 - lp1_a2 * lp1_y2;
                lp1_x2 = lp1_x1;
                lp1_x1 = in;
                lp1_y2 = lp1_y1;
                lp1_y1 = lp1_out;

                lp1_outbuf[i] = FM(lp1_out);
            }else{
                lp1_outbuf[i] = 0;
            }

            // lp2
            if (lp2_cf != 0) {
                float lp2_out = lp2_b0 * in + lp2_b1 * lp2_x1 + lp2_b2 * lp2_x2 + lp2_b3 * lp2_x3 + lp2_b4 * lp2_x4
                               - lp2_a1 * lp2_y1 - lp2_a2 * lp2_y2 - lp2_a3 * lp2_y3 - lp2_a4 * lp2_y4;
                lp2_x4 = lp2_x3;
                lp2_x3 = lp2_x2;
                lp2_x2 = lp2_x1;
                lp2_x1 = in;

                lp2_y4 = lp2_y3;
                lp2_y3 = lp2_y2;
                lp2_y2 = lp2_y1;
                lp2_y1 = lp2_out;

                lp2_outbuf[i] = FM(lp2_out);
            } else {
                lp2_outbuf[i] = 0;
            }
        }
    }

    inline void Ripples::calculateCoefficients_bp(const float& FC, const float& R) {
        auto v = static_cast<float>(0.5 * asinh(1 / sc_sqrt(pow(10, R/10) - 1)));
        float k = sinh(v);
        float k2 = sc_squared(k);
        float l = cosh(v);
        float l2 = sc_squared(l);
        float g = tanf((pi_f * FC) / FS);
        float g2 = sc_squared(g);
        float g2p = g2 + 1;
        float g2m = g2 - 1;
        float g4 = sc_squared(g2);
        float BW = in0(BW_in);
        float BW2 = sc_squared(BW);
        float FC2 = sc_squared(FC);

        float term_a = 2*FC2*(g4+1);
        float term_b = trt*k*BW*FC*g;
        float term_c = BW2*(k2+l2);
        float term_d = g2*(BW2*(k2 + l2) + 4*FC2);
        float term_e = 8*FC2*(g4 - 1);

        float D = term_a + term_b*g2p + term_d;

        bp_b0 = term_c * g2 / D;
        bp_b2 = -2 * bp_b0;
        bp_b4 = bp_b0;

        bp_a1 = (term_e + 2*term_b*g2m) / D;
        bp_a2 = (6*term_a - 2*term_d) / D;
        bp_a3 = (term_e - 2*term_b*g2m) / D;
        bp_a4 = (term_a - term_b*g2p + term_d) / D;
    }

    inline void Ripples::calculateCoefficients_lp1(const float& FC, const float& R){
        auto v = static_cast<float>(0.5 * asinh(1 / (sc_sqrt(pow(10, (R/10))))-1));
        float k = sinh(v);
        float k2 = sc_squared(k);
        float l = cosh(v);
        float l2 = sc_squared(l);
        float g = tanf((pi_f * FC) / FS);
        float g2 = sc_squared(g);
        float D = (k2 + l2) * g2 + trt * k * g + 2;

        lp1_b0 = (k2 + l2) * g2 / D;
        lp1_b1 = 2*lp1_b0;
        lp1_b2 = lp1_b0;

        lp1_a1 = 2 * (k2 + l2) * g2 - 4 / D;
        lp1_a2 = (k2 + l2) * g2 - trt * k * g + 2 / D;
    }

    inline void Ripples::calculateCoefficients_lp2(const float& FC, const float& R) {
         auto v = static_cast<float>(0.5 * asinh(1 / (sc_sqrt(pow(10, (R/10))))-1));
         float k = sinh(v);
         float k2 = sc_squared(k);
         float k4 = sc_squared(k2);
         float l = cosh(v);
         float l2 = sc_squared(l);
         float l4 = sc_squared(l2);
         float a = cosf((pi_f * 5)/8);
         float B = cosf((7*pi_f)/8);
         float g = tanf((pi_f * FC) / FS);
         float g2 = sc_squared(g);
         float g4 = sc_squared(g2);
         float aB = a + B;
         float kaBg = k*aB*g;

         float term_a = (g4 * (k4 + l4) / 8) + (g4*3*k2*l2/4);
         float term_a4 = 4 * term_a;
         float term_a6 = 6 * term_a;
         float term_b = k * (trt * k2 * aB + 2*l2*(sc_cubed(a) + sc_cubed(B))) * sc_cubed(g);
         float term_c = g2*((1 + sqrt2_f)*k2 + l2);
         float term_d = 2*kaBg + 1;
         float term_e = 4*kaBg - 4;
         float term_f = 2 * term_b;
         float D = term_a - term_b + term_c - term_d;

         lp2_b0 = term_a / D;
         lp2_b1 = 4*lp2_b0;
         lp2_b2 = 6*lp2_b0;
         lp2_b3 = lp2_b1;
         lp2_b4 = lp2_b0;

         lp2_a1 = (term_a4 - term_f + term_e)/D;
         lp2_a2 = (term_a6 - 2 * term_c + 6)/D;
         lp2_a3 = -1 * lp2_a1;
         lp2_a4 = (term_a + term_b + term_c + term_d) / D;

    }

     inline float Ripples::FM(const float& in) const{
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