#pragma once
#include "integral_types.hpp"
#include <vector>

namespace Compiler {

// Intended to be either 
struct Function {
    struct Integral return_type;
    std::string name;
    std::vector<struct Variable> argv;
};

class FunctionLabeller {
    const std::string name;
    static FunctionLabeller *last;

    FunctionLabeller(){}

public:

    FunctionLabeller(const std::string &n)
      : name(n){
        last = this;
    }

    FunctionLabeller(const struct Function &f)
      : name(f.name){
        last = this;
    }

    ~FunctionLabeller(){
        last = nullptr;
    }

    static const std::string &ScopePrefix();
    static bool InFunctionScope();
};

}