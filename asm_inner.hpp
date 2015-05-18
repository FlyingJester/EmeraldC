#pragma once
#include "asm.hpp"
#include <list>
#include <vector>

namespace Compiler{
struct Emitter{
    std::list<Op> operations;
    std::vector<struct Variable> variables;
};
}