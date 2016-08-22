/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab 0x07: myShellpp

	Description: myShellpp - A simple shell that executes multiple commands from a single command prompt line and performs I/O redirection: <, >, | (between two commands)

	Resources: EI, MGSP, Tommy Rooney, Katie Swafford, and Connor Mellett

	Bonus:
	1. [ 2 / 0 ]  Explain the origin of trustno1. [CP-0x04i for this question]
		special agent Fox Mulder's computer password in the X-Files TV series
		REFERENCE: Urban Dictionary
*/
//close read end, stdin buffer goes into write end
// Required includes
// Previous Uses
#include <stdlib.h>   	// Standard Library: EXIT_*, calloc(3), exit(3), free(3)
#include <stdio.h>    	// Standard I/O: fgets(3)
#include <string.h>   	// String Library: strtok_r(3)
#include <unistd.h>   	// POSIX Standard: fork(2), execvp(3)
#include <wait.h>     	// Wait: wait(2)
#include <sys/types.h>	// System Types: pid_t
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

// Global Constants and Variables
#define _GNU_SOURCE
// Error constants
#define STD_ERR_RETURN   -1
#define NULL_ERR_RETURN  NULL
// Length constants
#define MAX_CMD_LN_LEN   1024
#define MAX_NUM_ARGS     128
#define MAX_NUM_PIPE     128
#define MAX_STR_LEN      1024
// Read command line
#define READ_CMD_OKAY     	00
#define READ_CMD_GEN_ERROR	01
#define READ_CMD_BLANK    	02
#define READ_CMD_PREV_HAND_ERROR  03
// Parse flags
#define PARSE_OKAY           	00
#define PARSE_GEN_ERROR      	01
#define PARSE_PREV_HAND_ERROR	02
// Redirect flags
#define REDIR_NO         00
#define REDIR_STDIN      01
#define REDIR_STDOUT     02
// Pipe flags
#define PIPE_NO          00
#define PIPE_IN          01
#define PIPE_FIRST       02
#define PIPE_LAST        04
// Pipe ends
#define PIPE_BAD_FD      -1
#define PIPE_READ_FROM   0
#define PIPE_WRITE_TO    1

// Function prototypes
// Command line actions
int getFullCmdLn(char *strFullCmdLn);
int tokParseFullCmdLn(char *strFullCmdLn);
int tokParsePipeline(char *strPipeline);
int tokParseSingleCmd(char *strSingleCmd, char flgPipeline, int *fdPrevPipeRd);
// Array and string utilities
int strArrayNCopy(char *strDestArray[], char *strSrcArray[], int maxStrLen, int numElemsCopy, int maxNumElems);
int isOnlyWhiteSpace(char *strCheckForWhiteSpace);
void usage(char *strCalledName);
void errorFunc(int errNum);

// Main function
int main( int argc, char *argv[] ) {
	int returnVal;
	char * environment;
	if (argc > 1){
		usage(argv[0]);
		return 0;
	}
	//taken from Lab 0x06
	environment= getenv("PATH");
	if (environment == NULL){
		returnVal = setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);
		if (returnVal == -1){errorFunc(1);}
	}
	// Declare required variables
	int intFailure;      // Error checking
	char *strFullCmdLn;  // String for full command line
	// Perform function
	// Pre user prompting setup

	// Modularity is the key to solving complexity issues
	strFullCmdLn = calloc( MAX_STR_LEN, sizeof( char ) );
	// Infinitely prompt for a user entered command line
	while ( 1 ) {  // Command prompt Loop
		intFailure = getFullCmdLn(strFullCmdLn);
		switch (intFailure) {
			case READ_CMD_OKAY:
				intFailure = tokParseFullCmdLn(strFullCmdLn);
				switch (intFailure) {
					case PARSE_OKAY:
						break;
					case PARSE_PREV_HAND_ERROR:
						fprintf(stderr, "Previous handle error when parsing\n");
						break;
					case PARSE_GEN_ERROR:
						fprintf(stderr, "Parse general error\n");
					default:
						fprintf(stderr, "Unsuspected error.\n");
						break;
				}
				break;
			case READ_CMD_GEN_ERROR:
				fprintf(stderr, "General Command Line Read Error\n");
				break;
			case READ_CMD_BLANK:
				break;
			case READ_CMD_PREV_HAND_ERROR:
				fprintf(stderr, "Read Command Previous Handle Error\n");
				break;
			default:
				fprintf(stderr, "Unsuspected error.\n");
				break;
		}
	}  // Command prompt Loop
	free(strFullCmdLn);
	// Return to caller
	return EXIT_SUCCESS;
}

