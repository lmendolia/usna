#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int main(int argc, char *argv[]){
    struct addrinfo *hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    hints = calloc( 1, sizeof( struct addrinfo ));
    hints->ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints->ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints->ai_flags |= AI_CANONNAME;
    hints->ai_protocol = IPPROTO_TCP;          /* Any protocol */
    char *strServerPort;
    unsigned short returnVal = atoi(argv[optind + 1]);
    strServerPort = htons(returnVal);
    s = getaddrinfo(argv[1], strServerPort, hints, &result);
    if (s != 0) {
      fprintf(stderr, "ERROR: Unable to get server address information - %s\n", gai_strerror(s));
  		freeaddrinfo(result);
  		free(hints);
  		return 2;
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
     sfd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
     if (sfd == -1){perror("Unable to connect to socket\n");exit(1);}
     if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1){break;}
     close(sfd);
   }

   if (rp == NULL) {               /* No address succeeded */
      fprintf(stderr, "Could not connect\n");
      exit(EXIT_FAILURE);
   }

   freeaddrinfo(result);           /* No longer needed */

   for (j = 3; j < argc; j++) {
       len = strlen(argv[j]) + 1;
               /* +1 for terminating null byte */

       if (len + 1 > BUF_SIZE) {
           fprintf(stderr, "Ignoring long message in argument %d\n", j);
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
