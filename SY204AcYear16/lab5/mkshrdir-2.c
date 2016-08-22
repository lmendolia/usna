/*
	Name: MIDN Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab 0x04

	Description: Create directory specified by PATH and set access control lists,such that the users have RWX on directory & future files in directory.
	Resources: MGSP, EI
*/

// Required Includes
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <sys/acl.h>
#include <pwd.h>
#include <errno.h>
// Global Constants

// Function Prototypes
void usageFunction();
void tfacl(int argc, char *argv[]);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {
	// Declare Required Variables

	// Perform Function
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
//******************************************************************
//CALL TEXT FORM ACL FUNCTION
//******************************************************************
	int intFailure = -1;
	struct stat statFile;
	intFailure = stat(argv[1], &statFile);
	if (intFailure == -1){
		tfacl(argc, argv);

	}
	else{
		perror("directory already exists");
		exit(1);
	}
// Return to Caller
return 0;
}

//******************************************************************
//FUNCTION: TEXT FORM ACCESS CONTROL LIST
//******************************************************************
void tfacl(int argc, char *argv[]){
	char aclbuild[1024];
	aclbuild[0] = '\0';
	strncat(aclbuild,"u::", 1024- strlen(aclbuild)-1);
	int newDir;
	struct stat statFile;
	newDir = mkdir(argv[1], 0777); //used StackOverflow for syntax
	if (newDir == -1){
		perror("error with mkdir");
		exit(2);
	}
	if (errno == EACCES){
		perror("insufficient permissions");
		exit(3);
	}
	stat(argv[1], &statFile);
	mode_t mode = statFile.st_mode;//stat directory
	//*************************************************************
	//USER
	//*************************************************************
	if (mode & S_IRUSR){
		strncat(aclbuild, "r", 1024- strlen(aclbuild)-1);
	}
	else {strncat(aclbuild, "-",1024- strlen(aclbuild)-1 );}
	if (mode & S_IWUSR){
		strncat(aclbuild, "w",1024- strlen(aclbuild)-1);
	}
	else {strncat(aclbuild, "-",1024- strlen(aclbuild)-1 );}
	if (mode & S_IWUSR){
		strncat(aclbuild, "x",1024- strlen(aclbuild)-1);
	}
	else {strncat(aclbuild, "-",1024- strlen(aclbuild)-1 );}
	//******************************************************
	//ADDED USERS
	//******************************************************
	int count = 2;
	while (count < argc){
		struct passwd *pwReturn;
		pwReturn = getpwnam(argv[count]);
		if (pwReturn == NULL){
			printf("invalid username: %s\n", argv[count]);
			perror("invalid username");
			exit(4);
		}
		strncat(aclbuild, ",u:", 1024- strlen(aclbuild)-1);
		strncat(aclbuild, argv[count], 1024- strlen(aclbuild)-1);
		strncat(aclbuild, ":rwx", 1024- strlen(aclbuild)-1);
		count++;
	}
	//*****************************************************
	//GROUP
	//*****************************************************
	strncat(aclbuild, ",g::", 1024- strlen(aclbuild)-1);
	if (mode & S_IRGRP){
		strncat(aclbuild, "r", 1024- strlen(aclbuild)-1);
	}
	else{strncat(aclbuild, "-", 1024- strlen(aclbuild)-1);}
	if (mode & S_IWGRP){
		strncat(aclbuild, "w", 1024- strlen(aclbuild)-1);
	}
	else{strncat(aclbuild, "-", 1024- strlen(aclbuild)-1);}
	if (mode & S_IXGRP){
		strncat(aclbuild, "x", 1024- strlen(aclbuild)-1);
	}
	else{strncat(aclbuild, "-", 1024- strlen(aclbuild)-1);}
	//*****************************************************
	//OTHER
	//*****************************************************
	strncat(aclbuild, ",o::", 1024- strlen(aclbuild)-1);
	if (mode & S_IROTH){
		strncat(aclbuild, "r", 1024- strlen(aclbuild)-1);
	}
	else{strncat(aclbuild, "-", 1024- strlen(aclbuild)-1);}
	if (mode & S_IWOTH){
		strncat(aclbuild, "w", 1024- strlen(aclbuild)-1);
	}
	else{strncat(aclbuild, "-", 1024- strlen(aclbuild)-1);}
	if (mode & S_IXOTH){
		strncat(aclbuild, "x", 1024- strlen(aclbuild)-1);
	}
	else{strncat(aclbuild, "-", 1024- strlen(aclbuild)-1);}
	//printf("%s\n", aclbuild);
	//*****************************************************
	//BUILD ACL
	//*****************************************************
	acl_t textAcl;
	int maskreturn;
	int setfile;
	textAcl = acl_from_text(aclbuild);
	if (textAcl == NULL){
		perror("acl from text didn't work");
		exit(5);
	}
	maskreturn = acl_calc_mask(&textAcl);
	if (maskreturn == -1){
		perror("calc mask didn't work");
		exit(6);
	}
	setfile = acl_set_file(argv[1], ACL_TYPE_ACCESS, textAcl);
	if (setfile == -1){
		perror("set file didn't work");
		exit(7);
	}
	if (errno == EINVAL){
		perror("set file error");
		exit(8);
	}
	setfile = acl_set_file(argv[1], ACL_TYPE_DEFAULT, textAcl);
	if (setfile == -1){
		perror("set file didn't work");
		exit(7);
	}
	if (errno == EINVAL){
		perror("set file error");
		exit(8);
	}
}
//******************************************************************
void usageFunction(){
				printf("NAME\n \tMake shared directory\n");
				printf("SYNOPSIS\n \tmkshrdir <PATH> <USERNAME 1> ... [USERNAME N]\n");
				printf("DESCRIPTION\n\tCreate directory specified by PATH and set access control lists,\n\tsuch that the users (USERNAME 1 ... USERNAME N) have read, write,\n\tand execute permission on the directory, and such that the users\n\thave read, write, and execute permissions on future files created\n\tin the directory.\n");
				printf("\t-h: display this help\n");
				exit(0);//EXIT WITH NORMAL RETURN VALUE
			}
