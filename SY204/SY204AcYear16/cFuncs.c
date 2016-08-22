/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: C Functions & C Pointers
	
*/

// Required includes
// New uses

// Previous uses
#include <stdio.h>   // Standard I/O Library: printf(3)

// Global Constants and Variables

// Function prototypes
int sumInts( int intOperandA , int intOperandB ) ;
/*
	Description: Returns the arithmetic sum of the two passed in values
	Arguments: intOperandA - Value on left hand side of +
	           intOperandB - Value on right hand side of +
	Return: Integer sum of intOperand A and intOperandB
*/

int sumInts2( int intOperandA , int intOperandB , int iptSum ) ;
/*
	Description: Stores the sum of intOperandA and intOperandB into iptSum
	Arguments: intOperandA - Value on left hand side of +
	           intOperandB - Value on right hand side of +
	           intSum - Store result of summation
	Return: 0 - Normal return
	        -1 - Error
*/

// Main function, the function that starts the program
int main( int argc, char *argv[]) {
	// Declare required variables
	int intReturn = 0;  // Return variable, assume normal exit
	int intA = 10, intB = 6 , intSum = 0 ;
	// Perform function
	
	intSum = sumInts( intA , intB );
	printf("%d\n", intSum );
	
	intReturn = sumInts2( intB , intB , intSum );
	printf("%d\n", intSum );
	// Return to caller
	return intReturn ;
}

int sumInts( int intOperandA, int intOperandB ) {
    return intOperandA + intOperandB ;
}
 
// Note: At this point the code will not function as you expect, this activity serves as a segue to C Pointers
int sumInts2( int intOperandA, int intOperandB , int iptSum ) {
	// Declare required variables
	int intReturn = 0 ;  // Assume normal
	intSum = intOperandA + intOperandB ;
    return intReturn ;
}
