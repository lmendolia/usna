/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Creating Processes

	Description: non-dueling.c - Simple fork example, solve shared offset by synchronizing using signals

*/

// Required includes
// New uses

// Previous uses
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>      // Standard I/O
#include <stdlib.h>     // Standard Library: srandom(), random()
#include <unistd.h>     // Standard include: fork(), getpid(), getppid(), sleep()
#include <sys/stat.h>   // System File System
#include <sys/types.h>  // System Types: pid_t

// Global Constants and Variables
#define STD_ERROR_VALUE -1

// Function prototypes
void silentSigHand(int intSigNum, siginfo_t *sigInfo, void *sysDependContext);

// Main function, demonstrate simple calling of fork
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	pid_t pidFork;      // PID returned from fork
	int i;
	int fdOut, sigRegistered;
	struct sigaction sigHandler;
	// Perform function
	// Setup signal handler
	sigHandler.sa_sigaction = &silentSigHand;
	sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
	sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
	if ( sigRegistered == STD_ERROR_VALUE ) {
		fprintf( stderr, "ERROR: sigaction failed\n");
		return 1;
	}
	// Note: Review man 7 signal, does a child process inherit signal handlers from the parent?

	// Setup output file
	fdOut = open("fd-offset-fork-2.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if ( fdOut == STD_ERROR_VALUE ) {
		fprintf( stderr, "ERROR: open failed\n");
		return 2;
	}

	pidFork = fork();

	// Setup randomness
	srandom( (unsigned int) getpid() );  // Weak random generator seed
	// Explore (outside of class): man 3 srandom

	if ( pidFork == STD_ERROR_VALUE ) {
		fprintf( stderr, "ERROR: fork failed\n");
		return 2;
	} else if ( pidFork == 0 ) {  // Child

		// Activity 1: Child Goes First
		//   Use pause() and kill(???, SIGUSR1) within the child and parent appropriately to have the child process write to the file first

		// Activity 2: Parent Goes First
		//   Use pause() and kill(???, SIGUSR1) within the child and parent appropriately to have the process process write to the file first

		pause();

		for ( i = 0 ; i < 5 ; i++ ) {

			write( fdOut, "c", 1);
			sleep(1 + ( random() & 3 ) );  // Bitwise AND to perform quick and sloppy "modulo"
			// Explore (outside of class): man 3 sleep
			// Explore (outside of class): man 3 random


		}

		//kill(getppid(), SIGUSR1);

	} else {  // Parent

		// pause()

		for ( i = 0 ; i < 5 ; i++ ) {
			write( fdOut, "P", 1);
			sleep(1 + ( random() & 3 ) );
		}

		kill(pidFork, SIGUSR1);
	}

	close(fdOut);

	// Return to caller
	return 0;
}

// Silently catch and disregard signal
void silentSigHand(int intSigNum, siginfo_t *sigInfo, void *sysDependContext) {
	// Perform function
	return;
}
