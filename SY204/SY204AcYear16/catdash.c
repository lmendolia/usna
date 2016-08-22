#include <stdlib.h>     // Standard Library: exit(3)
#include <stdio.h>      // Standard I/O Library: printf(3)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <strings.h>

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
		write(1, newline, 1);
        	count++;
      	}
      }
      else{ //if no input
	       write(2, argv[0], strlen(argv[0]));
	       write(2, newline ,2);
      }

        // Return to Caller
        return 0;
}
