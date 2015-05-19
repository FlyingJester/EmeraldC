#include "relation.hpp"
#include "arithmetic_expression.hpp"
#include "CPU.hpp"

namespace Compiler {

void GreaterThan(CPU *cpu, Files file){
    Match('>', file);
    Arithmetic::Expression(cpu, file);
    cpu->PopValue();
    cpu->GreaterThan();
}

void GreaterThanOrEqual(CPU *cpu, Files file){
    Match(">=", file);
    Arithmetic::Expression(cpu, file);
    cpu->PopValue();
    cpu->GreaterThanOrEqual();
}

void LessThan(CPU *cpu, Files file){
    Match('<', file);
    Arithmetic::Expression(cpu, file);
    cpu->PopValue();
    cpu->LessThan();
}

void LessThanOrEqual(CPU *cpu, Files file){
    Match("<=", file);
    Arithmetic::Expression(cpu, file);
    cpu->PopValue();
    cpu->LessThanOrEqual();
}

void Equal(CPU *cpu, Files file){
    Match("==", file);
    Arithmetic::Expression(cpu, file);
    cpu->PopValue();
    cpu->Equal();
}

void NotEqual(CPU *cpu, Files file){
    Match("!=", file);
    Arithmetic::Expression(cpu, file);
    cpu->PopValue();    
    cpu->NotEqual();
}


}
