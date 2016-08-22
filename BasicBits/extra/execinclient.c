int executeFunc(int argc, char *argv[]){
  int returnVal;
  int intFailure;
	if (argc < 4){errorFunc(35);}

  intFailure = pipe(pipe1);
  if (intFailure==-1){errorFunc(31);}
  intFailure = pipe(pipe2);
  if (intFailure ==-1){errorFunc(31);}
  fcntl(pipe2[0], F_SETFL, O_NONBLOCK);

  pid_t pipeFork = fork();
  if ( pipeFork == -1 ) {errorFunc(34);}
  if (pipeFork == 0){
    intFailure = close(STDIN_FILENO);
    if (intFailure == -1){errorFunc(32);}

    intFailure = dup2(pipe1[0], STDIN_FILENO);
    if (intFailure == -1){errorFunc(33);}
    intFailure = close(STDOUT_FILENO);
    if (intFailure == -1){errorFunc(32);}
    intFailure = dup2(pipe2[1], STDOUT_FILENO);
    if (intFailure  == -1){errorFunc(33);}
    intFailure = close(pipe2[0]);
    if (intFailure==-1){errorFunc(32);}

    intFailure = close(pipe1[1]);
    if (intFailure==-1){errorFunc(32);}
    intFailure = execvp(argv[2], &argv[2]);
    if (intFailure == -1){errorFunc(36);}

  }
  else{ //parent
    int intFailure;  // Returned failure
    int intSockFamily = AF_INET;
    int intDataIO;
    struct addrinfo *aiHints;     // Hints for address search
    struct addrinfo *aiResults;  // Results from address search
    char strServName[BUF_SIZE];  // Server name
    char *strPortNum = argv[2];
    struct sockaddr saddrClient;
    socklen_t slenClientSockAddr = sizeof(struct sockaddr_in);  // Length information for IPv4
    strndup(argv[0], BUF_SIZE);
    slenClientSockAddr = sizeof(struct sockaddr_in);
    aiHints = calloc( 1, sizeof( struct addrinfo ));
    aiHints->ai_flags = AI_PASSIVE;
    aiHints->ai_family = intSockFamily;
    aiHints->ai_socktype = SOCK_STREAM;
    aiHints->ai_protocol = IPPROTO_TCP;
    intFailure = gethostname(strServName, BUF_SIZE);
    if ( intFailure == -1 ) {
      errorFunc(24);
      fprintf(stderr, "ERROR: Unable to get own hostname\n");
      return 3;
    }
    intFailure = getaddrinfo(NULL, strPortNum, aiHints, &aiResults);
    if ( intFailure != 0 ) {
      errorFunc(25);
      fprintf(stderr, "ERROR: Unable to get own address information - %s\n", gai_strerror(intFailure));
      free(aiHints);
      freeaddrinfo(aiResults);
    }
    free(aiHints);
    // Setup socket
    sfdServer = socket(aiResults->ai_family, aiResults->ai_socktype, aiResults->ai_protocol);
    if ( sfdServer == -1 ) {
      errorFunc(18);
      freeaddrinfo(aiResults);
    }
    // Bind to socket
    intFailure = bind(sfdServer, aiResults->ai_addr, aiResults->ai_addrlen);
    if ( intFailure == -1 ) {
      errorFunc(26);
      freeaddrinfo(aiResults);
    }
    // Listen on socket
    intFailure = listen(sfdServer, MAX_NUM_PEND_CONNECT);
    if ( intFailure == -1 ) {
      errorFunc(27);
      freeaddrinfo(aiResults);
    }
    freeaddrinfo(aiResults);
    fprintf(stdout, "listening...\n");

    char * readBuffer;
    intFailure = close(pipe1[0]);
    if (intFailure==-1){errorFunc(32);}

    intFailure = close(pipe2[1]);
    if (intFailure==-1){errorFunc(32);}

    while (1){
      readBuffer = calloc(1024, 1);
        read(sfdClient, readBuffer, sizeof(readBuffer));
        write(pipe1[1], readBuffer, sizeof(readBuffer));
        free(readBuffer);
        readBuffer = calloc(1024, 1);
      while(read(pipe2[0], readBuffer, sizeof(readBuffer))!= EOF){
      intFailure = write(sfdClient, readBuffer, sizeof(readBuffer));
      if (intFailure == -1){errorFunc(37);}
    }
      free(readBuffer);
    }
  }
return 0;
}
