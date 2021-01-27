C BOOTSTRAP STAGE 4

Stage 4 reimplements the compiler from stage 2 in the B-like language 
of stage 2 compiler.  This allows a significantly more advanced 
implementation, with the result that it supports most of syntax of K&R
C (most notably, a type system) and is more efficient.

New features in stage 4 compiler:
  - for loops
  - goto and labelled statements
  - switch, case labels and default (implemented inefficiently as a 
      sequence of if-else statements)
  - a type system, including all of C's integer and character types, 
      pointers, arrays and function pointers
  - structs
  - member access with the -> and . operators
  - sizeof operator
  - comma operator
  - type casts
  - C++-style comments
  - typedefs
  - #line directives (in the compiler proper) for improved diagnostics

The compiler is named ccx.

  Usage: ccx [OPTIONS] FILENAME

  Options:
    --help              Displays the help text
    -o FILENAME         Specifies the output file name

If no -o option is specified and the input file has a .c, .b, or .i
extension, the output is the same file name but with a .s extension.  
