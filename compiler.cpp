#include "compiler.hpp"
#include "program.hpp"
#include "io.hpp"
#include "error.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>

static void Init(Compiler::Files file){
    Compiler::PullChar(file);
    Compiler::SkipWhiteSpace(file);
}

// Just this one place we use C-strings. C-strings in, C-strings out.
static const char *GetArchitecture(int argc, char *argv[]){
    for(int i = 0; i<argc; i++){
        if(strnlen(argv[i], 3)<2) continue;
        if(memcmp(argv[i], "-m", 2)==0){
            return argv[i]+2;
        }        
    }
// The current testing architecture
    return "MIPS_opt";
}

static void Help(int argc, char *argv[]){
    for(int i = 0; i<argc; i++){
        if(strnlen(argv[i], 3)<2) continue;
        if((memcmp(argv[i], "-h", 2)==0) || (strcmp(argv[i], "--help")==0)){
            puts("Emerald C Comiler");
            puts("Copyright 2015, Martin McDonough");
            puts("Version <ALPHA>");
            exit(EXIT_SUCCESS);
            break;
        }
    }
}

int main(int argc, char *argv[]){

    Help(argc, argv);

    struct Compiler::Files console = {stdin, stdout, stderr};

    Compiler::CPU *test_cpu = Compiler::CPU::Create(GetArchitecture(argc, argv), console);
    Init(console);

    
    test_cpu->InitSource(console);
    Compiler::Program(test_cpu, console);
    test_cpu->Flush(console);
    test_cpu->WriteSymbols(console);
    return EXIT_SUCCESS;
}
