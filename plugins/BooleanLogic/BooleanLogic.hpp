// PluginBooleanLogic.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules {

class BooleanLogic : public SCUnit {
public:
    BooleanLogic();

    // Destructor
    // ~BooleanLogic();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace ModeledModules
