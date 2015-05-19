#include "math_expression.hpp"
#include "boolean_expression.hpp"
#include "program.hpp"
#include "error.hpp"

namespace Compiler {
namespace Math {

// <math_bits>       ::= <math_expression> [<bit_op> <math_expression>]*
void Bits(CPU *cpu, Files file){
    Expression(cpu, file);
    while(IsBitOp(file)){
        cpu->PushValue();
        BitOp(cpu, file);
    }
}

// <math_expression> ::= <math_term> [<add_op> <math_term>]*
void Expression(CPU *cpu, Files file){
    Term(cpu, file);
    while(IsAddOp(file)){
        cpu->PushValue();
        AddOp(cpu, file);
    }
}

// <math_term>       ::= <math_signed_factor> [<mult_op> <math_signed_factor>]*
void Term(CPU *cpu, Files file){
    SignedFactor(cpu, file);
    while(IsMultOp(file)){
        cpu->PushValue();
        MultOp(cpu, file);
    }
}

// <math_signed_factor> ::= [-] <math_factor>
void SignedFactor(CPU *cpu, Files file){
    const bool negate = Peek('-', file);
    if(negate) Match('-', file);
    Factor(cpu, file);
    if(negate)
        cpu->Negate();
}

// <factor>          ::= <number_literal> | <identifier> | ( <bool_expression> )
void Factor(CPU *cpu, Files file){
    if(Peek('(', file)){
        Match('(', file);
        Boolean::Expression(cpu, file);
        Match(')', file);
    }
    else if(IsDigit(Peek())){
        cpu->LoadNumber(GetNumber(file));
    }
    else if(IsAlpha(Peek()))
        Identifier(cpu, file);
    else
        Abort("Internal Error. FIXME!", file);
}

bool IsBitOp(Files file){
    return Peek(">>", file) || Peek("<<", file);
}

void BitOp(CPU *cpu, Files file){
    if(Peek(">>", file)){
        Match(">>", file);
        Expression(cpu, file);
        cpu->PopValue();
        cpu->BitShiftRight();
    }
    if(Peek("<<", file)){
        Match("<<", file);
        Expression(cpu, file);
        cpu->PopValue();
        cpu->BitShiftLeft();
    }
    else
        Expected("Bitshift Operation", file);
}

bool IsAddOp(Files file){
    return Peek('-', file) || Peek('+', file);
}

void AddOp(CPU *cpu, Files file){
    switch(Peek()){
        case '+':
            Match('+', file);
            Term(cpu, file);
            cpu->PopValue();
            cpu->Add();
            return;
        case'-':
            Match('-', file);
            Term(cpu, file);
            cpu->PopValue();
            cpu->Subtract();
            return;
        default:
        Expected("Addition Operation", file);
    }
}

bool IsMultOp(Files file){
    return Peek('*', file) || Peek('/', file) || Peek('%', file);
}

void MultOp(CPU *cpu, Files file){
    switch(Peek()){
        case '*':
            Match('*', file);
            SignedFactor(cpu, file);
            cpu->PopValue();
            cpu->Multiply();
            return;
        case '/':
            Match('/', file);
            SignedFactor(cpu, file);
            cpu->PopValue();
            cpu->Divide();
            return;
        case '%':
            Match('%', file);
            SignedFactor(cpu, file);
            cpu->PopValue();
            cpu->Modulo();
            return;
        default:
        Expected("Multiplication Operation", file);
    }
}

}
}
