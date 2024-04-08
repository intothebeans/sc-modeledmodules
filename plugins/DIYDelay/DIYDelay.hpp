// Plugin DIYDelay.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules {

class DIYDelay : public SCUnit {
public:
DIYDelay();

// Destructor
// ~DIYDelay();

private:
// Calc function
void next(int nSamples);

// Member variables
};

} // namespace ModeledModules