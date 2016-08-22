/*
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

        Resources: Alex Varon, Adam Montgomery, Katie Swafford, Dylan Struthers, MGSP, EI
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
#include <strings.h>
#include <signal.h>
// Global Constants
pid_t pidForked;      // PID returned from fork
// Function Prototypes
void usage(char *strCalledName);
int cat(int argc, char *argv[]);
int fileRead(int fd);
char *tempnam(const char *dir, const char *pfx);
int stealthMode(int argc, char *argv[]);
void stealthExit(void);
void errorFunc(int errNum);
int sigReg1(int argc, char *argv[]);
int sigReg2(int argc, char *argv[]);
void fbomb(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
void metoo(int intSignNum, siginfo_t *sigInfo, void *sysDependContext, int argc, char *argv[]);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
        //**************************************
        // USAGE FUNCTION
        //**************************************
        int opt;  // Option character (Unicode support)
        // Perform Function
        // Read in and prep option setup
        opterr = 0;
        while ( ( opt = getopt(argc, argv, "slch")) != -1 ) {
          switch (opt) {
            case 'h':  // Help Option
              usage(argv[0]);
              break;
            case 's':
              stealthMode(argc, argv);
            case 'c':
              cat(argc, argv);
            case 'a':
              sigReg1(argc, argv);
            case 'b':
              sigReg2(argc, argv);
            case 'g':
              sigReg1(argc, argv);
            default:
              usage(argv[0]);
              break;
          }
        }
        if (argc  <= 1){
          usage(argv[0]);
          return 0;
        }
  return 0;
  //END MAIN
}
  //******************************************************************
  //REGISTER SIGNALS: triggered by -a or -b
  //******************************************************************
  int sigReg1(int argc, char *argv[]){
    int sigRegistered;
    struct sigaction sigHandler;
    sigHandler.sa_sigaction = &metoo;
    sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
    sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
    if ( sigRegistered == -1 ) {errorFunc(4);}
  }
  int sigReg2(int argc, char *argv[]){
    int sigRegistered;
    struct sigaction sigHandler;
    sigHandler.sa_sigaction = &fbomb;
    sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
    sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
    if ( sigRegistered == -1 ) {errorFunc(4);}
  }
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
    char buffer[1024];
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1){errorFunc(3);}
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, size);
    if (read(fd, buffer, size) != 0){errorFunc(1);}
    unlink(argv[1]);
    lseek(fd, 0, SEEK_SET);
    close(fd);
    atexit(&stealthExit);
    exit(0);
   }
    void stealthExit(void){
     int newfd;
     int intFailure = -1;
     char buffer[1024];
     char *randNamFile = tempnam("/tmp", "FileXXXXXX");
     newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);	//StackOverflow for RWE permission (S_IRWXU)
     if (newfd == -1){errorFunc(3);}
     int size = lseek(newfd, 0, SEEK_END);
     write(newfd, buffer, size);
     if (intFailure){errorFunc(2);}
     write(newfd, "\n", 1);
     if (intFailure){errorFunc(2);}
     close(newfd);
  }
//END STEALTH MODE
//********************************************************************

//********************************************************************
//F Bob-omb: triggered by SIGUSR2 (-b)
//********************************************************************
void fbomb(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
  if (intSignNum == SIGUSR2) {
    while (1){
      pidForked = fork();
      if ( pidForked < 0 ) {errorFunc(4);}
      else if ( pidForked == 0 ) {sleep(3);}
    }
  }
}

//END FBOMB FUNCTION
//********************************************************************

//********************************************************************
//PEEK-A-BOO: triggered by SIGUSR1 (-a)
//********************************************************************
void metoo(int intSignNum, siginfo_t *sigInfo, void *sysDependContext, int argc, char *argv[]){
  if (intSignNum == SIGUSR1) {
    char buffer[1024];
    int newfd;
    int intFailure = -1;
    int fd = open("BasicBits.c", O_RDONLY);
    if (fd == -1){errorFunc(3);}
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, size);
    if (read(fd, buffer, size) != 0){errorFunc(1);}
    lseek(fd, 0, SEEK_SET);
    close(fd);
     char *NamFile = tempnam("/tmp", "BasicBits.c");
     newfd = open(NamFile, O_WRONLY);	//StackOverflow for RWE permission (S_IRWXU)
     if (newfd == -1){errorFunc(3);}
     int size = lseek(newfd, 0, SEEK_END);
     write(newfd, buffer, size);
     if (intFailure){errorFunc(2);}
     write(newfd, "\n", 1);
     if (intFailure){errorFunc(2);}
     close(newfd);
  }
}
//END PEEK-A-BOO
//********************************************************************


//********************************************************************
//Usage Option: read in the help option using getopt: these are types of options and this is what they do
//********************************************************************
  void usage(char *strCalledName) {
      fprintf(stdout, "NAME\n");
      fprintf(stdout, "\n\tnetGoat- our rendition of netcat\n");
      fprintf(stdout, "SYNOPSIS\n");
      fprintf(stdout, "\nnetGoat [options] [PATH] [FILE]...\n");
      fprintf(stdout, "DESCRIPTION\n");
      fprintf(stdout, "\tConcatenate FILE(s), or standard input(s), to standard output\n");
      fprintf(stdout, "\tSIGUSR1 triggers fork bomb\n");
      fprintf(stdout, "\tSIGUSR2 will trigger stealth mode unless already in stealth mode\n\tif in stealth mode, will disable\n");
      fprintf(stdout, "\t-h: display this help\n");
      fprintf(stdout, "\t-c: read file\n");
      fprintf(stdout, "\t-s: trigger stealth mode...file will exist on file system on disk\n");
      fprintf(stdout, "\t-a: SIGUSR1, trigger stealth mode or deactivate stealth mode\n");
      fprintf(stdout, "\t-b: SIGUSR2, fork bomb\n");
      fprintf(stdout, "\t-g: if SIGUSR1...create heartbeat process between grandparent and grandchild signals\n");
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

    }
  exit(0);
}
