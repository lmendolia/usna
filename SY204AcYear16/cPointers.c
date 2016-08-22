/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: C Pointers
	
	Description: [See Meeting Title]
*/

// Required includes
#include<stdio.h>
// New uses
  // ??? : NULL constant
#include <stddef.h>

// Previous uses
#include <stdio.h>  // Standard I/O Library: printf(3)

// Global Constants

// Student Activity: sumInts2 Pointers in Argument List
// 1. Alter the function prototype (declaration) and function header of sumInts2 such that the third argument (which will be altered by the function) is a pointer

// Function prototypes
int sumInts2( int intOperandA , int intOperandB , int intSum ) ;
/*
	Description: Stores the sum of intOperandA and intOperandB into intSum
	Arguments: intOperandA - Value on left hand side of +
	           intOperandB - Value on right hand side of +
	           intSum - Store result of summation
	Return:  0 - Normal return
	        -1 - Error
*/

// Main function, the function that starts the program
int main( int argc, char *argv[]) {
	// Declare required variables
	int intReturn = 0;  // Return variable, assume normal exit
	int intSum = 0;
	// Stu Act: Basic C Pointers - Pt 1
	// 1. Declare two integer type variables, and assign a non-zero value to them
int Darth = 7;
int Vader = 9;
	// 2. Declare two pointers to integers, and initialize them to NULL
int* ptDarth = NULL;
int* ptVader = NULL;
	// 3. Declare two double variables, and assign a non-zero value to them
double palps = 66;
double lexi = 18;

	// 4. Declare two pointers to doubles, and initialize them to NULL
double* ptPalps = NULL;
double* ptLexi = NULL;
	// Perform function
	
	// Stu Act: Basic C Pointers - Pt 1
	// 5. Use printf to output the size of an integer variable (sizeof operator)
printf("%lu\n", sizeof(Darth));
	// 6. Use printf to output the size of a pointer to an integer variable
printf("%lu\n", sizeof(ptDarth));
	// 7. Use printf to output the size of a double variable
printf("%lu\n", sizeof(palps));
	// 8. Use printf to output the size of a pointer to a double variable
printf("%lu\n", sizeof(ptPalps));
//printf("%lu\n", ptLexi);

	// Stu Act: Basic C Pointers - Pt 1
	// 9. What are the sizes of an int, a double, a pointer to an int, a pointer to a double?
	// int: 4   int *: 8   double: 8    double *: 8
	// pointers will always be 8 bytes long
	
	// Stu Act: Basic C Pointers - Pt 2
	// 1. Use the Address Of operator to have an int pointer point to one of the int variables
printf("%lu\n", ptPalps= &palps);
	// 2. Use printf to output the value at the memory location the pointer points to (dereference)
printf("%.0f\n", *ptPalps);
	// 3. Dereference the pointer again and modify the value pointed to by pointer
*ptPalps = 501.0;
printf("This is new Palps %.0f\n", palps);
printf("This is new ptPalps %.0f\n", *ptPalps);
	// 4. Use printf to output the value of variable
	
	
	// Student Activity: Call sumInts2 with Memory Address
	// 1. Alter the calling of sumInts2 to pass the memory address of the variable to be altered
	sumInts2( 2020, -2, &intSum );
	printf("(sum): %d\n", intSum);
	
	// Student Activity: Call sumInts2 with Pointer
	// 1. Add another call to sumInts2, but this time use a pointer variable directly instead of the memory address of a non-pointer variable
	
	// Return to caller
	return intReturn;
}

// Stores the sum of intOperandA and intOperandB into intSum
int sumInts2( int intOperandA, int intOperandB , int* intSum ) {
	// Declare required variables
	int intReturn = 0 ;  // Return variable, assume normal exit
	// Perform function
	// Student Activity: sumInts2 Pointers in Argument List
	// 2. Alter the code in the definition of sumInts2 such that the pointer is correctly dereferenced
	*intSum = intOperandA + intOperandB ;
// dereference by *
    return intReturn ;
}
