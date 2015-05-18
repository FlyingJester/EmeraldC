#pragma once
#include "io.hpp"
#include "asm.hpp"
#include "cpu.hpp"

namespace Compiler {
namespace Arithmetic {

void Add(CPU *cpu, Files file);
void Subtract(CPU *cpu, Files file);
void Multiply(CPU *cpu, Files file);
void Divide(CPU *cpu, Files file);
void Modulo(CPU *cpu, Files file);
void And(CPU *cpu, Files file);
void Or(CPU *cpu, Files file);
void Xor(CPU *cpu, Files file);

void Expression(CPU *cpu, Files file);
void Term(CPU *cpu, Files file);
void Identifier(CPU *cpu, Files file);
void Factor(CPU *cpu, Files file);
void Assignment(CPU *cpu, Files file);
void Relation(CPU *cpu, Files file);

}
}