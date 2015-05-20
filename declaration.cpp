#include "declaration.hpp"
#include "integral_types.hpp"
#include "function.hpp"
#include "asm.hpp"
#include "program.hpp"
#include "error.hpp"

namespace Compiler{

// <typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
void TypedDeclaration(CPU *cpu, Files file){
    if(!IsType(file))
        Expected("Type Specifier", file);
    struct Integral type;
    Type(type, file);
    IndirectionDeclaration(type, cpu, file);
    while(Peek(',', file)){
        Match(',', file);
        IndirectionDeclaration(type, cpu, file);
    }
}

// <indirection_declaration> ::= <[*]+> <symbol_declaration>
void IndirectionDeclaration(const struct Integral &type, CPU *cpu, Files file){
    struct Integral our_type = type;
    while(Peek('*', file)){
        if(our_type.indirection==0xFF)
            Abort("Maximum level of indirection (255) exceeded", file);

        Match('*', file);
        our_type.indirection++;
    }
    SymbolDeclaration(our_type, cpu, file);
}

// <symbol_declaration> ::= <variable_declaration> | <variable> ( [<type> <variable>] [, <type> <variable>]* )
void SymbolDeclaration(const struct Integral &type, CPU *cpu, Files file){
    const std::string name = GetName(file);
    if(Peek('(', file)){

        if(FunctionLabeller::InFunctionScope()){
            Abort(" Cannot define functions here, already in function scope.", file);
        }
        
        Match('(', file);
        
        struct Function<struct Variable> function_decl = {type, name, {}};
        
        UnMatch(',', file);
        
        while(Peek(',', file)){
            Match(',', file);
            struct Integral type;
            Type(type, file);
            const std::string name = IsName(file)?GetName(file):"";
            function_decl.argv.push_back({type, name});
        }
        
        Match(')', file);
        
        if(Peek('{', file)){
            
            FunctionLabeller function_label(function_decl);
            
            
            
            Block(cpu, file);
            UnMatch(';', file); // This is likely a sign that we put the semicolon in the wrong parsing function.
        }
    }
    else{
        UnMatch(name, file);
        VariableDeclaration(type, cpu, file);
    }
}

// TODO: Assignment for non-scoped variables.
// <variable_declaration> ::= <variable> [ = <bool_expression> ]
void VariableDeclaration(const struct Integral &type, CPU *cpu, Files file){
    const std::string name = GetName(file) + FunctionLabeller::ScopePrefix();

    cpu->CreateVariable(type, name, file);
    if(Peek('=', file)){
        UnMatch(name, file);
        Assignment(cpu, file);
    }
}

}