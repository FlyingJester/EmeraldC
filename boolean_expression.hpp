#pragma once
#include "io.hpp"
#include "CPU.hpp"

namespace Compiler {
namespace Boolean {

// <bool_statement>  ::= <bool_expression> [, <bool_expression]*
void Statement(CPU *cpu, Files file);

// <bool_expression> ::= <bool_term> [<or_op> <bool_term]*
void Expression(CPU *cpu, Files file);

// <bool_term>       ::= <bool_not_factor> [<and_op> <bool_not_factor]*
void Term(CPU *cpu, Files file);

// <bool_expression> ::= [<not_op>] <bool_factor>
void NotFactor(CPU *cpu, Files file);

// <bool_factor>     ::= <bool> | <relation>
// <bool>            ::= true | false
void Factor(CPU *cpu, Files file);

void Or(CPU *cpu, Files file);
void And(CPU *cpu, Files file);

bool IsOrOp(Files file);
bool IsAndOp(Files file);
bool IsNotOp(Files file);

}
}
