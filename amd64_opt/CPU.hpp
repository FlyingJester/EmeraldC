#pragma once
#include "../amd64/CPU.hpp"
#include <string>

namespace Compiler{
namespace amd64_opt{

class CPU : public Compiler::amd64::CPU {
protected:
    unsigned index;
    inline std::string registerFromIndex(){
        switch(index){
        case 0: return "rdx";
        case 1: return "rcx";
        case 2: return "rsi";
        case 3: return "rdi";
        default:
        return std::string("r")+std::to_string(index-4+8);
        }
    }
public:
    void PushValue() override;
    void PopValue() override;
};

}
}
