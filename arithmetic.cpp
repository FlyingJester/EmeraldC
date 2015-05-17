#include "arithmetic.hpp"
#include "boolean.hpp"
#include "equation.hpp"
#include "asm.hpp"
#include "error.hpp"

namespace Compiler {
namespace Arithmetic {

void Factor(Emitter *emit, Files file){
    if(Peek()=='('){
        Match('(', file);
        Expression(emit, file);
        Match(')', file);
    }
    else if(IsDigit(Peek()))
        EmitLine(emit, {"li", {"$t1", GetNumber(file)}});
    else if(IsAlpha(Peek()))
        Identifier(emit, file);
//    else
//        Boolean::Expression(emit, file);
}

void Assignment(Emitter *emit, Files file){
        const std::string name = GetName(file);
        Match('=', file);
        Boolean::Expression(emit, file);
        EmitLine(emit, {"sw", {"$t1", name}});
        EnsureVariable(emit, name);
        Match(';', file);
}



void Identifier(Emitter *emit, Files file){
    const std::string name = GetName(file);
    if(Peek()=='('){
        Match('(', file);
        Match(')', file);
        EmitLine(emit, {"jal", {name}});
    }
    else{
        EnsureVariable(emit, name);
        EmitLine(emit, {"lw", {"$t1", name}});
    }
}

void Term(Emitter *emit, Files file){
    Factor(emit, file);
    while(IsMultOp(Peek())){
        EmitLine(emit, {"addiu", {"$sp", "$sp", "-4"}});
        EmitLine(emit, {"sw", {"$t1", "0($sp)"}});
        switch(Peek()){
            case '*':
                Multiply(emit, file);
                break;
            case '/':
                Divide(emit, file);
                break;
        }// switch peek
    } // is multop
}


void Expression(Emitter *emit, Files file){
    if(IsAddOp(Peek()))
        EmitLine(emit, {"li", {"$t1", "0"}});
    else
        Term(emit, file);

    while(IsAddOp(Peek())){
        EmitLine(emit, {"addiu", {"$sp", "$sp", "-4"}});
        EmitLine(emit, {"sw", {"$t1", "0($sp)"}});
        switch(Peek()){
            case '%':
                Modulo(emit, file);
                break;
            case '+':
                Add(emit, file);
                break;
            case '-':
                Subtract(emit, file);
                break;
        } // switch peek
    } // while is binary op
}

void Relation(Emitter *emit, Files file){
    Expression(emit, file);
    if(Peek('>', file)){
        EmitLine(emit, {"addiu", {"$sp", "$sp", "-4"}});
        EmitLine(emit, {"sw", {"$t1", "0($sp)"}});
        Match('>', file);
        const unsigned next = Peek();
        UnMatch('>', file);
        if(next=='='){
            GreaterThanOrEqual(emit, file);
        }
        else
            GreaterThan(emit, file);
    }
    else if(Peek('<', file)){
        EmitLine(emit, {"addiu", {"$sp", "$sp", "-4"}});
        EmitLine(emit, {"sw", {"$t1", "0($sp)"}});
        Match('<', file);
        const unsigned next = Peek();
        UnMatch('<', file);
        if(next=='='){
            LessThanOrEqual(emit, file);
        }
        else
            LessThan(emit, file);
    }
    else if(Peek("==", file)){
        EmitLine(emit, {"addiu", {"$sp", "$sp", "-4"}});
        EmitLine(emit, {"sw", {"$t1", "0($sp)"}});
        Equal(emit, file);
    }
    else if(Peek("!=", file)){
        EmitLine(emit, {"addiu", {"$sp", "$sp", "-4"}});
        EmitLine(emit, {"sw", {"$t1", "0($sp)"}});
        NotEqual(emit, file);
    }
    // Allow no relations at all, too.
}

void Add(Emitter *emit, Files file){
    Match('+', file);
    Term(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"add", {"$t1", "$t1", "$t2"}});
}

void Subtract(Emitter *emit, Files file){
    Match('-', file);
    Term(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"sub", {"$t1", "$t2", "$t1"}});
}

void Multiply(Emitter *emit, Files file){
    Match('*', file);
    Factor(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"mult", {"$t2", "$t1"}});
    EmitLine(emit, {"mflo", {"$t1"}});
}

void Divide(Emitter *emit, Files file){
    Match('/', file);
    Factor(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"div", {"$t2", "$t1"}});
    EmitLine(emit, {"mflo", {"$t1"}});
}

void Modulo(Emitter *emit, Files file){
    Match('%', file);
    Factor(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"div", {"$t2", "$t1"}});
    EmitLine(emit, {"mfhi", {"$t1"}});
}

void And(Emitter *emit, Files file){
    Match('&', file);
    Factor(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});    
    EmitLine(emit, {"and", {"$t1", "$t1", "$t2"}});
}

void Or(Emitter *emit, Files file){
    Match('|', file);
    Factor(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}}); 
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});   
    EmitLine(emit, {"or", {"$t1", "$t1", "$t2"}});
}

void Xor(Emitter *emit, Files file){
    Match('^', file);
    Factor(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}}); 
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});   
    EmitLine(emit, {"xor", {"$t1", "$t1", "$t2"}});
}

}
}