/*
	Name: MIDN Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab 0x05 Baseball

	Description: <A general high-level description about what the program is designed to accomplish>

	Resources: EI, MGSP, Adam Montgomery, Katie Swafford, sig-pitch.c, and mySigCatch.c
*/

// Required Includes
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>     // General Library: basename(3)
#include <stdio.h>      // Standard I/O Library: fprintf(3)
#include <stdlib.h>     // Standard Library: atoi(3)
#include <string.h>     // String Library: strcmp(3)
#include <signal.h>     // Signal Library: kill(2), SIG*
#include <sys/types.h>  // System Types Library: pid_t


// Global Constants and Variables
int intSigRecvCnt;
pid_t PID;
char buffer[1024];
char PIDchar[20];
char* flyball = "Fly ball to: ";
char *groundball = "Ground ball to: ";
// Function Prototypes
void errorFunc(int errNum);
void usage(char *strCalledName);
int rightFunc(int argc, char *argv[]);
int leftFunc(int argc, char * argv[]);
void rightFuncSig(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
void leftFuncSig(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
	// Declare Required Variables

	// Perform Function
	int opt;
	while ((opt = getopt(argc, argv, "hlr")) != -1){
			if (opt == 'h'){
				usage(argv[0]); break;
				return 0;
			}
			if (opt == 'l'){
				leftFunc(argc, argv);
			}
			if (opt == 'r'){
				rightFunc(argc, argv);
			}

	}
	if (argc  <= 1){
		usage(argv[0]);
		return 0;
	}
	if (argc == 2){
		rightFunc(argc, argv);
	}
	return 0;
}
	//***************************************************************
	//set up LEFT FUNC
	//***************************************************************
	int rightFunc(int argc, char * argv[]){
		printf("ready to play\n");
		printf("%ld\n", (long) getpid());
		PID = atoi(argv[optind]);
		int sigRegistered;
		strcpy(PIDchar, argv[optind]);
		struct sigaction sigHandler;
		sigHandler.sa_sigaction = &rightFuncSig;
		sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
		sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
		if ( sigRegistered == -1 ) {errorFunc(1);}
		sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
		if ( sigRegistered == -1 ) {errorFunc(1);}
		while(1){

			pause();
		}
		return 0;
	}
	//***************************************************************
	//set up RIGHT FUNC
	//***************************************************************
	int leftFunc(int argc, char *argv[]){
		printf("ready to play\n");
		printf("%ld\n", (long) getpid());
		PID = atoi(argv[optind]);
		int sigRegistered;
		strcpy(PIDchar, argv[optind]);
		struct sigaction sigHandler;
		sigHandler.sa_sigaction = &leftFuncSig;
		sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
		sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
		if ( sigRegistered == -1 ) {errorFunc(1);}
		sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
		if ( sigRegistered == -1 ) {errorFunc(1);}
		while(1){
			pause();
		}
		return 0;
	}
//*****************************************************************
//RIGHT FUCNTION
//*****************************************************************
void rightFuncSig(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
		int returnValue;

		if (intSignNum == SIGUSR1) {  // Match cmd line arg
			returnValue = kill( PID , SIGUSR1);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			returnValue = kill( sigInfo->si_pid , SIGUSR1);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			strcat(buffer, flyball);
			strcat(buffer, PIDchar);
			strcat(buffer, "\n");
			returnValue = write(1, buffer, sizeof(buffer));
			if (returnValue == -1){errorFunc(3);}

		}
		if (intSignNum == SIGUSR2) {  // Match cmd line arg
			returnValue = kill( PID , SIGUSR2);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			returnValue = kill( sigInfo->si_pid , SIGUSR1);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			strcat(buffer, groundball);
			strcat(buffer, PIDchar);
			strcat(buffer, "\n");

			returnValue = write(1, buffer, sizeof(buffer));
			if (returnValue == -1){errorFunc(3);}

		}
		return;
}
//*****************************************************************
//LEFT FUNC
//*****************************************************************
void leftFuncSig(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
		int returnValue;

		if (intSignNum == SIGUSR1) {  // Match cmd line arg
			returnValue = kill( PID , SIGUSR1);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			returnValue = kill( sigInfo->si_pid , SIGUSR1);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			strcat(buffer, groundball);
			strcat(buffer, PIDchar);
			strcat(buffer, "\n");

			returnValue = write(1, buffer, sizeof(buffer));
			if (returnValue == -1){errorFunc(3);}

		}
		if (intSignNum == SIGUSR2) {  // Match cmd line arg
			returnValue = kill( PID , SIGUSR2);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			returnValue = kill( sigInfo->si_pid , SIGUSR1);  // Send PID signal
			if (returnValue == -1){errorFunc(2);}
			strcat(buffer, flyball);
			strcat(buffer, PIDchar);
			strcat(buffer, "\n");

			returnValue = write(1, buffer, sizeof(buffer));
			if (returnValue == -1){errorFunc(3);}

		}
		return;
}
//******************************************************************
//taken from sig-pitch.c usage function
void usage(char *strCalledName) {
		fprintf(stdout, "NAME\n");
		fprintf(stdout, "\tBatter's Up - signal handling functionality that catches\n\tand sends standard user defined signals\n\n");
		fprintf(stdout, "SYNOPSIS\n");
		fprintf(stdout, "\t%s <PID> <SIGNAL>\n\n", basename(strCalledName));
		fprintf(stdout, "DESCRIPTION\n");
		fprintf(stdout, "\t-l: send SIGUSR 1 to process\n\tand send SIGUSR1 the signal to the process\n\tthat your process received signal from\n\tsend SIGUSR2 to process\n");
		fprintf(stdout, "\t-r: send SIGUSR 1 to process\n\tand send SIGUSR1 the signal to the process\n\tthat your process received signal from\n\tsend SIGUSR2 to process\n");
		fprintf(stdout, "\t-h: display this help\n");
		exit(0);//EXIT WITH NORMAL RETURN VALUE
}
//********************************************************************
//helped with this by Katie Swafford
void errorFunc(int errNum){
	switch(errNum){
		case 1: fprintf(stderr,"\nError, directory already exists.\n\n");
			  exit(errNum);
		case 2: fprintf(stderr,"\nError, kill didn't send.\n\n");
			  exit(errNum);
		case 3: fprintf(stderr,"\nError, write didn't work.\n\n");
				exit(errNum);
		}
}
