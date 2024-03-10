// PluginRipples.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules {

    class Ripples : public SCUnit {
    public:
        Ripples();

        // Destructor
        // ~Ripples();

    private:
        // Calc function
        void next(int nSamples);

        // Member variables
    };

} // namespace ModeledModules