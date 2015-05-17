#pragma once
#include "io.hpp"
#include <string>

#ifdef _MSC_VER
#define TS_NORETURN __declspec(noreturn)
#elif defined __GNUC__
#define TS_NORETURN __attribute__((noreturn))
#else
#pragma warning "Fix this, or else syntax errors will almost certainly follow."
#define TS_NORETURN
#endif

namespace Compiler {

void Error(const std::string &s, Files file);
TS_NORETURN void Abort(const std::string &s, Files file);
TS_NORETURN void Unexpected(const std::string &s, Files file);
TS_NORETURN void Unexpected(unsigned s, Files file);
TS_NORETURN void Expected(const std::string &s, Files file);
TS_NORETURN void Expected(unsigned s, Files file);

}