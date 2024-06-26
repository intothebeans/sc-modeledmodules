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

        auto hold = static_cast<bool>(in0(HOLD));
        auto reverse = static_cast<bool>(in0(REVERSE));
        auto tape = static_cast<bool>(in0(TAPE));

        // local stateless variables
        float const *buf = m_buf;
        int mask = m_mask;
        int write = m_readIndex;
        int read = (m_reverseIndex == 0) ? m_bufSize - 1 : m_reverseIndex;

        if (delay > m_maxDelay)
        {
            delay = m_maxDelay;
        }

        // initialize delay time
        float delay_samples = sampleRate() * delay;
        auto offset = static_cast<int>(delay_samples);
        float frac = delay_samples - (float)offset;

        for (int i = 0; i < nSamples; ++i)
        {

            int phase1 = reverse ? read - offset : write - offset;
            int phase2 = phase1 - 1;
            int phase3 = phase1 - 2;
            int phase0 = phase1 + 1;
            float d0 = buf[phase0 & mask];
            float d1 = buf[phase1 & mask];
            float d2 = buf[phase2 & mask];
            float d3 = buf[phase3 & mask];

            float delayed = cubicinterp(frac, d0, d1, d2, d3);

            float outSample = zapgremlins(input[i] + (fb * delayed));

            if (tape)
            {
                outSample = saturation(outSample);
            }

            outbuf[i] = outSample;
            if (!hold)
            {
                m_buf[write] = outSample;
            }

            write = (write + 1) & mask;
            read = (read - 1) & mask;
        }
        m_readIndex = write;
        m_reverseIndex = read;
    }

} // namespace ModeledModules

PluginLoad(DIYDelayUGens)
{
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::DIYDelay>(ft, "DIYDelay", false);
}
