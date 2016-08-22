/*
	Name: MIDN Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab 0x05 Baseball

	Description: <A general high-level description about what the program is designed to accomplish>

	Resources: Katie Swafford, sig-pitch.c,mySigCatch.c, EI
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

// Function Prototypes
void errorFunc(int errNum);
void usage(char *strCalledName);
void advSigHandler(int intSigNum, siginfo_t *sigInfo, void *sysDependContext);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
	// Declare Required Variables

	// Perform Function
	int opt;
	while ((opt = getopt(argc, argv, "h")) != -1){
			if (opt == 'h'){
				usage(argv[0]); break;
				return 0;
			}
	}
	if (argc  <= 1){
		printf("argc is less than or equal to one");
		usage(argv[0]);
		return 0;
	}
	//***************************************************************
	printf("%ld\n", (long) getpid());
	PID = atoi(argv[1]);
	int sigRegistered;

	struct sigaction sigHandler;
	sigHandler.sa_sigaction = &advSigHandler;
	sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
	sigRegistered = sigaction(SIGUSR1, &sigHandler, NULL);
	if ( sigRegistered == -1 ) {errorFunc(1);}
	sigRegistered = sigaction(SIGUSR2, &sigHandler, NULL);
	if ( sigRegistered == -1 ) {errorFunc(1);}

	strcpy(PIDchar, argv[1]);

	while(1){
		pause();
	}

	// Return to Caller
	return 0;
}
//**************************************************************
//SIGNAL HANDLER FUNCTION
//**************************************************************

	void advSigHandler(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
			int returnValue;
			char* flyball = "Fly ball to: ";
			char *groundball = "Ground ball to: ";
			if (intSignNum == SIGUSR1) {  // Match cmd line arg
				returnValue = kill( PID , SIGUSR1);  // Send PID signal
				if (returnValue == -1){errorFunc(2);}
				strcat(buffer, flyball);
				strcat(buffer, PIDchar);
				returnValue = write(1, buffer, sizeof(buffer));
				if (returnValue == -1){errorFunc(3);}
			}
			if (intSignNum == SIGUSR2) {  // Match cmd line arg
				returnValue = kill( PID , SIGUSR2);  // Send PID signal
				if (returnValue == -1){errorFunc(2);}
				strcat(buffer, groundball);
				strcat(buffer, PIDchar);
				returnValue = write(1, buffer, sizeof(buffer));
				if (returnValue == -1){errorFunc(3);}
			}
			return;
	}
//*********************************************************************
//taken from sig-pitch.c usage function
void usage(char *strCalledName) {
		fprintf(stdout, "NAME\n");
		fprintf(stdout, "\tplay ball - signal handling functionality that catches\n\tstandard user defined signals\n\n");
		fprintf(stdout, "SYNOPSIS\n");
		fprintf(stdout, "\t%s <PID> <SIGNAL>\n\n", basename(strCalledName));
		fprintf(stdout, "DESCRIPTION\n");
		fprintf(stdout, "\toutput PID of signal specified via first command line argument\n");
		fprintf(stdout, "\t-h: display this help\n");
		exit(0);//EXIT WITH NORMAL RETURN VALUE

}
//********************************************************************
//helped with this by Katie Swafford
void errorFunc(int errNum){
	switch(errNum){
		case 1: fprintf(stderr,"\nError, sigaction failed.\n\n");
			  exit(errNum);
		case 2: fprintf(stderr,"\nkill switch didn't work...no signal sent\n\n");
			  exit(errNum);
		case 3: fprintf(stderr,"\nError, write didn't work.\n\n");
			  exit(errNum);

		}
}
