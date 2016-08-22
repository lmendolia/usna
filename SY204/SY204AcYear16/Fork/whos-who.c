/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Creating Processes

	Description: whos-who.c - Simple fork example
*/

// Required includes
// New uses
#include <unistd.h>     // Standard include: fork(), getpid(), getppid()
#include <sys/types.h>  // System Types: pid_t
// Previous uses
#include <stdio.h>      // Standard I/O

// Global Constants and Variables

// Function prototypes

// Main function, demonstrate simple calling of fork
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables

	// Perform function

	fork();

	printf("Howdy from:\t%ld\n", (long) getpid() );  // Who's who?
	//older one is probably the parent
	// Return to caller
	return 0;
}
