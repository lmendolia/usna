/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Pipes

	Description: pipe-sizer.c - Simple program to determine how many bytes a pipe supports on the system
*/

// Required Includes
// New Uses
#include <fcntl.h>   // File Descriptor Control Library: fcntl(2)
#include <unistd.h>  // UNIX Standard Library: pipe(2)
// Previous Uses
#include <errno.h>   // Errno Library: E*, errno
#include <stdio.h>   // Standard I/O Library: fprintf(3)
#include <string.h>  // String Library: strerror(3)

// Global Constants
#define PIPE_READ_FROM	0
#define PIPE_WRITE_TO	1
#define MAX_BUFF_LEN	1024
#define STD_ERR_RETURN	-1

// Function Prototypes

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturned, intDataPipe, pfdPipe[2];
	int intBytesWritten = 0;
	int intErrNum;
	char buffPipe[MAX_BUFF_LEN];
	// Perform function

	// Create pipe
	intReturned = pipe(pfdPipe);
	if ( intReturned == STD_ERR_RETURN ) {  // ERROR: Unable to create pipe
		return 1;
	}

	// Activity 1: More Piping (or Practice Makes Perfect - Pipes)
	//  1. Complete the code below based on the comments
	// Questions:
	//  1.1. What is the default size, in bytes, of a pipe on Linux? ???

	// Activity 2: I Don't Like Waiting
	//  1. Forth coming
	  // Activity 2: I Don't Like Waiting code addition
		intReturned = fcntl(pfdPipe[PIPE_WRITE_TO], F_SETFL, O_NONBLOCK);
		if (intReturned = STD_ERR_RETURN){
			//error
		}
	// Write to the pipe until full
	while ( write( pfdPipe[PIPE_WRITE_TO], "0", 1) > 0 ) {
		intBytesWritten++;
		fprintf(stdout, "%d\n", intBytesWritten);
	}
	// Question: What happens (default) once a pipe is full and another write is attempted?
	// Answer: Write Fails or Write Blocks
	intErrNum = errno;
	fprintf(stderr, "ERROR: write failed - (%d) %s\n", intErrNum, strerror(intErrNum));

	// Close write end of pipe to signify End-Of-File (EOF)
	close( pfdPipe[PIPE_WRITE_TO]);

	intDataPipe = read(pfdPipe[PIPE_READ_FROM], buffPipe, 15);
	write( STDOUT_FILENO, buffPipe, intDataPipe);

	// Close read end of pipe to manage file descriptor usage
	close(pfdPipe[PIPE_READ_FROM]);

	// Return to Caller
	return 0;
}
