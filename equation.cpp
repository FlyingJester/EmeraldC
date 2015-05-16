#include "equation.hpp"
#include "arithmetic.hpp"

namespace Compiler {

void GreaterThan(Emitter *emit, Files file){
    Match('>', file);
    Arithmetic::Expression(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}}); 
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"sge", {"$t1", "$t1", "$t2"}});
}

void GreaterThanOrEqual(Emitter *emit, Files file){
    Match(">=", file);
    Arithmetic::Expression(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});  
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"sge", {"$t1", "$t1", "$t2"}});
}

void LessThan(Emitter *emit, Files file){
    Match('<', file);
    Arithmetic::Expression(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});  
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"slt", {"$t1", "$t1", "$t2"}});
}

void LessThanOrEqual(Emitter *emit, Files file){
    Match("<=", file);
    Arithmetic::Expression(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"sle", {"$t1", "$t1", "$t2"}});
}

void Equal(Emitter *emit, Files file){
    Match("==", file);
    Arithmetic::Expression(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"seq", {"$t1", "$t1", "$t2"}});
}

void NotEqual(Emitter *emit, Files file){
    Match("!=", file);
    Arithmetic::Expression(emit, file);
    EmitLine(emit, {"lw", {"$t2", "0($sp)"}});
    EmitLine(emit, {"addiu", {"$sp", "$sp", "4"}});
    EmitLine(emit, {"sne", {"$t1", "$t1", "$t2"}});
}

}