/*
 * Description: Reads in a command line from standard input
 * Arguments:
 *   strFullCmdLn: String representing command line read from standard input
 * Return Value:
 *   0 (READ_CMD_OKAY): Normal exit
 *   1 (READ_CMD_GEN_ERROR): Unable to read command line, errno set accordingly
 *   2 (READ_CMD_BLANK): Blank command entered
 *   3 (READ_CMD_PREV_HAND_ERROR): Error handled internal to function
 * Notes:
 *   Student: Only need to setup prompt, and update error reporting.
 */
int getFullCmdLn(char *strFullCmdLn) {
	// Declare Required Variables
	char *strPrompt = "$ > ";  // It's your shell
	// Perform function
	printf("%s", strPrompt);
	if ( fgets(strFullCmdLn, MAX_CMD_LN_LEN, stdin) == NULL_ERR_RETURN ) {  // Read command line
		fprintf(stdout, "\n");
		fprintf(stderr, "Error with fgets.\n");
		return READ_CMD_PREV_HAND_ERROR;
	}
	if (strlen(strFullCmdLn) > 1) {
		if (strFullCmdLn[strlen(strFullCmdLn) - 1] == '\n') {
			strFullCmdLn[strlen(strFullCmdLn) - 1] = '\0';
		} else {
			fprintf(stdout, "\n");
		}
	} else {
		strFullCmdLn[0] = '\0';
		return READ_CMD_BLANK;
	}
	// Return to Caller
	return READ_CMD_OKAY;
}

/*
 * Description: Tokenizes (';' separator) and parses a full command line in individual commands
 * Arguments:
 *   strFullCmdLn: String representing full command line
 * Return Value:
 *    0: Normal exit
 *   -1: ERROR: Error detected
 * Notes:
 *   Student: Only modification needed is commenting/uncommenting lines
 */
int tokParseFullCmdLn(char *strFullCmdLn) {
	// Declare required variables
	int intFailure;             // Value returned
	char *strPipeline;          // String representing single pipeline of commands
	char *bufFullCmdLnTokSave;  // Buffer for strtok_r bookkeeping
	// Perform function
	strPipeline = strtok_r(strFullCmdLn, ";", &bufFullCmdLnTokSave);
	do {  // Iterate through pipelines
		if ( isOnlyWhiteSpace(strPipeline) ) {  // Blank command (NOP), okay to continue
			continue;
		}
		// Lab 0x07 - Part 2 & Part 3 (uncomment below line, comment out tokParsePipeline line)
		//intFailure = tokParseSingleCmd(strPipeline, PIPE_NO, NULL);
		// Lab 0x07 - Part 4 (uncomment below line, comment out tokParseSingleCmd line)
		intFailure = tokParsePipeline(strPipeline);
		switch ( intFailure ) {  // Tokenizing or parsing error
			case PARSE_OKAY:   // No Error
				break;
			case PARSE_PREV_HAND_ERROR:  // ERROR: fail and return error
				return PARSE_PREV_HAND_ERROR;
			default:
				fprintf(stderr, "Unsuspected error\n");
				return PARSE_GEN_ERROR;
		}
	} while ( (strPipeline = strtok_r(NULL, ";", &bufFullCmdLnTokSave)) != NULL );
	// Return to caller
	return PARSE_OKAY;
}

/*
 * Description: Tokenizes ('|' separator) and parses pipeline
 */
