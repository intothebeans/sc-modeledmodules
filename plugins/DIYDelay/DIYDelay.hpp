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
        float saturation(float sample) const;

        float maxdelay;
        int bufsize;
        int mask;
        float *buf;
        int writephase = 0;

        const float sampleRateF = static_cast<float>(sampleRate());

        enum inputs
        {
            INPUT,
            DELAY_TIME,
            FEEDBACK,
            TAPE,
            ADD,
            FREEZE,
            REVERSE,
            MAX_DELAY
        };
    };

}