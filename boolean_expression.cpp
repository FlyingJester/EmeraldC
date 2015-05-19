#include "boolean_expression.hpp"
#include "relation.hpp"

namespace Compiler {
namespace Boolean {

// <bool_statement>  ::= <bool_expression> [, <bool_expression]*
void Statement(CPU *cpu, Files file){
    Expression(cpu, file);
    while(Peek(',', file)){
        Match(',', file);
        Expression(cpu, file);
    }
}

// <bool_expression> ::= <bool_term> [<or_op> <bool_term]*
void Expression(CPU *cpu, Files file){
    Term(cpu, file);
    while(IsOrOp(file)){
        Match("||", file);
        Term(cpu, file);
        Or(cpu, file);
    }
}

// <bool_term>       ::= <bool_not_factor> [<and_op> <bool_not_factor]*
void Term(CPU *cpu, Files file){
    NotFactor(cpu, file);
    while(IsAndOp(file)){
        Match("&&", file);
        NotFactor(cpu, file);
        And(cpu, file);
    }
}

// <bool_expression> ::= [<not_op>] <bool_factor>
void NotFactor(CPU *cpu, Files file){
    bool negate = false;
    if(IsNotOp(file)){
        Match('!', file);
        negate = true;
    }
    Factor(cpu, file);
//    cpu->Negate();
}

// <bool_factor>     ::= <bool> | <relation>
// <bool>            ::= true | false
void Factor(CPU *cpu, Files file){
    if(Peek("true", file)){
        Match("true", file);
        cpu->True();
    }
    else if(Peek("false", file)){
        Match("false", file);
        cpu->False();
    }
    else
        Relation(cpu, file);
}

void And(CPU *cpu, Files file){
    Match("&&", file);
    Term(cpu, file);
    cpu->PopValue();
    cpu->BooleanAnd();
}

void Or(CPU *cpu, Files file){
    Match("||", file);
    Term(cpu, file);
    cpu->PopValue();
    cpu->BooleanOr();
}

bool IsOrOp(Files file){
    return Peek("||", file);
}

bool IsAndOp(Files file){
    return Peek("&&", file);
}

bool IsNotOp(Files file){
    if(Peek('!', file)){
        Match('!', file);
        const bool is_not_op = !Peek('=', file);
        UnMatch('!', file);
        return is_not_op;
    }
    return false;
}

}
}
