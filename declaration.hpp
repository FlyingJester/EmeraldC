#pragma once
#include "CPU.hpp"
#include "io.hpp"

namespace Compiler {

class FunctionLabeller {
    const std::string name;
    static FunctionLabeller *last;
public:
    FunctionLabeller(const std::string &n)
      : name(n){
        last = this;
    }
    ~FunctionLabeller(){
        last = nullptr;
    }

    static const std::string &ScopePrefix();
};

// <typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
void TypedDeclaration(CPU *cpu, Files file);

// <indirection_declaration> ::= <[*]+> <symbol_declaration>
void IndirectionDeclaration(const struct Integral &type, CPU *cpu, Files file);

// <symbol_declaration> ::= <variable_declaration> | <variable> ( [<type> <variable>] [, <type> <variable>]* )
void SymbolDeclaration(const struct Integral &type, CPU *cpu, Files file);
// <variable_declaration> ::= <variable> [ = <bool_expression> ]
void VariableDeclaration(const struct Integral &type, CPU *cpu, Files file);

}