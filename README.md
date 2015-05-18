# Emerald C
Simple Compiler for (somewhat) C

### What is Emerald C?

Emerald C is a compiler. It's language is a lot like C, although it is not
even close to compliant to ANSI or any other C standards. It is not even close
to finished yet.

It is based in part on the tutorial by Jack Crenshaw at 
http://compilers.iecc.com/crenshaw/ but there are marked differences. 
Obviously, Emerald C uses C syntax rather than a more Pascal-like one. Emerald
C also outputs MIPS or amd64 instead of 68k ASM.

Emerald C has an optimizer that targets simple peephole optimizations. It only
works with the MIPS target currently.

### License
Emerald C is subject to the terms of the Mozilla Public License, v. 2.0. If a
copy of the MPL was not distributed with this file, You can obtain one at 
http://mozilla.org/MPL/2.0/.
