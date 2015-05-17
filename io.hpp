#pragma once
#include "term_type.hpp"
#include <string>
#include <cstdio>

namespace Compiler {

struct Files {
    FILE *in, *out, *err;
};

void PullChar(Files file);
unsigned Peek();
bool Peek(unsigned x, Files file);
bool Peek(const std::string &str, Files file);

unsigned GetChar(Files file);

void SkipWhiteSpace(Files file);

void Match(unsigned x, Files file);
void Match(const std::string &s, Files file);

void UnMatch(unsigned x, Files file);
void UnMatch(const std::string &s, Files file);

std::string GetName(Files file);
std::string GetNumber(Files file);

bool EndOfInput(Files file);

unsigned GetLine(Files file);
unsigned GetColumn(Files file);

}