C BOOTSTRAP STAGE 5

Stage 5 implements a simple preprocessor, cpp, which is almost entirely
compliant with the C90 standard.  (The only known deviations from the
standard are that it fails to handle white-space correctly in
stringification, doesn't implement digraphs or trigraphs,
or the ... punctuator.)

  Usage: cpp [OPTIONS] FILENAME

  Options:
    --help              Displays the help text
    -o FILENAME         Specifies the output file name (default: stdout)
    -I DIRECTORY        Appends a directory to the header search path
    -D NAME[=VAL]       Pre-defines a macro, optionally with a value
    --include FILENAME  Prefixes the specified file to the input
    -P                  Don't put #line directives in output

Finally, a compiler driver called cc has been written to simplify the
use of the four build tools (cpp, ccx, as and ld).  

  Usage: cc [OPTIONS] FILES...

  Options:
    --help              Displays the help text
    -o FILENAME         Specifies the output file name
    -E                  Halt after preprocessing, generating .i files
    -S                  Halt after compiling, generating .s files
    -c                  Halt after assembling, generating .o files
    -I DIRECTORY        Appends a directory to the header search path
    -D NAME[=VAL]       Pre-defines a macro, optionally with a value
    --nostdlib          Do not link against crt0.o and libc.o
    --with-cpp=PROGRAM  Use the specified program as the preprocessor
    --with-ccx=PROGRAM  Use the specified program as the compiler
    --with-as=PROGRAM   Use the specified program as the assembler
    --with-ld=PROGRAM   Use the specified program as the linker

Input files are distinguished using their extensions.  A .c file as
assumed to be a C file that needs preprocessing; a .i file is assumed
not to require preprocessing; a .s file is assumed to be in assembly;
and a .o file is assumed to be an object file.

The compiler driver instructs the preprocessor to search the include/
directory.  The __DATE__ and __TIME__ macros are also defined by the 
driver and passed to the preprocessor viathe command line.


