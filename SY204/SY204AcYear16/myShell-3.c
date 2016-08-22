/*
	Name: MIDN Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab 0x06: myShell

	Note: You MAY modify return values from functions

	Description: <A general high-level description about what the program is designed to accomplish>

	Resources: <Cite any resources used in a style similar to MLA>
*/

// Required Includes
// New Uses
#include <unistd.h>  // UNIX Standard Library: ???
#include <stdio.h>   // Standard I/O Library: fgets(3)
#include <string.h>  // String Library: strtok_r(3)
#include <sys/types.h>
#include <sys/wait.h>
// Previous Uses
#include <stdlib.h>  // Standard Library: EXIT_*

// Global Constants and Variables
// Student: You may add constants, but shall not delete constants
// Error Constants
#define STD_ERR_RETURN 	-1
#define NULL_ERR_RETURN	NULL
// Length Constants
#define MAX_STR_LEN   	1024
#define MAX_CMD_LN_LEN	1024
#define MAX_NUM_ARGS  	128
// Read command line
#define READ_CMD_OKAY     	00
#define READ_CMD_GEN_ERROR	01
#define READ_CMD_BLANK    	02
// Parse flags
#define PARSE_OKAY           	00
#define PARSE_GEN_ERROR      	01
#define PARSE_PREV_HAND_ERROR	02

// Function Prototypes
// Command line utilities
int getFullCmdLn(char *strFullCmdLn);
int tokParseFullCmdLn(char *strFullCmdLn);
int tokParseSingleCmd(char *strSingleCmd);
void usage(char *strCalledName);
void errorFunc(int errNum);
// Array and string utilities
int isOnlyWhiteSpace(char *strCheckForWhiteSpace);

// main Function
int main( int argc, char *argv[]) {
	//set up usage function
	int opt;
	int returnVal;
	char * environment;
	while ((opt = getopt(argc, argv, "h")) != -1){
			if (opt == 'h'){
				usage(argv[0]); break;
				return 0;
			}
	}
	environment= getenv("PATH");
	if (environment == NULL){
		returnVal = setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);
		if (returnVal == -1){errorFunc(1);}
	}
	//printf("%s\n", environment);
	// Declare Required Variables
	/* Begin: DO NOT MODIFY - Variable Declarations */
	char *strFullCmdLn;  // String for full command line
	int intFailure = 0;  // Error checking
	/* End:   DO NOT MODIFY - Variable Declarations */
	// Perform Function
	// Pre-user prompting setup


	// Abstraction and modularity are the keys to solving complexity issues
	strFullCmdLn = calloc( MAX_STR_LEN, sizeof( char ) );
	// Infinitely prompt for a user entered command line
	while ( 1 ) {  // Command prompt Loop
		intFailure = getFullCmdLn(strFullCmdLn);
		switch (intFailure) {
			case READ_CMD_OKAY: //case 0
				intFailure = tokParseFullCmdLn(strFullCmdLn);
				switch (intFailure) {
					case PARSE_OKAY:
						break;
					case PARSE_GEN_ERROR:
						fprintf(stderr, "Previous handle error when parsing\n");
						break;
					case PARSE_PREV_HAND_ERROR:
						fprintf(stderr, "Previous handle error when parsing\n");
						break;
					default:
						fprintf(stderr, "Unsuspected error.\n");
						break;
				}
				break;
				case READ_CMD_GEN_ERROR:
					fprintf(stderr, "General Command Line Read Error\n");
					break;
				case READ_CMD_BLANK:
					fprintf(stderr, "Command  line left blank\n");
					break;
			default:
				fprintf(stderr, "Unsuspected error\n");
				break;
		}
	}  // Command prompt Loop


	// Memory management
	free(strFullCmdLn);
	// Return to Caller
	return EXIT_SUCCESS;
}

/*
 * Description: Reads in a command line from standard input
 * Arguments:
 *   strFullCmdLn: String representing command line read from standard input
 * Return Value:
 *   0: Normal exit
 		 1: general error
		 2: read command blank
 *   #: Unable to read command line, errno set accordingly
 * Notes:
 *   Student: Only need to setup prompt, and update error reporting.
 */
int getFullCmdLn(char *strFullCmdLn) {
	// Declare Required Variables
	char *strPrompt = "Enter your commands here: > ";  // It's your shell
	// Perform Function
	printf("%s", strPrompt);

	if ( fgets(strFullCmdLn, MAX_CMD_LN_LEN, stdin) == NULL_ERR_RETURN ) {  // Read command line
		fprintf(stdout, "\n");
		fprintf(stderr, "Error with fgets...\n");
		return READ_CMD_GEN_ERROR;
	}
	if (strlen(strFullCmdLn) > 1) {
		if (strFullCmdLn[strlen(strFullCmdLn) - 1] == '\n') {  // Explore 2.a.
			strFullCmdLn[strlen(strFullCmdLn) - 1] = '\0';
		} else {
			fprintf(stdout, "\n");
		}
	} else {
		strFullCmdLn[0] = '\0';
		//if no input is written, just return new command prompt
		//return READ_CMD_GEN_ERROR;
	}
	// Return to Caller
	return 0;
}

