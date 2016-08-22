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
	struct stat statFile;
	int newDir;
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
	acl_t aclTarget;
	acl_entry_t entryAcl;
	acl_permset_t aclPerm;
	aclTarget = acl_init(0);
	if (aclTarget == NULL){
		perror("init didn't work");
		exit(4);
	}
	//***********************
	//add owner permissions
	int returnVal;
	int addperm;
	returnVal = acl_create_entry(&aclTarget, &entryAcl);
	if (returnVal == -1){
		perror("create entry didn't work");
		exit(5);
	}
	returnVal = acl_set_tag_type(entryAcl, ACL_USER_OBJ);
	if (returnVal == -1){
		perror("set tag type didn't work");
		exit(6);
	}
	returnVal = acl_get_permset(entryAcl, &aclPerm);
	if (returnVal == -1){
		perror("get permset (group) didn't work");
		exit(7);
	}
	if (mode & S_IRUSR){
		addperm = acl_add_perm(aclPerm, ACL_READ);
		if (addperm == -1){
			perror("read add didn't work");
			exit(8);
		}
	}
	if (mode & S_IWUSR){
		addperm = acl_add_perm(aclPerm, ACL_WRITE);
		if (addperm == -1){
			perror("read add didn't work");
			exit(9);
		}
	}
	if (mode & S_IWUSR){
		addperm = acl_add_perm(aclPerm, ACL_EXECUTE);
		if (addperm == -1){
			perror("read add didn't work");
			exit(10);
		}
	}
	//***********************
	//add all the user permissions
	int count = 2;
	while (count < argc){
		struct passwd *pwReturn;
		returnVal = acl_create_entry(&aclTarget, &entryAcl);
		if (returnVal == -1){
			perror("create entry didn't work");
			exit(11);
		}
		returnVal = acl_set_tag_type(entryAcl, ACL_USER);
		if (returnVal == -1){
			perror("set tag type didn't work");
			exit(12);
		}
		pwReturn = getpwnam(argv[count]);
		if (pwReturn == NULL){
			printf("invalid username: %s\n", argv[count]);
			perror("invalid username");
			exit(13);
		}
		returnVal = acl_set_qualifier(entryAcl, &pwReturn->pw_uid);
		if (returnVal == -1){
			perror("set qualifier didn't work");
			exit(14);
		}
		returnVal = acl_get_permset(entryAcl, &aclPerm);
		if (returnVal == -1){
			perror("get permset didn't work");
			exit(15);
		}
		addperm = acl_add_perm(aclPerm, ACL_READ);
		if (addperm == -1){
			perror("read add didn't work");
			exit(16);
		}
		addperm = acl_add_perm(aclPerm, ACL_WRITE);
		if (addperm == -1){
			perror("read add didn't work");
			exit(17);
		}
		addperm = acl_add_perm(aclPerm, ACL_EXECUTE);
		if (addperm == -1){
			perror("read add didn't work");
			exit(18);
		}
	//************************
	//add group permissions
	returnVal = acl_create_entry(&aclTarget,&entryAcl);
	if (returnVal == -1){
		perror("create entry didn't work");
		exit(19);
	}
	returnVal = acl_set_tag_type(entryAcl, ACL_GROUP_OBJ);
	if (returnVal == -1){
		perror("set tag type (group) didn't work");
		exit(20);
	}
	returnVal = acl_get_permset(entryAcl, &aclPerm);
	if (returnVal == -1){
		perror("get permset (group) didn't work");
		exit(21);
	}
	if (mode & S_IRUSR){
		addperm = acl_add_perm(aclPerm, ACL_READ);
		if (addperm == -1){
			perror("read add didn't work");
			exit(22);
		}
	}
	if (mode & S_IWUSR){
		addperm = acl_add_perm(aclPerm, ACL_WRITE);
		if (addperm == -1){
			perror("write add didn't work");
			exit(23);
		}
	}
	if (mode & S_IWUSR){
		addperm = acl_add_perm(aclPerm, ACL_EXECUTE);
		if (addperm == -1){
			perror("execute add didn't work");
			exit(24);
		}
	}
	//*************************
	//add other permissions
	returnVal = acl_create_entry(&aclTarget,&entryAcl);
	if (returnVal == -1){
		perror("create entry didn't work");
		exit(25);
	}
	returnVal = acl_set_tag_type(entryAcl, ACL_OTHER);
	if (returnVal == -1){
		perror("set tag type (other) didn't work");
		exit(26);
	}
	returnVal = acl_get_permset(entryAcl, &aclPerm); //ACL_USR & ACL_USR_OBJ
	if (returnVal == -1){
		perror("get permset (other) didn't work");
		exit(27);
	}
	if (mode & S_IRUSR){
		addperm = acl_add_perm(aclPerm, ACL_READ);
		if (addperm == -1){
			perror("read add didn't work");
			exit(28);
		}
	}
	if (mode & S_IWUSR){
		addperm = acl_add_perm(aclPerm, ACL_WRITE);
		if (addperm == -1){
			perror("write add didn't work");
			exit(29);
		}
	}
	if (mode & S_IWUSR){
		addperm = acl_add_perm(aclPerm, ACL_EXECUTE);
		if (addperm == -1){
			perror("execute add didn't work");
			exit(30);
		}
	}

	count++;
}
//****************
//end while loop
//****************
		int maskreturn;
		int setfile;
		maskreturn = acl_calc_mask(&aclTarget);
		if (maskreturn == -1){
			perror("calc mask didn't work");
			exit(31);
		}
		setfile = acl_set_file(argv[1], ACL_TYPE_ACCESS, aclTarget);
		if (setfile == -1){
			perror("set file didn't work");
			exit(32);
		}
		if (errno == EINVAL){
			perror("set file error");
			exit(33);
		}
		setfile = acl_set_file(argv[1], ACL_TYPE_DEFAULT, aclTarget);
		if (setfile == -1){
			perror("set file didn't work");
			exit(34);
		}
		if (errno == EINVAL){
			perror("set file error");
			exit(35);
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
