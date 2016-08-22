/*
        Name: MIDN A.N. Mendolia
        Alpha: 184242
        Course: SY204 - Sys Prgm & OS Fund
        Assignment: Programming Laboratory - 0x01: Echo the Cat

        Description: This program is meant to replicate the cat and echo commands. It will read in a 		file and its arguments from the command line and print them to the screen. If there is an 		error, it will display that error.

        Resources: Alex Varon, Adam Montgomery, Katie Swafford, MGSP, EI
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
#include <getopt.h>ount < argc)
#include <string.h>
#include <strings.h>
// Global Constants

// Function Prototypes
void usageFunction();
int fileRead(int argc);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
        // Perform Function
        int opt;
        int count = optind;
        char *dash = "-";
        while ((opt = getopt(argc, argv, ":p:h")) != -1){
                if (opt == 'h'){
                        usageFunction(); break;
                }
                else{break;}
        }
        while(count < argc){
          //while (count < argc){
            int fd = open(argv[count], O_RDONLY);
            if (fd != -1){
            fileRead(fd);
            printf("\n");
        }
        if (strcmp(argv[count], dash) == 0){
            fileRead(0);
        }ount < argc)



        count++;
        }
        // Return to Caller
        return 0;
}

         //Usage Option: read in the help option using getopt: these are types of options and this is what they do
	void usageFunction(){
          printf("NAME\n \tmyCat = concatenate files to standard output\n");
          printf("SYNOPSIS\n \tmyCat [PATH] [FILE] ...\n");
	        printf("DESCRIPTION\n \tConcatenate FILE(s), or standard input(s), to standard output\n");
          printf("\t-h: display this help\n");
          exit(0);//EXIT WITH NORMAL RETURN VALUE
        }


	//READ FILES: will read in both single file and multiple file names from the command line and print contents
	int fileRead(int fd){
	  //int count = 1;
    //while (count < argc){
      //int fd = open(argv, O_RDONLY);
      char buffer[1024] ="";
      int  returnReadVal = 1;
      int returnVal = 1;
      while (returnReadVal > 0){
        returnReadVal = read(fd, buffer, 1024);
        if (returnReadVal < 0){
          printf("error\n"); exit(5);
        }
        if (returnReadVal > 0){
          returnVal = write(1, buffer, 1024);
        }
          //write(1, buffer, returnVal);
        //else{printf("return read value was less than zero\n");}
        if (returnVal < 0){printf("error message\n"); exit(29);}
      }
    close(fd);
  //  count++;
//  }
    return 0;
  }
