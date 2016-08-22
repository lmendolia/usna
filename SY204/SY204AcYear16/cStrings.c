/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: C Arrays & C Strings
	
	Description: [See Meeting Title]
	
	Student Questions:
	1. What is syntax for C arrays; i.e. what would a declaration for an array of Integers look like in C?
		char strPassword[25] (reserve 25 bytes, 24 characters and a null)
		int intPtsByQtr[4] (reserve 16 bytes)
		varType varName[size]

	2. What is the syntax for accessing an element in an array in C?

	
	3. (Student Activity: strcpy(3)) Which version of strcpy sounds better (more secure)?
		strncpy (copies at most n bytes)
		meanwhile strcpy could have buffer overruns
 		// all string literals end in \0 (null character)
		strncpy(source, source

	4. (Stu Act: printf String Format) What is the conversion specifier for a C String?
	
*/

// Required includes
#include<string.h>

// New uses
  // C String Library: ...

// Previous uses
#include<stdio.h>
  // Standard I/O Library: printf(3) family

// Global Constants and Variables
#define MAX_PSWD_LEN 25  // A preprocessor directive that defines a text substitution, MAX_PSWD_LEN will be replaced with 25 before the source code is compiled
			 // Caps = CONSTANT

// Function prototypes

// Main function, the function that starts the program
int main( int argc, char *argv[]) {
	// Declare required variables
	int intReturnStatus = 0;  // Return variable, assume normal exit
	// Stu Act: printf String Format
	// 1. Declare a C String variable
	char strArray[MAX_PSWD_LEN];
	char strManualPswd[MAX_PSWD_LEN] = {'b', 'R', '|', 0x74, 51, '\0'};
	// Perform function
	
	// Stu Act: printf String Variable
	// 2. Store a string literal in the character array
	printf("strArray\t%s\t%lu\n", strArray, sizeof(strArray));
	printf("manpswd\t%s\t%lu\n", strManualPswd, sizeof(strManualPswd));
	printf("element2\t%c\n", strManualPswd[1]);
	strManualPswd[1] = "X";
	printf("element2\t%c\n", strManualPswd[1]);
	strncpy(strArray, "P4Ssw0rd!0123456789012349", MAX_PSWD_LEN);
	printf("strArray\t%s\n", strArray);
	// Student Activity: printf String Format
	// 5. Add a printf statement to output the string variable
	
	// Return to caller
	return intReturnStatus;
}

