#pragma once
#include "io.hpp"
#include "asm.hpp"

namespace Compiler {

void GreaterThan(CPU *cpu, Files file);
void GreaterThanOrEqual(CPU *cpu, Files file);
void LessThan(CPU *cpu, Files file);
void LessThanOrEqual(CPU *cpu, Files file);
void Equal(CPU *cpu, Files file);
void NotEqual(CPU *cpu, Files file);

}