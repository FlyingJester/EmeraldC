#pragma once
#include "CPU.hpp"
#include "io.hpp"

namespace Compiler {
//
// <type> ::= <user_type> | <integral_type>
// <declaration> ::= <type> <identifier> [<argument_list> | <assignment>]
// 
void Declaration(CPU *cpu, Files file);

}