#pragma once
#include "io.hpp"
#include "asm.hpp"

namespace Compiler {

void GreaterThan(Emitter *emit, Files file);
void GreaterThanOrEqual(Emitter *emit, Files file);
void LessThan(Emitter *emit, Files file);
void LessThanOrEqual(Emitter *emit, Files file);
void Equal(Emitter *emit, Files file);
void NotEqual(Emitter *emit, Files file);

}