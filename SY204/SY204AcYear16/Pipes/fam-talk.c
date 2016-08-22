/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Pipes
	
	Description: fam-talk.c - Pipe communications between processes related via fork
*/

// Required Includes
// New Uses
#include <unistd.h>  // UNIX Standard Library: pipe(2)
// Previous Uses
#include <string.h>  // String Library: strncpy(3)

// Global Constants
#define PIPE_READ_FROM	0
#define PIPE_WRITE_TO	1
#define MAX_BUFF_LEN	1024
#define MAX_STR_LEN		1024

// Function Prototypes

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intFailure, intDataPipe, pfdToChild[2];
	pid_t pidFork;
	char buffPipe[MAX_BUFF_LEN];
	char strMsgParenToChld[MAX_STR_LEN];
	char strMsgChldToParen[MAX_STR_LEN];
	// Perform Function
	// Setup messages
	strncpy(strMsgParenToChld, "Don't do that\n", MAX_BUFF_LEN);
	strncpy(strMsgChldToParen, "But Mom!!!\n", MAX_BUFF_LEN);
	
	// Just like open, setup pipe(s) before fork
	if ( pipe(pfdToChild) == -1 ) {
		return 1;  // ERROR: Unable to create pipe
	}
	
	pidFork = fork();
	
	// Activity 1: Because I Said So
	//  1. Complete the code below to have the parent send data to the child via a pipe
	//  2. Have the child echo the data from the pipe to standard output
	//  3. You need to manage descriptors (close unused) connected to the pipe ends appropriately
	
	// Activity 2: Quivering Lips
	//  1. Add code to have the child send data to the parent via a pipe (Hint: pipes are unidirectional)
	
	switch (pidFork) {
		case -1:  // ERROR: fork failed
			return 2;
			break;
		case 0:   // Child
			// Close unused end of pipe(s)
			
			// Read data from pipe
			
			// Write read data to standard output
			
			// Activity 2: Quivering Lips
			
			// Close used end of pipe(s)
			
			break;
		default:  // Parent
			// Close unused end of pipe(s)
			
			// Write data to pipe
			
			// Activity 2: Quivering Lips
			
			// Close used end of pipe(s)
			
			break;
	}
	
	// Return to Caller
	return 0;
}
