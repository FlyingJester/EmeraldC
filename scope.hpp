#pragma once
#include "integral_types.hpp"
#include <vector>

namespace Compiler {

class CPU;

class Scope{
    // Keeps track of changes to the stack.
    //   It is of note that this only qualifies for addressable variables
    //   in a scope. Intermediate values can and will be pushed onto and popped
    //   from the stack. It is the job of the CPU to ensure that the stack stays
    //   balanced, and the job of the parser to ensure that an illegal
    //   expression which would expand the stack in the middle of a computation
    //   does not occur.
    unsigned size;

    // TODO: Is this necessary?
    static unsigned total_size;
    
    Scope * const up;
    static Scope *current;
    CPU * const cpu;
    Files file;
    
    void expand(unsigned byte_size);

    void loadVariable(const std::string &name, unsigned size_up);
    
    std::vector<struct Variable> scope_variables;
    
public:
    
    template<class T>
    Scope(unsigned byte_size, const T &variables, CPU *c, Files f)
      : size(byte_size)
      , up(current)
      , cpu(c)
      , file(f){
        total_size+=size;
        current = this;
        scope_variables.resize(variables.size());
        std::copy(variables.cbegin(), variables.cend(), scope_variables.begin());
    }
    
    ~Scope();
    
    
    // This will bind a variable name to a place on the stack.
    //   It will automatically call expand to hold the new variable.
    void addVariable(const struct Variable &);
    
    // Attempts to load a variable (same as cpu->LoadVariable used to).
    //  This will search the scope frames in top-to-bottom order, 
    //  and will then finally search the global scope. Will be
    //  called recursively.

    inline void loadVariable(const std::string &name){
        loadVariable(name, 0);
    }

};

}
