#pragma once
#include "io.hpp"
#include <string>

namespace Compiler {

struct Integral{
    unsigned char size, // In bytes.
        indirection; //  0 is value, 1 is address, (*), 2 is address of pointer (**) etc..
    bool is_signed;
    bool fpu;
    bool is_const; // Unused
    bool is_static; // Unused
    bool is_extern; // Unused
};

struct Variable {
    struct Integral type;
    std::string name;
};

class CPU;

// Some basic types.
const struct Integral &PointerType(CPU *cpu);
const struct Integral &CharType(CPU *cpu);
const struct Integral &IntType(CPU *cpu);

// C has a funny way about it.
//
//   <type>     ::= <fpu_type> | <int_type>
//   <fpu_type> ::= float | double
//   <int_type> ::= <sizing> [int | char] | <signage> <sized_int> | char
//   <sized_int>::= <sizing> int | <sizing> | char
//   <signage>  ::= signed | unsigned
//   <sizing>   ::= short | long | long long
//
// All type specifiers begin:
//
// float | double | unsigned | signed | char | int | short | long
//

/*
 Integral Type Sizes on amd64:
| - Name  - - - - |  Size |
+ - - - - - - - - + - - - +
| - char  - - - - | - 1 - |
| - short - - - - | - 2 - |
| - int - - - - - | - 4 - |
| - long  - - - - | - 8 - |
| - long long - - | - 16  |
+ - - - - - - - - + - - - +
| - float - - - - | - 4 - |
| - double  - - - | - 8 - |
| - long double - | - 16  |
*/

/*
 Integral Type Sizes on MIPS (Maybe?):
| - Name  - - - - |  Size |
+ - - - - - - - - + - - - +
| - char  - - - - | - 1 - |
| - short - - - - | - 2 - |
| - int - - - - - | - 4 - |
| - long  - - - - | - 4 - |
| - long long - - | - 4 - |
+ - - - - - - - - + - - - +
| - float - - - - | - 4 - |
| - double  - - - | - 4 - |
| - long double - | - 4 - |
*/

enum TypeSpecifier {};

bool IsType(Files file);

// Generates a struct Integral based on input from file.
//   <type>     ::= <fpu_type> | <int_type>
void Type(struct Integral &into, Files file);
//   <fpu_type> ::= float | double
void FPUType(struct Integral &into, Files file);
//   <int_type> ::= <sizing> [int | char] | <signage> [<sizing>] [int]
void IntType(struct Integral &into, Files file);
//   <sized_int>::= <sizing> [int] | char
void SizedIntType(struct Integral &into, Files file);
//   <signage>  ::= signed | unsigned
void TypeSignage(struct Integral &into, Files file);
//   <sizing>   ::= short | long | long long
void TypeSizing(struct Integral &into, Files file);

// Returns the canon name of the type specified by 'that'.
//
// Examples:
// unsigned;      => unsigned int;
// short;         => signed short;
// unsigned char; => unsigned char;
// long int;      => signed long;
std::string CanonType(const struct Integral &from);

}
