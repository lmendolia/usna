/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Program Execution
	
	Description: exec-sig.c - Simple exec demonstration, demonstrates how signal handling is affected by exec
	
*/

// Required includes
// New uses
#include <unistd.h>   // POSIX Standard: exec() (family of functions)
// Previous uses
#include <errno.h>    // Error Handling
#include <signal.h>   // Signaling
#include <stdio.h>    // Standard I/O
#include <sys/types.h>  // System Types

// Global Constants and Variables
#define STD_ERR_RETURN	-1

// Function prototypes
void simpSigHandler(int intSigNum);

// Main function
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	struct sigaction sigHandler;
	// Perform function
	// Activity: exec vs Signal Handling
	//   Explore: no-catching.c
	//   Compile no-catching
	//   Explore: exec-sig.c
	//   Run exec-sig
	//   Interesting Observations: ???
	//   Explain the Results: ???
	
	// Setup signal handling
	sigHandler.sa_handler = &simpSigHandler;
	sigHandler.sa_flags = SA_RESTART;
	if ( sigaction(SIGUSR1, &sigHandler, NULL) == STD_ERR_RETURN ) {
		return 1;
	}
	
	printf("(%ld) Paused for signaling...\n", (long) getpid());
	pause();  // Pause for signal
	
	execl("no-catching", "no-catching", (char *) NULL);
	// Return to caller
	return 2;
}

// Example simple signal handler
void simpSigHandler(int intSigNum) {
	// Declare required variables
	int intSavedErrNum;
	// Perform function
	intSavedErrNum = errno;
	write(STDOUT_FILENO, "Signal received loud and clear.\n", 32);
	errno = intSavedErrNum;
	return;
}
