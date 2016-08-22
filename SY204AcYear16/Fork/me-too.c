/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Creating Processes

	Description: me-too.c - Simple fork example, parent or child
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
	pid_t pidFork;      // PID returned from fork
	// Perform function

	pidFork = fork();

	if (pidFork == -1) {
		fprintf( stderr, "ERROR: fork failed\n");
		return 1;
	} else if (pidFork == 0) {  // UPDATE NEEDED
		fprintf( stdout, "I'm the child, my parent is:\t%ld\n", (long) getppid() );
	} else {
		fprintf( stdout, "I'm the parent, my child is:\t%ld\n", (long) pidFork );  // UPDATE NEEDED
	}

	// Return to caller
	return 0;
}
