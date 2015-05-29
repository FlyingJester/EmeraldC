#include "CPU.hpp"
#include "../asm_inner.hpp"
#include "../asm.hpp"
#include "../function.hpp"
#include "optimizer.hpp"
#include <cassert>


// Save ourselves from typing the same registers over and over.
#define INTEL_STYLE_1_2_1(OP) {OP, {"$t1", "$t2", "$t1"}}
#define INTEL_STYLE INTEL_STYLE_1_2_1
#define COMPARATOR INTEL_STYLE_1_2_1

namespace Compiler {
namespace MIPS{

int CPU::Optimize(unsigned level){
   return Compiler::MIPS::Optimize(emit, level);
}

void CPU::push(const std::string &reg_to) const {
    EmitLine(emit, {"addiu", {"$sp", "$sp", "-8"}});
    EmitLine(emit, {"sw", {reg_to, "0($sp)"}});
}
void CPU::pop(const std::string &reg_to) const {
    EmitLine(emit, {"lw", {reg_to, "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "8"}});
}

void CPU::booleanCast(const std::string &reg_to) const {
    EmitLine(emit, {"sgt", {reg_to, reg_to, "$zero"}});
}

void CPU::LoadVariable(const std::string &name){
    EmitLine(emit, {"lw", {"$t1", name}});
}

void CPU::StoreVariable(const std::string &name){
    EmitLine(emit, {"sw", {"$t1", name}});
}

void CPU::LoadNumber(const std::string &number){
    EmitLine(emit, {"li", {"$t1", number}});
}

void CPU::Call(const std::string &symbol){
    EmitLine(emit, {"mov", {"$s7", "$t2"}});
    EmitLine(emit, {"mov", {"$s8", "$t1"}});
    EmitLine(emit, {"jal", {symbol}});
    EmitLine(emit, {"mov", {"$t2", "$s7"}});
    EmitLine(emit, {"mov", {"$t1", "$s8"}});
}

void CPU::Return(const Function &from){
    EmitLine(emit, {"move", {"$v0", "$t1"}});
    EmitLine(emit, {"move", {"$v1", "$t2"}}); // This may be necessary later.
    EmitLine(emit, {"jr", {"$ra"}});
}

void CPU::AddToScope(unsigned bytes){
    int b = bytes;
    EmitLine(emit, {"addiu", {"$sp", "$sp", std::to_string(-b)}});
}

void CPU::LeaveScope(unsigned bytes){
    EmitLine(emit, {"addiu", {"$sp", "$sp", std::to_string(bytes)}});
}

void CPU::LoadFromStackAt(unsigned bytes){
    EmitLine(emit, {"lw", {"$t1", std::to_string(bytes)+"($sp)"}});
}

void CPU::loadFromArgument(unsigned argi, unsigned argc, const std::string &reg){
    if(argi<4){
        EmitLine(emit, {"mov", {reg, std::string("$a") + std::to_string(argi)}});
    }
    else{
        LoadFromStackAt((argi-4)<<2);
    }
}

void CPU::ArgumentsOntoStack(unsigned argc){
    for(unsigned i = 0; i<argc; i++){
        loadFromArgument(i, argc, "$t0");
        push("$t0");
    }
}

void CPU::Jump(const std::string &symbol){
    EmitLine(emit, {"j", {symbol}});
    assert(symbol!="4");
}

void CPU::JumpNotZero(const std::string &symbol){
    EmitLine(emit, {"bne", {"$t1", "$zero", symbol}});
}

void CPU::JumpZero(const std::string &symbol){
    EmitLine(emit, {"beq", {"$t1", "$zero", symbol}});
}

void CPU::Negate(){
    EmitLine(emit, {"li", {"$t0", "-1"}});
    EmitLine(emit, {"mult", {"$t1", "$t0"}});
    EmitLine(emit, {"mflo", {"$t1"}});
}

void CPU::Add(){
    EmitLine(emit, INTEL_STYLE("add"));
}

void CPU::Subtract(){
    EmitLine(emit, INTEL_STYLE("sub"));
}

void CPU::Multiply(){
    EmitLine(emit, {"mult", {"$t2", "$t1"}});
    EmitLine(emit, {"mflo", {"$t1"}});
}

void CPU::Divide(){
    EmitLine(emit, {"div", {"$t2", "$t1"}});
    EmitLine(emit, {"mflo", {"$t1"}});
}

void CPU::Modulo(){
    EmitLine(emit, {"div", {"$t2", "$t1"}});
    EmitLine(emit, {"mfhi", {"$t1"}});
}

void CPU::True(){
    EmitLine(emit, {"li", {"$t1", "1"}});
}

void CPU::False(){
    EmitLine(emit, {"move", {"$t1", "$zero"}});
}

void CPU::BitwiseAnd(){
    EmitLine(emit, INTEL_STYLE("and"));
}

void CPU::BitwiseOr(){
    EmitLine(emit, INTEL_STYLE("or"));
}

void CPU::BitwiseXor(){
    EmitLine(emit, INTEL_STYLE("xor"));
}

void CPU::BitShiftLeft(){
    EmitLine(emit, INTEL_STYLE("sllv"));
}

void CPU::BitShiftRight(){
    EmitLine(emit, INTEL_STYLE("srlv"));
}

void CPU::GreaterThan(){
    EmitLine(emit, COMPARATOR("sgt"));
}

void CPU::GreaterThanOrEqual(){
    EmitLine(emit, COMPARATOR("sge"));
}

void CPU::LessThan(){
    EmitLine(emit, COMPARATOR("slt"));
}

void CPU::LessThanOrEqual(){
    EmitLine(emit, COMPARATOR("sle"));
}

void CPU::Equal(){
    EmitLine(emit, COMPARATOR("seq"));
}
void CPU::NotEqual(){
    EmitLine(emit, COMPARATOR("sne"));
}

#undef COMPARATOR

void CPU::Exit(){
    EmitLine(emit, {"li", {"$v0", "10"}});
    EmitLine(emit, {"syscall", {}});
}

void CPU::WriteSymbols(Files file){
    fputs("    .data\n", file.out);
    while(!emit->variables.empty()){
        fputs(emit->variables.back().name.c_str(), file.out);
        fputs(": ", file.out);
        if(emit->variables.back().type.indirection)
            fputs(".word 0", file.out); // 32-bit only?
        else if(emit->variables.back().type.size==1)
            fputs(".byte 0", file.out);
        else if(emit->variables.back().type.size==2)
            fputs(".half 0", file.out);
        else if(emit->variables.back().type.size==4)
            fputs(".word 0", file.out);
        else {
            int i = 4;
            fputs(".word 0", file.out);
            while(i<emit->variables.back().type.size){
                fputc(',', file.out);
                fputc('0', file.out);
                i+=4;
            }
        }
        fputc('\n', file.out);
        emit->variables.pop_back();
    }
    fputc('\n', file.out);
}

}
}