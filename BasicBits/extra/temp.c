#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <signal.h>
#include <wait.h>
#include <netinet/in.h>  // Network Internet Library: IPPROTO_*
#include <sys/socket.h>
#include <arpa/inet.h> 	// ARPA Internet: IPPROTO_*, htons(3), inetntop(3)
#include <netdb.h>     	// Network Database: struct sockaddr*, struct addrinfo, getaddrinfo(3), gai_strerror(3)

int main(int argc, char* argv){

  int pid = argv[1];

  kill(pid, SIGUSR1);

}
