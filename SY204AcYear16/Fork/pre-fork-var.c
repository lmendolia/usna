/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Creating Processes
	
	Description: pre-fork-var.c - Simple fork example demonstration of child process memory space
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
	
	int intExample = 203;
	// Perform function
	
	pidFork = fork();
	
	// What do you expect the parent process to output?
	// What do you expect the child process to output?
	
	if ( pidFork == -1 ) {
		fprintf( stderr, "ERROR: fork failed\n");
		return 1;
	} else if ( pidFork == 0 ) {
		fprintf( stdout, "Child(%ld):\t%d\n", (long) getpid(), --intExample);
	} else {
		fprintf( stdout, "Parent(%ld):\t%d\n", (long) getpid(), ++intExample);
	}
	
	// Return to caller
	return 0;
}
