#include "error.hpp"
#include <cstdlib>
#include <cassert>

namespace Compiler {

static std::string NameChar(unsigned c){
    switch(c){
        case ' ':
            return "[space]";
        case '\n':
            return "[newline]";
        default:
        {
            char text[2] = {(char)c, 0};
            return text;
        }
    }
}

void Error(const std::string &s, Files file){
    fputs("Error ", file.err);
    fputs(s.c_str(), file.err);
    fputc('\n', file.err);
}

TS_NORETURN void Abort(const std::string &s, Files file){
    Error(s, file);
    assert(false);
}

TS_NORETURN void Expected(const std::string &s, Files file){
    Abort(s + " expected, was " + NameChar(Peek()), file);
}

// TODO: UTF8
TS_NORETURN void Expected(unsigned s, Files file){
    Expected(NameChar(s), file);
}



}