#include "function.hpp"
#include "error.hpp"

namespace Compiler {


static const Function *last_func = nullptr;

Function::Function(CPU *c, Files f)
  : cpu(c)
  , file(f)
  , scope(cpu, file){
}

Function::Function(CPU *c, Files f, const struct Integral &r, const std::string n)
  : cpu(c)
  , file(f)
  , scope(cpu, file)
  , return_type(r)
  , name(n){
}

void Function::leaveInnerScope() const{
    cpu->LeaveScope(scope.scopeSizeSince());
}

const Function &LastFunction(Files file){
    if(!last_func)
        Abort("Attempted retrieve the current function while not in a function", file);
    return *last_func;
}

void SetFunction(const Function &func, Files file){
    if(last_func)
        Abort("Attempted to nest functions", file);
    last_func = &func;
}

void ClearFunction(Files file){
    if(!last_func)
        Abort("Attempted leave a function while not in a function", file);
    last_func = nullptr;
}

FunctionLabeller *FunctionLabeller::last = nullptr;

const std::string &FunctionLabeller::ScopePrefix(){
    static const std::string f_l_empty = "";

    if(last)
        return last->name;
    else
        return f_l_empty;

}

bool FunctionLabeller::InFunctionScope(){
    return last;
}

}