#pragma once
#include "../CPU.hpp"
#include "../MIPS/CPU.hpp"
#include <string>

namespace Compiler{
namespace MIPS_opt{

// The MIPS_opt CPU attempts to use more registers and use them more effectively.
// This is attempted by replacing push and pop operations with moves into other
// registers. The ordinary MIPS optimizer should be able to simplify these ops
// even more.
// Note, It likely has bugs.
class CPU : public Compiler::MIPS::CPU {
protected:
    unsigned index;
    inline std::string registerFromIndex(){
        return std::string("$s")+std::to_string(index);
    }
public:
    void PushValue() override;
    void PopValue() override;
};


}
}