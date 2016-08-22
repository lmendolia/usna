/*
 * Name: LCDR C. W. Hoffmeister
 * Alpha: 00tamu
 * Course: SY204 - Sys Prgm & OS Fund
 * Meeting: Sockets - Internet Domain Pt 2
 *
 * Description: tcp-serv.c - A simple IPv4 TCP echo server.
 */

// Required Includes
// New Uses
#include <sys/types.h>   // System Types: [Socket Portability]
#include <sys/socket.h>  // System Socket: socket(2), bind(2), listen(2), accept(2)
#include <arpa/inet.h>   // ARPA Internet: IPPROTO_*, htons()
#include <netdb.h>       // Network Database: struct sockaddr*, freeaddrinfo(3), getaddrinfo(3), gai_strerror(3)
// Previous Uses
#include <libgen.h>  // General Library: basename(3)
#include <signal.h>  // Signal Library: struct sigaction, sigaction(2)
#include <stdio.h>   // Standard I/O Library: fprintf(3)
#include <stdlib.h>  // Standard Library: EXIT_*, calloc(3), exit(3), free(3)
#include <string.h>  // String Library: strndup(3)
#include <unistd.h>  // UNIX Standard Library: close(2), read(2), write(2), getopt(3)

// Global Constants and Variables
#define MAX_NUM_PEND_CONNECT 25
#define MAX_BUF_LEN 4096
#define MAX_STR_LEN 1024
#define SY204_TCP_SERVICE 50337
#define SY204_TCP_SERVICE_STR "50337"
// Sockets
int sfdServer;   // Socket for server
int sfdClient;   // Socket for client

