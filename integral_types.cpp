#include "integral_types.hpp"

namespace Compiler {

// Note that this is slightly reordered. I figure that 'int' and 'unsigned' are the two most common types.
bool IsType(Files file){
return
    Peek("int", file) || Peek("unsigned", file) ||
    Peek("char", file) || Peek("double", file) || 
    Peek("float", file) || Peek("short", file) || 
    Peek("long", file) || Peek("signed", file);
}

bool IsFloatingType(Files file){
    if(Peek("float", file) || Peek("double", file)) return true;
    if(Peek("long", file)){
        Match("long", file);
        bool is_long_double = Peek("double", file);
        UnMatch("long", file);
        return is_long_double;
    }
    return false;
}

void Type(struct Integral &into, Files file){
    if(IsFloatingType(file)){
        into.fpu = true;
        FloatingType(into, file);
    }
    else{
        if(Peek("unsigned", file)){
            Match("unsigned", file);
            into.is_signed = false;
        }
        else{
            if(Peek("signed", file))
                Match("signed", file);
            into.is_signed = true;
        }
        IntegralType(into, file);
    }
}

void FloatingType(struct Integral &into, Files file){
    if(Peek("float", file)){
        Match("float", file);
        into.size = 4;
    }
    else if(Peek("double", file)){
        Match("double", file);
        into.size = 8;
    }
    else if(Peek("long", file)){
        Match("long", file);
        Match("double", file);
        into.size = 16;
    }
}

void IntegralType(struct Integral &into, Files file){
    if(Peek("char", file)){
        Match("char", file);
        into.size = 1;
    }
    else if(Peek("int", file)){
        Match("int", file);
        into.size = 4;
    }
    else
        ExtendedType(into, file);
}

void ExtendedType(struct Integral &into, Files file){
    TypeSpecifier(into, file);
    if(Peek("int", file))
        Match("int", file);
}

void TypeSpecifier(struct Integral &into, Files file){
    if(Peek("short", file)){
        Match("short", file);
        into.size = 2;
    }
    else if(Peek("long", file)){
        Match("long", file);
        if(Peek("long", file)){
            Match("long", file);
            into.size = 16;
        }
        else
            into.size = 8;
    }
    else{
        into.size = 4;
    }
}

}