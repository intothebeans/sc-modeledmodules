// PluginBooleanLogic.cpp
// Aiden Benton (dev@aiden-benton.com)

#include "SC_PlugIn.hpp"
#include "BooleanLogic.hpp"

static InterfaceTable* ft;

namespace ModeledModules {

    BooleanLogic::BooleanLogic() {
        mCalcFunc = make_calc_function<BooleanLogic, &BooleanLogic::next>();
        next(1);
        mSelectedOperation = in0(Operation);
    }

    void BooleanLogic::next(int nSamples) {
        // Inputs
        float input1 = in0(Input1);
        float input2 = in0(Input2);


        switch (mSelectedOperation) {
            case AND:
                if(std::abs(input1) > 0 && std::abs(input2) > 0) {
                    out0(0) = 1.0;
                } else {
                    out0(0) = 0.0;
                }
                break;
            case OR:
                if(std::abs(input1) > 0 || std::abs(input2) > 0) {
                    out0(0) = 1.0;
                } else {
                    out0(0) = 0.0;
                }
                break;
            case XOR:
                if((std::abs(input1) > 0 && std::abs(input2) == 0) || (std::abs(input1) == 0 && std::abs(input2) > 0)) {
                    out0(0) = 1.0;
                } else {
                    out0(0) = 0.0;
                }
                break;
            case NAND:
                if(std::abs(input1) > 0 && std::abs(input2) > 0) {
                    out0(0) = 0.0;
                } else {
                    out0(0) = 1.0;
                }
                break;
            case NOR:
                if(std::abs(input1) > 0 || std::abs(input2) > 0) {
                    out0(0) = 0.0;
                } else {
                    out0(0) = 1.0;
                }
                break;
            case XNOR:
                if((std::abs(input1) > 0 && std::abs(input2) > 0) || (std::abs(input1) == 0 && std::abs(input2) == 0)) {
                    out0(0) = 1.0;
                } else {
                    out0(0) = 0.0;
                }
                break;
            default:
                out0(0) = 0.0;
                break;
        }

    }

} // namespace ModeledModules

PluginLoad(BooleanLogicUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<ModeledModules::BooleanLogic>(ft, "BooleanLogic", false);
}
