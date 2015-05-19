#include "CPU.hpp"
#include "asm.hpp"
#include "error.hpp"

// Target includes
#ifdef TARGET_MIPS
#include "MIPS/CPU.hpp"
#endif
#ifdef TARGET_MIPS_opt
#include "MIPS_opt/CPU.hpp"
#endif
#ifdef TARGET_amd64
#include "amd64/CPU.hpp"
#endif
#ifdef TARGET_amd64_opt
#include "amd64_opt/CPU.hpp"
#endif

namespace Compiler {
   
CPU::CPU(){
    emit = CreateEmitter();
}

void CPU::CreateVariable(const struct Integral& type, const std::string &name, Files file){
    CreateVariable({type, name}, file);
}

void CPU::CreateVariable(const struct Variable& var, Files file){
    Compiler::CreateVariable(var, emit, file);
}

void CPU::EnsureVariable(const std::string& name, Files file){
    Compiler::EnsureVariable(name, emit, file);
}

CPU *CPU::Create(const std::string &platform, Files file){

#ifdef TARGET_MIPS
    if((platform=="MIPS") || (platform=="mips") || 
    (platform=="MIPS32") || (platform=="mips32")){
        return new MIPS::CPU();
    }
#endif
#ifdef TARGET_MIPS_opt
    if((platform=="MIPS_opt") || (platform=="mips_opt") || 
    (platform=="MIPS32_opt") || (platform=="mips32_opt") || 
    (platform=="MIPS_opt32") || (platform=="mips_opt32")){
        return new MIPS_opt::CPU();
    }
#endif
#ifdef TARGET_amd64
    if((platform=="amd64") || (platform=="AMD64") || 
    (platform=="x86_64") || (platform=="EMT_64")){
        return new amd64::CPU();
    }
#endif
#ifdef TARGET_amd64_opt
    if((platform=="amd64_opt") || (platform=="AMD64_opt") || 
    (platform=="x86_64_opt") || (platform=="EMT_64_opt")){
        return new amd64_opt::CPU();
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

}
