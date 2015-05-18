#include "CPU.hpp"
#include "../asm.hpp"
#include "optimizer.hpp"
#include <cassert>

namespace Compiler {
namespace amd64_opt{

void CPU::PushValue(){
    if(index<12)
        EmitLine(emit, {"mov", {registerFromIndex(), "rax"}});
    else
        Compiler::amd64::CPU::PushValue();
    index++;
}

void CPU::PopValue(){
    index--;
    if(index<12)
        EmitLine(emit, {"mov", {"rbx", registerFromIndex()}});
    else
        Compiler::amd64::CPU::PopValue();
}

}
}