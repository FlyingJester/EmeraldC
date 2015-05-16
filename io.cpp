#include "io.hpp"
#include "error.hpp"
#include <vector>

namespace Compiler {

static std::string next;

// TODO: UTF8?
void PullChar(Files file){
    if(!next.empty())
        next.erase(0, 1);
    if(next.empty()){
        char c = fgetc(file.in);
        if(feof(file.in) || (c==EOF)){
            Abort("Unexpected end of input", file);
        }
        next.push_back(c);
    }
}

bool EndOfInput(Files file){
    SkipWhiteSpace(file);
    return feof(file.in) || (next.find(EOF)!=std::string::npos);
}

unsigned Peek(){
    return next.front();
}


bool Peek(unsigned x, Files file){
    return x==Peek();
}

void UnMatch(unsigned x, Files file){
    const char s[2] = {(char)x, 0};
    UnMatch(s, file);
}

void UnMatch(const std::string &s, Files file){
    next = s + next;
}

bool Peek(const std::string &s, Files file){
    std::string buffer;
    std::string::const_iterator i = s.cbegin();
    while(i!=s.cend()){
        if(*i==(char)Peek()){
            PullChar(file);
            i++;
        }
        else{
            break;
        }
    }

    UnMatch(std::string(s.cbegin(), i), file);
    return i==s.cend();
}

unsigned GetChar(Files file){
    unsigned r = Peek();
    PullChar(file);
    return r;
}

void SkipWhiteSpace(Files file){
    while(IsWhiteSpace(Peek())) PullChar(file);
}

void Match(const std::string &s, Files file){
    for(std::string::const_iterator i = s.cbegin(); i!=s.cend(); i++){
        if(*i==(char)Peek()){
            PullChar(file);
        }
        else{
            Expected(*i, file);
        }
    }
    SkipWhiteSpace(file);
}

void Match(unsigned x, Files file){
    if(Peek()==x){
        PullChar(file);
    }
    else
        Expected(x, file);
    SkipWhiteSpace(file);
}

std::string GetName(Files file){
    std::string s;

    if(!IsAlpha(Peek()))
        Expected("Name", file);

    while(IsAlpha(Peek()) || IsDigit(Peek())){
        s+=Peek();
        PullChar(file);
    }
    
    SkipWhiteSpace(file);
    
    return s;
}

std::string GetNumber(Files file){
    std::string s;

    if(!IsDigit(Peek()))
        Expected("Integer", file);

    while(IsDigit(Peek())){
        s+=Peek();
        PullChar(file);
        SkipWhiteSpace(file);
    }

    return s;
}

}