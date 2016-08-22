/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Exit Handling

	Description: procExit.c - Basic exit handlers using atexit(3)

	Usage: Use compiler options to define EXAMPLE accordingly
	  Example: $ gcc -Wall -DEXAMPLE=1 procExit.c ...
*/

// Required Includes
// Previous Uses
#include <stdio.h>   // Standard I/O Library: printf(3)
// New Uses
#include <stdlib.h>  // Standard Library: atexit(3)
#include <unistd.h>  // UNIX Standard Library: _exit(2)

// Global Constants

// Function Prototypes
void foo(void);
void bar(void);
void shorted(void);

// main Function: Register exit handlers and exit
int main(int argc, char *argv[]) {
	#if EXAMPLE == 1
		atexit(foo);
		exit(0);
	#elif EXAMPLE == 2
		atexit(foo);
		_exit(0);
	#elif EXAMPLE == 3
		atexit(foo);
		atexit(bar);
		exit(0);
	#elif EXAMPLE == 4
		atexit(foo);
		atexit(shorted);
		atexit(bar);
		exit(0);
	#elif EXAMPLE == 5
		// Student Activity: Is it LIFO or FIFO? (or We're in the pipe 5-by-5)
		// 1. Define an exit handler called: myHandler (see below)
		// 2. Register the appropriate exit handlers such that the output of the program is:
		// Keep Calm, and Exit On
		// Exiting: In foo()
		// Keep Calm, and Exit On
		// Exiting: In bar()
		// Exiting: In foo()
		// 3. Exit with a return value of 5

	#endif
	return 1;
}

// foo (Exit Handler)
void foo(void) {
    printf("Exiting: In foo()\n");
}

// bar (Exit Handler)
void bar(void) {
    printf("Exiting: In bar()\n");
}

// shorted (Exit Handler)
void shorted(void) {
    printf("Exiting: In shorted()\n");
    _exit(0);
}

// myHandler (Exit Handler): Output to standard output "Keep Calm, and Exit On"
