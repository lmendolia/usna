/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Program Execution

	Description: a-new-me.c - Simple exec demonstration, current process image is replaced with a new process image

	Defaults to using execlp()
	Compile using -DEXECLP: gcc -DEXECLP a-new-me.c -o a-new-me
*/

// Required includes
// New uses
#include <unistd.h>  // POSIX Standard include: exec() (family of functions)
// Previous uses
#include <stdio.h>   // Standard I/O

// Global Constants and Variables

// Function prototypes

// Main function
int main( int argc, char *argv[], char *envp[] ) {
	// Declare required variables
	// Perform function
	printf("exec()ing\n");
	// Activity 1: Add an Option
	//   Add the -a option to the call to ls in execl
	//   Compile and run a-new-me
	//   Add the -a option to the call to ls in execlp
	//   Compile using the -DEXECLP option
	//   Run a-new-me
	// Activity 2: No PATH
	//   Open a new terminal session
	//   Compile using the -DEXECLP option
	//   Enter this command in the new terminal: unset PATH
	//   Run a-new-me
	//   Interesting Observations: ???
	//   Explain the Results: ???
	// Activity 3: Subverted execlp
	//   Continuing in the terminal from Activity 2
	//   Enter this command in the terminal: /bin/cp /bin/ps ls
	//   Run a-new-me
	//   Interesting Observations: ???
	//   Explain the Results: ???
	#ifndef EXECLP
		execl("/bin/ls", "ls -a", (char *) NULL);
	#else
		execlp("ls", "ls", "-a", (char *) NULL);
	#endif
	printf("After exec()\n");
	// Return to caller
	return 0;
}
