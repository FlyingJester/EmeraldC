#pragma once
#include "function.hpp"
#include "io.hpp"
#include "CPU.hpp"

namespace Compiler {

/*

We will define global variables and scoped variables in different functions.
Although the parsing is almost/completely identical, the actual behavious is
vastly different.

Perhaps a shared parsing routine, that returns an instance of struct Variable?

This is right?
<program>         ::= <global_declaration>* <end_of_input>
<global_declaration> ::= <global_variable> ; | <global_function>
<global_function> ::= <function_definition> | <forward_declaration>
<global_variable> ::= extern <type> <variable_name> | <typed_declaration>

<forward_declaration> ::= <function_declaration> ;
<function_declaration> ::= <type> <function_name> (  [<type> [<variable>] ] [, <type> [<variable>] ]* )
<function_definition> ::= <function_declaration> { <operation> * }

<operation>       ::= <block> | <control>
<block>           ::= { <logical_line>* } | <logical_line>
<logical_line>    ::= <logical_statement> ;
<logical_statement> ::= <statement> [, <statement> ] | <typed_declaration>
<statement>       ::= <assignment> | <bool_statement> | <control_operator>
<assignment>      ::= <variable> = <bool_statement>


Questionable.
<typed_declaration>::= <type> <indirection_declaration> [, <indirection_declaration>]
<indirection_declaration> ::= <[*]+> <symbol_declaration>
<symbol_declaration> ::= <declaration> [ ( [<type> <variable>] [, <type> <variable>]* ) ]
<declaration>     ::= <variable> | <assignment>


NOTE: Do we really need to handle commas here?
<bool_statement>  ::= <bool_expression> [, <bool_expression>]*
<bool_expression> ::= <bool_term> [<log_op> <bool_term]*
<bool_term>       ::= <bool_factor>
<bool_factor>     ::= <bool> | <relation>
<bool>            ::= true | false

<relation>        ::= | <math_bits> [<relate_op> <math_bits>]
<math_bits>       ::= <math_expression> [<bit_op> <math_expression>]*
<math_expression> ::= <math_term> [<add_op> <math_term>]*
<math_term>       ::= <math_signed_factor> [<mult_op> <math_signed_factor>]*
<math_signed_factor> ::= [-] <math_factor>
<factor>          ::= <call> | <index> | <number_literal> | <variable> | ( <bool_expression> )
<identifier>      ::= <variable> [<call> | <index>]
<index>           ::= <variable> [ <bool_statement> ]

<call>            ::= <variable> ( [<bool_statement>] [, <bool_statement>]* )

<control>         ::= <if> | <while> | <do> | <for>

<control_operator> ::= <break> | <return>

<if>    ::= if ( <logical_statement> ) <block> [elif] *
<elif>  ::= [else <if>] [ <else> ]
<else>  ::= else <block>

<while> ::= while ( <logical_statement> ) block
<do>    ::= do <block> while ( <logical_statement> ) ;
<for>   ::= for ( <logical_line> <logical_line> <logical_statement> ) <block>

<break> ::= break
<return>::= return ( <logical_statement> )

*/

void Program(CPU *cpu, Files file);

// Retrieves the types and names to be passed to GlobalFunction and GlobalVariable.
//   Note that indirection operators are left when calling GlobalVariable, but 
//   swallowed when calling GlobalFunction. Hence, return_type for GlobalFunction 
//   will include indirection if it exists, but type for GlobalVariable never will.
//   This is also why we don't pass the name to GlobalVariable.
void GlobalDeclaration(CPU *cpu, Files file);
void GlobalFunction(const struct Integral &return_type, const std::string &name, CPU *cpu, Files file);
void GlobalVariable(const struct Integral &type, CPU *cpu, Files file);

// Constructs the function to be passed to ForwardDeclaration and FunctionDefinition
void FunctionDeclaration(const struct Integral &return_type, const std::string &name, CPU *cpu, Files file);

// These are templated, and so are internal.
void ForwardDeclaration(const Function &func, CPU *cpu, Files file);
void FunctionDefinition(const Function &func, CPU *cpu, Files file);

void Block(CPU *cpu, Files file);

void Operation(CPU *cpu, Files file);
void LogicalLine(CPU *cpu, Files file);
void LogicalStatement(CPU *cpu, Files file);
void Statement(CPU *cpu, Files file);
void Assignment(CPU *cpu, Files file);

void Identifier(CPU *cpu, Files file);
// Note that this call will error out if the variable is not known yet.
//   Declarations must back off the name to call this.
std::string Variable(CPU *cpu, Files file);

bool IsControlOperator(Files file);
void ControlOperator(CPU *cpu, Files file);

bool IsControl(Files file);
void Control(CPU *cpu, Files file);

void If(CPU *cpu, Files file);
void While(CPU *cpu, Files file);
void DoWhile(CPU *cpu, Files file);
void For(CPU *cpu, Files file);
void Break(CPU *cpu, Files file);
void Return(CPU *cpu, Files file);

}