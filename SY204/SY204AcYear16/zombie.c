/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Monitoring Child Processes
	
	Description: zombie.c - Demonstrates a child process becoming a zombie when a child process dies before the parent process.
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
		sleep(3);  // Naive chance for parent process to run first
		printf("Child:\t%ld\tParent:\t%ld\n", (long) getpid(), (long) getppid());
		printf("Child: Hard Coded Pause Point\n");
		scanf("%d", &intPausePoint);
		// Ctrl-z
		// $ ps -fu username
		// $ fg 1
		printf("Child Terminates\n");
		// Ctrl-z (Note: Your actually back in the parent process when executing these steps
		// $ ps -fu username
		// $ kill <Child PID>
		// $ ps -fu username
		// Can a zombie process be killed?
	} else {  // Parent
		printf("Parent:\t%ld\tChild:\t%ld\n", (long) getpid(), (long) pidFork);
		pause();
	}
	
	// Return to caller
	return 0;
}
