/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Creating Processes
	
	Description: fd-offset-fork.c - Simple fork example, file descriptor offset vs. fork
*/

// Required includes
// New uses
#include <unistd.h>     // Standard include: fork(), getpid(), getppid(), sleep()
#include <sys/types.h>  // System Types: pid_t
#include <stdlib.h>     // Standard Library: srandom(), random()
// Previous uses
#include <fcntl.h>
#include <stdio.h>      // Standard I/O
#include <sys/stat.h>   // System File System

// Global Constants and Variables
#define STD_ERROR_VALUE -1

// Function prototypes

// Main function, demonstrate simple calling of fork
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	pid_t pidFork;      // PID returned from fork
	int i;
	int fdOut;
	// Perform function
	// Setup output file
	fdOut = open("fd-offset-fork.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if ( fdOut == STD_ERROR_VALUE ) {
		fprintf( stderr, "ERROR: open failed\n");
		return 1;
	}
	
	pidFork = fork();
	
	// Setup randomness
	srandom( (unsigned int) getpid() );  // Weak random generator seed
	// Explore (outside of class): man 3 srandom
	
	if ( pidFork == STD_ERROR_VALUE ) {
		fprintf( stderr, "ERROR: fork failed\n");
		return 2;
	} else if ( pidFork == 0 ) {  // Child
		for ( i = 0 ; i < 5 ; i++ ) {
			write( fdOut, "c", 1);
			sleep(1 + ( random() & 3 ) );  // Bitwise AND to perform quick and sloppy "modulo"
			// Explore (outside of class): man 3 sleep
			// Explore (outside of class): man 3 random
		}
	} else {  // Parent
		for ( i = 0 ; i < 5 ; i++ ) {
			write( fdOut, "P", 1);
			sleep(1 + ( random() & 3 ) );
		}
	}
	
	close(fdOut);
	
	// Return to caller
	return 0;
}