// Function Prototypes
void usage(char *strCalledName);
void sigExit(int intSigNum);

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	// General
	int intFailure;  // Returned failure
	int uniCodeOpt;  // Unicode character for option
	char *strCalledAs;     // String for original argv[0]
	char *strServRplyMsg;  // String for server reply message
	// Signal handling
	struct sigaction sigHandler;
	// General Socket Information
	int intSockFamily = AF_INET;
	int intDataIO;
	char bufData[MAX_BUF_LEN];
	// Server Socket Information
	struct addrinfo *aiHints;     // Hints for address search
	struct addrinfo *aiResults;  // Results from address search
	char strServName[MAX_STR_LEN];  // Server name
	char *strPortNum = SY204_TCP_SERVICE_STR;
	// Client Socket Information
	struct sockaddr saddrClient;
	//struct sockaddr_in saddrClientIPv4;
	//struct sockaddr_in6 saddrClientIPv6;
	socklen_t slenClientSockAddr = sizeof(struct sockaddr_in);  // Length information for IPv4
	// Perform Function
	// Setup signal handling
	sigHandler.sa_handler = &sigExit;
	intFailure = sigaction(SIGINT, &sigHandler, NULL);
	if ( intFailure == -1 ) {
		fprintf(stderr, "CAUTION: Unable to register signal handler.\n");
	}
	// Handle command line arguments
	strCalledAs = strndup(argv[0], MAX_STR_LEN);
	opterr = 0;  // Tell getopt not to report errors
	while ( (uniCodeOpt=getopt(argc, argv, ":46p:")) != -1 ) {
		switch (uniCodeOpt) {
			case '4':
				intSockFamily = AF_INET;
				slenClientSockAddr = sizeof(struct sockaddr_in);
				break;
			case '6':
				intSockFamily = AF_INET6;
				slenClientSockAddr = sizeof(struct sockaddr_in6);
				break;
			case 'p':
				strPortNum = strdup(optarg);
				break;
			case ':':  // Missing required argument to option
				usage(strCalledAs);
				return 2;
				break;
			case '?':  // Unknown option
			default:   // Unhandled option
				usage(strCalledAs);
				return 1;
				break;
		}
	}
	if ( argv[optind] != NULL ) {
		strServRplyMsg = argv[optind];
	} else {
		strServRplyMsg = "-SY204: h4c|< th3 Planet\n";
	}

	// Setup search information: server (self)
	aiHints = calloc( 1, sizeof( struct addrinfo ));
	aiHints->ai_flags = AI_PASSIVE;
	aiHints->ai_family = intSockFamily;
	aiHints->ai_socktype = SOCK_STREAM;
	aiHints->ai_protocol = IPPROTO_TCP;
	intFailure = gethostname(strServName, MAX_STR_LEN);
	if ( intFailure == -1 ) {
		fprintf(stderr, "ERROR: Unable to get own hostname\n");
		return 3;
	}
	// Search for server (self) information
	intFailure = getaddrinfo(NULL, strPortNum, aiHints, &aiResults);
	if ( intFailure != 0 ) {
		fprintf(stderr, "ERROR: Unable to get own address information - %s\n", gai_strerror(intFailure));
		free(aiHints);
		freeaddrinfo(aiResults);
		return 16;
	}
	free(aiHints);
	// Setup socket
	sfdServer = socket(aiResults->ai_family, aiResults->ai_socktype, aiResults->ai_protocol);
	if ( sfdServer == -1 ) {
		fprintf(stderr, "ERROR: Unable to create socket\n");
		freeaddrinfo(aiResults);
		return 17;
	}
	// Bind to socket
	intFailure = bind(sfdServer, aiResults->ai_addr, aiResults->ai_addrlen);
	if ( intFailure == -1 ) {
		fprintf(stderr, "ERROR: Unable to bind to socket\n");
		freeaddrinfo(aiResults);
		return 18;
	}
	// Listen on socket
	intFailure = listen(sfdServer, MAX_NUM_PEND_CONNECT);
	if ( intFailure == -1 ) {
		fprintf(stderr, "ERROR: Unable to listen on socket\n");
		freeaddrinfo(aiResults);
		return 19;
	}
	freeaddrinfo(aiResults);
	while (1) {  // Handle incoming connections
		fprintf(stdout, "listening...\n");
		// Accept connection
		sfdClient = accept(sfdServer, &saddrClient, &slenClientSockAddr);
		if ( sfdClient == -1 ) {
			fprintf(stderr, "ERROR: Unable to accept connection\n");
			return 32;
		}
		fprintf(stdout, "connected...\n");
		// Socket I/O
		intDataIO = read(sfdClient, bufData, MAX_BUF_LEN);
		if ( intDataIO == -1 ) {
			fprintf(stderr, "ERROR: Unable to read from socket\n");
			return 33;
		}
		write(STDOUT_FILENO, bufData, intDataIO);
		intDataIO = write(sfdClient, bufData, intDataIO);
		if ( intDataIO == -1 ) {
			fprintf(stderr, "ERROR: Unable to write to socket\n");
			return 34;
		}
		intDataIO = write(sfdClient, strServRplyMsg, strlen(strServRplyMsg));
		// Close client socket
		intFailure = close(sfdClient);
		if ( intFailure == -1 ) {
			fprintf(stderr, "ERROR: Unable to close server socket\n");
			return 35;
		}
		fprintf(stdout, "closed...\n");
	}
	// Close server socket
	intFailure = close(sfdServer);
	if ( intFailure == -1 ) {
		fprintf(stderr, "ERROR: Unable to close server socket\n");
		return 20;
	}
	// Return to Caller
	return EXIT_SUCCESS;
}

void usage(char *strCalledName) {
	char *strCalledNameCopy;
	char *strBaseName;
	// Perform Function
	strCalledNameCopy = strdup(strCalledName);
	strBaseName = basename(strCalledNameCopy);
	fprintf(stdout, "NAME\n");
	fprintf(stdout, "\t%s\t- Listen for TCP connections, echo with message received and MESSAGE.\n\n", strBaseName);
	fprintf(stdout, "SYNOPSIS\n");
	fprintf(stdout, "\t%s [-46] [-pPORT] <MESSAGE>\n\n", strBaseName);
	fprintf(stdout, "DESCRIPTION\n");
	fprintf(stdout, "\t???\n\n");
	fprintf(stdout, "\t-4\tUse IPv4.\n\n");
	fprintf(stdout, "\t-6\tUse IPv6.\n\n");
	fprintf(stdout, "\t-pPORT\tUse port number PORT.\n");
	free(strCalledNameCopy);
	return;
}

void sigExit(int intSigNum) {
	// Perform Function
	shutdown(sfdClient, SHUT_RDWR);
	close(sfdClient);
	shutdown(sfdServer, SHUT_RDWR);
	close(sfdServer);
	exit(EXIT_SUCCESS);
}
