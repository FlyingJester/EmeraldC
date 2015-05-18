#pragma once
#include "io.hpp"
#include "CPU.hpp"

namespace Compiler {

void Program(CPU *cpu, Files file);
void Block(CPU *cpu, Files file);

void Conditional(CPU *cpu, Files file);

void If(CPU *cpu, Files file);
void While(CPU *cpu, Files file);
void DoWhile(CPU *cpu, Files file);
void For(CPU *cpu, Files file);
void Break(CPU *cpu, Files file);

}