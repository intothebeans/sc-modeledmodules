// PluginBooleanLogic.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "BooleanLogic.hpp"

static InterfaceTable* ft;

namespace ModeledModules {

BooleanLogic::BooleanLogic() {
    mCalcFunc = make_calc_function<BooleanLogic, &BooleanLogic::next>();
    next(1);
}

void BooleanLogic::next(int nSamples) {

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

PluginLoad(BooleanLogicUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::BooleanLogic>(ft, "BooleanLogic", false);
}
