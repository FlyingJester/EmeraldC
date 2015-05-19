#include "CPU.hpp"
#include "../asm.hpp"
#include "optimizer.hpp"
#include <cassert>

namespace Compiler {
namespace new_cpu{


int CPU::Optimize(unsigned level){
   return Compiler::new_cpu::Optimize(emit, level);
}

void PushValue(){

#error Implement Push

}

void PopValue(){

#error Implement Pop

}

void CPU::LoadVariable(const std::string &name){

#error Implement Load value

}

void CPU::StoreVariable(const std::string &name){

#error Implement Store value

}

void CPU::LoadNumber(const std::string &name){

#error Implement load immediate number

}

void CPU::Call(const std::string &symbol){

#error Implement call


}

void CPU::Jump(const std::string &symbol){

#error Implement unconditional Jump


}

void CPU::JumpNotZero(const std::string &symbol){

#error Implement Jump if not 0


}

void CPU::JumpZero(const std::string &symbol){

#error Implement Jump if 0

}

void CPU::Add(){

#error Implement +

}

void CPU::Subtract(){

#error Implement -

}

void CPU::Multiply(){

#error Implement *

}

void CPU::Divide(){

#error Implement /

}

void CPU::Modulo(){

#error Implement %

}

void CPU::True(){

#error Implement true

}

void CPU::False(){

#error Implement false

}

void CPU::BooleanAnd(){

#error Implement &&

}

void CPU::BooleanOr(){

#error Implement ||

}

void CPU::BitwiseAnd(){

#error Implement &

}

void CPU::BitwiseOr(){

#error Implement |

}

void CPU::BitwiseXor(){

#error Implement ^

}

void CPU::GreaterThan(){

#error Implement >

}

void CPU::GreaterThanOrEqual(){

#error Implement >=

}

void CPU::LessThan(){

#error Implement <

}

void CPU::LessThanOrEqual(){

#error Implement <=

}

void CPU::Equal(){

#error Implement ==

}
void CPU::NotEqual(){

#error Implement !=

}

void CPU::Exit(){

#error Implement Sys Exit

}

}
}