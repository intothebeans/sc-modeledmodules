// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "Ripples.hpp"
#include "../../external/iir1/Iir.h"

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

        float width = in0(BP_RQ) * in0(BP_CF);

        if(in0(LP2_CF) != 0){
            LPF2.setup(sampleRate(), in0(LP2_CF), in0(LP2_R));
        }
        if(in0(LP4_CF) != 0){
            LPF4.setup(sampleRate(), in0(LP4_CF), in0(LP4_R));
        }
        BPF.setup(sampleRate(), in0(BP_CF), width, in0(BP_R));

        for(int i = 0; i < nSamples; ++i){
            outbuf0[i] = zapgremlins(frequencyModulation(BPF.filter(input[i])));
            outbuf1[i] = zapgremlins(frequencyModulation(LPF2.filter(input[i])));
            outbuf2[i] = zapgremlins(frequencyModulation(LPF4.filter(input[i])));
        }

    }

    float Ripples::frequencyModulation(float s) const{
        return in0(FM_MUL) * sinf(twopi_f * 440.f * (s / (float) sampleRate()) + in0(FM_INDEX)*sinf(twopi_f * in0(FM_FREQ) * (s / (float) sampleRate())));
    }


} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::Ripples>(ft, "Ripples", false);
}