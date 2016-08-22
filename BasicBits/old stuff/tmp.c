int lsHiding(int fd){
int intPause = 0;  // Variable to force pausing point within program, to support external testing
char buffer[1024];
int size = lseek(fd, 0, SEEK_END);
lseek(fd, 0, SEEK_SET);
read(fd, buffer, size);

if (read(fd, buffer, size) != 0){
  perror("read didn't work");
  exit(7);
}
fprintf(stdout, "Pause Point: File appears in ls listing\n");
scanf("%d", &intPause);  // Read in integer as forced pause point
// 1. Use Ctrl-Z to pause the program
// 2. Use ls -a to see the file within the ls listing
// 3. Use fg 1 (assumes this program is the only paused program) to resume the program
/*
 * End Hard Coded Pause Point: 1
 */
unlink(argv[1]);
fprintf( stdout, "Pause Point: File does not appear in ls listing, but is still open\n");
scanf("%d", &intPause);  // Read in integer as forced pause point
// 1. Use Ctrl-Z to pause the program
// 2. Use ls -a to see the file is not in the ls listing
// 3. Use fg 1 (assumes this program is the only paused program) to resume the program
 lseek(fd, 0, SEEK_SET);
 close(fd);
 int newfd;
 char *randNamFile = tempnam("/tmp", "FileXXXXXX");
 newfd = open(randNamFile, O_CREAT | O_WRONLY, S_IRWXU);
 if (newfd == -1){
   perror("open file 2 didn't work");
   exit(68);
 }
   write(newfd, buffer, size);
   write(newfd, "\n", 1);
   close(newfd);

return 0; //return to caller
}
