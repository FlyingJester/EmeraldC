#include "program.hpp"
#include "error.hpp"
#include "arithmetic.hpp"
#include "boolean.hpp"

namespace Compiler {

template<bool breakable>
class JumpLabeller {
    static unsigned n;
    static unsigned i;
    
    const std::string str_;

    JumpLabeller(unsigned a)
      : str_(std::string(breakable?"loop":"jump")+"_label_"+std::to_string(a)){
    }

public:

    JumpLabeller()
      : JumpLabeller(n){
        n++;
        i++;
    }

    ~JumpLabeller(){
        i--;
    }

    const std::string &str() const {
        return str_;
    }

    void emit(Emitter *emit_) const{
        EmitLine(emit_, {str_+":", {}});
    }

    static const JumpLabeller<true> breakTo(Files file){
        static_assert(breakable, "You can only gather back to a breakable loop.");
        if(i==0){
            Abort("breaking jump must be inside a loop.", file);
        }
        return JumpLabeller(i-1);
    }

    void jump(Emitter *emit_) const {
        EmitLine(emit_, {"j", {str_}});
    }
    void jumpNotZero(Emitter *emit_) const {
        EmitLine(emit_, {"bne", {"$t1", "$zero", str_}});
    }
    void jumpZero(Emitter *emit_) const {
        EmitLine(emit_, {"beq", {"$t1", "$zero", str_}});
    }

    

};

template<>
unsigned JumpLabeller<true>::n = 0;
template<>
unsigned JumpLabeller<false>::n = 0;
template<>
unsigned JumpLabeller<true>::i = 0;
template<>
unsigned JumpLabeller<false>::i = 0;

void Program(Emitter *emit, Files file){
    EmitLine(emit, {"main:", {}});

    do{
        Block(emit, file);
    }while(!(EndOfInput(file)||(Peek()=='@')));

    EmitLine(emit, {"li", {"$v0", "10"}});
    EmitLine(emit, {"syscall", {}});
}

void Statement(Emitter *emit, Files file){
    if(Peek("if", file))
        If(emit, file);
    else if(Peek("while", file))
        While(emit, file);
    else if(Peek("do", file))
        DoWhile(emit, file);
    else if(Peek("break", file))
        Break(emit, file);
    else
        Boolean::Expression(emit, file);
    Match(';', file);
}

void Block(Emitter *emit, Files file){
    if(Peek()=='{'){
        Match('{', file);
        while(Peek()!='}')
            Statement(emit, file);
        Match('}', file);
    }
    else
        Statement(emit, file);

}

void Conditional(Emitter *emit, Files file){
    Match('(', file);
    Boolean::Expression(emit, file);
    Match(')', file);
}

void If(Emitter *emit, Files file){
    Match("if", file);
    JumpLabeller<false> label;

    Conditional(emit, file);

    label.jumpZero(emit);

    Block(emit, file);

    if(Peek("else", file)){
        Match("else", file);
        
        JumpLabeller<false> not_else_label;
        
        not_else_label.jump(emit);
        label.emit(emit);
        Block(emit, file);
        not_else_label.emit(emit);
// The ender for this if will get swallowed by the else block.
// Put another one on to fix this. Inelegant, but effective.
        UnMatch(';', file);
    }
    else{
        label.emit(emit);
    }

}

void While(Emitter *emit, Files file){
    JumpLabeller<false> start;
    JumpLabeller<true> end;

    Match("while", file);
   
    start.emit(emit);

    Conditional(emit, file);

    end.jumpZero(emit);

    Block(emit, file);
    
    start.jump(emit);
    end.emit(emit);
}

void DoWhile(Emitter *emit, Files file){
    JumpLabeller<false> start;
    JumpLabeller<true> end;

    Match("do", file);

    start.emit(emit);
    Block(emit, file);

    Match("while", file);
    
    Conditional(emit, file);
    start.jumpNotZero(emit);
    end.emit(emit);
}

void Break(Emitter *emit, Files file){
    Match("break", file);
    JumpLabeller<true>::breakTo(file).jump(emit);
}

}
