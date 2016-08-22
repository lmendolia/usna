/*
        Name: MIDN A.N. Mendolia
        Alpha: 184242
        Course: SY204 - Sys Prgm & OS Fund
        Assignment: Programming Laboratory - 0x01: Echo the Cat

        Description: takes in arguments and prints them out with space between
        Resources: Alex Varon, Adam Montgomery, Katie Swafford, MGSP, EI
                tutorialspoint.com (how to output simple error message)
*/

// Required Includes
#include <stdlib.h>     // Standard Library: exit(3)
#include <stdio.h>      // Standard I/O Library: printf(3)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
// Global Constants

// Function Prototypes
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
      int count = 1;
      char * space = " ";
      char * newline = "\n";
      int retInt = 0;
      if (argc > 1){
      	while (count < argc){
        	retInt = write(1, argv[count], strlen(argv[count])); //fd, buffer, bytes
		//need a loop so there isn't a space after last argument
		if (retInt == -1){
			fprintf(stderr, "A problem happened: %s", strerror(errno));
		}
		write(1, space, 1);
        	count++;
      	}
      write(1, newline ,1);
      }
      else{ //if no input
	write(2, argv[0], strlen(argv[0]));
	write(2, newline ,2);
      }

        // Return to Caller
        return 0;
}
