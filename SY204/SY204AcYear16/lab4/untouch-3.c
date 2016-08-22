/*
 * Name: MIDN Mendolia
 * Alpha: 184242
 * Course: SY204 - Sys Prgm & OS Fund
 * Assignment: Lab 0x03- Untouchables
 *
 * Description: Code will open file and append data to it, but last access time/modification time displayed will remain as if they had not been modified.
 *
 * Resources: MGSP, EI, class notes, Dylan Struthers, and Katie Swafford
 */
// Required Includes
#include <getopt.h>  // Get Options Library: optarg, opterr, optind, optopt, getopt(3) && std=c99
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <utime.h>
// Global Constants

// Function Prototypes
void usageFunction();
int stat(const char *path, struct stat *buf);

// Main function <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
	// Declare Required Variables
	int opt;
	while ((opt = getopt(argc, argv, "h")) != -1){
      if (opt == 'h'){
        usageFunction(); break;
        return 0;
			}
	}
			if (argc  <= 1){
			  usageFunction();
        return 0;
			}
				//*******************************************************
				int intFailure = -1;
				struct stat statFileAttrs;
				intFailure = stat(argv[1], &statFileAttrs);
				if (intFailure == -1){
					intFailure = stat("./", &statFileAttrs);
					open(argv[1], O_CREAT, S_IRUSR|S_IWUSR);
				}
				struct timespec atime = statFileAttrs.st_atim; //time_t
				struct timespec mtime = statFileAttrs.st_mtim; //time_t
				struct timespec *oTimes;
				oTimes = calloc(2, sizeof(struct timespec)); //zeroes it
				oTimes[0] = atime;
				oTimes[1] = mtime;
				//nTimes.tv_nsec = atime;
				//nTimes.tv_nsec = mtime;
				//********************************************************
				int fd = open(argv[1],O_WRONLY);
				int size = lseek(fd,0, SEEK_END);
				if(size == -1){
					perror("lseek didn't work");
					exit(423);
				}
				char * letter = "z";
				if (fd == -1){
			    perror("open didn't work");
					exit(89);
				}
				else{
					int returnVal;
					returnVal =  write(fd, letter, 1);
					if (returnVal == -1){
						perror("write didn't work");
						exit(121);
					}
					returnVal= futimens(fd, oTimes);
					if (returnVal == -1){
						perror("time change didn't work");
						exit(342);
					}
					returnVal = close(fd);
					if (returnVal == -1){
						perror("close didn't work");
						exit(32);
					}
					free(oTimes);
			}
				//***********************************************************
				/*
				the following block of code was taken from course notes: stat-demo.c
				*/
				if (! intFailure){
					printf("Last Access Time: %s", ctime(&statFileAttrs.st_atime));
					printf("Last Modification Time: %s", ctime(&statFileAttrs.st_mtime));
				}
				else{
					perror("stat failed :(");
					exit(64);
				}

	// Return to Caller
	return 0;
}

void usageFunction(){
				printf("NAME\n \tuntouch = modify regular file but do not change access time or modify time\n");
				printf("SYNOPSIS\n \tuntouch FILE\n");
				printf("DESCRIPTION\n\tmodify FILE (regular file) but do not change the last accessed and last modified times.\n\tIf FILE does not exist...\n\t\tcreate FILE assignning last accessed\n\t\tand last modified times of the parent directory.\n\tIf the command line argument is not a regular file:\n\t\tno actions are taken, and the program exits normally.\n");
				printf("\t-h: display this help\n");
				exit(0);//EXIT WITH NORMAL RETURN VALUE
			}
