#include "declaration.hpp"
#include "integral_types.hpp"
#include "asm.hpp"
#include "program.hpp"
#include "error.hpp"

namespace Compiler{


// <typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
void TypedDeclaration(CPU *cpu, Files file){
    if(!IsType(file))
        Expected("Type Specifier", file);
    
}

// <indirection_declaration> ::= <[*]+> <symbol_declaration>
void IndirectionDeclaration(CPU *cpu, Files file);
// <symbol_declaration> ::= <declaration> [ ( [<type> <variable>] [, <type> <variable>]* ) ]
void SymbolDeclaration(CPU *cpu, Files file);
// <declaration>     ::= <variable> | <assignment>
void Declaration(CPU *cpu, Files file);

}