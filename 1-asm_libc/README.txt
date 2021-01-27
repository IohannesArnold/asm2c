C BOOTSTRAP STAGE 1

In this stage we provide a trivial I/O library that implements the basic
C I/O functions in an unbuffered manner, doing one syscall per call to
getchar() or putchar().  Similarly, malloc() is implemented by sending
each allocation request to the kernel as ammap(MAP_ANON) call. These are
linked together with ld -r into a proto-C-library, asmlibc.o. There is
also a trivial startup file, crt0.o, that implements _start() by
calling exit(main()). 

Linking a program with these is typically achieved by the command:

  ld -o prog libc.o crt0.o file1.o file2.o ...

