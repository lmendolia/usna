/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Pipes

	Description: howdy-pipe.c - Simple hello world through a pipe
*/

// Required Includes
// New Uses
#include <unistd.h>  // UNIX Standard Library: pipe(2)
// Previous Uses

// Global Constants
#define PIPE_READ_FROM	0
#define PIPE_WRITE_TO	1
#define MAX_BUFF_LEN	1024

// Function Prototypes

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturned, intDataPipe, pfdPipe[2];
	char buffPipe[MAX_BUFF_LEN];
	// Perform Function

	// Create a pipe
	intReturned = pipe(pfdPipe);

	// Activity 1: Handle Error
	//   Update the error handling below to correctly detect an error from creating the pipe

	if ( intReturned ) {  // ERROR: Unable to create pipe
		return 1;
	}

	// Activity 2: Pipe I/O
	//   Update the lines below to use the correct ends of the pipe based on the comments

	// Write to the pipe
	write( pfdPipe[PIPE_WRITE_TO], "Howdy world!\n", 13);

	// Close write end of pipe to signify End-Of-File (EOF)
	close(pfdPipe[PIPE_WRITE_TO]);

	// Read from the pipe, output to standard output
	do {
		intDataPipe = read(pfdPipe[PIPE_READ_FROM], buffPipe, MAX_BUFF_LEN);
	} while ( write( STDOUT_FILENO, buffPipe, intDataPipe) > 0 );

	// Close read end of pipe to manage file descriptor usage
	close(pfdPipe[PIPE_READ_FROM]);

	// Post Activity Questions:
	//  1. In what order does a pipe process data? ???
	//  Explore (outside of class): man 7 pipe

	// Return to Caller
	return 0;
}
