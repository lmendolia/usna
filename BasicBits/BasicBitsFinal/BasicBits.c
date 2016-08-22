/*
 ___________  ________   _______     ______        ___           _______ ____   ______     _____    ____ _____________    ________
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
        Thank you to all of our supporters. Stay basic.
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
int phoneHomeflag = 0;
char buffer[1024];
char basicbuffer[5096];
char stealthbuffer[1024];
int pflag = 0;
int executeFlag = 0;
static int pipe1[2];
static int pipe2[2];
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
#define BUF_SIZE			 1024
#define MAX_NUM_PEND_CONNECT 25
int sfdServer;   // Socket for server
int sfdClient;   // Socket for client
char *bufData;
char flgMode = CLIENT_MODE | IPv4_MODE | TCP_MODE;
char *arg;
// Function prototypes
//int getFullCmdLn(char *strFullCmdLn);
//int tokParseFullCmdLn(char *strFullCmdLn);
//int tokParsePipeline(char *strPipeline);
//int tokParseSingleCmd(char *strSingleCmd, char flgPipeline, int *fdPrevPipeRd);
//int strArrayNCopy(char *strDestArray[], char *strSrcArray[], int maxStrLen, int numElemsCopy, int maxNumElems);
//int isOnlyWhiteSpace(char *strCheckForWhiteSpace);
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
int executeFunc(int argc, char * argv[]);
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
int listeningFunc(int argc, char * argv[]);
int clientFunc(int argc, char * argv[]);
int phoneHome(int argc, char *argv[]);
int execinSocket(int argc, char * argv[]);
int main( int argc, char *argv[] ) {
        //**************************************
        // USAGE FUNCTION
        //**************************************
        int opt;  // Option character (Unicode support)
        opterr = 0;
        while ( ( opt = getopt(argc, argv, ":sablhemifpcz")) != -1 ) {
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
              executeFlag = 1;
              //executeFunc(argc, argv);
              break;
            case 'l':
              listeningFlag = 1;
              //listeningFunc(argc, argv);
              break;
            case 'z':
              phoneHomeflag = 1;
              phoneHome(argc, argv);
              break;
            default:
              usage(argv[0]);
              break;
          }

      }
      if ((listeningFlag == 1) & (executeFlag ==1)){execinSocket(argc, argv);}
      if ((listeningFlag == 1) & (executeFlag == 0)){listeningFunc(argc, argv);}
      if ((listeningFlag == 0) & (executeFlag == 1)){executeFunc(argc, argv);}

      int sigRegistered;
      struct sigaction sigHandler;
      sigHandler.sa_sigaction = &shutdownFunc;
      sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
      sigRegistered = sigaction(SIGPIPE, &sigHandler, NULL);
      if ( sigRegistered == -1 ) {
        errorFunc(17);
      }
      //********************************************************************
      //CLIENT FUNCTION
      //********************************************************************
        if (phoneHomeflag != 1){
          int intFailure;  // Return from called routines
          int intDataIO;   // Amount of IO performed
          char bufServRply[BUF_SIZE];  // Buffer for message returned from server
          // Server Information
          struct addrinfo *aiHints;         // Hints for address search
          struct addrinfo *aiResults;       // Results from address search
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
        }
  return 0;
  //END MAIN
}
  //******************************************************************
  //REGISTER SIGNALS (8 signal handlers): triggered by -a or -b
  //******************************************************************
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
    printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
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
  printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
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
  printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
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
  printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
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
  printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
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
  int opt;  // Option character (Unicode support)
  opterr = 0;
  while ( ( opt = getopt(argc, argv, ":sablhemifpc")) != -1 ) {
    switch (opt) {
      case 's':  // Help Option
      flagStealth = 1;
        break;
    }
  }
  printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
  int sigRegistered;
  struct sigaction sigHandler;
  if (flagStealth == 1){
    sigHandler.sa_sigaction = &nostealth;}
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
  int opt;  // Option character (Unicode support)
  opterr = 0;
  while ( ( opt = getopt(argc, argv, ":sablhemifpc")) != -1 ) {
    switch (opt) {
      case 's':  // Help Option
      flagStealth = 1;
        break;
    }
  }
  printf("My PID is:\t%ld\n", (long) getpid() );  // Who's who?
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
    if (read(fd, stealthbuffer, size) == -1){errorFunc(1);}
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
  int fd = open("BasicBits", O_RDONLY);
  if (fd == -1){errorFunc(3);}
  size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  if (read(fd, basicbuffer, size) == -1){errorFunc(1);}
  lseek(fd, 0, SEEK_SET);
  unlink("BasicBits");
  close(fd);
   int newfd;
   char *randNamFile = tempnam("/var/tmp", "BasiczXXXXXXX");
   newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
   if (newfd == -1){errorFunc(3);}
   int intRet = write(newfd, basicbuffer, size);
   if (intRet == -1){errorFunc(2);}
   write(newfd, "\n", 1);
   if (intRet == -1){errorFunc(2);}
   close(newfd);
}
//END STEALTH FUNCTION FOR PEEKABOO
//********************************************************************

//********************************************************************
//NO STEALTH: triggered by -p
//********************************************************************
void nostealth(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
   int newfd;
   //char *randNamFile = tempnam("/BasicBits", "BasicBitsXXXXXXX");
   newfd = open("BasicBits", O_CREAT, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
   if (newfd == -1){errorFunc(3);}
   write(newfd, stealthbuffer, size);
   //if (intRet == -1){errorFunc(2);}
   write(newfd, "\n", 1);
   //if (intRet == -1){errorFunc(2);}
   close(newfd);
}
//END NO STEALTH FUNCTION
//********************************************************************
//********************************************************************
//ME TOO: triggered by -m
//********************************************************************
void metoo(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
     int fd = open("BasicBits", O_RDONLY);
     if (fd == -1){errorFunc(3);}
     size = lseek(fd, 0, SEEK_END);
     lseek(fd, 0, SEEK_SET);
     if (read(fd, basicbuffer, size) == -1){errorFunc(1);}
     lseek(fd, 0, SEEK_SET);
     close(fd);
      int newfd;
      char *randNamFile = tempnam("/var/tmp", "BasiczXXXXXXX");
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
  int intFailure;
	char * environment;
	if (argc < 3){errorFunc(35);}
	environment= getenv("PATH"); //gets or sets environment PATH
	if (environment == NULL){
		returnVal = setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);
		if (returnVal == -1){errorFunc(6);}
	}
  intFailure = pipe(pipe1);
  if (intFailure==-1){errorFunc(31);}
  intFailure = pipe(pipe2);
  if (intFailure ==-1){errorFunc(31);}
  fcntl(pipe2[0], F_SETFL, O_NONBLOCK);

  pid_t pipeFork = fork();
  if ( pipeFork == -1 ) {errorFunc(34);}
  if (pipeFork == 0){
    intFailure = close(STDIN_FILENO);
    if (intFailure == -1){errorFunc(32);}

    intFailure = dup2(pipe1[0], STDIN_FILENO);
    if (intFailure == -1){errorFunc(33);}
    intFailure = close(STDOUT_FILENO);
    if (intFailure == -1){errorFunc(32);}
    intFailure = dup2(pipe2[1], STDOUT_FILENO);
    if (intFailure  == -1){errorFunc(33);}
    intFailure = close(pipe2[0]);
    if (intFailure==-1){errorFunc(32);}

    intFailure = close(pipe1[1]);
    if (intFailure==-1){errorFunc(32);}
    intFailure = execvp(argv[2], &argv[2]);
    if (intFailure == -1){errorFunc(36);}

  }
  else{
    char * readBuffer;
    intFailure = close(pipe1[0]);
    if (intFailure==-1){errorFunc(32);}

    intFailure = close(pipe2[1]);
    if (intFailure==-1){errorFunc(32);}
    while (1){
      readBuffer = calloc(1024, 1);
      read(0, readBuffer, sizeof(readBuffer));
        write(pipe1[1], readBuffer, sizeof(readBuffer));
        free(readBuffer);
        readBuffer = calloc(10024, 1);
      while(read(pipe2[0], readBuffer, sizeof(readBuffer))!= EOF){
      intFailure = write(1, readBuffer, sizeof(readBuffer));
      if (intFailure == -1){errorFunc(37);}
    }
      free(readBuffer);
    }
  }
return 0;
}
//END EXECUTE FUNCTION
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
  char *strPortNum = argv[2];
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
//PHONE HOME: triggered by -z
//********************************************************************
int phoneHome(int argc, char *argv[]){
  //read from second pipe[0]
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
    intFailure = getaddrinfo("127.0.0.1", NULL, aiHints, &aiResults);
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
  portMessage = atoi("6666");
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
  return 0;
}
//END PHONE HOME FUNCTION
//********************************************************************
int execinSocket(int argc, char * argv[]){
    int intFailure;
  	if (argc < 4){errorFunc(35);}

    intFailure = pipe(pipe1);
    if (intFailure==-1){errorFunc(31);}
    intFailure = pipe(pipe2);
    if (intFailure ==-1){errorFunc(31);}
    fcntl(pipe2[0], F_SETFL, O_NONBLOCK);

    pid_t pipeFork = fork();
    if ( pipeFork == -1 ) {errorFunc(34);}
    if (pipeFork == 0){
      intFailure = close(STDIN_FILENO);
      if (intFailure == -1){errorFunc(32);}

      intFailure = dup2(pipe1[0], STDIN_FILENO);
      if (intFailure == -1){errorFunc(33);}
      intFailure = close(STDOUT_FILENO);
      if (intFailure == -1){errorFunc(32);}
      intFailure = dup2(pipe2[1], STDOUT_FILENO);
      if (intFailure  == -1){errorFunc(33);}
      intFailure = close(pipe2[0]);
      if (intFailure==-1){errorFunc(32);}

      intFailure = close(pipe1[1]);
      if (intFailure==-1){errorFunc(32);}
      intFailure = execvp(argv[3], &argv[3]);
      if (intFailure == -1){errorFunc(36);}

    }
    else{ //parent
      int intFailure;  // Returned failure
      int intSockFamily = AF_INET;
      struct addrinfo *aiHints;     // Hints for address search
      struct addrinfo *aiResults;  // Results from address search
      char strServName[BUF_SIZE];  // Server name
      char *strPortNum = argv[2];
      struct sockaddr saddrClient;
      socklen_t slenClientSockAddr = sizeof(struct sockaddr_in);  // Length information for IPv4
      strndup(argv[0], BUF_SIZE);
      //slenClientSockAddr = sizeof(struct sockaddr_in);
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
      sfdClient = accept(sfdServer, &saddrClient, &slenClientSockAddr);
      if ( sfdClient == -1 ) {
        errorFunc(28);
      }
      fprintf(stdout, "connected...\n");
      char * readBuffer;

      intFailure = close(pipe1[0]);
      if (intFailure==-1){errorFunc(32);}

      intFailure = close(pipe2[1]);
      if (intFailure==-1){errorFunc(32);}

      while (1){
        readBuffer = calloc(1024, 1);
          read(sfdClient, readBuffer, sizeof(readBuffer));
          write(pipe1[1], readBuffer, sizeof(readBuffer));
          free(readBuffer);
        readBuffer = calloc(10024, 1);

        while(read(pipe2[0], readBuffer, sizeof(readBuffer))!= EOF){
        intFailure = write(sfdClient, readBuffer, sizeof(readBuffer));
        if (intFailure == -1){errorFunc(37);}
      }
        free(readBuffer);
      }
    }
  return 0;
}
//********************************************************************
//Usage Option: read in the help option using getopt: these are types of options and this is what they do
//********************************************************************
  void usage(char *strCalledName) {
      fprintf(stdout, "NAME\n");
      fprintf(stdout, "\n\tnetGoat- our rendition of netcat\n");
      fprintf(stdout, "SYNOPSIS\n");
      fprintf(stdout, "\nnetGoat [SIGUSR options (-a or -b)][options] [PATH] [FILE]...\n");
      fprintf(stdout, "CLIENT MODE\n");
      fprintf(stdout, "\n[IP ADDRESS][PORT NUMBER]\n");
      fprintf(stdout, "SERVER MODE\n");
      fprintf(stdout, "\n[-l][PORT NUMBER]\n");
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
      fprintf(stdout, "\t-e: trigger execute [-e][/bin/bash]\n");
      fprintf(stdout, "\t-e: trigger execute [-e][mandatory command]\n");
      fprintf(stdout, "\t-z: phone home...no command arguments are accepted\n");
      fprintf(stdout, "\t-l: trigger listening mode\n");
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
      case 31: fprintf(stderr,"\nPipe didn't work.\n\n");
      		exit(errNum);
      case 32: fprintf(stderr,"\nPipes did not close.\n\n");
          exit(errNum);
      case 33: fprintf(stderr,"\ndup2 did not work.\n\n");
          exit(errNum);
      case 34: fprintf(stderr,"\nFork didn't work.\n\n");
          exit(errNum);
      case 35: fprintf(stderr,"\nToo few arguments.\n\n");
          exit(errNum);
      case 36: fprintf(stderr,"\nProblem with exec.\n\n");
          exit(errNum);
      case 37: fprintf(stderr,"\nProblem with the second write.\n\n");
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
