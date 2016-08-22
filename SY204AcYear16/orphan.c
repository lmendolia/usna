/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Monitoring Child Processes
	
	Description: orphan.c - Demonstrates a child process becoming an orphan when the parent process dies.
*/

// Required includes
// New
// Previously Used
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Global Constants and Variables

// Function prototypes

// Main function
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	pid_t pidFork;      // PID returned from fork
	int intPausePoint;  // Hard Coded Pause Point
	// Perform function
	pidFork = fork();
	
	if ( pidFork < 0 ) {  // ERROR
		return 1;
	} else if ( pidFork == 0 ) {  // Child
		printf("Child:\t%ld\tParent:\t%ld\n", (long) getpid(), (long) getppid());
		pause();
	} else {  // Parent
		sleep(3);  // Naive chance for child process to run first
		printf("Parent:\t%ld\tChild:\t%ld\n", (long) getpid(), (long) pidFork);
		printf("Parent: Hard Coded Pause Point\n");
		scanf("%d", &intPausePoint);
		// Ctrl-z
		// $ ps -fu username
		// $ fg 1
		printf("Parent Terminates\n");
		// $ ps -fu username
	}
	
	// Return to caller
	return 0;
}
