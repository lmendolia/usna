/*
 * Name: MIDN Mendolia
 * Alpha: 184242
 * Course: SY204 - Sys Prgm & OS Fund
 * Assignment: Lab 0x08: netKitty
 *
 * Description: <A general high-level description about what the program is designed to accomplish>
 *
 * Resources: MGSP, Katie Swafford, man page (3) getaddrinfo examples
 */

// Required Includes
// New Uses
#include <netinet/in.h>  // Network Internet Library: IPPROTO_*
// Previous Uses
#include <stdlib.h>  // Standard Library: EXIT_*, exit(3)
#include <unistd.h>  // UNIX Standard Library: getopt(3)
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h> 	// ARPA Internet: IPPROTO_*, htons(3), inetntop(3)
#include <netdb.h>     	// Network Database: struct sockaddr*, struct addrinfo, getaddrinfo(3), gai_strerror(3)
#include <string.h>
#include <libgen.h>
#include <stdio.h>
//note to lexi: htons with the port number specified at argv[optind+1] or something like that
// Previous Uses

// Global Constants
// Network Communications
#define UNSET_MODE      000
#define CLIENT_MODE     001  // Default
#define SERVER_MODE     002
#define CLNT_SERV_MASK  003
#define IPv4_MODE       004  // Default
#define IPv6_MODE       010
#define NET_MASK        014
#define TCP_MODE        020  // Default
#define UDP_MODE        040
#define TRANS_MASK      060
#define BUF_SIZE			 1024
#define MAX_NUM_PEND_CONNECT 25
int sfdServer;   // Socket for server
int sfdClient;   // Socket for client
char *bufData;

