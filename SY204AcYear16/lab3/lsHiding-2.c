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
	int intPause = 0;  // Variable to force pausing point within program, to support external testing
  char buffer[1024];

	// Perform Function
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1){
    perror("open didn't work");
    exit(6534);
  }
  int size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  read(fd, buffer, size);

  if (read(fd, buffer, size) != 0){
    perror("read didn't work");
    exit(7);
  }

	/*
	 * Begin Hard Coded Pause Point: 1
	 * Do not alter the given code between pause points
	 */
	fprintf(stdout, "Pause Point: File appears in ls listing\n");
	scanf("%d", &intPause);  // Read in integer as forced pause point
	// 1. Use Ctrl-Z to pause the program
	// 2. Use ls -a to see the file within the ls listing
	// 3. Use fg 1 (assumes this program is the only paused program) to resume the program
	/*
	 * End Hard Coded Pause Point: 1
	 */
   unlink(argv[1]);

	/*
	 * Begin Hard Coded Pause Point: 2
	 * Do not alter the given code between pause points
	 */
	fprintf( stdout, "Pause Point: File does not appear in ls listing, but is still open\n");
	scanf("%d", &intPause);  // Read in integer as forced pause point
	// 1. Use Ctrl-Z to pause the program
	// 2. Use ls -a to see the file is not in the ls listing
	// 3. Use fg 1 (assumes this program is the only paused program) to resume the program
	/*
	 * End Hard Coded Pause Point: 2
	 */
   lseek(fd, 0, SEEK_SET);
   close(fd);

   /*
   figured out how to create file/use tempnam from:
   http://pubs.opengroup.org/onlinepubs/009695399/functions/tempnam.html
   */
   int newfd;
   char *randNamFile = tempnam("/tmp", "FileXXXXXX");
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
