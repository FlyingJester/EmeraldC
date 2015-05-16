#pragma once
#include "asm.hpp"
#include "io.hpp"

namespace Compiler {
namespace Boolean{

void Expression(Emitter *emit, Files file);
void Term(Emitter *emit, Files file);
void OpFactor(Emitter *emit, Files file);
void Factor(Emitter *emit, Files file);

void True(Emitter *emit, Files file);
void False(Emitter *emit, Files file);

void And(Emitter *emit, Files file);
void Or(Emitter *emit, Files file);

void Not(Emitter *emit, Files file);

}
}