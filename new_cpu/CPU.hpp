#pragma once
#include "../CPU.hpp"

namespace Compiler{
namespace new_cpu{

class CPU : public Compiler::CPU {
public:
    
    void LoadVariable(const std::string &name) override;
    void StoreVariable(const std::string &name) override;
    void LoadNumber(const std::string &number) override;
    int Optimize(unsigned level) override;
    
    void PushValue() override;
    void PopValue() override;
    
// Jumps
    void Call(const std::string &symbol) override;
    void Jump(const std::string &symbol) override;
    void JumpZero(const std::string &symbol) override;
    void JumpNotZero(const std::string &symbol) override;

    void Exit() override;

// Arithmetic operations

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

// Relational operations

    void GreaterThan() override;
    void GreaterThanOrEqual() override;
    void LessThan() override;
    void LessThanOrEqual() override;
    void Equal() override;
    void NotEqual() override;

};
