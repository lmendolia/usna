/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Assignment: Lab- 0x00: Password Complexity

	Description: This program should ask user for parameters of password complexity and ask for a password twice. Program verifies passwords match and then ensures password matches complexity 				requirements. 

	Resources: MGSP, Katie Swafford, Connor Mellett, Chris Kay, Jeff Gao, Adam Montgomery, Dylan Struthers & Whitman Groves-formatting questions
*/

//right now, the code is in a loop and won't exit. it is the only error i can see and i think if the loop was found, the code would run properly

// Required Includes
#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<strings.h>
#include<ctype.h>
#include<stdlib.h>
// Global Constants
#define MAX_PSWD_LEN 80

// Function Prototypes
int getInput(int *minLen, int *minUp, int *minLow, int *minNum, int *minSpec);
void getPassword(char *pswd1, char *pswd2);
int checkPassword(char *pswd1);
int verifyPassword(char *pswd1, char *pswd2);
int stringLen(char *pswd1, int minLen);
int checkUpper(char *pswd1, int minUp);
int checkLower(char *pswd1, int minLow);
int checkDigits(char *pswd1, int minNum);
int checkSpec(char *pswd1, int minSpec);
// main Function: <A general high-level description about what the function is designed to accomplish>
int main( int argc, char *argv[] ) {

// Declare Required Variables
char pswd1[MAX_PSWD_LEN];
char pswd2[MAX_PSWD_LEN];
int minLen, minUp, minLow, minNum, minSpec =0;
int returnValue = 0;
int checker = 0;

// Perform Function
returnValue = getInput(&minLen, &minUp, &minLow, &minNum, &minSpec);
if (returnValue != 0) {
perror("get Input failed");
}
else{
printf("Values are %d %d %d %d %d\n", minLen, minUp, minLow, minNum, minSpec);
}

getPassword(pswd1, pswd2);
//checkPassword returns an int indicating success or failure
checker = checkPassword(pswd1);
checker = stringLen(pswd1, minLen);

if (checker == 0){;}
else {printf("wrong length\n");
checker++;}

checker = checkUpper(pswd1, minUp);
if (checker == 0){;}
else {printf("wrong number of uppercase\n");
checker++;}

checker = checkLower(pswd1, minLow);
if (checker == 0){;}
else {printf("wrong number of lowercase\n");
checker++;}

checker = checkDigits(pswd1, minNum);
if (checker == 0){;}
else {printf("wrong number of digits\n");
checker++;}

checker = checkSpec(pswd1, minSpec);
if (checker == 0){;}
else {printf("wrong number of specials\n");
checker++;}

if (checker == 0){
return 0;}
else {exit(10);}
}

//GET INPUT function: asks for min numbers of len/up/low/num/spec
int getInput(int *minLen, int *minUp, int *minLow, int *minNum, int *minSpec) {
printf("Please enter minimum length/upper/lower/numbers/special characters: \n");
int returnValue = 0;
returnValue = scanf("%d%d%d%d%d", minLen, minUp, minLow, minNum, minSpec); // will scanf all the input at once

if (returnValue != 5){returnValue = -1;}
else returnValue = 0;
return returnValue;
}
//GET PASSWORD and verify match
void getPassword(char *pswd1, char *pswd2) {
printf("Please enter a password: ");
 scanf("%79s", pswd1);
printf("\nPlease enter your password again: ");
 scanf("%79s", pswd2);

printf("\nPassword 1 was: %s\nPassword 2 was: %s\n", pswd1, pswd2);
//return 0;
}

//check for no invalid characters in password
int checkPassword(char *pswd1){
  char invalid[] = {'\t','\n','\r','%','\"','\\'};
  int count1 = 0;
  int count2 = 0;

  while (count1 < strlen(pswd1)){
    count2 =0;
    while (count2 < strlen(invalid)){
	//for (int count2 = 0; count2 < length; count2++){
      if (invalid[count2] == pswd1[count1]){
        printf("Invalid characters: %c, check: %c\n", invalid[count2],pswd1[count1]);
        return 7;
      }
      count2++;
    }
    count1++;
  }
  return 0;
}
  //verify passwords are the same
int verifyPassword(char *pswd1, char *pswd2){  
  if (!strcmp(pswd1, pswd2)){
	//if (strcmp(pswd1, pswd2) != 0){
  return 0;}

  return 45;
}

  //check minimum length requirement
int stringLen(char *pswd1, int minLen){
  int length = strlen(pswd1);
  if (length < minLen){
      return -1;}
  return 0;
}

  //check Upper
int checkUpper(char *pswd1, int minUp){
  int count1 = 0;
  int count2 = 0;
  int length = strlen(pswd1);
  while (count1 < length){
    if (isupper(pswd1[count1]))
      count2++;
    count1++;
    if (count2 < minUp)
      return -1;
  }
  return 0;
}
//check Lower
int checkLower(char *pswd1, int minLow){
  int count1 = 0;
  int count2 = 0;
  int length = strlen(pswd1);
  while (count1 < length){
    if (islower(pswd1[count1]))
      count2++;
    count1++;
  }
  if (count2 < minLow)
    return -1;
  else
    return 0;
}
    // check Digits
int checkDigits(char *pswd1, int minNum){
  int count1 = 0;
  int count2 = 0;
  int length = strlen(pswd1);
  while (count1 < length){
    if (islower(pswd1[count1]))
      count2++;
    count1++;
  }
  if (count2 < minNum)
    return -1;
  else
    return 0;
}
//verify special character criteria
int checkSpec(char *pswd1, int minSpec){
  char spec[] = "`~@#$^*()_-+=?/<>;:[]{}";
  int count1 = 0;
  int count2 = 0;
  int length = strlen(pswd1);
  while (count1 < length) {
    for (int charac = 0; charac < strlen(spec); charac++){
      if (pswd1[count1] == spec[charac]){
        count2++;}
      else {continue;}
    }
    count1++;
  }
  if (count2 >= minSpec){
    return 0;}
  else {return 1;}
}

