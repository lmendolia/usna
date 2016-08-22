/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Pipes
	
	Description: pipe-desc.c - Pipes are file descriptors and show up in a processes file descriptor table
*/

// Required Includes
// New Uses
#include <unistd.h>  // UNIX Standard Library: pipe(2)
// Previous Uses
#include <fcntl.h>   // File Descriptor Control Library:
#include <stdio.h>   // Standard I/O Library: fprintf(3)

// Global Constants
#define PIPE_READ_FROM	0
#define PIPE_WRITE_TO	1
#define MAX_BUFF_LEN	1024

// Function Prototypes

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intFailure, intDataPipe, pfdPipe[2];
	int fdFile;
	char buffPipe[MAX_BUFF_LEN];
	// Perform Function
	// Setup descriptors
	if ( pipe(pfdPipe) == -1 ) {
		return 1;  // ERROR: Unable to create pipe
	}
	fdFile = open("pipe-desc.c", O_RDONLY);
	
	fprintf(stdout, "Pipe Read:\t%d\n", pfdPipe[0]);   // File descriptor: ???
	fprintf(stdout, "Pipe Write:\t%d\n", pfdPipe[1]);  // File descriptor: ???
	fprintf(stdout, "File Desc:\t%d\n", fdFile);       // File descriptor: ???
	
	// Return to Caller
	return 0;
}
