#pragma once
#include "../asm.hpp"

namespace Compiler{
namespace amd64{

int Optimize(Emitter *emit, unsigned level);

}
}
