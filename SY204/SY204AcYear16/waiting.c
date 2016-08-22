/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Monitoring Child Processes
	
	Description: waiting.c - Demonstrates the parent process waiting on a child process.
*/

// Required includes
// New
#include <signal.h>    // Signal Library: raise()
#include <sys/wait.h>  // Wait Library: wait(), waitpid(), WIFEXITED(), WIFSIGNALED(), WIFSTOPPED(), WIFCONTINUED(), ...
// Previously Used
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// Global Constants and Variables

// Function prototypes

// Main function
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	pid_t pidFork;      // PID returned from fork
	
	char flgMoreChildren = 1;
	int intChildStatus;
	pid_t pidChild;
	// Perform function
	
	pidFork = fork();
	
	if ( pidFork < 0 ) {  // ERROR
		return 1;
	} else if ( pidFork == 0 ) {  // Child
		srandom( (unsigned int) getpid() );  // Weak random generator seed
		sleep(1 + ( random() & 3));  // Quick and sloppy "modulo"
		printf("Child(%ld): Normal return/exit\n", (long) getpid());
		return 42;
	}
	
	pidFork = fork();
	
	if ( pidFork < 0 ) {  // ERROR
		return 1;
	} else if ( pidFork == 0 ) {  // Child
		srandom( (unsigned int) getpid() );  // Weak random generator seed
		sleep(1 + ( random() & 3));  // Quick and sloppy "modulo"
		printf("Child(%ld): Terminating by signal\n", (long) getpid());
		raise( SIGKILL );  // Assume no errors
		// Explore (outside of class): man 3 raise
		return 1;
	}
	
	pidFork = fork();
	
	if ( pidFork < 0 ) {  // ERROR
		return 1;
	} else if ( pidFork == 0 ) {  // Child
		srandom( (unsigned int) getpid() );  // Weak random generator seed
		sleep(1 + ( random() & 3));  // Quick and sloppy "modulo"
		printf("Child(%ld): Stopping by signal\n", (long) getpid());
		raise( SIGSTOP );
		printf("Child(%ld): Continued by signal\n", (long) getpid());
		sleep(5 + ( random() & 3));  // Quick and sloppy "modulo"
		return 13;
	}
	
	while ( flgMoreChildren ) {
		pidChild = wait( &intChildStatus);
		if ( pidChild == -1 ) {  // All My Children are dead (it was crappy day time television show anyway)
			flgMoreChildren = 0;
			continue;
		}
		// Note: You must always use the following defined wait macros for portability
		if ( WIFEXITED(intChildStatus) ) {  // True if child process called _exit(#)
			printf("Child (?): Exit with status - %d\n", WEXITSTATUS(intChildStatus) );
		}
		if ( WIFSIGNALED(intChildStatus) ) {  // True if child process terminated due to receiving a signal (no signal handler in child process)
			printf("Child (?): Terminated due to signal - %d\n", WTERMSIG(intChildStatus) );
		}
	}
	// Modify the above while loop to make use of pidChild and output the PID of the child process that the status was associated with
	
	// Return to caller
	return 0;
}
