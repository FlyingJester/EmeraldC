#pragma once
#include "../CPU.hpp"

namespace Compiler{
namespace MIPS{

class CPU : public Compiler::CPU {
protected:
    void push(const std::string &reg) const;
    void pop(const std::string &reg) const;
    void booleanCast(const std::string &reg) const;

    void loadFromArgument(unsigned argi, unsigned argc, const std::string &reg);

public:
    
    void LoadVariable(const std::string &name) override;
    void StoreVariable(const std::string &name) override;
    void LoadNumber(const std::string &number) override;
    int Optimize(unsigned level) override;
    
    void PushValue() override {
        push("$t1");
    }
    void PopValue() override {
        pop("$t2");
    }
    
// Jumps
    void Call(const std::string &symbol) override;
    void Return(const struct Function &from) override;

    void Jump(const std::string &symbol) override;
    void JumpZero(const std::string &symbol) override;
    void JumpNotZero(const std::string &symbol) override;

    void Exit() override;

// Scoping

    void CreateScope(unsigned bytes_in) override {
        AddToScope(bytes_in);
    }
    void AddToScope(unsigned bytes) override;
    void LeaveScope(unsigned bytes) override;

    void LoadFromStackAt(unsigned bytes) override;

    void LoadFromArgument(unsigned argi, unsigned argc) override {
        loadFromArgument(argi, argc, "$t1");
    }

    void ArgumentsOntoStack(unsigned argc) override;

// Arithmetic operations

    void Negate() override;
    void Add() override;
    void Subtract() override;
    void Multiply() override;
    void Divide() override;
    void Modulo() override;

    void BitwiseAnd() override;
    void BitwiseOr() override;
    void BitwiseXor() override;

    void BitShiftLeft() override;
    void BitShiftRight() override;

// Boolean operations

    void True() override;
    void False() override;

    void BooleanAnd() override{
        booleanCast("$t1");
        booleanCast("$t2");
        BitwiseAnd();
    }

    void BooleanOr() override{
        booleanCast("$t1");
        booleanCast("$t2");
        BitwiseOr();        
    }

// Relational operations

    void GreaterThan() override;
    void GreaterThanOrEqual() override;
    void LessThan() override;
    void LessThanOrEqual() override;
    void Equal() override;
    void NotEqual() override;

    void WriteSymbols(Files file) override;

};


}
}