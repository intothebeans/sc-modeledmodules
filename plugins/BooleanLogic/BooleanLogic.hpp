// PluginBooleanLogic.hpp
// Aiden Benton (dev@aiden-benton.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace ModeledModules {

    class BooleanLogic : public SCUnit {
        public:
            BooleanLogic();

        private:
            // Calc function
            void next(int nSamples);

            enum Operation {AND, OR, XOR, NAND, NOR, XNOR};
            enum Inputs {Input1, Input2, Operation};
            // Member variables
            int mSelectedOperation;
        };

} // namespace ModeledModules
