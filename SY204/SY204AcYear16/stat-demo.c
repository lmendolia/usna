/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: File Systems - Attributes

	Description: Example use of stat() system call
*/

// Required Includes
// New Uses
#include <time.h>       // Time Library: ctime(3)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>     // Standard include
#include <stdlib.h>     // Standard include
#include <stdio.h>      // Standard include

// Global Constants

// Function Prototypes

// main Function: access file attributes using stat(2)
int main( int argc, char *argv[] ) {
	// Declare required variables
	int intReturn = 0;    // Return variable, assume normal exit
	int intFailure = -1;  // Check return values, assume error
	struct stat statFileAttrs; // Variable to store attributes from stat(2)
	// Perform Function
	if (argc > 1) {
		intFailure = stat(argv[1], &statFileAttrs); // Call stat() for file specified via command line argument (argv[1])

		if ( ! intFailure ) {
			// Use Stream I/O (simple printf) to output file attributes
			// We access fields in a struct using '.' notation
			// Example: statFileAttrs.st_ino
			// Replace 0 with field from struct stat variable
			printf("i-node:\t%ld\n", (long) statFileAttrs.st_ino);
			// The (long) is type casting the value to be of type long.
			// Replace 0 with field from struct stat variable
			printf("Disk Ref Cnt:\t%ld\n", (long) statFileAttrs.st_nlink);
			// Output file type
			printf("File type:\t");
			// Replace 0 with field from struct stat variable
			switch ( statFileAttrs.st_mode & S_IFMT ) {  // Replace the #'s with a defined constant
				// S_IFMT is the bit mask for st_mode to determine file type information; if you bitwise AND (&) a value with a mask you are left with the significant bits (the bits of interest), a mask is bitwise all 1's.
				// You have already seen bitwise OR (|) used to set a series of flags/modes (see open(2)).
				case S_IFREG : printf("regular file\n");   break;
				case S_IFDIR: printf("directory\n");      break;
				case S_IFLNK: printf("symbolic link\n");  break;
				default :      printf("other\n");          break;
			}
			// Timestamps
			// Replace 0's with field from struct stat variable
			// Note: ctime() returns a "\n\0" terminated string
			// char *ctime(const time *timep);
			printf("Last Access:\t%s", ctime( &statFileAttrs.st_atime));
			// Last Access: ???
			printf("Last Modification:\t%s", ctime(&statFileAttrs.st_mtime ));
			// Last Modification: ???
			printf("Last Status Change:\t%s", ctime(&statFileAttrs.st_ctime));
			// Last Status Change: ???
		} else {  // Error stat() failed
			intReturn = 2;
		}
	} else {  // Error incorrect number of command line arguments
		intReturn = 1;
	}
	// Return to caller
	return intReturn;
}
