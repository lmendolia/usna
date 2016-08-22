/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: C Conditionals & C Comparisons

	Description: [See Meeting Title]

	Student Questions:
1. Given your experience with C thus far, what would you expect an "if" statement to look like (syntax)?

if(conditional) {statement}

2. What would the syntax look like for an "else"?

else {statement}

3. What would the syntax look like for an "else-if"?

if (expression) if (expression) {} else {statement}

4. Given that 0 is FALSE, and that 1 is TRUE, what about the truth value of other values? How would you test (scientific experiment)? What values would you test?

5. What comparison operations do we expect?
==, !=, >=

6. What boolean operators do we expect?
&&
||
!
no logical XOR


*/

// Required Includes
#include<stdio.h>
// New Uses

// Previous Uses
  // Standard I/O Library: printf(3) family of functions

// Global Constants

// Function Prototypes

// main Function: activities that explore conditionals
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturn = 0;  // Return variable, assume normal exit
	// Perform Function

	// Stu Act: Determine Truth Values
	// 1. Design and implement an experiment to determine the truth values of numbers other than 0 and 1
	if(0){
	printf("0 is true??\n");
	}
	else{
	printf("Ah, 0 is false.\n");
	}
	if (1) {printf("1 is true. Good. \n");}
	else{printf("what the deuce. 1 is false???\n");}

	if (3){printf("if you this, 3 is true\n");}
	if (-1){printf("if you see this, -1 is true\n");}

	// Stu Act: else Association?
	// 1. Design and implement an experiment to determine which if the else is associated with in:
	//    if ( expression ) if ( expression ) { } else statement
if (0) if (1){}else {printf("its to the first if \n");}
	// 2. Which if is the else associated with?
	// else is associated with the if right in front of it unless it is forced otherwise

	// Stu Act: Boolean Operators
	// 1. Design and implement if else statements that use boolean operators

	// Stu Act: Comparison Operators
	// 1. Design and implement if else statements that use comparison operators
	// 2. What data types can be used with comparison operators?
	//

	// Return to Caller
	return intReturn;
}
