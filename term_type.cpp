#include "term_type.hpp"

namespace Compiler {

bool IsWhiteSpace(unsigned x){
    return (x==' ') || (x=='\t') || (x=='\n');
}

bool IsAlpha(unsigned x){
    return ((x>='a') && (x<='z')) || ((x>='A')&&(x<='Z')) || (x=='_');
}

bool IsDigit(unsigned x){
    static_assert('0'<'1', "You've forgotten how ASCII works."); // Just put my mind at ease.
    return (x>='0') && (x<='9');
}

bool IsHexDigit(unsigned x){
    return IsDigit(x) || ((x>='a') && (x<='f')) || ((x>='A') && (x<='F'));
}

bool IsBinaryOperator(unsigned c){
    if(IsAddOp(c)||IsMultOp(c))
       return true;
    switch(c){
        case '%':
            return true;
        default:
            return false;
    }
}

bool IsAddOp(unsigned c){
    switch(c){
        case '+':
        case '-':
            return true;
        default:
            return false;
    }
}
bool IsMultOp(unsigned c){
    switch(c){
        case '*':
        case '/':
            return true;
        default:
            return false;
    }
}

}