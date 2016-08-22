/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Process Memory
	
	Description: In the code below, identify which section of process memory each variable will reside in.
*/

// Required Includes
// Previous Uses
#include <stdio.h>   // Standard I/O Library: printf(3)
// New Uses
#include <stdlib.h>  // Standard Library: calloc(3), free(3) [Dynamic Memory routines]

// Type Defines
typedef struct {
	int YG;
	char company;
	char *firstName;
	char *lastName;
	char *realName;
	char major[4];
} cyber_oper;

// Global Constants and Variables
int numSCY;                 // Section of Memory: ???
cyber_oper basit;           // Section of Memory: ???
char majorCode[4] = "SCY";  // Section of Memory: ???
cyber_oper yg2017[54];      // Section of Memory: ???

// Function Prototypes
void haxors(cyber_oper *SCYers, int numSCYers);

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturn = 0 ;     // Section of Memory: ???
	int intFailure = -1;    // Section of Memory: ???
	double *someQPRs;       // Section of Memory: ???
	// Perform Function
	basit.realName = "earlyOne";
	yg2017[0] = basit;
	yg2017[1].realName = "trinity";
	numSCY = 2;
	someQPRs = calloc(2, sizeof(double));  // Section of Memory (calloc == Dynamic Memory): ???
	
	haxors(yg2017, numSCY);
	haxors(yg2017, numSCY);
	
	free(someQPRs);
	// Return to caller
	return intReturn;
}

void haxors(cyber_oper *SCYers, int numSCYers) {  // Section of Memory: ???
	// Declare Required Variables
	int intI = 0 ;
	static int namesPrinted;  // Section of Memory: ???
	// Perform function
	fprintf(stdout, "There are %d %s majors. Their names are:\n", numSCYers, majorCode);
	while ( intI < numSCYers ) {
		fprintf( stdout, "%s\n", SCYers[intI].realName );
		intI++;
	}
	namesPrinted += numSCYers;
	fprintf(stdout, "Total Names Printed: %d\n", namesPrinted);
	return;
}
