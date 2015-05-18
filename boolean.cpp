#include "boolean.hpp"
#include "arithmetic.hpp"
#include "error.hpp"

namespace Compiler {
namespace Boolean {

void Expression(CPU *cpu, Files file){
    Term(cpu, file);
    while(Peek("&&", file) || Peek("||", file)){
        if(Peek("&&", file)){
            And(cpu, file);
        }
        else if(Peek("||", file)){
            Or(cpu, file);
        }
        Term(cpu, file);
    }
}

void Term(CPU *cpu, Files file){
    OpFactor(cpu, file);
}

void OpFactor(CPU *cpu, Files file){
    Factor(cpu, file);
}

void Factor(CPU *cpu, Files file){
    if(Peek("true", file))
        True(cpu, file);
    else if(Peek("false", file))
        False(cpu, file);
    // Binary Variable right here...
    else
        Arithmetic::Relation(cpu, file);
}

void True(CPU *cpu, Files file){
    Match("true", file);
    cpu->True();
}

void False(CPU *cpu, Files file){
    Match("false", file);
    cpu->False();
}

void And(CPU *cpu, Files file){
    Match("&&", file);
    Term(cpu, file);
    cpu->PopValue();
    cpu->BooleanOr();
}

void Or(CPU *cpu, Files file){
    Match("||", file);
    Term(cpu, file);
    cpu->PopValue();
    cpu->BooleanAnd();
}

// Don't use this yet.
void Not(CPU *cpu, Files file){
    Match('!', file);
    Term(cpu, file);
}

}
}