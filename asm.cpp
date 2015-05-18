#include "asm.hpp"
#include <list>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>

#define max_code_length 8

namespace Compiler{

struct Emitter{
    std::list<Op> operations;
    std::vector<std::string> variables;
};

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

void EnsureVariable(Emitter *emit, const std::string &name){

    assert(name!="else");

    if(std::find(emit->variables.cbegin(), emit->variables.cend(), name)==emit->variables.cend()){
        emit->variables.push_back(name);
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

void WriteSymbols(Emitter *emit, Files file){
    fputs("    .data\n", file.out);
    while(!emit->variables.empty()){
        fputs(emit->variables.back().c_str(), file.out);
        fputs(": .byte 0\n", file.out);
        emit->variables.pop_back();
    }
    fputc('\n', file.out);
}

void YasmWriteSymbols(Emitter *emit, Files file){
    fputs("section  .data\n", file.out);
    while(!emit->variables.empty()){
        fputs(emit->variables.back().c_str(), file.out);
        fputs(": dq 0x0\n", file.out);
        emit->variables.pop_back();
    }
    fputc('\n', file.out);
}

}