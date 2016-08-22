/*
	Name: LCDR C. W. Hoffmeister
	Alpha: 00tamu
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Sockets - Internet Domain Pt 1
	
	Description: not2scan.c - An example of endianness using TCP ports.
*/

// Exploration Activity: Endianness (Part 1)
//   1. Participate in instructor lead discussion

// Exploration Activity: Endianness (Part 2)
//   1. Add code to covert the network byte order port to host byte order

// Student Activity: IPv6 Uses Ports Too
//   1. Add port functionality to the IPv6 search results

// Required Includes
// New Uses
#include <sys/types.h>   // System Types: [Socket Portability]
#include <sys/socket.h>  // System Socket: socket(2), connect(2)
#include <arpa/inet.h>   // ARPA Internet: IPPROTO_*, ntohs(3), htons(3)
#include <netdb.h>       // Network Database: struct sockaddr*, struct addrinfo, getaddrinfo(3), gai_strerror(3)
// Previous Uses
#include <libgen.h>  // General: basename(3)
#include <stdio.h>   // Standard I/O
#include <stdlib.h>  // Standard: EXIT_*, calloc(3), free(3)
#include <string.h>  // String: strdup(3)
#include <unistd.h>  // UNIX Standard Library: close(2)

// Global Constants
// Buffer length
#define MAX_STR_LEN 	128
// Networking related
#define MAX_PORT_NUM	65355

// Function Prototypes
void usage(char *strCalledName);

// main Function
int main(int argc, char *argv[]) {
	// Declare Required Variables
	// Error checking
	int intFailure;  // Called routine returned failure
	// Flags
	char flgFirst = 0;
	// Loop control
	int i;
	// getaddrinfo(3) related
	char strServiceNum[MAX_STR_LEN];  // String for service number search criteria
	struct addrinfo *aiHints;          // Search hints to getaddrinfo()
	struct addrinfo *aiResults;       // Search results from getaddrinfo()
	struct addrinfo *aiResult;        // Traverse through search results (Linked List [SY301 - Data Structures])
	// IP [socket] address related
	unsigned short ushrtHostPort = 0; // TCP/UDP port number (host byte order)
	unsigned short ushrtNetPort = 0;  // TCP/UDP port number (network byte order)
	struct in_addr addrIPv4;          // IPv4 address
	//struct in6_addr addrIPv6;         // IPv6 address
	//struct sockaddr_in saddrIPv4;     // IPv4 socket address (IP address and port)
	//struct sockaddr_in6 saddrIPv6;    // IPv6 socket address (IP address and port)
	char strHostAddr[MAX_STR_LEN];  // String for presentation formatted address
	// Socket related
	int sfdPortScanner;  // Socket for port scanning
	// Perform function
	// Correct/incorrect usage
	if ( argc < 2 ) {
		usage(argv[0]);
		return 0;
	}
	
	i = 1;
	while ( i <= MAX_PORT_NUM ) {
		// Setup hints to getaddrinfo
		aiHints = calloc(1, sizeof( struct addrinfo ));
		//aiHints.ai_flags   |= AI_PASSIVE;      // If we are searching for address information to be used as a local server
		aiHints->ai_flags   |= AI_CANONNAME;    // Set flag for including canonical name in search results
		aiHints->ai_flags   |= AI_NUMERICSERV;  // Set flag for numeric service string search criteria
		aiHints->ai_family   = AF_INET;         // IPv4
		aiHints->ai_socktype = SOCK_STREAM;     // In SY204 will only be using Stream Sockets
		aiHints->ai_protocol = IPPROTO_TCP;     // In SY204 will only be using TCP (Note: IPPROTO_UDP)
		snprintf(strServiceNum, MAX_STR_LEN, "%d", i);
		// Search for host address information based on hostname (argv[1])
		intFailure = getaddrinfo(argv[1], strServiceNum, aiHints, &aiResults);
		if ( intFailure != 0 ) {
			fprintf(stderr, "ERROR: Unable to get address information - %s\n", gai_strerror(intFailure));
			free(aiHints);  // Memory management
			return EXIT_FAILURE;
		}
		free(aiHints);  // Memory management
		
		// Traverse search results
		aiResult = aiResults;
		if ( ! flgFirst ) {
			fprintf(stdout, "Results for: %s\n", argv[1]);
			if ( aiResult->ai_canonname != NULL ) {
				fprintf(stdout, "Canonical Name: %s\n", aiResult->ai_canonname);
				switch ( aiResult->ai_family ) {
					case AF_INET:
						addrIPv4 = ((struct sockaddr_in*)aiResult->ai_addr)->sin_addr;
						if ( inet_ntop(AF_INET, &addrIPv4, strHostAddr, MAX_STR_LEN) != NULL ) {
							fprintf(stdout, "IP Address: %s\n", strHostAddr);
						}
						break;
					case AF_INET6:
						break;
					default:
						break;
				}
			}
			flgFirst = 1;
		}
		while (aiResult != NULL ) {  // While not pointing to NULL
			sfdPortScanner = socket(aiResult->ai_family, aiResult->ai_socktype, aiResult->ai_protocol);
			if ( sfdPortScanner == -1 ) {
				fprintf(stderr, "ERROR: Unable to create socket\n");
				aiResult = aiResult->ai_next;  // Point to next element in list
				continue;
			}
			intFailure = connect(sfdPortScanner, aiResult->ai_addr, aiResult->ai_addrlen);
			if ( intFailure == -1 ) {
				close(sfdPortScanner);         // Manage descriptor usage
				aiResult = aiResult->ai_next;  // Point to next element in list
				continue;
			}
			close(sfdPortScanner);  // Manage descriptor usage
			switch ( aiResult->ai_family ) {
				case AF_INET:
					ushrtNetPort = ((struct sockaddr_in*)aiResult->ai_addr)->sin_port;
					break;
				case AF_INET6:
					break;
				default:
					break;
			}
			// Student Activity: Endianness (Part 2)
			// Convert from network byte order to host byte order
			
			// Output open ports
			// Hint: If you have nmap in your VM, compare the nmap port scan output to this
			fprintf(stdout, "(Net)  Port: %hu\t", ushrtNetPort);
			fprintf(stdout, "(Host) Port: %hu\n", ushrtHostPort);
			aiResult = aiResult->ai_next;  // Point to next element in list
		}
		// Memory management
		freeaddrinfo(aiResults);
		i++;
	}
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
	fprintf(stdout, "\t%s\t- Crude TCP port scanner\n\n", strBaseName);
	fprintf(stdout, "SYNOPSIS\n");
	fprintf(stdout, "\t%s <hostname>\n\n", strBaseName);
	fprintf(stdout, "DESCRIPTION\n");
	fprintf(stdout, "\t???\n");
	free(strCalledNameCopy);
	return ;
}
