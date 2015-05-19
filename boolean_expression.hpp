#pragma once
#include "io.hpp"
#include "CPU.hpp"

namespace Compiler {
namespace Boolean {

// <bool_statement>  ::= <bool_expression> [, <bool_expression]*
void Statement(CPU *cpu, Files file);

// <bool_expression> ::= <bool_term> [<log_op> <bool_term]*
void Expression(CPU *cpu, Files file);

// <bool_term>       ::= <bool_factor>
void Term(CPU *cpu, Files file);

// <bool_factor>     ::= <bool> | <variable> | <relation>
void Factor(CPU *cpu, Files file);

// <bool>            ::= true | false
void Bool(CPU *cpu, Files file);

}
}
