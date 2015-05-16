#pragma once
#include "asm.hpp"

namespace Compiler{

// Returns the number of passes.
int Optimize(Emitter *emit, unsigned level);

}
