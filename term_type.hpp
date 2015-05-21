#pragma once

namespace Compiler {

// TODO: UTF8?
bool IsWhiteSpace(unsigned x);
bool IsAlpha(unsigned x);
bool IsDigit(unsigned x);
bool IsHexDigit(unsigned x);
bool IsBinaryOperator(unsigned c);
bool IsAddOp(unsigned c);
bool IsMultOp(unsigned c);

template<typename T = unsigned>
inline bool IsName(T c){
   return IsAlpha(c);
}

}