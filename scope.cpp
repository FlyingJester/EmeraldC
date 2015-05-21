#include "scope.hpp"
#include "CPU.hpp"

namespace Compiler {
    
unsigned Scope::total_size = 0;
Scope *Scope::current = nullptr;

void Scope::expand(unsigned byte_size){
    cpu->AddToScope(byte_size);
    size+=byte_size;
}
    
Scope::~Scope(){
    cpu->LeaveScope(size);
    total_size-=size;
    current = up;
}

void Scope::loadVariable(const std::string &name, unsigned size_up){
    
    
    
}

}