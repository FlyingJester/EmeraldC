#pragma once
#include "io.hpp"
#include "asm.hpp"

namespace Compiler {

void Program(Emitter *emit, Files file);
void Block(Emitter *emit, Files file);

void Conditional(Emitter *emit, Files file);

void If(Emitter *emit, Files file);
void While(Emitter *emit, Files file);
void DoWhile(Emitter *emit, Files file);
void For(Emitter *emit, Files file);
void Break(Emitter *emit, Files file);

}