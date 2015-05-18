#pragma once
#include "../asm.hpp"

namespace Compiler{

namespace MIPS{
    int Optimize(Emitter *emit, unsigned level);
}

}
