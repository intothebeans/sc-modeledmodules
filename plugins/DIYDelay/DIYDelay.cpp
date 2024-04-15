// PluginDIYDelay.cpp
// Aiden Benton (dev@aiden-cloud.space)

#include "SC_PlugIn.hpp"
#include "DIYDelay.hpp"

static InterfaceTable *ft;

namespace ModeledModules
{

    DIYDelay::DIYDelay()
    {
        mCalcFunc = make_calc_function<DIYDelay, &DIYDelay::next>();
        maxdelay = in0(MAX_DELAY);
        bufsize = NEXTPOWEROFTWO(sampleRateF * maxdelay);
        mask = bufsize - 1;

        buf = (float *)RTAlloc(this->mWorld, bufsize * sizeof(float));

        if (buf == nullptr)
        {
            mCalcFunc = make_calc_function<DIYDelay, nullptr>();
            ClearUnitOutputs(this, 1);

            if (this->mWorld->mVerbosity > -2)
            {
                Print("DIYDelay: failed to allocate memory for buffer\n");
            }
            return;
        }
        memset(buf, 0, bufsize * sizeof(float));

        next(1);
    }

    DIYDelay::~DIYDelay()
    {
        RTFree(this->mWorld, buf);
    }

    float DIYDelay::saturation(float sample) const
    {
        float clipped = tanhf(sample * 20);
        return sample + (clipped - sample) * 0.5f;
    }

    void DIYDelay::next(int nSamples)
    {
        // from https://github.com/supercollider/example-plugins/blob/main/03-AnalogEcho/AnalogEcho.cpp
        const float *input = in(INPUT);
        float *outbuf = out(0);
        float delay = in0(DELAY_TIME);
        float fb = in0(FEEDBACK);

        float add = in0(ADD);
        float freeze = in0(FREEZE);
        float reverse = in0(REVERSE);
        float tape = in0(TAPE);

        if (delay > maxdelay)
        {
            delay = maxdelay;
        }

        float delay_samples = sampleRateF * delay;
        float offset = delay_samples;
        float frac = delay_samples - offset;

        for (int i = 0; i < nSamples; ++i)
        {
            auto phase1 = static_cast<int>(writephase - offset);
            int phase2 = phase1 - 1;
            int phase3 = phase1 - 2;
            int phase0 = phase1 + 1;
            float d0 = buf[phase0 & mask];
            float d1 = buf[phase1 & mask];
            float d2 = buf[phase2 & mask];
            float d3 = buf[phase3 & mask];

            float delayed = cubicinterp(frac, d0, d1, d2, d3);

            if (tape == 1)
            {
                delayed = saturation(delayed);
            }

            outbuf[i] = zapgremlins(input[i] + fb * delayed);
            buf[writephase] = outbuf[i];

            writephase = (writephase + 1) & mask;
        }
    }

} // namespace ModeledModules

PluginLoad(DIYDelayUGens)
{
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::DIYDelay>(ft, "DIYDelay", false);
}
