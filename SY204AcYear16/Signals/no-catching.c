/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Program Execution
	
	Description: no-catching.c - Simple exec demonstration, demonstrates how signal handling is affected by exec
	
*/

// Required includes
// New uses
// Previous uses
#include <stdio.h>      // Standard I/O
#include <unistd.h>     // POSIX Standard
#include <sys/types.h>  // System Types

int main(int argc, char *argv[], char *envp[]) {
	// Declares required variables
	// Performs function
	printf("(%ld) Paused for signaling...\n", (long) getpid());
	pause();  // Pause for signaling
	printf("(%ld) Signal received?\n", (long) getpid());
	return 0;
}
