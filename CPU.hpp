#pragma once
#include "io.hpp"
#include "function.hpp"
#include "asm.hpp"
#include <string>
#include <stack>
#include <vector>

namespace Compiler {

struct Variable;
struct Integral;
struct Emitter;

// Note that since many of the methods are also keywords in C/C++,
//  The CPU class uses CamelCase rather than dromedaryCase.

class CPU {
protected:
    Emitter *emit;
    CPU();

public:
    
    virtual int Optimize(unsigned level) = 0;
    virtual void LoadVariable(const std::string &variable) = 0;
    virtual void StoreVariable(const std::string &variable) = 0;
    virtual void LoadNumber(const std::string &number) = 0;

    virtual void PushValue() = 0;
    virtual void PopValue() = 0;

// Jumps
    virtual void Call(const std::string &symbol) = 0;
    virtual void Return() = 0;

    virtual void Jump(const std::string &symbol) = 0;
    virtual void JumpNotZero(const std::string &symbol) = 0;
    virtual void JumpZero(const std::string &symbol) = 0;

    virtual void Exit() = 0;

// Scoping
// It is up to the caller to keep the stack balanced regarding scoping calls.
    virtual void CreateScope(unsigned bytes_in) = 0;
    virtual void AddToScope(unsigned bytes) = 0;
    virtual void LeaveScope(unsigned bytes) = 0;

    virtual void LoadFromStackAt(unsigned bytes) = 0;

    virtual void LoadFromArgument(unsigned argi, unsigned argc) = 0;

// Arithmetic operations
    virtual void Negate() = 0;
    virtual void Add() = 0;
    virtual void Subtract() = 0;
    virtual void Multiply() = 0;
    virtual void Divide() = 0;
    virtual void Modulo() = 0;

    virtual void BitwiseAnd() = 0;
    virtual void BitwiseOr() = 0;
    virtual void BitwiseXor() = 0;

    virtual void BitShiftLeft() = 0;
    virtual void BitShiftRight() = 0;
// Boolean operations
    virtual void True() = 0;
    virtual void False() = 0;

    virtual void BooleanAnd() = 0;
    virtual void BooleanOr() = 0;

// Relational operations
    virtual void GreaterThan() = 0;
    virtual void GreaterThanOrEqual() = 0;
    virtual void LessThan() = 0;
    virtual void LessThanOrEqual() = 0;
    virtual void Equal() = 0;
    virtual void NotEqual() = 0;

//    virtual void Not(Emitter *emit, Files file) = 0;

    virtual void InitSource(Files file);
    virtual void Flush(Files file);
    virtual void WriteSymbols(Files file) = 0;
    void EnsureVariable(const std::string& var, Files file);
    void CreateVariable(const struct Variable& var, Files file);
    virtual void CreateVariable(const struct Integral& type, const std::string &name, Files file) final;
    virtual void Label(const std::string& name);

    static CPU *Create(const std::string &platform, Files file);
    
};

}
