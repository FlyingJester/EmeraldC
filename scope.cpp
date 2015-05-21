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
    total_size-=size;
    current = up;
    cpu->LeaveScope(size);
}



// size_up is how far burried this scope is.
void Scope::loadVariable_(const std::string &name, unsigned size_up){
    for(std::vector<struct Variable>::const_iterator i = scope_variables.cbegin();
        i!=scope_variables.cend(); i++){
        if(i->name==name){
            cpu->LoadFromStackAt(size_up);
        }
        size_up+=i->type.size;
    }
    if(up){
        up->loadVariable_(name, size_up);
    }
    else{
        cpu->LoadVariable(name);
    }
}

void Scope::ensureVariable_(const std::string &name){
    for(std::vector<struct Variable>::const_iterator i = scope_variables.cbegin();
        i!=scope_variables.cend(); i++){
        if(i->name==name) return;
    }
    if(up){
        up->ensureVariable_(name);
    }
    else{
        cpu->EnsureVariable(name, file);
    }
}

}