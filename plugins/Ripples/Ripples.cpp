// PluginRipples.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "Ripples.hpp"

static InterfaceTable* ft;

namespace ModeledModules {

    Ripples::Ripples() {
        mCalcFunc = make_calc_function<Ripples, &Ripples::next>();
        next(1);
    }

    void Ripples::next(int nSamples) {

        // Audio rate input
        const float* input = in(0);

        // Control rate parameter: gain.
        const float gain = in0(1);

        // Output buffer
        float* outbuf = out(0);

        // simple gain function
        for (int i = 0; i < nSamples; ++i) {
            outbuf[i] = input[i] * gain;
        }
    }

} // namespace ModeledModules

PluginLoad(RipplesUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::Ripples>(ft, "Ripples", false);
}