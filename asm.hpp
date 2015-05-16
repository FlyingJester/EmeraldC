#pragma once
#include "io.hpp"
#include <string>

#define max_operands 3

namespace Compiler {

struct Emitter;

struct Op {
    std::string code, operands[max_operands];
};

Emitter *CreateEmitter();

// Unconditionally emits an op
void Emit(const Op &s, Files file);

// Queues an op for emmission.
void EmitLine(Emitter *emit, const Op &s);

// Ensures that a variable will appear in the emitted assembly
void EnsureVariable(Emitter *emit, const std::string &name);

void Flush(Emitter *emit, Files file);
void WriteSymbols(Emitter *emit, Files file);
void InitSource(Emitter *emit, Files file);

}