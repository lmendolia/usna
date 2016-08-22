/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: C Loops
	
	Description: [See Meeting Title]
	
	Student Questions:
1. How many different types of loops do we need?

pretty much just a while loop
but there are for loops, and do-while loops (run at least once)

2. What would you expect the syntax of a "while" loop to be in C?

while (conditional) {statement}

3. What if we want to skip to the next iteration of a loop? What reserved word would you use to skip to the next iteration of a loop?

continue, break, return

4. What if we want to exit out of a loop? What reserved word would you use to exit out of a loop?
	
*/

// Required Includes
// New Uses

// Previous Uses
  // Standard I/O Library: printf(3) family of functions

// Global Constants

// Function Prototypes

// main Function: activities that explore loops
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturn = 0;  // Return variable, assume normal exit
	// Perform Function
	
	// Stu Act: while Serving as for
	// 1. Design and implement the functionality of a for loop using a while loop
/*for (int x = 0; x < 10; x++;) {
printf("%d", x);
}*/
int x = 0;
while(x < 11){
printf("%d\n", x);
x++;
}
	// 2. Implement summing the numbers [1..10] counting up
int x = 0;
int sum = 0;
while(x < 11){
sum = sum + x;
//printf("%d\n", x);
x++;
}
printf("%d\n", sum);

	// 3. Implement summing the numbers [1..10] counting down

int x = 10;
int sum = 0;
while(x > 0){
sum = sum + x;
//printf("%d\n", x);
x--;
}
printf("%d\n", sum);

	// Stu Challenges: Looping++
	// Hint: Explore the following C operators: ++, --, +=, -=
	// 4. Implement summing the numbers [1..10] counting up using a while loop with a single statement
int x = 0;
int sum = 0;
while(x < 11) sum+= ++x;
printf("%d\n", sum);

	// 5. Implement summing the numbers [1..10] counting down using a while loop with a single statement
	// 6. Implement summing the numbers [1..10] counting up using a for loop with no statements (a blank statement)
for (int x = 0, int sum = 0; x < 11; sum += x++){};
printf("%d\n"), sum);
	
	// Return to Caller
	return intReturn;
}
