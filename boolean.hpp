#pragma once
#include "asm.hpp"
#include "io.hpp"

namespace Compiler {
namespace Boolean{

void Expression(CPU *cpu, Files file);
void Term(CPU *cpu, Files file);
void OpFactor(CPU *cpu, Files file);
void Factor(CPU *cpu, Files file);

void True(CPU *cpu, Files file);
void False(CPU *cpu, Files file);

void And(CPU *cpu, Files file);
void Or(CPU *cpu, Files file);

void Not(CPU *cpu, Files file);

}
}