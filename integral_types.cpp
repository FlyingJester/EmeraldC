#include "integral_types.hpp"
#include "CPU.hpp"
#include "error.hpp"

namespace Compiler {

// TODO: Make this depend on the CPU address size.
const struct Integral &PointerType(CPU *cpu){
    const static struct Integral p = { 4, 1, true, false, false, false, false};
    return p;
}

const struct Integral &CharType(CPU *cpu){
    const static struct Integral c = { 1, 0, false, false, false, false, false};
    return c;
}

const struct Integral &IntType(CPU *cpu){
    const static struct Integral c = { 4, 0, true, false, false, false, false};
    return c;
}

// Note that this is slightly reordered. I figure that 'int' and 'unsigned' are the two most common types.
bool IsType(Files file){
return
    Peek("int", file) || Peek("unsigned", file) ||
    Peek("char", file) || Peek("double", file) || 
    Peek("float", file) || Peek("short", file) || 
    Peek("long", file) || Peek("signed", file);
}

bool IsFPUType(Files file){
    if(Peek("float", file) || Peek("double", file)) return true;
    if(Peek("long", file)){
        Match("long", file);
        bool is_long_double = Peek("double", file);
        UnMatch("long", file);
        return is_long_double;
    }
    return false;
}

//   <type>     ::= <fpu_type> | <int_type>
void Type(struct Integral &into, Files file){
    if(IsFPUType(file))
        FPUType(into, file);
    else
        IntType(into, file);
}

//   <fpu_type> ::= float | double
void FPUType(struct Integral &into, Files file){
    into.fpu = true;
    if(Peek("float", file)){
        Match("float", file);
        into.size = 4;
        
    }
    else if(Peek("double", file)){
        Match("double", file);
        into.size = 8;
    }
    else{
        Match("long", file);
        Match("double", file);
        into.size = 16;
    }
}

// We preserve char here to allow for single chars to be unsigned.
//   <int_type> ::= <sizing> [int] | <signage> [<sized_int>] | char | int
void IntType(struct Integral &into, Files file){
    if(Peek("char", file)){
        Match("char", file);
        into.size = 1;
        into.is_signed = false;
    }
    else if(Peek("short", file) || Peek("long", file)){
        TypeSizing(into, file);
        if(Peek("int", file))
            Match("int", file);
    }
    else if(Peek("int", file)){
        Match("int", file);
        into.size = 4;
        into.is_signed = true;
    }
    else{
        TypeSignage(into, file);
        if(IsType(file))
            SizedIntType(into, file);
        else
            into.size = 4;
    }
}

//   <sized_int>::= <sizing> [int] | int | char
void SizedIntType(struct Integral &into, Files file){
    if(Peek("char", file)){
        Match("char", file);
        into.size  = 1;
    }
    else if(Peek("int", file)){
        Match("int", file);
        into.size = 4;
    }
    else{
        TypeSizing(into, file);
        if(Peek("int", file))
            Match("int", file);
    }
}

//   <signage>  ::= signed | unsigned
void TypeSignage(struct Integral &into, Files file){
    if(Peek("signed", file)){
        Match("signed", file);
        into.is_signed = true;
    }
    else{
        Match("unsigned", file);
        into.is_signed = false;
    }
}

//   <sizing>   ::= short | long | long long
void TypeSizing(struct Integral &into, Files file){
    if(Peek("long", file)){
        Match("long", file);
        if(Peek("long", file)){
            Match("long", file);
            into.size = 16;
        }
        else {
            into.size = 8;
        }
    }
    else{
        Match("short", file);
        into.size = 2;
    }
}

}