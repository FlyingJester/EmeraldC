#include "arithmetic.hpp"
#include "boolean.hpp"
#include "equation.hpp"
#include "asm.hpp"
#include "error.hpp"

namespace Compiler {
namespace Arithmetic {

void Factor(CPU *cpu, Files file){
    if(Peek()=='('){
        Match('(', file);
        Expression(cpu, file);
        Match(')', file);
    }
    else if(IsDigit(Peek()))
        cpu->LoadNumber(GetNumber(file));
    else if(IsAlpha(Peek()))
        Identifier(cpu, file);
}

void Assignment(CPU *cpu, Files file){
    const std::string name = GetName(file);
    Match('=', file);
    Boolean::Expression(cpu, file);
        
    cpu->StoreVariable(name);
        
    cpu->EnsureVariable(name, file);
    Match(';', file);
}



void Identifier(CPU *cpu, Files file){
    const std::string name = GetName(file);
    if(Peek()=='('){
        Match('(', file);
        Match(')', file);
        cpu->Call(name);
    }
    else{
        cpu->EnsureVariable(name, file);
        cpu->LoadVariable(name);
    }
}

void Term(CPU *cpu, Files file){
    Factor(cpu, file);
    while(IsMultOp(Peek())){
        cpu->PushValue();
        switch(Peek()){
            case '*':
                Multiply(cpu, file);
                break;
            case '/':
                Divide(cpu, file);
                break;
        }// switch peek
    } // is multop
}


void Expression(CPU *cpu, Files file){
    if(IsAddOp(Peek()))
        cpu->LoadNumber("0");
    else
        Term(cpu, file);

    while(IsAddOp(Peek())){
        cpu->PushValue();
        switch(Peek()){
            case '%':
                Modulo(cpu, file);
                break;
            case '+':
                Add(cpu, file);
                break;
            case '-':
                Subtract(cpu, file);
                break;
        } // switch peek
    } // while is binary op
}

void Relation(CPU *cpu, Files file){
    Expression(cpu, file);
    if(Peek('>', file)){
        cpu->PushValue();
        Match('>', file);
        const unsigned next = Peek();
        UnMatch('>', file);
        if(next=='='){
            GreaterThanOrEqual(cpu, file);
        }
        else
            GreaterThan(cpu, file);
    }
    else if(Peek('<', file)){
        cpu->PushValue();
        Match('<', file);
        const unsigned next = Peek();
        UnMatch('<', file);
        if(next=='='){
            LessThanOrEqual(cpu, file);
        }
        else
            LessThan(cpu, file);
    }
    else if(Peek("==", file)){
        cpu->PopValue();
        Equal(cpu, file);
    }
    else if(Peek("!=", file)){
        cpu->PopValue();
        NotEqual(cpu, file);
    }
    // Allow no relations at all, too.
}

void Add(CPU *cpu, Files file){
    Match('+', file);
    Term(cpu, file);
    cpu->PopValue();
    cpu->Add();
}

void Subtract(CPU *cpu, Files file){
    Match('-', file);
    Term(cpu, file);
    cpu->PopValue();
    cpu->Subtract();
}

void Multiply(CPU *cpu, Files file){
    Match('*', file);
    Factor(cpu, file);
    cpu->PopValue();
    cpu->Multiply();
}

void Divide(CPU *cpu, Files file){
    Match('/', file);
    Factor(cpu, file);
    cpu->PopValue();
    cpu->Divide();

}

void Modulo(CPU *cpu, Files file){
    Match('%', file);
    Factor(cpu, file);
    cpu->PopValue();
    cpu->Modulo();
}

void And(CPU *cpu, Files file){
    Match('&', file);
    Factor(cpu, file);
    cpu->PopValue();
    cpu->BitwiseAnd();
}

void Or(CPU *cpu, Files file){
    Match('|', file);
    Factor(cpu, file);
    cpu->PopValue();
    cpu->BitwiseOr();
}

void Xor(CPU *cpu, Files file){
    Match('^', file);
    Factor(cpu, file);
    cpu->PopValue();
    cpu->BitwiseXor();
}

}
}