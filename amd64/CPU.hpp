#pragma once
#include "../CPU.hpp"

namespace Compiler{
namespace amd64{

class CPU : public Compiler::CPU {
protected:
    void test(const std::string &reg);
    void booleanCast(const std::string &reg);
public:
    
    void LoadVariable(const std::string &name) override;
    void StoreVariable(const std::string &name) override;
    void LoadNumber(const std::string &number) override;
    int Optimize(unsigned level) override;
    
    void PushValue() override;
    void PopValue() override;
    
// Jumps
    void Call(const std::string &symbol) override;
    void Return(const Function &from) override;

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

    void LoadFromArgument(unsigned argi, unsigned argc) override;

    void ArgumentsOntoStack(unsigned argc) override;

// Arithmetic operations

    void Negate() override;

    void Add() override;
    void Subtract() override;
    void Multiply() override;
    void Divide() override;
    void Modulo() override;

// Boolean operations

    void True() override;
    void False() override;

    void BooleanAnd() override;
    void BooleanOr() override;

    void BitwiseAnd() override;
    void BitwiseOr() override;
    void BitwiseXor() override;

    void BitShiftLeft() override;
    void BitShiftRight() override;

// Relational operations

    void GreaterThan() override;
    void GreaterThanOrEqual() override;
    void LessThan() override;
    void LessThanOrEqual() override;
    void Equal() override;
    void NotEqual() override;

    void InitSource(Files file) override;
    void WriteSymbols(Files file) override;

};

}
}
