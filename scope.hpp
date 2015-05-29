#pragma once
#include "integral_types.hpp"
#include "CPU.hpp"
#include <vector>

namespace Compiler {

class CPU;

// TODO: Make the calling convention work! This is kind of nonsense right now.
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

    void loadVariable_(const std::string &name, unsigned size_up);
    void ensureVariable_(const std::string &name);
    
    std::vector<struct Variable> scope_variables;
    
    Scope() = delete;
    Scope(Scope &) = delete;
    Scope(const Scope &) = delete;
    
public:

    template<class T>
    Scope(unsigned s, const T &variables, CPU *c, Files f)
      : size(s)
      , up(current)
      , cpu(c)
      , file(f){
        total_size+=size;
        current = this;
        scope_variables.resize(variables.size());
        std::copy(variables.cbegin(), variables.cend(), scope_variables.begin());
        if(size)
            cpu->CreateScope(size);
    }

    template<class T>
    Scope(const T &variables, CPU *c, Files f)
      : size(0)
      , up(current)
      , cpu(c)
      , file(f){
        total_size+=size;
        current = this;
        scope_variables.resize(variables.size());
        std::copy(variables.cbegin(), variables.cend(), scope_variables.begin());

        for(typename T::const_iterator i = variables.cbegin(); i!=variables.cend(); i++){
            size+=i->type.size;
            printf("%i\n", size);
        }
        if(size)
            cpu->CreateScope(size);
    }

    Scope(CPU *c, Files f)
      : size(0)
      , up(current)
      , cpu(c)
      , file(f){
        current = this;
    }
    
    ~Scope();
    
    
    // This will bind a variable name to a place on the stack.
    //   It will automatically call expand to hold the new variable.
    void addVariable(const struct Variable &);
    
    // Attempts to load a variable (same as cpu->LoadVariable used to).
    //  This will search the scope frames in top-to-bottom order, 
    //  and will then finally search the global scope. Will be
    //  called recursively.

    inline static void loadVariable(const std::string &name, CPU *cpu_, Files file_){
        if(!current)
            cpu_->LoadVariable(name);
        else
            current->loadVariable_(name, 0u);
    }
    static void ensureVariable(const std::string &name, CPU *cpu_, Files file_){
        if(!current)
            cpu_->EnsureVariable(name, file_);
        else
            current->ensureVariable_(name);
    }

    static unsigned currentScopeSize() {
        return current->size;
    }

    unsigned scopeSizeSince() const{
        const Scope *ex = current;
        unsigned accum = 0;
        while(ex->up!=this){
            ex = ex->up;
            accum+=ex->size;
        }
        return accum;
    }
};

}
