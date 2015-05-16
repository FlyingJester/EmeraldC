#include "compiler.hpp"
#include "program.hpp"
#include "io.hpp"
#include "error.hpp"
#include <cstdlib>

static void Init(Compiler::Emitter * &emit, Compiler::Files file){
    Compiler::PullChar(file);
    Compiler::SkipWhiteSpace(file);
    emit = Compiler::CreateEmitter();
}

int main(int argc, char *argv[]){

    struct Compiler::Files console = {stdin, stdout, stderr};

    Compiler::Emitter *emit;
    Init(emit, console);
    Compiler::InitSource(emit, console);
    Compiler::Program(emit, console);
    Flush(emit, console);
    Compiler::WriteSymbols(emit, console);
    return EXIT_SUCCESS;
}
