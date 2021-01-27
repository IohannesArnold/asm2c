# x86 asm to C Bootstrapping Kit 

This project is a continuation of the
[hex2asm](https://github.com/IohannesArnold/hex2asm)
bootstrapping project. In that project, we developed a working an x86
assembler and linker from as small a binary seed as possible. Here, we
use the assembler and linker produced in that project to build up to
a C89 complient C compiler.

This programs produced in this project are 32-bit ELF executables for a
Linux kernel running on an Intel x86 processor. They run fine on modern
64-bit systems.

## Stage 1: crt0.o & asmlibc.o

We start by building a startup file (crt0.o) and a simple C library,
all written in assembly.

## Stage 2: B compiler

The project's first compiler is added at this stage. Its input language
is a typeless subset of C similar to B, and it emits assembly language.  

## Stage 3: libc.o

The B compiler is used to replace and add to certain functions in our
libc.

## Stage 4: C compiler

The compiler is rewritten in its source language, and a type system
added.

## Stage 5: cpp & cc

We use the C compiler to implement a fairly standards-compliant C 
preprocessor, and a compiler driver that spawns the cpp, ccx (the
compiler proper), as and ld.

### Licensing
The code in this project is copyright (C) Richard Smith, 2009-2020, and
is licensed for use under version 3 or later of the GNU General Public
License, a copy of which can be found in the file LICENCE.txt.  The
documentation in these README.txt files is licensed under the Creative
Commons BY-NC-SA licence, version 4.
