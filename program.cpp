#include "program.hpp"
#include "boolean_expression.hpp"
#include "integral_types.hpp"
#include "declaration.hpp"
#include "function.hpp"
#include "scope.hpp"
#include "io.hpp"
#include "error.hpp"

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

/*
<program>         ::= <global_declaration>* <end_of_input>
<global_declaration> ::= <global_variable> ; | <global_function>
<global_function> ::= <function_definition> | <forward_declaration>
<global_variable> ::= extern <variable> ; | <typed_declaration> ;

<forward_declaration> ::= <function_declaration> ;
<function_declaration> ::= <type> <function_name> (  [<type> [<variable>] ] [, <type> [<variable>] ]* )
<function_definition> ::= <function_declaration> { <operation> * }

<operation>       ::= { <logical_line>* } | <logical_line> | <control>
<logical_line>    ::= <logical_statement> ;
<logical_statement> ::= <statement> [, <statement> ]
<statement>       ::= <assignment> | <bool_statement> | <control_operator>
<assignment>      ::= <variable> = <bool_statement>

<bool_statement>  ::= <bool_expression> [, <bool_expression]*
<bool_expression> ::= <bool_term> [<log_op> <bool_term]*
<bool_term>       ::= <bool_factor>
<bool_factor>     ::= <bool> | <variable> | <relation>
<bool>            ::= true | false

<relation>        ::= | <math_expression> [<relate_op> <math_expression>]
<math_bits>       ::= <math_expression> [<bit_op> <math_expression>]*
<math_expression> ::= <math_term> [<add_op> <math_term>]*
<math_term>       ::= <math_signed_factor> [<mult_op> <math_signed_factor>]*
<math_signed_factor> ::= [-] <math_factor>
<factor>          ::= <call> | <index> | <number_literal> | <variable> | (<bool_expression>)
<index>           ::= <variable> [ <bool_statement> ]

<call>            ::= <variable> ( [<bool_statement>] [, <bool_statement>]* )

<control>         ::= <if> | <while> | <do> | <for>

<control_operator> ::= <break> | <return>

<if>    ::= if ( <statement> ) <block> [elif] *
<elif>  ::= [else <if>] [ <else> ]
<else>  ::= else <block>

<while> ::= while ( <statement> ) block
<do>    ::= do <block> while ( <statement> ) ;
<for>   ::= for ( <logical_line> <logical_line> <logical_statement> ) <block>

<break> ::= break
<return>::= return ( <logical_line> )
*/


// <block>           ::= { <logical_line>* } | <logical_line>
void Block(CPU *cpu, Files file){
    Scope scope(cpu, file);
    if(Peek('{', file)){
        Match('{', file);
        while(!Peek('}', file))
            LogicalLine(cpu, file);
        Match('}', file);
    }
    else
        LogicalLine(cpu, file);
}
/*
void Program(CPU *cpu, Files file){
    while((!EndOfInput(file)) && (!Peek('@', file))){
        Block(cpu, file);
    }
}
*/
// <program>         ::= <global_declaration>* <end_of_input>
void Program(CPU *cpu, Files file){
    while((!EndOfInput(file)) && (!Peek('@', file))){
        GlobalDeclaration(cpu, file);
    }
}

// <global_declaration> ::= <global_variable> ; | <function>
void GlobalDeclaration(CPU *cpu, Files file){
    struct Integral type;
    if(Peek("extern", file)){
        Match("extern", file);
        Type(type, file);
        type.is_extern = true;
        GlobalVariable(type, cpu, file);
    }
    else{
        Type(type, file);
        GatherIndirection(type, file);
        const std::string name = GetName(file);
        if(Peek('(', file)){
            GlobalFunction(type, name, cpu, file);
        }
        else{
            UnMatch(name, file);
            while(type.indirection)
                UnMatch('*', file), type.indirection--;
            GlobalVariable(type, cpu, file);
        }
    }
}

//<global_function> ::= <function_definition> | <forward_declaration>
void GlobalFunction(const struct Integral &return_type, const std::string &name, CPU *cpu, Files file){

    Function func(cpu, file, return_type, name);
    Match('(', file);
    if(IsType(file)){
        struct Integral type;
        Type(type, file);
        GatherIndirection(type, file);
        func.argv.push_back({type, (IsName(file)?GetName(file):"")});
    }
    while(Peek(',', file)){
        Match(',', file);
        struct Integral type;
        Type(type, file);
        GatherIndirection(type, file);
        func.argv.push_back({type, (IsName(file)?GetName(file):"")});
    }
    Match(')', file);
    if(Peek(';', file))
        ForwardDeclaration(func, cpu, file);
    else
        FunctionDefinition(func, cpu, file);
}

// <global_variable> ::= extern <variable> ; | <typed_declaration> ;
void GlobalVariable(const struct Integral &type, CPU *cpu, Files file){
    if(Peek("extern", file)){
        Abort("This is awkward. GlobalDeclaration is supposed to swallow the extern declaration.", file);
    }
    TypedDeclaration(type, cpu, file);
    Match(';', file);
}

