#include "compiler.hpp"
#include "program.hpp"
#include "io.hpp"
#include "error.hpp"
#include <cstdlib>

static void Init(Compiler::Files file){
    Compiler::PullChar(file);
    Compiler::SkipWhiteSpace(file);
}

int main(int argc, char *argv[]){

    struct Compiler::Files console = {stdin, stdout, stderr};

    Compiler::CPU *mips_test_cpu = Compiler::CPU::Create("amd64", console);
    Init(console);

    
    mips_test_cpu->InitSource(console);
    Compiler::Program(mips_test_cpu, console);
    mips_test_cpu->Flush(console);
    mips_test_cpu->WriteSymbols(console);
    return EXIT_SUCCESS;
}
