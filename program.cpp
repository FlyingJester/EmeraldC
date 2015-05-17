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
    else if(Peek("for", file))
        For(emit, file);
    else
        Arithmetic::Assignment(emit, file);
//        Boolean::Expression(emit, file);
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

void For(Emitter *emit, Files file){
    JumpLabeller<true> end;
    JumpLabeller<false> start_iter, end_iter, tester;

// A for loop looks like this:
// for(
//   init ;
//   condition ;
//   iter )
//   block
//
// Including the labels used in Compiler:
//
// for(
//   init ;
//     tester:
//   condition ;
//     start_iter:
//   iter )
//     end_iter:
//   block
//   end:
// 
// With jumps:
//
// for(
//   init ;
//     tester:
//   if(
//   condition ;
//   ) goto end_iter;
//     goto end;
//     start_iter:
//
//   iter ) 
//     goto tester; 
//     end_iter:
//   block
//   end:
//
    Match("for", file);
    Match('(', file);
        
    Boolean::Expression(emit, file);

    Match(';', file);
    
    tester.emit(emit);

    Boolean::Expression(emit, file);

    end_iter.jumpNotZero(emit);
    end.jump(emit);

    Match(';', file);
    
    start_iter.emit(emit);
    
    Boolean::Expression(emit, file);
    
    Match(')', file);
    
    tester.jump(emit);
    end_iter.emit(emit);

    Block(emit, file);

    start_iter.jump(emit);
    end.emit(emit);
}

void Break(Emitter *emit, Files file){
    Match("break", file);
    JumpLabeller<true>::breakTo(file).jump(emit);
}

}