// <forward_declaration> ::= <function_declaration> ;
void ForwardDeclaration(const Function &func, CPU *cpu, Files file){
    Match(';', file);
    // TODO: Log the function for later use?
}

// <function_declaration> ::= <type> <function_name> (  [<type> [<variable>] ] [, <type> [<variable>] ]* )
void FunctionDeclaration(CPU *cpu, Files file);
// <function_definition> ::= <function_declaration> { <operation> * }
void FunctionDefinition(const Function &func, CPU *cpu, Files file){
    
    cpu->Label(func.name);
    { // Force the Scope scope().
        SetFunction(func, file);
        Scope scope(func.argv, cpu, file);
    
        Match('{', file);

        while(!Peek('}', file))
            Operation(cpu, file);

        Match('}', file);

        ClearFunction(file);
    }
    cpu->Return(func);
}

// <operation>       ::= <block> | <control>
void Operation(CPU *cpu, Files file){
    if(IsControl(file))
        Control(cpu, file);
    else
        Block(cpu, file);
}

// <logical_line>    ::= <logical_statement> ;
void LogicalLine(CPU *cpu, Files file){
    LogicalStatement(cpu, file);
    Match(';', file);
}

// <logical_statement> ::= <statement> [, <statement> ] | <typed_declaration>
void LogicalStatement(CPU *cpu, Files file){
    if(IsType(file)){
        TypedDeclaration(cpu, file);
    }
    else{
        Statement(cpu, file);
        while(Peek(',', file)){
            Match(',', file);
            Statement(cpu, file);
        }
    }
}

// <statement>       ::= <assignment> | <bool_statement> | <control_operator>
void Statement(CPU *cpu, Files file){
    if(IsControlOperator(file)){
        ControlOperator(cpu, file);
    }
    else if(IsName(file)){
        const std::string name = GetName(file);
        const bool is_assign = Peek('=', file);
        UnMatch(name, file);
        if(is_assign){
            Assignment(cpu, file);
            return;
        }
        else{
            Boolean::Statement(cpu, file);
        }
    }
    else{
        Boolean::Statement(cpu, file);
    }
}

void Assignment(CPU *cpu, Files file);

// <identifier>      ::= <variable> [<call> | <index>]
void Identifier(CPU *cpu, Files file){
    const std::string name = GetName(file);
    switch(Peek()){
        case '(':
            Match('(', file);
            Match(')', file);
            cpu->Call(name);
            Abort("Implement Call. FIXME!", file);
            return;
        case '[':
            Match('[', file);
            Match(']', file);
            Abort("Implement Subscript. FIXME!", file);
            return;
        default:
            Scope::ensureVariable(name, cpu, file);
            Scope::loadVariable(name, cpu, file);
    }
}

bool IsControl(Files file){
    return
        Peek("if", file) || Peek("while", file) ||
        Peek("do", file) || Peek("for", file);
}

void Control(CPU *cpu, Files file){
    if(Peek("if", file)){
        If(cpu, file);
    }
    else if(Peek("while", file)){
        While(cpu, file);
    }
    else if(Peek("do", file)){
        DoWhile(cpu, file);
    }
    else if(Peek("for", file)){
        For(cpu, file);
    }
    else
        Expected("Control Statement", file);
}

bool IsControlOperator(Files file){
    return
        Peek("break", file) || Peek("return", file);
}

void ControlOperator(CPU *cpu, Files file){
    if(Peek("break", file)){
        Break(cpu, file);
    }
    else if (Peek("return", file)){
        Return(cpu, file);
    }
    else
        Expected("Control Operator", file);
}

// <assignment>      ::= <variable> = <bool_statement>
void Assignment(CPU *cpu, Files file){
    const std::string name = Variable(cpu, file);
    Match('=', file);
    Boolean::Statement(cpu, file);
    cpu->StoreVariable(name);
}

std::string Variable(CPU *cpu, Files file){
    const std::string name = GetName(file);
    Scope::ensureVariable(name, cpu, file);
    return name;
}

void If(CPU *cpu, Files file){
    Match("if", file);
    JumpLabeller<false> label;
    
    Match('(', file);
    
    LogicalStatement(cpu, file);

    Match(')', file);

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

    LogicalStatement(cpu, file);

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
    
    LogicalStatement(cpu, file);
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
        
    LogicalLine(cpu, file);

    Match(';', file);
    
    tester.emit(cpu);

    LogicalLine(cpu, file);

    end_iter.jumpNotZero(cpu);
    end.jump(cpu);

    Match(';', file);
    
    start_iter.emit(cpu);
    
    LogicalStatement(cpu, file);
    
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

void Return(CPU *cpu, Files file){
    Match("return", file);
    LogicalStatement(cpu, file);
    LastFunction(file).leaveInnerScope();
    cpu->Return(LastFunction(file));
}


}