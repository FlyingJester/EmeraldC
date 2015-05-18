#pragma once
#include "io.hpp"
#include <string>

#define max_operands 3

// In general, only use this file if you are modifying or adding a new CPU.

namespace Compiler {

class CPU;

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
void Write(Emitter *emit, Files file);
void WriteSymbols(Emitter *emit, Files file);
void YasmWriteSymbols(Emitter *emit, Files file);
void YasmInitSource(Emitter *emit, Files file, unsigned char bits = 64);

}