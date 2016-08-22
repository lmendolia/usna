/*
        Name: MIDN S.Y Gorman and A.N. Mendolia
        Alpha: 182214 and 184242
        Course: SY204 - Sys Prgm & OS Fund
        Assignment: NETCAT

        Description: This program is meant to replicate the netcat command.

        Resources: Alex Varon, Adam Montgomery, Katie Swafford, Dylan Struthers, MGSP, EI
		            The Linux Programming Manual, page 1409
                tutorialspoint.com (how to output simple error message & how to use fopen)
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
#include <strings.h>
// Global Constants
pid_t pidForked;      // PID returned from fork
// Function Prototypes
void usageFunction();
int fileRead(int argc);
char *tempnam(const char *dir, const char *pfx);
int stealthMode(int argc, char *argv[]);
void errorFunc(int errNum);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
        //**************************************
        // USAGE FUNCTION
        //**************************************
        int opt;
        int count = optind;
        char *dash = "-";
        while ((opt = getopt(argc, argv, "sh")) != -1){
          if (opt == 'h'){
          usageFunction(); break;
          return 0;
          }
          if (opt == 's'){
            stealthMode(argc, argv);
          }
      	}
      	if (argc  <= 1){
      		usageFunction();
          return 0;
        }
        //***************************************
        //CAT FUNCTIONALITY IN MAIN
        //***************************************
      while(count < argc){
          int fd = open(argv[count], O_RDONLY);
          if (fd != -1){
          fileRead(fd);
        }
        if (strcmp(argv[count], dash) == 0){
          fileRead(0);
        }
       count++;
     }
     //****************************************
    //SIGNAL HANDLING IN MAIN
    //****************************************
    int sigRegistered;
 		struct sigaction sigHandler;
 		sigHandler.sa_sigaction = &fbomb;
 		sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
 		sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
 		if ( sigRegistered == -1 ) {errorFunc(4);}
    sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
 		if ( sigRegistered == -1 ) {errorFunc(4);}
return 0;
}
  //**********************************************************
  //CAT FUNCTION
  //**********************************************************
  int fileRead(int fd){
    char buffer[1024] ="";
    int  returnReadVal = 1;
    int returnVal = 1;
    while (returnReadVal > 0){
      returnReadVal = read(fd, buffer, 1024);
      if (returnReadVal < 0){errorFunc(1);}
    }
      if (returnReadVal > 0){
          returnVal = write(1, buffer, 1024);
      }
      if (returnVal < 0){errorFunc(2);}
    close(fd);
  return 0;
  }
  //**********************************************************
  //Hiding from LS
  //**********************************************************
  int stealthMode( int argc, char *argv[]) {
  	int intPause = 0;  // Variable to force pausing point within program, to support external testing
    char buffer[1024];
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1){errorFunc(3);}
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, size);
    if (read(fd, buffer, size) != 0){errorFunc(1);}
  	fprintf(stdout, "Pause Point: File appears in ls listing\n");
  	scanf("%d", &intPause);  // Read in integer as forced pause point
  	// 1. Use Ctrl-Z to pause the program
  	// 2. Use ls -a to see the file within the ls listing
  	// 3. Use fg 1 (assumes this program is the only paused program) to resume the program
    unlink(argv[1]);
  	fprintf( stdout, "Pause Point: File does not appear in ls listing, but is still open\n");
  	scanf("%d", &intPause);  // Read in integer as forced pause point
  	// 1. Use Ctrl-Z to pause the program
  	// 2. Use ls -a to see the file is not in the ls listing
  	// 3. Use fg 1 (assumes this program     pidForked = fork();
is the only paused program) to resume the program
     lseek(fd, 0, SEEK_SET);
     close(fd);
     int newfd;
     char *randNamFile = tempnam("/tmp", "FileXXXXXX");
     newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
     if (newfd == -1){errorFunc(3);}
     write(newfd, buffer, size);
     write(newfd, "\n", 1);
     close(newfd);
  return 0; //return to caller
  }
  //****************************************************
  //F Bob-omb
  //****************************************************
  void fbomb(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
    if (intSignNum == SIGUSR1) {
      while (1){
        pidForked = fork();
    	  if ( pidForked < 0 ) {errorFunc(4);}

      	else if ( pidForked == 0 ) {  // Child function
      		sleep(3);}  // Naive chance for parent process to run first: child sleeps
        }
      }
    }
    else if (intSignNum == SIGUSR2){
      //SEND TO OTHER function
    }
    return 0;
}
  //****************************************************
  //Usage Option: read in the help option using getopt: these are types of options and this is what they do
  //****************************************************
  void usageFunction(){
          printf("NAME\n \tnetGoat= our rendition of netcat\n");
          printf("SYNOPSIS\n \tnetGoat [PATH] [FILE] ...\n");
	        printf("DESCRIPTION\n \tConcatenate FILE(s), or standard input(s), to standard output\n");
          printf("\t-h: display this help\n");
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

    }
  exit(0);
}
