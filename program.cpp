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

    void emit(CPU *cpu) const{
        cpu->Label(str_);
    }

    static const JumpLabeller<true> breakTo(Files file){
        static_assert(breakable, "You can only gather back to a breakable loop.");
        if(i==0){
            Abort("breaking jump must be inside a loop.", file);
        }
        return JumpLabeller(i-1);
    }

    void jump(CPU *cpu) const {
        cpu->Jump(str_);
    }
    void jumpNotZero(CPU *cpu) const {
        cpu->JumpNotZero(str_);
    }
    void jumpZero(CPU *cpu) const {
        cpu->JumpZero(str_);
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

void Program(CPU *cpu, Files file){
    cpu->Label("main");

    do{
        Block(cpu, file);
    }while(!(EndOfInput(file)||(Peek()=='@')));

    cpu->Exit();

}

void Statement(CPU *cpu, Files file){
    if(Peek("if", file))
        If(cpu, file);
    else if(Peek("while", file))
        While(cpu, file);
    else if(Peek("do", file))
        DoWhile(cpu, file);
    else if(Peek("break", file))
        Break(cpu, file);
    else if(Peek("for", file))
        For(cpu, file);
    else
        Arithmetic::Assignment(cpu, file);
//        Boolean::Expression(emit, file);
}

void Block(CPU *cpu, Files file){
    if(Peek()=='{'){
        Match('{', file);
        while(Peek()!='}')
            Statement(cpu, file);
        Match('}', file);
    }
    else
        Statement(cpu, file);

}

void Conditional(CPU *cpu, Files file){
    Match('(', file);
    Boolean::Expression(cpu, file);
    Match(')', file);
}

void If(CPU *cpu, Files file){
    Match("if", file);
    JumpLabeller<false> label;

    Conditional(cpu, file);

    label.jumpZero(cpu);

    Block(cpu, file);

    if(Peek("else", file)){
        Match("else", file);
        
        JumpLabeller<false> not_else_label;
        
        not_else_label.jump(cpu);
        label.emit(cpu);
        Block(cpu, file);
        not_else_label.emit(cpu);
    }
    else{
        label.emit(cpu);
    }

}

void While(CPU *cpu, Files file){
    JumpLabeller<false> start;
    JumpLabeller<true> end;

    Match("while", file);
   
    start.emit(cpu);

    Conditional(cpu, file);

    end.jumpZero(cpu);

    Block(cpu, file);
    
    start.jump(cpu);
    end.emit(cpu);
}

void DoWhile(CPU *cpu, Files file){
    JumpLabeller<false> start;
    JumpLabeller<true> end;

    Match("do", file);

    start.emit(cpu);
    Block(cpu, file);

    Match("while", file);
    
    Conditional(cpu, file);
    start.jumpNotZero(cpu);
    end.emit(cpu);
}

void For(CPU *cpu, Files file){
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
        
    Boolean::Expression(cpu, file);

    Match(';', file);
    
    tester.emit(cpu);

    Boolean::Expression(cpu, file);

    end_iter.jumpNotZero(cpu);
    end.jump(cpu);

    Match(';', file);
    
    start_iter.emit(cpu);
    
    Boolean::Expression(cpu, file);
    
    Match(')', file);
    
    tester.jump(cpu);
    end_iter.emit(cpu);

    Block(cpu, file);

    start_iter.jump(cpu);
    end.emit(cpu);
}

void Break(CPU *cpu, Files file){
    Match("break", file);
    JumpLabeller<true>::breakTo(file).jump(cpu);
}

}
