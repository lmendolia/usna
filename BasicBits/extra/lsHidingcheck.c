/*
	Name: MIDN Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab 0x02 - Hiding from ls

	Description: "hides file from ls by unlinking inode table entry and moving contents of file into a new, randomly named file"

	Resources: MGSP, Katie Swafford, Connor Mellett, Tommy Rooney, Brandon Pullig, and Dylan Struthers
    http://pubs.opengroup.org/onlinepubs/009695399/functions/tempnam.html & StackOverflow for syntax Qs

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
char *tempnam(const char *dir, const char *pfx);

// main Function <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[]) {
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
	// Declare Required Variables
  char buffer[1024];

	// Perform Function
  int fd = open(argv[1], O_RDONLY);
  printf("here 1\n");
  if (fd == -1){
    perror("open didn't work");
    exit(6534);
  }
  int size = lseek(fd, 0, SEEK_END);
  printf("here 2\n");

  lseek(fd, 0, SEEK_SET);
  printf("here 3\n");

  read(fd, buffer, size);
  printf("here 4\n");


  if (read(fd, buffer, size) != 0){
    printf("here 5\n");

    perror("read didn't work");
    exit(7);
  }
   unlink(argv[1]);
   printf("here 6\n");

   lseek(fd, 0, SEEK_SET);
   printf("here 7\n");

   close(fd);
   int newfd;
   char *randNamFile = tempnam("/tmp", "FileLEXXXXXXX");
   newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
   if (newfd == -1){
     perror("open file 2 didn't work");
     exit(68);
   }
     write(newfd, buffer, size);
     write(newfd, "\n", 1);
     close(newfd);

  return 0; //return to caller
}

  //Usage Option: read in the help option using getopt: these are types of options and this is what they do
	void usageFunction(){
          printf("NAME\n \tlsHiding\n");
          printf("SYNOPSIS\n \tlsHiding [PATH] [FILE] ...\n");
  	      printf("DESCRIPTION\n \t hides i-node entry and moves bytes from file into a new randomly named file\n");
          printf("\t-h: display this help\n");
          exit(0);//EXIT WITH NORMAL RETURN VALUE
  }
