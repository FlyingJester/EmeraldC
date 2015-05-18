#include "declaration.hpp"
#include "integral_types.hpp"
#include "asm.hpp"
#include "arithmetic.hpp"
#include "error.hpp"

namespace Compiler{
//
// <type> ::= <user_type> | <integral_type>
// <declaration> ::= <type> <identifier> [<argument_list> | <assignment>]
// 
void Declaration(CPU *cpu, Files file){
    if(IsType(file)){
        struct Variable var;
        Type(var.type, file);
        var.name = GetName(file);
        if(Peek('=', file)){
            cpu->CreateVariable(var, file);
            UnMatch(var.name, file);
            Arithmetic::Assignment(cpu, file);
        }
        else if(Peek('(', file)){
            Abort("Functions not implemented yet.", file);
        }
    }
    else{
        Expected("Type", file);
    }
}

}