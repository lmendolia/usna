/*
	Name: A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Descriptor I/O
	
	Description: An example program that uses Descriptor I/O to access a file
*/

/*
	Student Notes:
	file descriptor (fd):
	Descriptor I/O Steps/System Calls:
	1.
	2.
	3.
	4.
*/

// Required Includes
// New Uses
#include <stdlib.h>     // Standard Library: exit(3)

// Previous Uses
#include <stdio.h>      // Standard I/O Library: printf(3)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Global Constants
#define CHAR_BUFF_LEN 5

// Function Prototypes

// main Function: Perform Descriptor I/O
int main( int argc, char *argv[]) {
	// Declare required variables
	int intReturn = 0;  // Return variable, assume normal exit
	int intCalledStatus = 0;  // Variable to check called return values
	int fdASCIIText = -2, fdBinData = -2;  // File Descriptors for accessing files
	int intDataIOCnt = 0 ;  // Variable to check actual I/O bytes
	char strYG[CHAR_BUFF_LEN] ;  // Buffer for reading in 4 characters + NULL byte
	int intYG = 2000 ;  // Variable (buffer) for reading in an integer
	// Perform function
	strYG[CHAR_BUFF_LEN - 1] = '\0' ;  // Explicitly NULL terminate string
	// Descriptor I/O Step 1: 
	// yg.txt
	fdASCIIText = open("yg.txt", O_RDONLY);
	if ( fdASCIIText == -1 ) exit(1);  // Unable to _____ file
	// yg.dat
	fdBinData = open("yg.dat", O_RDONLY);
	if ( fdBinData == -1 ) exit(1);  // Unable to _____ file
	// Descriptor I/O Step 2:
	// yg.txt
	intDataIOCnt = read(fdASCIIText, strYG, 4);
	if ( intDataIOCnt == -1 ) exit(2);  // Unable to __________ file
	printf("yg.txt: %s\n", strYG);
	// yg.dat
	intDataIOCnt = read(fdBinData, &intYG, sizeof(intYG));
	if ( intDataIOCnt == -1 ) exit(2);  // Unable to __________ file
	printf("yg.dat: %d\n", intYG);
	// Descriptor I/O Step 4:
	// yg.txt
	intCalledStatus = close(fdASCIIText);
	if ( intCalledStatus == -1 ) exit(3);  // Unable to ________ file
	// yg.dat
	intCalledStatus = close(fdBinData);
	if ( intCalledStatus == -1 ) exit(3);  // Unable to ________ file
	// Return to caller
	return intReturn;
}
