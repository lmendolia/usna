/*
 * Name: LCDR C. W. Hoffmeister
 * Alpha: 00tamu
 * Course: SY204 - Sys Prgm & OS Fund
 * Meeting: Sockets - Internet Domain Pt 2
 *
 * Description: tcp-client.c - A simple TCP client.
 */

/*
 * To use IPv4 compile: gcc -Wall tcp-client-soln.c -o tcp-client-soln
 *
 * To use IPv6 compile: gcc -Wall -DUSE_IPv6 tcp-client-soln.c -o tcp-client-soln
 */

// Student Activity: Howdy Internet
//   1. Setup a IPv4 TCP socket to communicate
//   2. Search for the server to communicate with (getaddrinfo)
//   3. Connect to the server
//   4. Send a simple message to the server via the socket
//   5. Receive a simple message from the server via the socket
//   6. Output the received message to standard output

// Student Activity: Level 2
//   Desc: Without using getaddrinfo service hints, connect to the server using the port number specified as the third command line argument
//   1. Search for the server using getaddrinfo with service search criteria of NULL (already done)
//   2. Setup the port information, 3rd command line arg, for the found server address
//      argv[2] --> atoi --> htons --> sockaddr from getaddrinfo
//   3. Connect to the server, and send/receive messages as in Howdy Internet

// Student Activity: The Core Network
//   1. Use an IPv6 TCP socket to communicate to the server

// Required Includes
// New Uses
#include <sys/types.h>   // System Types Library: [Socket Portability]
#include <sys/socket.h>  // System Socket Library: socket(2), bind(2), connect(2)
#include <arpa/inet.h>   // ARPA Internet Library: IPPROTO_*, htons(3)
#include <netdb.h>       // Network Database Library: struct addrinfo, sturct sockaddr*, freeaddrinfo(3), getaddrinfo(3), gai_strerror(3)
// Previous Uses
#include <libgen.h>  // General Library: basename(3)
#include <stdio.h>   // Standard I/O Library: fprintf(3)
#include <stdlib.h>  // Standard Library: EXIT_*, atoi(3), calloc(3), free(3)
#include <string.h>  // String Library: strdup(3)
#include <unistd.h>  // UNIX Standard Library: close(2), read(2), write(2)

// Global Constants and Variables
#define MAX_STR_LEN 1024
// SY204 Network Services
#define MAX_NUM_PEND_CONNECT 25
#define SY204_TCP_SERVICE 50337
#define SY204_TCP_SERVICE_STR "50337"

// Function Prototypes
void usage(char *strCalledName);

// main Function
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	// General
	int intFailure;  // Return from called routines
	int intDataIO;   // Amount of IO performed
	char bufServRply[MAX_STR_LEN];  // Buffer for message returned from server
	// Server Information
	char *strServerPort;              // String for server port, input to getaddrinfo
	struct addrinfo *aiHints;         // Hints for address search
	struct addrinfo *aiResults;       // Results from address search
	unsigned short ushrtServPortHBO;  // Server port number in host byte order
	unsigned short ushrtServPortNBO;  // Server port number in network byte order
	// Client Information
	int sfdSocket;        // Socket used to communicate with server
	int intSockFamily;    // Socket family
	// Perform function
	// Setup IPv4 or IPv6
	#ifndef USE_IPv6
		intSockFamily = AF_INET;   // IPv4
	#else
		intSockFamily = AF_INET6;  // IPv6
	#endif
	// Correct/incorrect usage
	switch (argc) {
		case 2:  // Use default SY204 port (Howdy Internet)
			strServerPort = SY204_TCP_SERVICE_STR;
			break;
		case 3:  // No port/service searching (Level 2 - 1)
			strServerPort = NULL;
			break;
		default:
			usage(argv[0]);
			return EXIT_SUCCESS;
			break;
	}
	// Setup socket (Howdy Internet - 1)
	sfdSocket = socket(intSockFamily, SOCK_STREAM, IPPROTO_TCP);
	if ( sfdSocket == -1 ) {
		fprintf(stderr, "ERROR: Unable to create socket\n");
		return 1;
	}
	// Search for server (Howdy Internet - 2)
    // Setup search criteria
	aiHints = calloc( 1, sizeof( struct addrinfo ));
    aiHints->ai_family = intSockFamily;  // Generics: IPv4 or IPv6
	aiHints->ai_socktype = SOCK_STREAM;  // Steam (connection oriented) socket
	aiHints->ai_protocol = IPPROTO_TCP;  // Future proof (explicitly specify TCP
	aiHints->ai_flags |= AI_CANONNAME;   // Return canonical name (not needed)
	if ( strServerPort != NULL ) {  // Only if Howdy Internet
		aiHints->ai_flags |= AI_NUMERICSERV;  // Numeric service (port number)
	}
    // Search for server
    intFailure = getaddrinfo(argv[1], strServerPort, aiHints, &aiResults);
	if ( intFailure != 0 ) {
		fprintf(stderr, "ERROR: Unable to get server address information - %s\n", gai_strerror(intFailure));
		freeaddrinfo(aiResults);
		free(aiHints);
		return 2;
	}
	free(aiHints);
	if ( aiResults == NULL ) {
		fprintf(stderr, "ERROR: Unable to find server information\n");
		return 3;
	}
	// Set server port (Level 2 - 2)
	if ( strServerPort == NULL ) {
		ushrtServPortHBO = atoi(argv[2]);
		ushrtServPortNBO = htons(ushrtServPortHBO);
		#ifndef USE_IPv6
			((struct sockaddr_in *)aiResults->ai_addr)->sin_port = ushrtServPortNBO;
		#else
			((struct sockaddr_in6 *)aiResults->ai_addr)->sin6_port = ushrtServPortNBO;
		#endif
	}
	// Connect to socket (Howdy Internet - 3)
	intFailure = connect(sfdSocket, aiResults->ai_addr, aiResults->ai_addrlen);
	if ( intFailure == -1 ) {
		fprintf(stderr, "ERROR: Unable to connect to server\n");
		freeaddrinfo(aiResults);
		return 16;
	}
	freeaddrinfo(aiResults);
	// Perform I/O
	// Send a message to the server (Howdy Internet - 4)
	intDataIO = write(sfdSocket, "A simple message\n", 17);
	if ( intDataIO == -1 ) {
		fprintf(stderr, "ERROR: Unable to write to socket\n");
		return 17;
	}
	// Receive a message from the server (Howdy Internet - 5)
	intDataIO = read(sfdSocket, bufServRply, MAX_STR_LEN);
	if ( intDataIO == -1 ) {
		fprintf(stderr, "ERROR: Unable to read from socket\n");
		return 18;
	}
	// Output message from server to standard output (Howdy Internet - 6)
	intDataIO = write(STDOUT_FILENO, bufServRply, intDataIO);
	// Close socket
	intFailure = close(sfdSocket);
	if ( intFailure == -1 ) {
		fprintf(stderr, "ERROR: Unable to close socket\n");
		return 32;
	}
	// Return to Caller
	return EXIT_SUCCESS;
}

void usage(char *strCalledName) {
	char *strCalledNameCopy;
	char *strBaseName;
	// Perform function
	strCalledNameCopy = strdup(strCalledName);
	strBaseName = basename(strCalledNameCopy);
	fprintf(stdout, "NAME\n");
	fprintf(stdout, "\t%s\t- Connect to hostname specified and send/receive data\n\n", strBaseName);
	fprintf(stdout, "SYNOPSIS\n");
	fprintf(stdout, "\t%s <HOSTNAME> [PORT]\n\n", strBaseName);
	free(strCalledNameCopy);
	return;
}
