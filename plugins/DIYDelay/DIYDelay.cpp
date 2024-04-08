// Plugin DIYDelay.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "DIYDelay.hpp"

static InterfaceTable* ft;

namespace ModeledModules {

DIYDelay::DIYDelay() {
mCalcFunc = make_calc_function<DIYDelay, &DIYDelay::next>();
next(1);
}

void DIYDelay::next(int nSamples) {

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

} // namespace {{ cookiecutter.project_namespace }}

PluginLoad(DIYDelayUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<{{ cookiecutter.project_namespace }}::DIYDelay>(ft, "DIYDelay", false);
}