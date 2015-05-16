#pragma once
#include "io.hpp"
#include "asm.hpp"

namespace Compiler {
namespace Arithmetic {

void Add(Emitter *emit, Files file);
void Subtract(Emitter *emit, Files file);
void Multiply(Emitter *emit, Files file);
void Divide(Emitter *emit, Files file);
void Modulo(Emitter *emit, Files file);
void And(Emitter *emit, Files file);
void Or(Emitter *emit, Files file);
void Xor(Emitter *emit, Files file);

void Expression(Emitter *emit, Files file);
void Term(Emitter *emit, Files file);
void Identifier(Emitter *emit, Files file);
void Factor(Emitter *emit, Files file);
void Assignment(Emitter *emit, Files file);
void Relation(Emitter *emit, Files file);

}
}