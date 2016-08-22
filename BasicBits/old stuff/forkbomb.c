
#include <stdio.h>	// standard i/o
#include <sys/types.h>	// system types: pid_t
#include <unistd.h>	// standard library: fork(), getpid(), getppid()

// Main function
// Child process become zombies when the child process dies before the parent process
int main( int argc, char *argv[], char *envp[] ) {

	// Declare required variables
	pid_t pidFork;      // PID returned from fork

	// Perform function

	while(1) {
	// creates child function
			pidFork = fork();

			if ( pidFork < 0 ) {  // ERROR with forking
				return 1; }

			if ( pidFork == 0 ) {  // Child function
					sleep(3);}  // Naive chance for parent process to run first: child sleeps
			}
//	else {  // Parent function
	//	pause(); }	// parent pauses

	// Return to caller
	return 0;
}
