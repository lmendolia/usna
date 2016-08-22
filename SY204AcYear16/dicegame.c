#include<stdio.h>
#include<stddef.h>

void askUser(int * ptr1);
int main(int argc, char *argv[]){

	int intReturn = 0; //assume normal exit
	char *userInput = "";
	//char userInput[];
	askUser(&userInput);

	printf("User wanted %d dice. \n", userInput);


return intReturn;

}

void askUser(char *ptr1){
printf("What would you like as your password? ");
scanf("%80s", ptr1);

}
