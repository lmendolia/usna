/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Homework 03 - Basic C Programming

	Description: This program is intended to get you familiar with type conversions in C.
*/

// Required includes
#include <stddef.h>  // Standard Definition Library
#include <stdlib.h>  // Standard Library
#include <stdio.h>   // Standard I/O Library

// Global constants and variables

// Function prototypes
int foo( int *int1, int *int2, int *int3, int int4 ) ;

// Familiarization with C data types
int main( int argc, char *argv[]) {
	// Declare required variables
	int intReturnStatus = 0 ;  // Assume normal exit
	char chrBlank = ' ' ;
	char chrA = 4, chrB = 'B', chrC = 68 , chrD = 10 ;
	char *cptE = NULL,  *cptF = NULL ;
	unsigned short shrtLocalPort = 22, shrtRemotePort ;
	short shrtPort;
	int intG = 26000, intH = 20, intI = 17, intJ = 4, intZ = 3 ;
	int *iptK = &intG, *iptL = &intJ ;
	double dblM = 20.6, dblN = 20.00, dblO = 16.00, dblP = 0.2 ;
	double *dptQ = NULL, *dptR = NULL ;
	// Perform function
	// char and short
	printf("8.-: Example%c[Make this appear on a new line using only the variables provided]\n", chrD );
	printf("8.1: not %c%d%c [Make me say: \"not B4D\"]\n", chrBlank, chrBlank, chrBlank );
	chrB = chrB + 0x20 ;  // In C "0x" is used to specify a hexadecimal value
	chrC = chrC + 0x20 ;
	chrD = chrB - ( chrD + dblN + chrA / 2 ) ;
	printf("8.2: %c%d%c%cling [Make me say: \"d4BBling\"]\n", chrBlank, chrBlank, chrBlank, chrBlank );
	chrB = dblM ;
	chrC = dblP ;
	printf("9: %d %d [Observe output, no changes needed]\n", chrB, chrC );
	shrtRemotePort = 0xffff ;
	shrtPort = -22 ;
	printf("10: %u %#x %u %#x %ld %d [Observe output, no changes needed]\n", shrtLocalPort, shrtLocalPort, shrtRemotePort, shrtRemotePort, sizeof(shrtPort), shrtPort );
	// int and double
	intG = intH / intZ ;
	dblM = intH / intZ ;
	intH = intH / intJ ;
	intZ = intJ / dblP ;
	printf("11: %d %f %d %d [Observe output, no changes needed]\n", intG, dblM, intH, intZ );
	// pointers
	intG = 200 ;
	intJ = 0 ;
	intH = 20 ;
	intI = 17 ;
	foo(&intZ, &intZ, &intZ, intZ);  // Alter the arguments to foo to solve the below
	printf("12: Class of %d%d [Make me say: \"Class of 2017\"]\n", *iptK, *iptL );
	// Return
	return intReturnStatus ;
}

// Alter the values pointed to by the arguments
// Recall: int1 is a pointer to int
int foo( int *int1, int *int2, int *int3, int int4 ) {
	// Declare required variables
	int intReturnValue = 0;
	// Perform function
	*int2 = int4 ;  // Store the expression value at the memory address pointed to by int2
	*int3 = *int1 ;
	return intReturnValue ;
}
