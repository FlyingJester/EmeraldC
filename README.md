# Emerald C
Simple Compiler for (somewhat) C

### What is Emerald C?

Emerald C is a compiler. It's language is a lot like C, although it is not
even close to compliant to ANSI or any other C standards. It is not even close
to finished yet.

Emerald C is aimed to some day be ANSI C compliant, and after that to be C89
and possible C11 compliant.

It is based in part on the tutorial by Jack Crenshaw at 
http://compilers.iecc.com/crenshaw/ but there are marked differences. 
Obviously, Emerald C uses C syntax rather than a more Pascal-like one. Emerald
C also outputs MIPS or amd64 instead of 68k ASM.

The amd64 code generator is known to be wrong.

Emerald C has an optimizer that targets simple peephole optimizations. It only
works with the MIPS target currently.

### How Emerald C Generates Code

Unlike some more complex compilers, Emerald C does not have an explicit
bytecode or intermediate representation. Rather, an abstract CPU interface
is defined (CPU.hpp) that is called by the parser. This is interface then
outputs a series of target-specific op codes, which are written using the
Flush method. How exactly the virtual CPU operations are translated into
actual Assembly statements is totally independant of the parser and the
actual Assembly writing features.

The actual CPU interface is currently implemented as very simple calls
that immediately output op codes, a so-called pure compiler.

### License
Emerald C is subject to the terms of the Mozilla Public License, v. 2.0. If a
copy of the MPL was not distributed with this file, You can obtain one at 
http://mozilla.org/MPL/2.0/.
