#pragma once
#include "integral_types.hpp"
#include "CPU.hpp"
#include <vector>
#include "scope.hpp"


namespace Compiler {

// Intended to be either 
class Function {
    CPU *cpu;
    Files file;
public:
    Scope scope;

    Function(CPU *c, Files f);
    Function(CPU *c, Files f, const struct Integral &r, const std::string n);

    struct Integral return_type;
    std::string name;
    std::vector<struct Variable> argv;

    // Takes care of stack cleanup from scoping.
    // This leaves the internal scope alone, and so can be used for return statements.
    void leaveInnerScope() const;
};

const Function &LastFunction(Files file);
void SetFunction(const Function &func, Files file);
void ClearFunction(Files file);

class FunctionLabeller {
    const std::string name;
    static FunctionLabeller *last;

    FunctionLabeller(){}

public:

    FunctionLabeller(const std::string &n)
      : name(n){
        last = this;
    }

    FunctionLabeller(const Function &f)
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