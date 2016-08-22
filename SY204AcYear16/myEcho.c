/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: I/O Duplication
	
	Description: myEcho.c - Simple echo of command line arguments to standard output
*/

// Required Includes
// New Uses
#include <stdio.h>   // Standard I/O Library: sprintf(3)
// Previouse Uses
#include <string.h>  // String Library: strlen(3)
#include <unistd.h>  // UNIX Standard Library: STD*, write(2)

// Global Constants
#define MAX_STR_LEN 1024

// Function Prototypes

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int i = 1;  // Loop control
	char strStdOutFileNum[MAX_STR_LEN];
	// Perform Function
	// Echoing to STDOUT_FILENO, should be 1
	snprintf(strStdOutFileNum, MAX_STR_LEN, "%d", STDOUT_FILENO);
	write(STDERR_FILENO, "STDOUT_FILENO: ", 15);
	write(STDERR_FILENO, strStdOutFileNum, strlen(strStdOutFileNum));
	write(STDERR_FILENO, "\n", 1);
	// Echo to standard output
	while ( i < argc ) {
		write(STDOUT_FILENO, argv[i], strlen(argv[i]));
		if ( argc != ++i ) {  // As long as not on the last arg, print a space
			write(STDOUT_FILENO, " ", 1 );
		}
	}
	write(STDOUT_FILENO, "\n", 1);
	// Return to Caller
	return 0;
}