int tokParsePipeline(char *strPipeline) {
	// Declares required variables
	char *strPipeCmds[MAX_NUM_PIPE];  // Array of commands (w/ args & redirects) in pipeline
	//char strWhiteSpaceCheck[MAX_STR_LEN];  // Used to check for blank command in pipeline
	char strSingleCmd[MAX_STR_LEN];        // Single command (w/ args & redirects)
	char *strPipe;                         // Pipe token
	char *bufPipelineTokSave;              // Pipeline strtok_r save buffer
	//char *bufWhiteSpaceStrip;              // Whitespace check strtok_r save buffer
	char flgPipeline = PIPE_NO;            // Flag indicating place in pipeline
	int intNumPipeCmds = 0;                // Number of commands in pipeline
	int intPipeLpCntl = 0;                 // Loop control
	int intFailure;                        // Called function return value
	int fdPrevPipeRd = PIPE_BAD_FD;        // fd management in case of error
	int intChildStatus;                    // Status of child waited on
	// Performs function
	if ( strPipeline[strlen(strPipeline)- 1] == '|' ) {  // ERROR: Cannot end in a pipe symbol '|'
		fprintf(stderr, "Cannot end in a pipe.\n");
		return PARSE_GEN_ERROR;
	}
	// Determine if blank first command in pipeline
	// Build pipeline array
	strPipe = strtok_r(strPipeline, "|", &bufPipelineTokSave);
	do {
		// Determine if blank command within pipeline
		if ( isOnlyWhiteSpace(strPipe) ) {  // Blank command (NOP) in pipeline, ERROR
			fprintf(stderr, "Blank command (NOP) in pipeline.\n");  // SCY Class of 2016
			return PARSE_GEN_ERROR;
		}
		strPipeCmds[intNumPipeCmds] = strPipe;
		intNumPipeCmds++;
	} while ( ((strPipe = strtok_r(NULL, "|", &bufPipelineTokSave)) != NULL) &&
	          (intNumPipeCmds < (MAX_NUM_PIPE - 1)) );
	strPipeCmds[intNumPipeCmds] = NULL;
	// Iterate through commands in pipeline
	if (intNumPipeCmds > 1) {  // Determine and set in pipeline
		flgPipeline |= PIPE_IN;
	}
	while ( (intPipeLpCntl < intNumPipeCmds) &&
	        (intPipeLpCntl < MAX_NUM_PIPE) ) {  // Pipeline loop
		// Set flgPipeline approriately
		if ( flgPipeline & PIPE_IN ) {  // In pipeline
			// Determine and set beginning of pipeline
			if (intPipeLpCntl == 0) {
				flgPipeline |= PIPE_FIRST;   // Set beginning of pipeline
			} else {
				flgPipeline &= ~PIPE_FIRST;  // Unset beginning of pipeline
			}
			// Determine and set end of pipeline
			if (intPipeLpCntl == (intNumPipeCmds - 1)) {
				flgPipeline |= PIPE_LAST;   // Set end of pipeline
			} else {
				flgPipeline &= ~PIPE_LAST;  // Unset end of pipeline
			}
		}
		// Copy single command w/ args & redirect
		strncpy(strSingleCmd, strPipeCmds[intPipeLpCntl], MAX_CMD_LN_LEN);
		intFailure = tokParseSingleCmd(strSingleCmd, flgPipeline, &fdPrevPipeRd);
		switch (intFailure) {  // Determine error returned
			case PARSE_OKAY:   // Success
				break;
			case PARSE_GEN_ERROR:  // New error
			case PARSE_PREV_HAND_ERROR:  // Previously handled error
			default:
				// Wait for All My Children
				while ( wait(&intChildStatus) > 0) {
				}
				// Determine and close previous pipe read end
				if ( fdPrevPipeRd != PIPE_BAD_FD) {
					close(fdPrevPipeRd);
				}
				return PARSE_PREV_HAND_ERROR;
		}
		intPipeLpCntl++;
	}  // Pipeline loop
	// Wait for All My Children
	while ( wait(&intChildStatus) > 0) {
	}
	if ( fdPrevPipeRd != PIPE_BAD_FD) {
		close(fdPrevPipeRd);
	}
	// Return to caller
	return PARSE_OKAY;
}

