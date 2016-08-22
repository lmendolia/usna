/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Program Execution
	
	Description: exec-fd.c - Simple exec demonstration, demonstrates how file descriptors are affected by exec
	
*/

// Required includes
// New uses
#include <unistd.h>      // POSIX Standard: exec() (family of functions)
// Previous uses
#include <fcntl.h>       // File Control
#include <errno.h>       // Error Handling
#include <stdio.h>       // Standard I/O
#include <sys/stat.h>    // File Characteristics
#include <sys/types.h>   // System Types

// Global Constants and Variables
#define STD_ERR_RETURN	-1

// Main function
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	int fdOut;
	char strFileDesc[10];
	// Perform function
	// Activity 1: No close Before exec
	//   Explore: out-fd.c
	//   Compile out-fd
	//   Explore: exec-fd.c
	//   Add code as necessary to have out-fd run without the usage statement appearing
	//   Compile and run exec-sig
	//   Interesting Observations: ???
	//   Explain the Results: ???
	// Activity 2: close Before exec
	//   Add code as necessary to close fdOut before execing
	//   Compile exec-fd using -DCLOSER
	//   Run exec-fd
	//   Interesting Observations: ???
	//   Explain the Results: ???
	// Activity 3: O_CLOEXEC
	//   Compile exec-fd using -DCLOSE_ON_EXEC
	//   Run exec-fd
	//   Interesting Observations: ???
	//   Explain the Results: ???
	
	// Setup file for output
	#ifndef CLOSE_ON_EXEC
		#ifndef CLOSER
			fdOut = open("exec-fd1.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		#else
			fdOut = open("exec-fd2.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		#endif
	#else
		fdOut = open("exec-fd3.txt", O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR);
		// Note: Use of O_CLOEXEC
	#endif
	if ( fdOut == STD_ERR_RETURN ) {
		return 1;
	}
	if ( snprintf(strFileDesc, 10, "%d", fdOut) == STD_ERR_RETURN ) {  // Convert fdOut (int) to char *
	  // Explore (Outside of Class): man 3 snprintf
		return 2;
	}
	if ( write(fdOut, "Cyber ", 6) == STD_ERR_RETURN ) {
		return 3;
	}
	#ifdef CLOSER
		// Close fdOut
	#endif
	execl("out-fd", "out-fd", (char *) NULL);  // *** What argument(s) does out-fd need?
	// Return to caller
	return 4;
}
