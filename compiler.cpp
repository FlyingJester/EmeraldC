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

    Compiler::CPU *test_cpu = Compiler::CPU::Create("amd64_opt", console);
    Init(console);

    
    test_cpu->InitSource(console);
    Compiler::Program(test_cpu, console);
    test_cpu->Flush(console);
    test_cpu->WriteSymbols(console);
    return EXIT_SUCCESS;
}
