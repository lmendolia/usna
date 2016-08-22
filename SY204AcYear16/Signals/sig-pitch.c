/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Signals

	Description: sig-pitch.c - Send PID (1st cmd ln arg) and signal name (2nd cmd arg)
*/

// Required Includes
// Previous Uses
#include <libgen.h>     // General Library: basename(3)
#include <stdio.h>      // Standard I/O Library: fprintf(3)
#include <stdlib.h>     // Standard Library: atoi(3)
#include <string.h>     // String Library: strcmp(3)
// New Uses
#include <signal.h>     // Signal Library: kill(2), SIG*
#include <sys/types.h>  // System Types Library: pid_t

// Global Constants

// Function Prototypes
void usage(char *strCalledName);

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int intReturn = 0;  // Return variable, assume normal exit
	pid_t pidSendSig;   // PID to signal
	// Perform Function
	if ( argc != 3 ) {  // Incorrect number of command line arguments
		usage(argv[0]);
		return 0;
	}
	// Convert (and type cast) 1st cmd ln arg to pid_t
	pidSendSig = (pid_t) atoi(argv[1]);  // Explore (outside of class)

	// SIGTSTP - stop from tty (Ctrl-z), default action: stop process (process can be continued later); recall untouchables
	if ( ! strcmp(argv[2], "SIGTSTP") ) {  // Match cmd line arg
		kill( pidSendSig , SIGTSTP );  // Send PID signal
	}

	// SIGINT - interrupt (Ctrl-c), default action: ???
	if ( ! strcmp(argv[2], "SIGINT") ) {  // Match cmd line arg
		kill( pidSendSig , SIGINT );  // Send PID signal
	}
	// SIGQUIT - quit (Ctrl-\), default action: terminate process and dump core
	if ( ! strcmp(argv[2], "SIGQUIT") ) {  // Match cmd line arg
		kill( pidSendSig , SIGTSTP );  // Send PID signal
	}
	// SIGCHLD - child terminated, default action: ???
	if ( ! strcmp(argv[2], "SIGCHLD") ) {  // Match cmd line arg
		kill( pidSendSig , SIGCHLD );  // Send PID signal
	}
	// SIGKILL - sure kill, action: terminate process
	if ( ! strcmp(argv[2], "SIGKILL") ) {  // Match cmd line arg
		kill( pidSendSig , SIGKILL );  // Send PID signal
	}
	// Note: a process cannot block, ignore, or catch via a signal handler SIGKILL

	// SIGSTOP - sure stop, action: stop process
	if ( ! strcmp(argv[2], "SIGSTOP") ) {  // Match cmd line arg
		kill( pidSendSig , SIGSTOP );  // Send PID signal
	}
	// Note: a process cannot block, ignore, or catch via a signal handler SIGSTOP

	// SIGURG - urgent condition on socket
	if ( ! strcmp(argv[2], "SIGURG") ) {  // Match cmd line arg
		kill( pidSendSig , SIGURG );  // Send PID signal
	}
	// SIGALRM - alarm
	if ( ! strcmp(argv[2], "SIGALRM") ) {  // Match cmd line arg
		kill( pidSendSig , SIGALRM );  // Send PID signal
	}
	// SIGUSR1 - User-defined signal 1, will the kernel generate this?
	if ( ! strcmp(argv[2], "SIGUSR1") ) {  // Match cmd line arg
		kill( pidSendSig , SIGUSR1);  // Send PID signal
	}
	// SIGUSR2 - User-defined signal 2, will the kernel generate this?
	if ( ! strcmp(argv[2], "SIGUSR2") ) {  // Match cmd line arg
		kill( pidSendSig , SIGUSR2);  // Send PID signal
	}
	// Return to Caller
	return intReturn;
}

// Outputs usage information
void usage(char *strCalledName) {
	fprintf(stdout, "NAME\n");
	fprintf(stdout, "\tsig-pitch - Send PID SIGNAL\n\n");
	fprintf(stdout, "SYNOPSIS\n");
	fprintf(stdout, "\t%s <PID> <SIGNAL>\n\n", basename(strCalledName));
	fprintf(stdout, "DESCRIPTION\n");
	fprintf(stdout, "\tSend process with id of PID the signal SIGNAL. SIGNAL is a string\n");
	fprintf(stdout, "\tthat is the defined constant associated with the signal to be sent.\n");
}