/*
 * Description: Tokenizes (' ','\t' separator) and parses a single command
 * Notes:
 *   Student: Most of your code should be in here
 */
int tokParseSingleCmd(char *strSingleCmd, char flgPipeline, int *fdPrevPipeRd) {
	// Declare required variables
	// Single command line
	char *strSingleCmdLnArgs[MAX_NUM_ARGS];  // Array of strings representing a single command with arguments and with redirects
	char *strSingle;         // Copy of single command to pass to strtok_r [trustno1]
	char *strToken;          // Single token
	char *bufSingleTokSave;  // Buffer for strtok_r bookkeeping
	int intNumTokens = 0;    // Number of tokens
	int intFailure = -1;
	// Parsing
	char flgParse = PARSE_OKAY;  // Flag for parsing
	// Process management
	pid_t pidFork;       // PID from fork()
	// Redirection
	char *strSingleCmdLnArgsSansRedir[MAX_NUM_ARGS];  // Array of single commands w/ args w/o redirects
	char flgRedirect = REDIR_NO;    // Flag for redirection
	int intToken = 0;               // Loop control
	int intRedirInTargetPos = -1;   // Position in strSingleCmdLnArgs of stdin redirect target
	int intRedirOutTargetPos = -1;  // Position in strSignleCmdLnArgs of stdout redirect target
	int fdNewStdIn = STDIN_FILENO;    // New file descriptor for stdin
	int fdNewStdOut = STDOUT_FILENO;  // New file descriptor for stdout
	// Pipeline
	static int pfdCurrPipe[2] = { PIPE_BAD_FD, PIPE_BAD_FD };  // Current pipe
	static int pfdPrevPipe[2] = { PIPE_BAD_FD, PIPE_BAD_FD };  // Previous pipe
	// Lab 0x07 - Part 2 & 3 (uncomment below variable declaration line
	//int intChildStatus;  // Child status
	int dupRet = 0;
	int redirect = 0;
	// Lab 0x07 - Part 4 (comment out above variable declaration line
	// Perform Function

	// Setup strSingle; before returning need to free(strSingle)
	strSingle = calloc( MAX_STR_LEN, sizeof( char ) );  // Assume nothing
	strncpy(strSingle, strSingleCmd, MAX_CMD_LN_LEN);

	// Build strSingleCmdLnArgs, tokenize command line based on ' ', '\t'
	strToken = strtok_r(strSingle, " \t", &bufSingleTokSave);
	do {
		strSingleCmdLnArgs[intNumTokens] = strToken;
		intNumTokens++;
	} while ( (strToken = strtok_r(NULL, " \t", &bufSingleTokSave)) != NULL );
	strSingleCmdLnArgs[intNumTokens] = NULL;

	/* Begin DETECT REDIRECTION */
	while ( (intToken < intNumTokens) &&
	        (intToken < MAX_NUM_ARGS) &&
			!(flgParse & PARSE_GEN_ERROR) ) {
		// Lab 0x07 - Part 2: From Here
		if ( strncmp(strSingleCmdLnArgs[intToken], "<", MAX_STR_LEN) == 0) {  // Redirect standard input
			// Student: Fill in code here

			//   If okay to redirect, then setup redirect position, else PARSE_ERROR
			flgRedirect |= 1;
			intRedirInTargetPos = intToken + 1;
			redirect++;
			if (redirect > 1){ //if more than one redirect
				errorFunc(5);
			}
			if (strSingleCmdLnArgs[intToken + 1] == NULL){ //if there is no source file listed
				errorFunc(10);
			}

			// Lab 0x07 - Part 4: Slight of Hand
			// Student: Address "What if we are in a pipeline?"
			if ((flgPipeline & PIPE_IN) && ! (flgRedirect & PIPE_LAST)){
				errorFunc(8);
			}
		}
		// Lab 0x07 - Part 3: To There
		if ( strncmp(strSingleCmdLnArgs[intToken], ">", MAX_STR_LEN) == 0) {  // Redirect standard output
			// Student: Fill in code here
			//   If okay to redirect, then setup redirect position, else PARSE_ERROR
			flgRedirect |= 2;
			intRedirOutTargetPos = intToken + 1;
			redirect++;
			if (redirect > 1){
				errorFunc(5);
			}
			// Lab 0x07 - Part 4: Slight of Hand
			// Student: Address "What if we are in a pipeline?"
			if ( strncmp(strSingleCmdLnArgs[intToken], "|", MAX_STR_LEN) == 0) {
				flgPipeline = PIPE_IN; //added line
			}
		}
		intToken++;
	}
	/*  End  DETECT REDIRECTION */

	if ( flgParse & PARSE_GEN_ERROR ) {
		// Student: Update below error message based on this block of code
		fprintf(stderr, "Flag parse error.\n");
		free(strSingle);
		return PARSE_GEN_ERROR;
	}
	// No redirection, copy all command line arguments
	if ( ! flgRedirect ) {
		strArrayNCopy(strSingleCmdLnArgsSansRedir, strSingleCmdLnArgs, MAX_STR_LEN, intNumTokens, MAX_NUM_ARGS);
	}
	// Standard input redirection, copy all command line arguments except "<" and redirect target
	if ( flgRedirect & REDIR_STDIN ) {
		if (strSingleCmdLnArgs[intRedirInTargetPos] != NULL) {
			strArrayNCopy(strSingleCmdLnArgsSansRedir, strSingleCmdLnArgs, MAX_STR_LEN, intRedirInTargetPos - 1, MAX_NUM_ARGS);
		} else {  // Student: Update below error message based on this block of code
			fprintf(stderr, "Standard Input Redirection Error.\n");
			free(strSingle);
			return PARSE_GEN_ERROR;
		}
	}
	// Standard output redirection, copy all command line arguments except ">" and redirect target
	if ( flgRedirect & REDIR_STDOUT ) {
		if (strSingleCmdLnArgs[intRedirOutTargetPos] != NULL) {
			strArrayNCopy(strSingleCmdLnArgsSansRedir, strSingleCmdLnArgs, MAX_STR_LEN, intRedirOutTargetPos - 1, MAX_NUM_ARGS);
		} else {  // Student: Update below error message based on this block of code
			fprintf(stderr, "Standard Output Redirection Error.\n");
			free(strSingle);
			return PARSE_GEN_ERROR;
		}
	}

	// Lab 0x07 - Part 4: Slight of Hand
	if ( (flgPipeline & PIPE_IN) && ! (flgPipeline & PIPE_LAST) ) {  // In pipeline
		// Student: Fill in code
		intFailure = pipe(pfdCurrPipe);
		if (intFailure){
			errorFunc(7);
		}
	}

	pidFork = fork();  // Create child
	switch (pidFork) {  // Status of fork()
		case -1:   // ERROR: fork() failed
			// Student: Update below error message based on this block of code
			fprintf(stderr, "Fork failed.\n");
			free(strSingle);
			return PARSE_PREV_HAND_ERROR;
		case 0:    // Child

			// Lab 0x07 - Part 2: From Here
			if ( flgRedirect & REDIR_STDIN ) {  // Setup standard input redirect
				// Student: Fill in code here
					close(STDIN_FILENO);
					int fdNewStdIn = open(strSingleCmdLnArgs[intRedirInTargetPos], O_RDONLY);
					if (fdNewStdIn != -1){
						dupRet = dup2(fdNewStdIn, STDIN_FILENO);
						if (dupRet == -1){
							errorFunc(3);
						}
					}
					else if (errno == EACCES){
						errorFunc(9);
					}
					else{errorFunc(4);}
			}

			// Lab 0x07 - Part 3: To There
			if ( flgRedirect & REDIR_STDOUT ) {  // Setup standard output redirect
				// Student: Fill in code here
				close(STDOUT_FILENO);
				int fdNewStdOut = open(strSingleCmdLnArgs[intRedirOutTargetPos], O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
				if (fdNewStdOut != -1){
					dupRet = dup2(fdNewStdOut, STDOUT_FILENO);
					if (dupRet == -1){
						errorFunc(3);
					}
				}
					else if (errno == EACCES){
						errorFunc(9);
					}
					else{errorFunc(4);}
			}

			// Lab 0x07 - Part 4: Slight of Hand
			if (flgPipeline & PIPE_IN) {  // Setup redirect based on pipeline
				if ( ! (flgPipeline & PIPE_FIRST) ) {  // Not beginning of pipeline
					// Student: Fill in code
					fdNewStdIn = pfdPrevPipe[PIPE_READ_FROM];
				}
				if ( ! (flgPipeline & PIPE_LAST) ) {  // Not end of pipeline
					// Student: Fill in code
					close(pfdCurrPipe[PIPE_READ_FROM]);
					fdNewStdOut = pfdCurrPipe[PIPE_WRITE_TO];

				}
			}

			// Lab 0x07 - Part 2: From Here
			// Lab 0x07 - Part 4: Slight of Hand (should not require changes from Part 2)
			if ( fdNewStdIn != STDIN_FILENO ) {  // Standard input redirect or pipe previously setup
				// Student: Fill in code here
				close(STDIN_FILENO);
				dup2(fdNewStdIn, STDIN_FILENO);
			}

			// Lab 0x07 - Part 3: To There
			// Lab 0x07 - Part 4: Slight of Hand (should not require changes from Part 3)
			if ( fdNewStdOut != STDOUT_FILENO ) {  // Standard output redirect or pipe previously setup
				// Student: Fill in code here
				close(STDOUT_FILENO);
				dup2(fdNewStdOut, STDOUT_FILENO);
			}

			execvp(strSingleCmdLnArgsSansRedir[0], strSingleCmdLnArgsSansRedir);
			// Student: Update below error message based on this block of code
			fprintf(stderr, "Error with exec.\n");
			free(strSingle);
			exit(EXIT_FAILURE);
		default :  // Parent
			// Lab 0x07 - Part 2 & Part 3 (uncomment below line)
			//wait(&intChildStatus);
			// Lab 0x07 - Part 4: Slight of Hand (comment out above line)
			if ( flgPipeline & PIPE_IN ) {  // In pipeline
				if ( ! (flgPipeline & PIPE_FIRST) ) {  // Not beginning of pipeline
					if ( pfdPrevPipe[PIPE_READ_FROM] != PIPE_BAD_FD ) {
						close(pfdPrevPipe[PIPE_READ_FROM]);
					}
				}
				if ( ! (flgPipeline & PIPE_LAST) ) {  // Not end of pipeline
					*fdPrevPipeRd = pfdCurrPipe[PIPE_READ_FROM];
					pfdPrevPipe[PIPE_READ_FROM] = pfdCurrPipe[PIPE_READ_FROM];
					pfdPrevPipe[PIPE_WRITE_TO ] = pfdCurrPipe[PIPE_WRITE_TO ];
					close(pfdCurrPipe[PIPE_WRITE_TO]);
				} else {  // End of pipeline
					if ( pfdCurrPipe[PIPE_READ_FROM] != PIPE_BAD_FD ) {
						close(pfdCurrPipe[PIPE_READ_FROM]);
					}
					if ( pfdCurrPipe[PIPE_WRITE_TO] != PIPE_BAD_FD ) {
						close(pfdCurrPipe[PIPE_WRITE_TO]);
					}
					if ( pfdPrevPipe[PIPE_READ_FROM] != PIPE_BAD_FD ) {
						close(pfdCurrPipe[PIPE_READ_FROM]);
					}
					if ( pfdPrevPipe[PIPE_WRITE_TO] != PIPE_BAD_FD ) {
						close(pfdCurrPipe[PIPE_WRITE_TO]);
					}
					*fdPrevPipeRd = PIPE_BAD_FD;
					pfdCurrPipe[PIPE_READ_FROM] = PIPE_BAD_FD;
					pfdCurrPipe[PIPE_WRITE_TO ] = PIPE_BAD_FD;
					pfdPrevPipe[PIPE_READ_FROM] = PIPE_BAD_FD;
					pfdPrevPipe[PIPE_WRITE_TO ] = PIPE_BAD_FD;
				}
			}
			break;
	}
	free(strSingle);
	// Return to Caller
	return PARSE_OKAY;
}

/* Copy up to N elements of srcCmdLnArgs to destCmdLnArgs
 * Notes:
 *   Student: No changes needed
 */
int strArrayNCopy(char *strDestArray[], char *strSrcArray[], int maxStrLen, int numElemsCopy, int maxNumElems) {
	// Declare Required Variables
	int i = 0;	// Loop control
	// Perform Function
	while ((i < numElemsCopy) && (i < maxNumElems)) {  // Iterate through elements to copy
		if (strSrcArray[i] != NULL) {  // Non-NULL element to copy
			strDestArray[i] = strSrcArray[i];
		} else {  // Nothing (NULL) to copy
			if (strDestArray[i] != NULL) {
				strDestArray[i] = NULL;
			}
		}
		i++;
	}
	while (i < maxNumElems) {  // Clear out the rest of the destination array
		if (strDestArray[i] != NULL) {
			strDestArray[i] = NULL;
		}
		i++;
	}
	// Return to Caller
	return EXIT_SUCCESS;
}

/*
 * Description: Tests whether strCheckForWhiteSpace is only white space (' ', '\t')
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
	strSansWhiteSpace = strtok_r(strWhiteSpaceCheck, " \t", &bufWhiteSpaceCheck);
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
		fprintf(stdout, "\tmyShellpp- A Simple Shell\n");
		fprintf(stdout, "SYNOPSIS\n");
		fprintf(stdout, "\tmyShellpp\n");
		fprintf(stdout, "DESCRIPTION\n");
		fprintf(stdout, "\tExecutes single command lines. All tokens shall be space separated.\n");
		fprintf(stdout, "\t';' is used as a separator between commands on the same line.\n");
		fprintf(stdout, "\t'<' is used to redirect standard input to the command on the left to\n\tcome from the specified on the right.\n");
		fprintf(stdout, "\t'>' is used to redirect standard output to the command on the left to\n\tcome from the specified on the right.\n");
		fprintf(stdout, "\t'|' is used to redirect standard output to the command on the left to\n\tstandard input of the command on the right.\n");
		exit(0);//EXIT WITH NORMAL RETURN VALUE
}
void errorFunc(int errNum){
	switch(errNum){
		case 1: fprintf(stderr,"\nsetenv didn't work.\n\n");
			  exit(errNum);
		case 2: fprintf(stderr, "\nFile specified is inaccessible\n\n");
				exit(errNum);
		case 3: fprintf(stderr, "\nCouldn't dup fd.\n\n");
				exit(errNum);
		case 4: fprintf(stderr, "\nFile does not exist.\n\n");
				exit(errNum);
		case 5: fprintf(stderr, "\nToo many redirects, bro\n\n");
				exit(errNum);
		case 6: fprintf(stderr, "\nToo many arguments\n\n");
				exit(errNum);
		case 7: fprintf(stderr, "\nCouldn't initialize pipe.\n\n");
				exit(errNum);
		case 8: fprintf(stderr, "\nCannot redirect in a pipe.\n\n");
				exit(errNum);
		case 9: fprintf(stderr, "\nDid not have effective permissions to open file.\n\n");
				exit(errNum);
		case 10: fprintf(stderr, "\nDid not enter file in command line.\n\n");
				exit(errNum);
		}
}
