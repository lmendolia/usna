/*
        Name: MIDN A.N. Mendolia
        Alpha: 184242
        Course: SY204 - Sys Prgm & OS Fund
        Assignment: Programming Laboratory - 0x01: Echo the Cat

        Description: This program is meant to replicate the cat and echo commands. It will read in a 		file and its arguments from the command line and print them to the screen. If there is an 		error, it will display that error.

        Resources: Alex Varon, Adam Montgomery, Katie Swafford, Dylan Struthers, MGSP, EI
		            The Linux Programming Manual, page 1409
                tutorialspoint.com (how to output simple error message & how to use fopen)
*/

// Required Includes
#include <stdlib.h>     // Standard Library: exit(3)
#include <stdio.h>      // Standard I/O Library: printf(3)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
// Global Constants

// Function Prototypes
void usageFunction();
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
        // Perform Function
        int opt;
        while ((opt = getopt(argc, argv, "h")) != -1){
          if (opt == 'h'){
          usageFunction(); break;
          return 0;
          }
      	}
      	if (argc  <= 1){
      		usageFunction();
          return 0;
      	}
}
return 0;
         //Usage Option: read in the help option using getopt: these are types of options and this is what they do
	void usageFunction(){
          printf("NAME\n \tnetGoat= our rendition of netcat\n");
          printf("SYNOPSIS\n \tnetGoat [PATH] [FILE] ...\n");
	  printf("DESCRIPTION\n \tConcatenate FILE(s), or standard input(s), to standard output\n");
          printf("\t-h: display this help\n");
          exit(0);//EXIT WITH NORMAL RETURN VALUE
        }
