#include "function.hpp"

namespace Compiler {

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