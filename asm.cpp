#include "asm.hpp"
#include "asm_inner.hpp"
#include "error.hpp"
#include <list>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>

#define max_code_length 8

namespace Compiler{

Emitter *CreateEmitter(){
    return new Emitter();
}

// Write ASM'z
void Emit(const Op &s, Files file){

    std::string accumulator=s.code;

    if(s.code.size() < max_code_length)
        for(int i = s.code.size(); i<max_code_length; i++)
            accumulator+=' ';
    else
        accumulator+=' ';

    for(int i = 0; i<max_operands-1; i++){
        accumulator.append(s.operands[i]);
        if(!s.operands[i+1].empty()){
            accumulator+=',';
            accumulator+=' ';
        }
    }

    accumulator+=s.operands[max_operands-1];
    
    
    fputs(accumulator.c_str(), file.out);
    fputc('\n', file.out);

}

void EmitLine(Emitter *emit, const Op &s){
    emit->operations.push_back(s);
}

void CreateVariable(const struct Variable &var, Emitter *emit, Files file){
    std::vector<struct Variable>::const_iterator i = emit->variables.cbegin();
    while(i!=emit->variables.cend()){
        if(i->name==var.name) Abort(std::string("Redefinition of variable ") + var.name, file);
        i++;
    }

    emit->variables.push_back(var); 
}

void EnsureVariable(const std::string &name, Emitter *emit, Files file){
    std::vector<struct Variable>::const_iterator i = emit->variables.cbegin();
    while(i!=emit->variables.cend()){
        if(i->name==name) break;
        i++;
    }

    if(i==emit->variables.cend()){
        Abort(std::string("Use of undefined variable ") + name, file);
    }
}

void InitSource(Emitter *emit, Files file){
    EmitLine(emit, {"    .text", {}});
}

void YasmInitSource(Emitter *emit, Files file, unsigned char bits){
    EmitLine(emit, {"bits", {"64"}});
    EmitLine(emit, {"section", {".text"}});
    EmitLine(emit, {"global", {"main"}});
}

void Flush(Emitter *emit, Files file){
    while(!emit->operations.empty()){
        Emit(emit->operations.front(), file);
        emit->operations.pop_front();
    }
}

void Write(Emitter *emit, Files file){
    for(std::list<Op>::const_iterator i = emit->operations.cbegin();
        i!=emit->operations.cend(); i++){
        Emit(*i, file);
    }
}

}