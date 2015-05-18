#include "CPU.hpp"
#include "../asm.hpp"
#include "optimizer.hpp"
#include <cassert>

namespace Compiler {
namespace MIPS_opt{

void CPU::PushValue(){
    if(index<6){
        // Store the value in $t#
        EmitLine(emit, {"move", {registerFromIndex(), "$t1"}});
    }
    else{
        Compiler::MIPS::CPU::PushValue();
    }
    index++;
}

void CPU::PopValue(){
    index--;
    if(index<6){
        EmitLine(emit, {"move", {"$t2", registerFromIndex()}});
    }
    else{
        Compiler::MIPS::CPU::PushValue();
    }
}

}
}