char flgMode = CLIENT_MODE | IPv4_MODE | TCP_MODE;
// Function Prototypes
void errorFunc(int errNum);
void usage(char *strCalledName);
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
//int calling(int argc, char * argv[]);
int listening(int argc, char * argv[]);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[]) {
	// Declare Required Variables
	int option;  // Option character (Unicode support)
	// Perform Function
	// Read in and prep option setup
	opterr = 0;
	while ( ( option = getopt(argc, argv, ":lh:")) != -1 ) {
		switch (option) {
			case 'h':  // Help Option
				usage(argv[0]);
				break;
			case 'l':
				listening(argc, argv);
			default:
				usage(argv[0]);
				break;
		}
	}

	int sigRegistered;
	struct sigaction sigHandler;
	sigHandler.sa_sigaction = &shutdownFunc;
	sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
	sigRegistered = sigaction(SIGPIPE, &sigHandler, NULL);
	if ( sigRegistered == -1 ) {
		errorFunc(2);
	}
//********************************************************
//CALLING FUNCTION: Part 1 ...Client Side
//********************************************************
//int calling(int argc, char * argv[]){
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
    intFailure = getaddrinfo(argv[1], argv[2], aiHints, &aiResults);
		if ( intFailure != 0 ) {
			fprintf(stderr, "ERROR: Unable to get server address information - %s\n", gai_strerror(intFailure));
			freeaddrinfo(aiResults);
			free(aiHints);
			return 4;
		}
	free(aiHints);
	if ( aiResults == NULL ) {
		errorFunc(4);
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
		errorFunc(5);
		freeaddrinfo(aiResults);
	}
	freeaddrinfo(aiResults);
	char strFullCmdLn[1024];  // String for full command line
	while (1){
	if (fgets(strFullCmdLn, BUF_SIZE, stdin) == NULL ) {  // Read command line
		//fprintf(stdout, "\n");
		errorFunc(15);
	}
	// Perform I/O
	// Send a message to the server (Howdy Internet - 4)
	intDataIO = write(sfdSocket, strFullCmdLn, strlen(strFullCmdLn));
	if ( intDataIO == -1 ) {
		errorFunc(6);
	}
	// Receive a message from the server (Howdy Internet - 5)
	intDataIO = read(sfdSocket, bufServRply, BUF_SIZE);
	if ( intDataIO == -1 ) {
		errorFunc(7);
	}

	// Output message from server to standard output (Howdy Internet - 6)
	intDataIO = write(STDOUT_FILENO, bufServRply, intDataIO);
}
	// Close socket
	intFailure = close(sfdSocket);
	if ( intFailure == -1 ) {
		errorFunc(8);
	}
	// Return to Caller
	return 0;
}
//********************************************************
//LISTENING FUNCTION: Part 2... Server Side
//********************************************************
int listening(int argc, char * argv[]){
	int intFailure;  // Returned failure

	//char *strServRplyMsg;  // String for server reply message
	// Signal handling
	// General Socket Information
	int intSockFamily = AF_INET;
	int intDataIO;

	// Server Socket Information
	struct addrinfo *aiHints;     // Hints for address search
	struct addrinfo *aiResults;  // Results from address search
	char strServName[BUF_SIZE];  // Server name
	char *strPortNum = argv[optind];
	// Client Socket Information
	struct sockaddr saddrClient;
	//struct sockaddr_in saddrClientIPv4;
	socklen_t slenClientSockAddr = sizeof(struct sockaddr_in);  // Length information for IPv4
	// Perform Function
	// Handle command line arguments
	strndup(argv[0], BUF_SIZE);
	slenClientSockAddr = sizeof(struct sockaddr_in);

	// Setup search information: server (self)
	aiHints = calloc( 1, sizeof( struct addrinfo ));
	aiHints->ai_flags = AI_PASSIVE;
	aiHints->ai_family = intSockFamily;
	aiHints->ai_socktype = SOCK_STREAM;
	aiHints->ai_protocol = IPPROTO_TCP;
	intFailure = gethostname(strServName, BUF_SIZE);
	if ( intFailure == -1 ) {
		errorFunc(9);
		fprintf(stderr, "ERROR: Unable to get own hostname\n");
		return 3;
	}
	// Search for server (self) information
	intFailure = getaddrinfo(NULL, strPortNum, aiHints, &aiResults);
	if ( intFailure != 0 ) {
		errorFunc(10);
		fprintf(stderr, "ERROR: Unable to get own address information - %s\n", gai_strerror(intFailure));
		free(aiHints);
		freeaddrinfo(aiResults);
	}
	free(aiHints);
	// Setup socket
	sfdServer = socket(aiResults->ai_family, aiResults->ai_socktype, aiResults->ai_protocol);
	if ( sfdServer == -1 ) {
		errorFunc(3);
		freeaddrinfo(aiResults);
	}
	// Bind to socket
	intFailure = bind(sfdServer, aiResults->ai_addr, aiResults->ai_addrlen);
	if ( intFailure == -1 ) {
		errorFunc(11);
		freeaddrinfo(aiResults);
	}
	// Listen on socket
	intFailure = listen(sfdServer, MAX_NUM_PEND_CONNECT);
	if ( intFailure == -1 ) {
		errorFunc(12);
		freeaddrinfo(aiResults);
	}
	freeaddrinfo(aiResults);
	fprintf(stdout, "listening...\n");

		while (1){
			bufData = calloc(1024, 1);
	 // Handle incoming connections
		// Accept connection
		sfdClient = accept(sfdServer, &saddrClient, &slenClientSockAddr);
		if ( sfdClient == -1 ) {
			errorFunc(13);
		}
		fprintf(stdout, "connected...\n");
		// Socket I/O

		intDataIO = read(sfdClient, bufData, BUF_SIZE);
		if ( intDataIO == -1 ) {
			errorFunc(7);
		}
		write(STDOUT_FILENO, bufData, intDataIO);
		char strFullCmdLn[1024];  // String for full command line

			if (fgets(strFullCmdLn, BUF_SIZE, stdin) == NULL ) {  // Read command line
				fprintf(stdout, "\n");
				errorFunc(15);
			}
			/*
			intDataIO = write(sfdClient, bufData, intDataIO);
			if ( intDataIO == -1 ) {
				errorFunc(6);
			} */
			free(bufData);

			intDataIO = write(sfdClient, strFullCmdLn, strlen(strFullCmdLn));
			// Close client socket
			/*
			intFailure = close(sfdClient);
			if ( intFailure == -1 ) {
				errorFunc(14);
			}
			fprintf(stdout, "closed...\n");
			*/
}
		// Close server socket
		intFailure = close(sfdServer);
		if ( intFailure == -1 ) {
			errorFunc(14);
	}
	// Return to Caller
	return 0;
}
//********************************************************
//SHUTDOWN FUNCTION
//********************************************************
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
	int intFailure = -1;
	intFailure = shutdown(sfdClient, SHUT_RDWR);
	close(sfdClient);
	if ( intFailure){errorFunc(1);}
	intFailure = shutdown(sfdServer, SHUT_RDWR);
	close(sfdServer);
	if ( intFailure){errorFunc(1);}
	exit(0);
}
//********************************************************
//USAGE FUNCTION
//********************************************************
void usage(char *strCalledName) {
		char *strCalledNameCopy;
		char *strBaseName;
		// Perform function
		strCalledNameCopy = strdup(strCalledName);
		strBaseName = basename(strCalledNameCopy);
		fprintf(stdout, "NAME\n");
		fprintf(stdout, "\tnetKitty- a simple version of netcat\n\n");
		fprintf(stdout, "SYNOPSIS\n");
		fprintf(stdout, "\t%s CALLING MODE: <hostname> <port> <msg>\n\n", strBaseName);
		fprintf(stdout, "\t%s LISTENING MODE: -l <port>\n\n", strBaseName);
		fprintf(stdout, "DESCRIPTION\n");
		fprintf(stdout, "\t-l: trigger listening mode\n");
		fprintf(stdout, "\t-h: display this help\n");
		free(strCalledNameCopy);
		exit(0);//EXIT WITH NORMAL RETURN VALUE
}
//*********************************************************
//ERROR FUNCTION
//*********************************************************
void errorFunc(int errNum){
	switch(errNum){
		case 1: fprintf(stderr,"\nDidn't shut down socket\n\n");
			  exit(errNum);
		case 2: fprintf(stderr,"\nDidn't register signal.\n\n");
			  exit(errNum);
		case 3: fprintf(stderr,"Unable to create socket\n");
				exit(errNum);
		case 4: fprintf(stderr,"\nUnable to find server information\n\n");
				exit(errNum);
		case 5: fprintf(stderr,"\nUnable to connect to server.\n\n");
				exit(errNum);
		case 6: fprintf(stderr,"\nUnable to write to socket.\n\n");
				exit(errNum);
		case 7: fprintf(stderr,"\nUnable to read from socket.\n\n");
				exit(errNum);
		case 8: fprintf(stderr,"\nUnable to close socket.\n\n");
				exit(errNum);
		case 9: fprintf(stderr,"\nUnable to get own hostname.\n\n");
				exit(errNum);
		case 10: fprintf(stderr,"\nUnable to get own address information.\n\n");
				exit(errNum);
		case 11: fprintf(stderr,"\nUnable to bind socket.\n\n");
				exit(errNum);
		case 12: fprintf(stderr,"\nUnable to listen on socket.\n\n");
				exit(errNum);
	  case 13: fprintf(stderr,"\nUnable to accept connection.\n\n");
			exit(errNum);
		case 14: fprintf(stderr,"\nUnable to close server socket.\n\n");
				exit(errNum);
		case 15: fprintf(stderr,"\nError with fgets.\n\n");
				exit(errNum);
		}
}
