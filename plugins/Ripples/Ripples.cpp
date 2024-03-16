// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "Ripples.hpp"
#include <cmath>
#include <stdexcept>

static InterfaceTable* ft;

namespace ModeledModules {

    Ripples::Ripples() {
        initializeFilter(Bandpass, BP_CF, BP_R, BP, BW_in);
        initializeFilter(Lowpass2, LP2_CF, LP2_R, LP2);
        initializeFilter(Lowpass4, LP4_CF, LP4_R, LP4);

        mCalcFunc = make_calc_function<Ripples, &Ripples::next>();
        next(1);
    }

    void Ripples::next(int nSamples) {
        const float* input = in(IN);
        float * bp_outbuf = out(0);
        float * lp2_outbuf = out(1);
        float * lp4_outbuf = out(2);

        updateFilter(Bandpass.cf, Bandpass.r, BP_CF, BP_R, Bandpass);
        updateFilter(Lowpass2.cf, Lowpass2.r, LP2_CF, LP2_R, Lowpass2);
        updateFilter(Lowpass4.cf, Lowpass4.r, LP4_CF, LP4_R, Lowpass4);

        for (int i = 0; i < nSamples; ++i) {
            float in = input[i];
            bp_outbuf[i] = FM(sampleCalculation(in, Bandpass));
            lp2_outbuf[i] = FM(sampleCalculation(in, Lowpass2));
            lp4_outbuf[i] = FM(sampleCalculation(in, Lowpass4));

        }
    }

    void Ripples::initializeFilter(Ripples::Filter &filter, int cfIndex, int rIndex, int type, int bwIndex) const {
        filter.cf = in0(cfIndex);
        filter.r = in0(rIndex);
        filter.type = type;
        if (bwIndex != -1) {
            filter.bw = in0(bwIndex);
        }
        updateFilter(filter.cf, filter.r, cfIndex, rIndex, filter);
    }
    void Ripples::updateFilter(float &cf, float &r, int cfIndex, int rIndex, Ripples::Filter &filter) const {
        if (cf != in0(cfIndex) || r != in0(rIndex) && cf != 0) {
            cf = in0(cfIndex);
            r = in0(rIndex);
            calculateCoefficients(cf, r, filter);
        }
    }
    inline float Ripples::FM(const float& in) const{
        float fm_index = in0(FM_INDEX);
        float fm_mul = in0(FM_MUL);
        float fm_freq = in0(FM_FREQ);

        return zapgremlins(sinf(in * twopi_f + (fm_index *
        (fm_mul * sinf(fm_freq * twopi_f)))));
    }

    inline void Ripples::calculateCoefficients(const float &FC, const float &R, ModeledModules::Ripples::Filter &filter) const {
        switch (filter.type) {
            case BP:
                bpCoefficients(FC, R, filter);
                break;
            case LP2:
                lp2Coefficients(FC, R, filter);
                break;
            case LP4:
                lp4Coefficients(FC, R, filter);
                break;
            default:
                throw std::invalid_argument("Invalid filter type");
        }
    }
    inline float Ripples::sampleCalculation(const float& in, Ripples::Filter &filter) {
        float out = filter.b0 * in + filter.b1 * filter.x1 + filter.b2 * filter.x2 + filter.b3 * filter.x3 + filter.b4 * filter.x4
                   - filter.a1 * filter.y1 - filter.a2 * filter.y2 - filter.a3 * filter.y3 - filter.a4 * filter.y4;
        out = zapgremlins(out);
        filter.x4 = filter.x3;
        filter.x3 = filter.x2;
        filter.x2 = filter.x1;
        filter.x1 = in;

        filter.y4 = filter.y3;
        filter.y3 = filter.y2;
        filter.y2 = filter.y1;
        filter.y1 = out;

        return out;
    }

    inline void Ripples::bpCoefficients(const float &FC, const float &R, ModeledModules::Ripples::Filter &filter) const {
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
        float BW = filter.bw;
        float BW2 = sc_squared(BW);
        float FC2 = sc_squared(FC);

        float term_a = 2*FC2*(g4+1);
        float term_b = trt*k*BW*FC*g;
        float term_c = BW2*(k2+l2);
        float term_d = g2*(BW2*(k2 + l2) + 4*FC2);
        float term_e = 8*FC2*(g4 - 1);
        float term_f = 2*term_b*g2m;
        float term_g = term_b*g2p;


        float D = term_a + term_g + term_d;

        filter.b0 = term_c * g2 / D;
        filter.b2 = -2 * filter.b0;
        filter.b4 = filter.b0;

        filter.a1 = (term_e + term_f) / D;
        filter.a2 = (6*term_a - 2*term_d) / D;
        filter.a3 = (term_e - term_f) / D;
        filter.a4 = (term_a - term_g + term_d) / D;
    }
    inline void Ripples::lp2Coefficients(const float &FC, const float &R, ModeledModules::Ripples::Filter &filter) const{
        auto v = static_cast<float>(0.5 * asinh(1 / (sc_sqrt(pow(10, (R/10))))-1));
        float k = sinh(v);
        float k2 = sc_squared(k);
        float l = cosh(v);
        float l2 = sc_squared(l);
        float g = tanf((pi_f * FC) / FS);
        float g2 = sc_squared(g);

        float term_a = (k2 + l2) * g2;
        float term_b = trt * k * g + 2;

        float D = term_a + term_b;

        filter.b0 = term_a / D;
        filter.b1 = 2*filter.b0;
        filter.b2 = filter.b0;

        filter.a1 = 2 * term_a - 4 / D;
        filter.a2 = term_a - term_b / D;
    }
    inline void Ripples::lp4Coefficients(const float &FC, const float &R, ModeledModules::Ripples::Filter &filter) const {
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

        filter.b0 = term_a / D;
        filter.b1 = 4*filter.b0;
        filter.b2 = 6*filter.b0;
        filter.b3 = filter.b1;
        filter.b4 = filter.b0;

        filter.a1 = (term_a4 - term_f + term_e)/D;
        filter.a2 = (term_a6 - 2 * term_c + 6)/D;
        filter.a3 = -1 * filter.a1;
        filter.a4 = (term_a + term_b + term_c + term_d) / D;
    }

} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::Ripples>(ft, "Ripples", false);
}