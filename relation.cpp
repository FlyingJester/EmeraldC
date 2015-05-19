#include "relation.hpp"
#include "math_expression.hpp"
#include "CPU.hpp"
#include "error.hpp"

namespace Compiler {

// <relation>        ::= | <math_expression> [<relate_op> <math_expression>]
void Relation(CPU *cpu, Files file){
    Math::Bits(cpu, file);
    while(IsRelationalOp(file)){
        RelationalOperator(cpu, file);
    }
}


void RelationalOperator(CPU *cpu, Files file){
    if(Peek(">=", file)){
        Match(">=", file);
        Math::Bits(cpu, file);
        cpu->PopValue();
        cpu->GreaterThanOrEqual();
    }
    else if(Peek('>', file)){
        Match('>', file);
        Math::Bits(cpu, file);
        cpu->PopValue();
        cpu->GreaterThan();
    }
    else if(Peek("<=", file)){
        Match("<=", file);
        Math::Bits(cpu, file);
        cpu->PopValue();
        cpu->LessThanOrEqual();
    }
    else if(Peek('<', file)){
        Match('<', file);
        Math::Bits(cpu, file);
        cpu->PopValue();
        cpu->LessThan();
    }
    else if(Peek("==", file)){
        Match("==", file);
        Math::Bits(cpu, file);
        cpu->PopValue();
        cpu->Equal();
    }
    else if(Peek("!=", file)){
        Match("!=", file);
        Math::Bits(cpu, file);
        cpu->PopValue();
        cpu->NotEqual();
    }
    else
        Expected("Relational Operator", file);
}

bool IsRelationalOp(Files file){
    return Peek('>', file) || Peek('<', file) ||
    Peek("==", file) || Peek("!=", file);
}

}
