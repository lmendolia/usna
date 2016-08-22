/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Signal Handling

	Description: mySigCatch.c - Student activity to catch signals and perform simple output
*/

// Required Includes
// Previous Uses
#include <stdio.h>      // Standard I/O Library: fprintf(3), perror(3)
#include <stdlib.h>     // Standard Library: exit(3)
// New Uses
#include <signal.h>     // Signal Library: sigaction(2), SIG*
#include <unistd.h>     // UNIX Standard Library: getpid(2), pause(2)
#include <sys/types.h>  // System Type Library: pid_t

// Global Constants and Variables
int intSigRecvCnt;  // Naive variable available to signal handler
// Why does intSigRecvCnt not need to be initialized to zero?

// Function Prototypes
void simpSigHandler(int intSigNum);
void advSigHandler(int intSigNum, siginfo_t *sigInfo, void *sysDependContext);
// main Function: Register the signal handler, sleep until signal received
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturn = 0;    // Return variable, assume normal exit
	int intFailure = -1;  // Called failure status
	  // *** sigaction for signal handler information
	struct sigaction sigHandler;
	// Perform Function
	sigHandler.sa_flags = SA_RESTART;
	// Setup the information to register the signal handler
	  // *** Setup the function to serve as the signal handler
	sigHandler.sa_sigaction = &advSigHandler;
	sigHandler.sa_flags|= SA_SIGINFO;
	// Register the signal handler
	// Catch SIGINT (Ctrl-c)
	intFailure = sigaction(SIGINT, &sigHandler, NULL); // ***
	if ( intFailure == -1 ) {
		perror("ERROR: Failed to register signal handler");
		exit(1);
	}

	// Infinitely let user know awaiting signal
	while (1) {
		fprintf( stdout, "PID:\t%ld  Received:\t%d signals (awaiting signal)\n", (long) getpid(), intSigRecvCnt );
		/*
		 * Explore (Outside of Class): getpid(2)
		 */
		pause();
		/*
		 * Explore (Outside of Class): pause(2)
		 */
	}

	// Return to Caller
	return intReturn;
}

// Simple signal handler
void simpSigHandler(int intSigNum) {
	// Declare Required Variables
	// Perform Function
	intSigRecvCnt += 1;  // Note: ++/-- are considered UNSAFE (non-atomic)
	printf("Caught:\t%d\n", intSigNum);  // UNSAFE - Only Used for In-Class Demonstration
	/*
	 * Some functions (system calls and library functions) are considered unsafe regarding being interrupted, due to their use of static memory or dynamic data structures (SY301).
	 * Unsafe functions should not be used in production.
	 * DO NOT use unsafe functions in signal handlers for programming assignments, unless specifically authorized in the assignment.
	 * LPI lists system calls that are safe
	 */
	return;
}
void advSigHandler(int intSigNum, siginfo_t *sigInfo, void *sysDependContext){
	intSigRecvCnt += 1;
	printf("Caught:\t%d\nin the ADV signal handler", intSigNum);
	return;
}
