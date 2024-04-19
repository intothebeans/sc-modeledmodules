// PluginDIYDelay.hpp
// Aiden Benton (intothebeans@site.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules
{

    class DIYDelay : public SCUnit
    {
    public:
        DIYDelay();
        ~DIYDelay();

    private:
        // Calc function
        void next(int nSamples);
        static float saturation(float sample) ;

        float m_maxDelay;
        int m_bufSize;
        int m_mask;
        float *m_buf;
        int m_writeIndex = 0;
        int m_readIndex = 0;

        const float sampleRateF = static_cast<float>(sampleRate());

        enum inputs
        {
            INPUT,
            DELAY_TIME,
            FEEDBACK,
            TAPE,
            FREEZE,
            REVERSE,
            MAX_DELAY
        };
    };

}