#pragma once
#include "io.hpp"
#include "CPU.hpp"

namespace Compiler {
namespace Math {

// <math_bits>       ::= <math_expression> [<bit_op> <math_expression>]*
void Bits(CPU *cpu, Files file);

// <math_expression> ::= <math_term> [<add_op> <math_term>]*
void Expression(CPU *cpu, Files file);

// <math_term>       ::= <math_signed_factor> [<mult_op> <math_signed_factor>]*
void Term(CPU *cpu, Files file);

// <math_signed_factor> ::= [-] <math_factor>
void SignedFactor(CPU *cpu, Files file);

// <factor>          ::= <call> | <index> | <number_literal> | <variable> | ( <bool_expression> )
void Factor(CPU *cpu, Files file);

// <identifier>      ::= <variable> [<call> | <index>]

bool IsBitOp(Files file);
void BitOp(CPU *cpu, Files file);

bool IsAddOp(Files file);
void AddOp(CPU *cpu, Files file);

bool IsMultOp(Files file);
void MultOp(CPU *cpu, Files file);

}
}
