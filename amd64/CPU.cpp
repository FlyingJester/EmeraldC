#include "CPU.hpp"
#include "../asm_inner.hpp"
#include "../asm.hpp"
#include "../function.hpp"
#include "optimizer.hpp"
#include <cassert>

namespace Compiler {
namespace amd64{


int CPU::Optimize(unsigned level){
   return Compiler::amd64::Optimize(emit, level);
}

void CPU::test(const std::string &reg){

    EmitLine(emit, {"test", {reg}});

}

void CPU::PushValue(){

    EmitLine(emit, {"push", {"rax"}});

}

void CPU::PopValue(){

    EmitLine(emit, {"pop", {"rbx"}});

}

void CPU::LoadVariable(const std::string &name){

    EmitLine(emit, {"mov", {"rax", name}});

}

void CPU::StoreVariable(const std::string &name){

    EmitLine(emit, {"mov", {std::string("[qword ")+name+"]", "rax"}});

}

void CPU::LoadNumber(const std::string &number){

    EmitLine(emit, {"mov", {"rax", number}});

}

void CPU::Call(const std::string &symbol){

    EmitLine(emit, {"call", {symbol}});

}

void CPU::Return(const Function &from){

    LeaveScope((from.argv.size()>6)?6:from.argv.size());
    
    EmitLine(emit, {"ret", {}});

}

void CPU::AddToScope(unsigned bytes){

    int b = bytes;
    EmitLine(emit, {"add", {"rsp", std::to_string(-b)}});

}

void CPU::LeaveScope(unsigned bytes){

    EmitLine(emit, {"add", {"rsp", std::to_string(bytes)}});

}

void CPU::LoadFromStackAt(unsigned bytes){

    EmitLine(emit, {"mov", {"rax", 
        std::string("[qword rsp + ")+std::to_string(bytes)+"]"}});

}

static inline std::string LoadArgReg(unsigned i){
    switch(i){
        case 0:
            return "rdi";
        case 1:
            return "rsi";
        case 2:
            return "rdx";
        case 3:
            return "rcx";
        case 4:
            return "r8";
        case 5:
            return "r9";
    }
    return std::string("[qword rsp + ") + std::to_string((i-6)<<3) + "]";
}

void CPU::LoadFromArgument(unsigned argi, unsigned argc){

    EmitLine(emit, {"mov", {"rax", LoadArgReg(argi)}});

}

void CPU::ArgumentsOntoStack(unsigned argc){
    for(unsigned i = 0; i<argc && i<6; i++){
        LoadFromArgument(i, argc);
        PushValue();
    }
}

void CPU::Jump(const std::string &symbol){

    EmitLine(emit, {"jmp", {symbol}});

}

void CPU::JumpNotZero(const std::string &symbol){

    test("rax");
    EmitLine(emit, {"jnz", {symbol}});

}

void CPU::JumpZero(const std::string &symbol){

    test("rax");
    EmitLine(emit, {"jz", {symbol}});

}

void CPU::Negate(){

    EmitLine(emit, {"imul", {"rax", "-1"}});

}

void CPU::Add(){

    EmitLine(emit, {"add", {"rax", "rbx"}});

}

void CPU::Subtract(){

    EmitLine(emit, {"sub", {"rbx", "rax"}});
    EmitLine(emit, {"mov", {"rax", "rbx"}});

}

void CPU::Multiply(){

    EmitLine(emit, {"imul", {"rax", "rbx"}});

}

void CPU::Divide(){

// Swap rax and rbx
    EmitLine(emit, {"xor", {"rbx", "rax"}});
    EmitLine(emit, {"xor", {"rax", "rbx"}});
    EmitLine(emit, {"xor", {"rbx", "rax"}});
// clear rdx to make room for the remainder.
    EmitLine(emit, {"mov", {"rdx", "0x0"}});
   
// Division
    EmitLine(emit, {"div", {"rax"}});
}

void CPU::Modulo(){

    Divide();
    EmitLine(emit, {"mov", {"rax", "rdx"}});

}

void CPU::True(){

    EmitLine(emit, {"mov", {"rax", "0x1"}});

}

void CPU::False(){

    EmitLine(emit, {"mov", {"rax", "0x0"}});

}

void CPU::booleanCast(const std::string &reg){
    EmitLine(emit, {"cmp", {reg, "0x0"}});
    EmitLine(emit, {"seta", {reg}});
}

void CPU::BooleanAnd(){
    
    booleanCast("rax");
    booleanCast("rbx");
    BitwiseAnd();
    
}

void CPU::BooleanOr(){

    EmitLine(emit, {"cmp", {"rax", "0x0"}});
    EmitLine(emit, {"seta", {"rax"}});
    EmitLine(emit, {"cmp", {"rbx", "0x0"}});
    EmitLine(emit, {"seta", {"rbx"}});
    BitwiseOr();

}

void CPU::BitwiseAnd(){

    EmitLine(emit, {"and", {"rax", "rbx"}});

}

void CPU::BitwiseOr(){

    EmitLine(emit, {"or", {"rax", "rbx"}});

}

void CPU::BitwiseXor(){

    EmitLine(emit, {"xor", {"rax", "rbx"}});

}

void CPU::BitShiftLeft(){
    EmitLine(emit, {"shl", {"rax", "rbx"}});
}

void CPU::BitShiftRight(){
    EmitLine(emit, {"shr", {"rax", "rbx"}});
}

void CPU::GreaterThan(){

    EmitLine(emit, {"cmp", {"rax", "rbx"}});
    EmitLine(emit, {"seta", {"rax"}});

}

void CPU::GreaterThanOrEqual(){

    EmitLine(emit, {"cmp", {"rax", "rbx"}});
    EmitLine(emit, {"setae", {"rax"}});

}

void CPU::LessThan(){

    EmitLine(emit, {"cmp", {"rax", "rbx"}});
    EmitLine(emit, {"setb", {"rax"}});

}

void CPU::LessThanOrEqual(){

    EmitLine(emit, {"cmp", {"rax", "rbx"}});
    EmitLine(emit, {"setbe", {"rax"}});

}

void CPU::Equal(){

    EmitLine(emit, {"cmp", {"rax", "rbx"}});
    EmitLine(emit, {"sete", {"rax"}});

}
void CPU::NotEqual(){

    EmitLine(emit, {"cmp", {"rax", "rbx"}});
    EmitLine(emit, {"setne", {"rax"}});

}

void CPU::Exit(){
    EmitLine(emit, {"ret", {}});
}

void CPU::InitSource(Files file){
    Compiler::YasmInitSource(emit, file, 64);
}

void CPU::WriteSymbols(Files file){
    fputs("section .text\n", file.out);
    fputs("align 16\n", file.out);
    for(std::vector<struct Variable>::const_iterator i = emit->variables.cbegin();
        i!=emit->variables.cend(); i++){
        fputs(i->name.c_str(), file.out);
        fputc(':', file.out);
        fputs("resb  ", file.out);
        fputs(std::to_string(i->type.size).c_str(), file.out);
        fputc('\n', file.out);
    }
}

}
}