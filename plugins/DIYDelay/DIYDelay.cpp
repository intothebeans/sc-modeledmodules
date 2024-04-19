// PluginDIYDelay.cpp
// Aiden Benton (dev@aiden-cloud.space)

#include "SC_PlugIn.hpp"
#include "DIYDelay.hpp"
#include <iostream>

static InterfaceTable *ft;

namespace ModeledModules
{

    DIYDelay::DIYDelay()
    {
        mCalcFunc = make_calc_function<DIYDelay, &DIYDelay::next>();
        m_maxDelay = in0(MAX_DELAY);
        m_bufSize = NEXTPOWEROFTWO(sampleRate() * m_maxDelay);
        m_mask = m_bufSize - 1;
        m_buf = (float *)RTAlloc(mWorld, m_bufSize * sizeof(float));
        if (m_buf == nullptr)
        {
            ClearUnitOutputs(this, 1);
            if (mWorld->mVerbosity > -2)
            {
                Print("DIYDelay: failed to allocate memory for buffer\n");
            }
            return;
        }
        memset(m_buf, 0, m_bufSize * sizeof(float));

        next(1);
    }

    DIYDelay::~DIYDelay()
    {
        RTFree(mWorld, m_buf);
    }

    float DIYDelay::saturation(float sample)
    {
        float scaled = sample * 0.5f;
        float saturated = tanhf(scaled * 0.5f);
        return saturated;
    }

    void DIYDelay::next(int nSamples)
    {
        // from https://github.com/supercollider/example-plugins/blob/main/03-AnalogEcho/AnalogEcho.cpp
        // Inputs from SC
        const float *input = in(INPUT);
        float *outbuf = out(0);
        float delay = in0(DELAY_TIME);
        float fb = in0(FEEDBACK);

        float freeze = in0(FREEZE);
        auto reverse = static_cast<bool>(in0(REVERSE));
        float tape = in0(TAPE);


        // local stateless variables
        float const *buf = m_buf;
        int mask = m_mask;
        int write = m_writeIndex;
        int read = (m_readIndex == 0) ? m_bufSize - 1 : m_readIndex;

        if (delay > m_maxDelay)
        {
            Print("Delay set to %f, but max delay is %f\n. Setting delay to %f.", delay, m_maxDelay, m_maxDelay);
            delay = m_maxDelay;
        }

        // initialize delay time
        float delay_samples = sampleRateF * delay;
        auto offset = static_cast<int>(delay_samples);
        float frac = delay_samples - (float)offset;

        float delayed;
        int phase1;
        int phase2;
        int phase3;
        int phase0;
        float d0;
        float d1;
        float d2;
        float d3;

        for (int i = 0; i < nSamples; ++i)
        {
            phase1 = reverse ? read - offset : write - offset;
            phase2 = phase1 - 1;
            phase3 = phase1 - 2;
            phase0 = phase1 + 1;
            d0 = buf[phase0 & mask];
            d1 = buf[phase1 & mask];
            d2 = buf[phase2 & mask];
            d3 = buf[phase3 & mask];

            delayed = cubicinterp(frac, d0, d1, d2, d3);

            float outSample = zapgremlins(input[i] + (fb * delayed));

            if (tape == 1)
            {
                outSample = saturation(outSample);
            }

            outbuf[i] = outSample;
            m_buf[write] = outSample; //feedback

            write = (write + 1) & mask;
            read = (read - 1) & mask;
        }
        m_writeIndex = write;
        m_readIndex = read;
    }

} // namespace ModeledModules

PluginLoad(DIYDelayUGens)
{
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::DIYDelay>(ft, "DIYDelay", false);
}
