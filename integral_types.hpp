#pragma once
#include "io.hpp"

namespace Compiler {

struct Integral{
    unsigned char size; // In bytes.
    bool is_signed;
    bool fpu;
    bool is_const; // Unused
    bool is_static; // Unused
};

// C has a funny way about it.
//
// <uncanon_type> ::= <floating_type> | [<signed> | <unsigned>] integral_type
// <floating_type> ::= float | [long] double
// <integral_type> ::= char | <extended_type> | int
// <extended_type> ::= <type_specifier> [int]
// <type_specifier> ::= short | long | long long

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
void Type(struct Integral &into, Files file);
void FloatingType(struct Integral &into, Files file);
void IntegralType(struct Integral &into, Files file);
void ExtendedType(struct Integral &into, Files file);
void TypeSpecifier(struct Integral &into, Files file);

// Returns the canon name of the type specified by 'that'.
//
// Examples:
// unsigned;      => unsigned int;
// short;         => signed short;
// unsigned char; => unsigned char;
// long int;      => signed long;
std::string CanonType(const struct Integral &from);

}
