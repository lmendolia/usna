/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: C Variables
	
	Description: Example file that declares and uses variables
	
	Resources: <Cite any resources used in a style similar to MLA>
*/

// Required includes
#include<stdio.h>
// New uses

// Previous uses
  // Need to use printf???

// Global Constants and Variables

// Function prototypes

// Student notes
/*
	Student Questions:
	1. What are the basic types of data that we deal with?
		boolean, char, int, float
	
	2. What order would you specify the type and the variable name?
		VariableType varName ;
	
	3. What symbol/syntax would you use to initialize a variable when declaring it?
		VariableType varName = value
	
	4. What syntax would you use to declare a list of variables of the same type?
		can separate with comma
 		Variable Type varName1 = value , varName2, ... , varNameN = value ; 
		
	
	Student Activity: Exploring printf(3) Formats
	1-3. Review the printf(3) man page, and complete the below table for: int, double, unsigned hexadecimal
	
	C Basic Types
  
	C Reserved Word   Type      Size (B)   printf	Comments
	                                       format
	_Bool             Boolean     0 or 1   (none)	In C99 and beyond
	
	int		  integer     4		%d

	unsigned hex		      1		%x
	
	double			      8		%f
	
	float			      4		%e

	char		  character   1		%c
	 
*/

// Main function
int main( int argc, char *argv[]) {
	// Declare required variables
	// Student Activity: Sizes of Types/Variables
	// 1. Declare and initialize a variable of type: char, int, double, _Bool
	char myChar = 'a';
		//used single quotes instead of double quotes
	int myInt = 234;
	double myDub = 3.45;
	float myFloat = 3.14;
	_Bool myBool = 0;
	char myHex = 0x42;

	  // Return status
	
	// Perform function
	
	// Student Activity: Sizes of Types/Variables
	// 2. For each type/variable, use printf to output the size of the type in bytes using sizeof( ... )
		printf("(char):\t%c\t%lu\n",myChar,sizeof(myChar));
		printf("(char):\tchar\t%lu\n",sizeof(char));
		printf("(int):\t%d\t%lu\n",myInt, sizeof(myInt));
		printf("(double):\t%.3f\t%lu\n",myDub, sizeof(myDub));
		printf("(float):\t%.3e\t%lu\n",myFloat, sizeof(myFloat));
		printf("(uHex):\t%x\t%lu\n",myHex, sizeof(myHex));
		printf("(bool):\t_Bool\t%lu\n",myBool, sizeof(myBool));
	// Return to caller

	return myDub;  // Alter this to return the value of a variable instead of a static value. Use $ echo $? to check that the value you specified was returned (* caveats)
		//will return a rounded value
		//otherwise type return 0;
}

