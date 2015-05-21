#pragma once
#include "CPU.hpp"
#include "io.hpp"

namespace Compiler {

// <typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
void TypedDeclaration(const struct Integral &type, CPU *cpu, Files file);
void TypedDeclaration(CPU *cpu, Files file);

void GatherIndirection(unsigned char &to, Files file);
void GatherIndirection(struct Integral &to, Files file);

inline unsigned char GatherIndirection(Files file){
    unsigned char to;
    GatherIndirection(to, file);
    return to;
}

// <indirection_declaration> ::= <[*]+> <symbol_declaration>
void IndirectionDeclaration(const struct Integral &type, CPU *cpu, Files file);

// <symbol_declaration> ::= <variable_declaration> | <variable> ( [<type> <variable>] [, <type> <variable>]* )
void SymbolDeclaration(const struct Integral &type, CPU *cpu, Files file);
// <variable_declaration> ::= <variable> [ = <bool_expression> ]
void VariableDeclaration(const struct Integral &type, CPU *cpu, Files file);

}