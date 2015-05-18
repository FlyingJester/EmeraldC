#pragma once
#include "io.hpp"
#include "integral_types.hpp"
#include <string>

#define max_operands 3

// In general, only use this file if you are modifying or adding a new CPU.

namespace Compiler {

class CPU;

struct Emitter;

struct Op {
    std::string code, operands[max_operands];
};

struct Variable {
    struct Integral type;
    std::string name;
};

Emitter *CreateEmitter();

// Unconditionally emits an op
void Emit(const Op &s, Files file);

// Queues an op for emmission.
void EmitLine(Emitter *emit, const Op &s);

void EnsureVariable(const std::string &var, Emitter *emit, Files file);
void CreateVariable(const struct Variable &var, Emitter *emit, Files file);

void Flush(Emitter *emit, Files file);
void Write(Emitter *emit, Files file);
void YasmInitSource(Emitter *emit, Files file, unsigned char bits = 64);

}