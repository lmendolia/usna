/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Sockets - Internet Domain Pt 1
	
	Description: name2addr.c - Hostname entered as command line argument is translated into an IP address.
*/

// Exploration Activity: Names to Addresses
//   1. Participate in instructor lead discussion
// Student Activity: I know IPv6
//   1. Setup the hints to search for IPv6 addresses instead of IPv4
// Student Activity: I know IPv4 and IPv6
//   1. Setup the hints to search for IPv6 and IPv4 addresses (see getaddrinfo(3))
// Student Activity: Many hostnames on command line
//   1. Add functionality to get address information for all of the hosts specified by name as command line arguments

// Required Includes
// New Uses
#include <sys/types.h> 	// System Types Library: [Socket Portability]
//#include <sys/socket.h>	// System Socket Library: socket(2), bind(2), listen(2), accept(2)
#include <arpa/inet.h> 	// ARPA Internet: IPPROTO_*, htons(3), inetntop(3)
#include <netdb.h>     	// Network Database: struct sockaddr*, struct addrinfo, getaddrinfo(3), gai_strerror(3)
// Previous Uses
#include <libgen.h>	// General Library: basename(3)
#include <stdio.h> 	// Standard I/O Library: std*, fprintf(3)
#include <stdlib.h>	// Standard Library: EXIT_*, calloc(3), free(3)
#include <string.h>	// String Library: strdup(3)

// Global Constants
#define MAX_STR_LEN	128

// Function Prototypes
void usage(char *strCalledName);

// main Function
int main(int argc, char *argv[]) {
	// Declare Required Variables
	// Error checking
	int intFailure;	// Called routine returned failure
	// getaddrinfo(3) related
	struct addrinfo *aiHints;  	// Search hints to getaddrinfo()
	struct addrinfo *aiResults;	// Search results from getaddrinfo()
	struct addrinfo *aiResult; 	// Traverse through search results (Linked List [SY301 - Data Structures])
	// IP [socket] address related
	struct in_addr addrIPv4;      	// IPv4 address
	//struct in6_addr addrIPv6;     	// IPv6 address
	//struct sockaddr_in saddrIPv4;	// IPv4 socket address (IP address and port)
	//struct sockaddr_in6 saddrIPv6;	// IPv6 socket address (IP address and port)
	// inet_ntop(3) related
	char strHostAddr[MAX_STR_LEN];	// String for presentation formatted address
	// Perform function
	// Correct/incorrect usage
	if ( argc < 2 ) {
		usage(argv[0]);
		return EXIT_SUCCESS;
	}
	
	// Setup hints to getaddrinfo
	//memset(&aiHints, 0, sizeof(struct addrinfo));  // MUST clear out hints before setting
	aiHints = calloc(1 , sizeof( struct addrinfo ));
	//aiHints->ai_flags   |= AI_PASSIVE;    // If we are searching for address information to be used as a local server
	aiHints->ai_flags   |= AI_CANONNAME;  // Set flag for including canonical name in search results
	aiHints->ai_family   = AF_INET;       // IPv4
	aiHints->ai_socktype = SOCK_STREAM;   // In SY204 will only be using Stream Sockets
	aiHints->ai_protocol = IPPROTO_TCP;   // In SY204 will only be using TCP (Note: IPPROTO_UDP)
	// Note: LPI discusses using 0 vice IPPROTO_TCP/IPPROTO_UDP, 0 means use the default protocol for the given address family (AF_INET, AF_INET6) and socket type (SOCK_STREAM, SOCK_DGRAM). There may be more than one protocol for a given address family and socket type.
	
	// Search for host address information based on hostname (argv[1])
	intFailure = getaddrinfo(argv[1], NULL, aiHints, &aiResults);
	if ( intFailure != 0 ) {
		fprintf(stderr, "ERROR: Unable to get address information - %s\n", gai_strerror(intFailure));
		free(aiHints);  // Memory management
		return EXIT_FAILURE;
	}
	free(aiHints);  // Memory management
	
	// Traverse search results
	aiResult = aiResults;
	fprintf(stdout, "Results for: %s\n", argv[1]);
	while (aiResult != NULL ) {  // While not pointing to NULL
		if ( aiResult->ai_canonname != NULL ) {
			fprintf(stdout, "Canonical Name: %s\n", aiResult->ai_canonname);
		}
		switch ( aiResult->ai_family ) {
			case AF_INET:
				// Due to generics, C socket programming requires pointer and casting kung fu
				// Something of type struct sockaddr can either be of type struct sockaddr_in (IPv4) or struct sockaddr_in6 (IPv6)
				//           Type Cast: To This     From This
				//          (-------------------)-----------------
				addrIPv4 = ((struct sockaddr_in*)aiResult->ai_addr)->sin_addr;
				//         (~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~)~~~~~~~~~~
				//          Pointer to struct sockaddr_in (IPv4)     Specific field pointing to
				if ( inet_ntop(AF_INET, &addrIPv4, strHostAddr, MAX_STR_LEN) != NULL ) {
					fprintf(stdout, "IP Address: %s\n", strHostAddr);
				}
				break;
			case AF_INET6:
				break;
			default:
				break;
		}
		aiResult = aiResult->ai_next;  // Point to next element in list
	}
	
	// Memory management
	freeaddrinfo(aiResults);
	// Return to caller
	return EXIT_SUCCESS;
}

void usage(char *strCalledName) {
	char *strCalledNameCopy;
	char *strBaseName;
	// Perform function
	strCalledNameCopy = strdup(strCalledName);
	strBaseName = basename(strCalledNameCopy);
	fprintf(stdout, "NAME\n");
	fprintf(stdout, "\t%s\t- Output the IP address of the hostname specified\n\n", strBaseName);
	fprintf(stdout, "SYNOPSIS\n");
	fprintf(stdout, "\t%s <hostname>\n\n", strBaseName);
	free(strCalledNameCopy);
	return ;
}
