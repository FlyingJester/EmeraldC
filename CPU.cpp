#include "CPU.hpp"
#include "asm.hpp"
#include "error.hpp"

// Target includes
#ifdef TARGET_MIPS
#include "MIPS/CPU.hpp"
#endif
#ifdef TARGET_amd64
#include "amd64/CPU.hpp"
#endif

namespace Compiler {
   
CPU::CPU(){
    emit = CreateEmitter();
}

void CPU::EnsureVariable(const std::string& name){
    Compiler::EnsureVariable(emit, name);
}

CPU *CPU::Create(const std::string &platform, Files file){

#ifdef TARGET_MIPS
    if((platform=="MIPS") || (platform=="mips") || 
    (platform=="MIPS32") || (platform=="mips32")){
        return new MIPS::CPU();
    }
#endif
#ifdef TARGET_amd64
    if((platform=="amd64") || (platform=="AMD64") || 
    (platform=="x86_64") || (platform=="EMT_64")){
        return new amd64::CPU();
    }
#endif
    Abort(std::string("Unrecognized architecture ") + platform, file);
}

void CPU::Label(const std::string& name){
    EmitLine(emit, {name+":", {}});
}

void CPU::InitSource(Files file){
    fputs(".text\n", file.out);
}

void CPU::Flush(Files file){
    Compiler::Write(emit, file);
    Optimize(0xFF);
    Compiler::Flush(emit, file);
}

void CPU::WriteSymbols(Files file){
    Compiler::WriteSymbols(emit, file);
}

}
