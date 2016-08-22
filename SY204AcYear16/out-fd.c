/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Program Execution
	
	Description: out-fd.c - Simple exec demonstration, demonstrates how file descriptors are affected by exec
	
*/

// Required includes
// New uses
// Previous uses
#include <stdio.h>      // Standard I/O
#include <unistd.h>     // POSIX Standard
#include <sys/types.h>  // System Types

// Global Constants and Variables
#define STD_ERR_RETURN	-1

// Function prototypes
void usage(void);

int main(int argc, char *argv[], char *envp[]) {
	// Declares required variables
	int fdOut;
	// Performs function
	if ( argc != 2 ) {
		usage();
		return 0;
	}
	fdOut = atoi(argv[1]);
	if ( write(fdOut, "Operations\n", 11) == STD_ERR_RETURN ) {
		return 5;
	}
	return 0;
}

void usage(void) {
	// Preforms function
	fprintf(stdout, "SYNPOSIS\n");
	fprintf(stdout, "\tout-fd <FD>\n");
	return;
}
