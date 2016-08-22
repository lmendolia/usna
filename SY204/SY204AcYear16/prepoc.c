/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Preprocessor Directives
	
	Description: (See Meeting Title)
*/

/*
 * Defining Macros Using gcc Command Line Options:
 * gcc has a command line option to define a macro, equivalent to #define.
 * The syntax is: $ gcc -DMACRO_DEFINED
 * or
 * $ gcc -DMACRO_DEFINED=Value
 * The lack of space between the 'D' option and the macro name is important.
 * From the gcc man page "Many options have multi-letter names; therefore multiple single-letter options may not be grouped: -dv is very different from -d -v."
 */

// As normal, your code should compile without warnings or errors.

#define SWORRIOR 0  // DO NOT alter this line

// Required Includes
// New Uses
// Previous Uses
#include <stdio.h>  // Standard I/O Librbary: printf(3) family, standard streams (stdin, stdout, stderr)

// Student Activity: Include for Prototypes
// 1. Add an include statement for this projects header file
#include "prepoc.h"

// Global Constants

// Function Prototypes
// You MAY NOT add function prototypes to this file

// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturn = 0;  // Assume normal return
	
	// Perform Function
	// Stu Act: DEBUG Only
	// 1. Make the below fprintf(3) only execute if DEBUG is defined; DO NOT use a preprocessor directive to define DEBUG, use gcc command line options
	#ifdef DEBUG
		fprintf(stderr, "DEBUG: Debugging mode. Debugging information output to stderr\n");  // We will discuss stderr and the other input and output sources in coming discussions
	#endif
	// Stu Act: Select 1110
	// 1. The SWO designator is 1110, make the below say SWORRIOR == 1110
	// 2. DO NOT alter the next line of code
	#undef SWORRIOR
	#define SWORRIOR 1110
	printf("SWORRIOR == %d\n", SWORRIOR);
	
	// Stu Act: Find the Needle
	// 1. Get the program to output: FOUND: Ne3d1E
	// 2. You may not define JAMMING (no self jamming)
	// Hint: Think outside the norm to get past the jamming hurdle
	printf("FOUND: ");
	#if defined(PROTOED)
		wheresMyPrototype();
	#endif
	printf("\n");
	// Stu Act: This Wont Hz a Bit
	// 1. Meet the requirements
	// 2. DO NOT alter the next four lines of code
	#ifdef HOPPER
		printf("Desired Frequency:\t2017\nTuned Frequency:\t");
		freqHop(FREQUENCY);
	#endif
	
	// Return to Caller
	return intReturn;
}

// DO NOT alter the code in wheresMyPrototype to solve the student activities. You may use preprocessing directives outside of wheresMyPrototype and gcc command line options.
void wheresMyPrototype(void) {
	// Decleare Required Variables
	int obfus = 1;
	// Perform Function
	#ifdef DEBUG
		fprintf(stderr, "HINT (Level 1): \x54\x68\x65\x72\x65'\x73 \x61 \x6e\x65\x65\x64\x6c\x65 \x69\x6e \x74\x68\x69\x73 \x68\x61\x79\x73\x74\x61\x63\x6b\n");
		#if defined(HINT_2)
			fprintf(stderr, "HINT (Level 2): \x52\x65\x76\x69\x65\x77 \x70\x72\x65\x70\x72\x6f\x63.\x68, \x68\x6f\x77 \x63\x61\x6e \x79\x6f\x75 \x67\x65\x74 \x74\x68\x65 #\x65\x6c\x73\x65 \x74\x6f \x74\x72\x69\x67\x67\x65\x72?\n");
		#endif
	#endif
	#ifdef HAYSTACK
		obfus += printf("!");
	#endif
	#ifndef JAMMING
		obfus += printf("%c", '\x4e');
	#endif
	#ifndef HAYSTACK
	#else
		obfus += printf("|");
	#endif
	#ifdef CHAFF
		obfus += printf("%c", '\x63');
	#elif defined(HAYSTACK)
		obfus += printf("%c", '\x43');
	#endif
	obfus += printf("%c", '\x4e');
	#if defined(JAMMING)
		#ifdef BURN_THROUGH
			obfus += printf("%c", 101);
		#else
			obfus += printf("%x", 11);
			obfus += printf("%x", 10);
			obfus += printf("%x", 015);
		#endif
	#endif
	obfus -= printf("%d", obfus);
	#ifdef JAMMING
		#ifndef BURN_THROUGH
			obfus -= printf("%c", '\x42');
			obfus -= printf("%c", 'e');
		#else
			obfus -= printf("%c", '\x64');
		#endif
	#endif
	#ifdef HAYSTACK
		obfus -= printf("%c", 'E');
	#endif
	#ifndef JAMMING
		obfus -= printf("\\");
		#ifndef BURN_THROUGH
			obfus -= printf("/");
		#endif
	#endif
	#ifndef CHAFF
		printf("%d", obfus);
	#endif
	#ifdef HAYSTACK
		printf("E");
	#endif
	#if BURN_THROUGH
		printf("%X", BURN_THROUGH );
	#endif
	#if defined(HAYSTACK) || defined(CHAFF)
		printf("&%%");
	#endif
	return ;
}

// DO NOT alter the code in freqHop to solve student activities. You may use preprocessing directives outside of freqHop.
void freqHop(int obfus) {
	// Perform Function
	#ifdef FREQUENCY
		printf("%d%d\n", obfus, FREQUENCY);
	#endif
	return ;
}
