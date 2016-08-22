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
