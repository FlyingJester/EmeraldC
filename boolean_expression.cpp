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
    while(Peek("||", file)){
        Match("||", file);
        Term(cpu, file);
        cpu->PopValue();
        cpu->BooleanOr();
    }
}

// <bool_term>       ::= <bool_not_factor> [<and_op> <bool_not_factor]*
void Term(CPU *cpu, Files file){
    NotFactor(cpu, file);
    while(Peek("&&", file)){
        Match("&&", file);
        NotFactor(cpu, file);
        cpu->PopValue();
        cpu->BooleanAnd();
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
