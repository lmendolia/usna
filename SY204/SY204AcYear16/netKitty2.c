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
#define BUF_SIZE				500
char flgMode = CLIENT_MODE | IPv4_MODE | TCP_MODE;
int sfdSocket = -1; //socket fd global due to signal registering
// Function Prototypes
void errorFunc(int errNum);
void usage(char *strCalledName);
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext);
int calling(int argc, char * argv[]);
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
				calling(argc, argv);
				break;
		}
	}
	if (argc  <= 1){
		usage(argv[0]);
		return 0;
	}
	int sigRegistered;
	struct sigaction sigHandler;
	sigHandler.sa_sigaction = &shutdownFunc;
	sigHandler.sa_flags = SA_RESTART | SA_SIGINFO;
	sigRegistered = sigaction(SIGPIPE, &sigHandler, NULL);
	if ( sigRegistered == -1 ) {
		errorFunc(2);
	}
return EXIT_SUCCESS;
}
//********************************************************
//CALLING FUNCTION: Part 1 ...Client Side
//********************************************************
int calling(int argc, char * argv[]){
	struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

   if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   /* Obtain address(es) matching host/port */

   memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

   s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

   /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully connect(2).
       If socket(2) (or connect(2)) fails, we (close the socket
       and) try the next address. */

   for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

       if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

       close(sfd);
    }

   if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

   freeaddrinfo(result);           /* No longer needed */

   /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

   for (j = 3; j < argc; j++) {
        len = strlen(argv[j]) + 1;
                /* +1 for terminating null byte */

       if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

       if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

       nread = read(sfd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

       printf("Received %ld bytes: %s\n", (long) nread, buf);
    }

   exit(EXIT_SUCCESS);
}

//********************************************************
//LISTENING FUNCTION: Part 2... Server Side
//********************************************************
int listening(int argc, char * argv[]){
	printf("entering listening mode\n");
	struct addrinfo *hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	char buf[BUF_SIZE];
	if (argc != 3) {
		usage(argv[0]);
	}
	hints = calloc( 1, sizeof( struct addrinfo ));
	hints->ai_flags = AI_PASSIVE;
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_protocol = IPPROTO_TCP;


	s = getaddrinfo(NULL, argv[1], hints, &result);
	if (s != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
			errorFunc(3);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		 sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		 if (sfd == -1) {continue;}
		 if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0){break;}                  /* Success */
		 close(sfd);
	}
	if (rp == NULL) {errorFunc(7);}               /* No address succeeded */
	freeaddrinfo(result);           /* No longer needed */
	for (;;) {
		 peer_addr_len = sizeof(struct sockaddr_storage);
		 nread = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
		 if (nread == -1) {continue;}             /* Ignore failed request */
		 char host[NI_MAXHOST], service[NI_MAXSERV];
		 s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
		 if (s == 0){
			 printf("Received %ld bytes from %s:%s\n", (long) nread, host, service);
		 }
			else{
				fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
			}
		 if (sendto(sfd, buf, nread, 0,(struct sockaddr *) &peer_addr, peer_addr_len) != nread){
			 errorFunc(8);
		 }
	 }
}
//********************************************************
//SHUTDOWN FUNCTION
//********************************************************
// Close socket
void shutdownFunc(int intSignNum, siginfo_t *sigInfo, void *sysDependContext){
	int intFailure = -1;
	intFailure = shutdown(sfdSocket, SHUT_RD);
	if ( intFailure){errorFunc(1);}
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
		case 3: fprintf(stderr,"\n");
				exit(errNum);
		case 4: fprintf(stderr,"\npartial/failed write\n\n");
				exit(errNum);
		case 5: fprintf(stderr,"\nread error\n\n");
				exit(errNum);
		case 6: fprintf(stderr,"\ncould not connect\n\n");
				exit(errNum);
		case 7: fprintf(stderr,"\ncould not bind\n\n");
				exit(errNum);
		case 8: fprintf(stderr,"\nError sending response\n\n");
				exit(errNum);
		}
}
