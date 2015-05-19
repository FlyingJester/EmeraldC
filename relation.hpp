#pragma once
#include "io.hpp"
#include "asm.hpp"

namespace Compiler {

// <relation>        ::= | <math_expression> [<relate_op> <math_expression>]
void Relation(CPU *cpu, Files file);

void RelationalOperator(CPU *cpu, Files file);
bool IsRelationalOp(Files file);

}