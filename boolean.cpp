#include "boolean.hpp"
#include "arithmetic.hpp"
#include "error.hpp"

namespace Compiler {
namespace Boolean {

void Expression(Emitter *emit, Files file){
    Term(emit, file);
    while(Peek("&&", file) || Peek("||", file)){
        if(Peek("&&", file)){
            And(emit, file);
        }
        else if(Peek("||", file)){
            Or(emit, file);
        }
        Term(emit, file);
    }
}

void Term(Emitter *emit, Files file){
    OpFactor(emit, file);
}

void OpFactor(Emitter *emit, Files file){
    Factor(emit, file);
}

void Factor(Emitter *emit, Files file){
    if(Peek("true", file))
        True(emit, file);
    else if(Peek("false", file))
        False(emit, file);
    // Binary Variable right here...
    else
        Arithmetic::Relation(emit, file);
}

void True(Emitter *emit, Files file){
    Match("true", file);
    EmitLine(emit, {"li", {"$t1", "1"}});
}

void False(Emitter *emit, Files file){
    Match("false", file);
    EmitLine(emit, {"li", {"$t1", "0"}});
}

void And(Emitter *emit, Files file){
    Match("&&", file);
    Term(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    // In case of arithmetic $t2, cast to boolean.
    EmitLine(emit, {"sgt", {"$t2", "$t2", "$zero"}});

    EmitLine(emit, {"and", {"$t1", "$t1", "$t2"}});
}

void Or(Emitter *emit, Files file){
    Match("||", file);
    Term(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    // In case of arithmetic $t2, cast to boolean.
    EmitLine(emit, {"sgt", {"$t2", "$t2", "$zero"}});

    EmitLine(emit, {"or", {"$t1", "$t1", "$t2"}});
}

// Don't use this yet.
void Not(Emitter *emit, Files file){
    Match('!', file);
    Term(emit, file);
}

}
}