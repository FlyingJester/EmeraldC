#pragma once
#include "CPU.hpp"
#include "io.hpp"

namespace Compiler {

/*
<typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
<indirection_declaration> ::= <[*]+> <symbol_declaration>
<symbol_declaration> ::= <declaration> [ ( [<type> <variable>] [, <type> <variable>]* ) ]
<declaration>     ::= <variable> | <assignment>
*/
// <type> ::= <user_type> | <integral_type>


// <typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
void TypedDeclaration(CPU *cpu, Files file);
// <indirection_declaration> ::= <[*]+> <symbol_declaration>
void IndirectionDeclaration(CPU *cpu, Files file);
// <symbol_declaration> ::= <declaration> [ ( [<type> <variable>] [, <type> <variable>]* ) ]
void SymbolDeclaration(CPU *cpu, Files file);
// <declaration>     ::= <variable> | <assignment>
void Declaration(CPU *cpu, Files file);

}