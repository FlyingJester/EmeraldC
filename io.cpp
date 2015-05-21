#include "io.hpp"
#include "error.hpp"
#include <vector>
#include <cassert>

namespace Compiler {

static std::string next;

static unsigned line = 1;
static unsigned column = 1;

// TODO: UTF8?
void PullChar(Files file){
    if(!next.empty()){

        if(next[0]=='\n'){
            column = 1;
            line++;
        }
        else if(next[0]=='\t'){
            column+=4;
        }
        else
            column++;

        next.erase(0, 1);
    }
    if(next.empty()){
        char c = fgetc(file.in);
        if(feof(file.in) || (c==EOF)){
            Abort("Unexpected end of input", file);
        }
        next.push_back(c);
    }
}

unsigned GetLine(Files file){
    unsigned n_line = line;
    for(std::string::const_iterator i = next.cbegin(); i != next.cend(); i++){
        if(*i=='\n')
            n_line++;
    }
    return n_line;
}

unsigned GetColumn(Files file){
    unsigned n_column = column;
    for(std::string::const_iterator i = next.cbegin(); i != next.cend(); i++){
        if(*i=='\n')
            n_column = 1;
        else if(*i=='\t')
            n_column+=4;
        else
            n_column++;
    }
    return n_column;
}


bool EndOfInput(Files file){
    SkipWhiteSpace(file);
    return feof(file.in) || (next.front()==EOF);
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

bool IsKeyword(Files file){
    std::string s;

    if(!IsAlpha(Peek()))
        Expected("Name", file);

    while(IsAlpha(Peek()) || IsDigit(Peek())){
        s+=Peek();
        PullChar(file);
    }
    return IsKeyword(s);
}

bool IsKeyword(const std::string &s){
  if(
  s=="auto" ||
  s=="break" ||
  s=="case" ||
  s=="char" ||
  s=="const" ||
  s=="continue" ||
  s=="default" ||
  s=="do" ||
  s=="double" ||
  s=="else" ||
  s=="enum" ||
  s=="extern" ||
  s=="float" ||
  s=="for" ||
  s=="goto" ||
  s=="if" ||
  s=="int" ||
  s=="long" ||
  s=="register" ||
  s=="return" ||
  s=="short" ||
  s=="signed" ||
  s=="sizeof" ||
  s=="static" ||
  s=="struct" ||
  s=="switch" ||
  s=="typedef" ||
  s=="union" ||
  s=="unsigned" ||
  s=="void" ||
  s=="volatile" ||
  s=="while")
    return true;
  else
    return false;
}

std::string GetName(Files file){
    std::string s;

    if(!IsName(file))
        Expected("Name", file);

    if(!IsAlpha(Peek()))
        Expected("Name", file);

    while(IsAlpha(Peek()) || IsDigit(Peek())){
        s+=Peek();
        PullChar(file);
    }
    
    SkipWhiteSpace(file);
    
    if(IsKeyword(s))
        Unexpected("Keyword", file);

    return s;
}

std::string GetDecNumber(Files file){
    std::string s;
    while(IsDigit(Peek()))
        s+=GetChar(file);

    if(IsAlpha(Peek()))
        Unexpected("Character in decimal literal ", file);

    SkipWhiteSpace(file);
    return s;
}

std::string GetHexNumber(Files file){
    unsigned int n = 0;

    if(Peek("0X", file))
        Match("0X", file);
    else
        Match("0x", file);
    
    while(IsHexDigit(Peek())){
        char c = GetChar(file);
        unsigned char d= ((c<='9')&&(c>='0'))?(c-'0'):(c-'A'+10);
        n<<=4;
        n |= d;
    }
    if(IsAlpha(Peek()))
        Unexpected("Character in hexadecimal literal ", file);

    SkipWhiteSpace(file);

    return std::to_string(n);

}

std::string GetBinNumber(Files file){
    unsigned n = 0, chars = 0;
    if(Peek("0B", file))
        Match("0B", file);
    else
        Match("0b", file);

    while((Peek()=='1') || (Peek()=='0')){
        unsigned l = GetChar(file)-'0';
        n|=l;
        n<<=1;
        chars++;
    }

    if(chars++>32){
        Error("Warning too many characters in binary literal", file);
    }

    if(IsAlpha(Peek()) || IsDigit(Peek()))
        Unexpected("Character in binary literal ", file);

    return std::to_string(n);

}

std::string GetNumber(Files file){
    std::string s;
    
    if(!IsNumber(file))
        Expected("Integer", file);

    if(Peek("0x", file) || Peek("0X", file))
        return GetHexNumber(file);
    else if(Peek("0b", file) || Peek("0B", file))
        return GetBinNumber(file);
    else
        return GetDecNumber(file);
}

bool IsName(Files file){
    return IsAlpha(Peek());
}

bool IsNumber(Files file){
    return IsDigit(Peek());
}

}