/*
 * Description: Tokenizes (';' separator) and parses a full command line in individual commands
 * Arguments:
 *   strFullCmdLn: String representing full command line
 * Return Value:
 *   0: Normal exit
 		 1: gen error
		 2: read command blank
 *   #: ERROR: Error detected
 * Notes:
 *   Student: Add and modify as needed to complete Part 3 - It's a Party Line, and for error reporting
 */
int tokParseFullCmdLn(char *strFullCmdLn) {
	// Declare Required Variables
	int intFailure;      // Value returned
	char *strSingleCmd;  // String representing single command
	char *bufSingleTokSave;
	// Perform Function
	strSingleCmd = strtok_r(strFullCmdLn, ";", &bufSingleTokSave);
	do {
		if (isOnlyWhiteSpace(strSingleCmd)){continue;}
		intFailure = tokParseSingleCmd(strSingleCmd);
		switch ( intFailure ) {  // Tokenizing or parsing error
			case PARSE_OKAY:   // No Error
				break;
			// Student: Add error cases as needed
			case PARSE_PREV_HAND_ERROR:
				fprintf(stderr, "Previous handle error when parsing\n");
				break;
			default:
				return PARSE_GEN_ERROR;
		}
	} while ( (strSingleCmd = strtok_r(NULL, ";", &bufSingleTokSave)) != NULL );
	// Return to Caller
	return PARSE_OKAY;
}

/*
 * Description: Tokenizes (' ','\t' separator) and parses a single command
 * Notes:
 *   Student: Most of your code should be in here
 */
int tokParseSingleCmd(char *strSingleCmd) {
	// Declare Required Variables
	// Single command line
	char *strSingleCmdLnArgs[MAX_NUM_ARGS];  // Array of strings representing a single command with arguments and with redirects
	char *strSingle;         // Copy of single command to pass to strtok_r [trustno1]
	char *strToken;          // Single token
	char *bufSingleTokSave;  // Buffer for strtok_r bookkeeping
	int intNumTokens = 0;    // Number of tokens
	// Process management
	pid_t PID = -1;
	int child;

	// Perform Function


	/* Begin: DO NOT MODIFY - Single Command Tokenizing */
	// If you are writing code in here, you are wrong
	// Setup strSingle; before returning need to free(strSingle)
	strSingle = calloc( MAX_STR_LEN, sizeof( char ) );  // Assume nothing
	strncpy(strSingle, strSingleCmd, MAX_CMD_LN_LEN);
	// Build strSingleCmdLnArgs, tokenize command line based on ' ', '\t', '\n', '\v'
	strToken = strtok_r(strSingle, " \t\n\v", &bufSingleTokSave);
	do {
		strSingleCmdLnArgs[intNumTokens] = strToken;
		intNumTokens++;
	} while ( (strToken = strtok_r(NULL, " \t\n\v", &bufSingleTokSave)) != NULL );
	strSingleCmdLnArgs[intNumTokens] = NULL;  // Explore 2.b.
	/* End:   DO NOT MODIFY - Single Command Tokenizing */
	PID = fork();
	switch (PID){
		case -1: //error
			fprintf(stderr, "Error forking\n");
			free(strSingle);
		case 0: //child
			execvp(strSingleCmdLnArgs[0], strSingleCmdLnArgs); //path and argv
			break;
		default: //parent
			wait(&child);
			break;
	}
	// Memory management
	free(strSingle);
	// Return to Caller
	return PARSE_OKAY;
}

/*
 * Description: Tests whether strCheckForWhiteSpace is only white space (' ', '\t', '\n', '\v')
 * Arguments:
 *   strCheckForWhiteSpace: String of upto MAX_STR_LEN that is to be checked for only having white space characters
 * Return Value:
 *   0: strCheckForWhiteSpace IS NOT only comprised of white space characters
 *   1: strCheckForWhiteSpace IS     only comprised of white space characters
 */
int isOnlyWhiteSpace(char *strCheckForWhiteSpace) {
	// Declare Required Variables
	// White space checking
	char strWhiteSpaceCheck[MAX_STR_LEN];  // Copy of command to check for "only white space"
	char *strSansWhiteSpace;               // Command without white space
	char *bufWhiteSpaceCheck;              // Buffer for strtok_r bookkeeping
	// Perform Function
	strncpy(strWhiteSpaceCheck, strCheckForWhiteSpace, MAX_STR_LEN);  // trustno1
	strSansWhiteSpace = strtok_r(strWhiteSpaceCheck, " \t\n\v", &bufWhiteSpaceCheck);
	if ( strSansWhiteSpace == NULL ) {
		return 1;
	} else {
		return 0;
	}
}
//*********************************************************************
//taken from sig-pitch.c usage function
void usage(char *strCalledName) {
		fprintf(stdout, "NAME\n");
		fprintf(stdout, "\tmyShell- A Simple Shell\n");
		fprintf(stdout, "SYNOPSIS\n");
		fprintf(stdout, "\tmyShell\n");
		fprintf(stdout, "DESCRIPTION\n");
		fprintf(stdout, "\tTokenizes and parses single line command lines\n");
		fprintf(stdout, "\t';' is used as a separator between commands on the same line.\n");
		fprintf(stdout, "\t-h: display this help\n");
		exit(0);//EXIT WITH NORMAL RETURN VALUE

}
void errorFunc(int errNum){
	switch(errNum){
		case 1: fprintf(stderr,"\nsetenv didn't work.\n\n");
			  exit(errNum);

		}
}
