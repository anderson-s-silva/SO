/**
* @file ex1.c
* @author Anderson Soares da Silva
* @date 10 Mar 2022
* @brief  exercicio 1 aula posix
*/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void myprintf(int x, int y, char* msg){
	
	int size = strlen(msg);
    	char msg2[size+10];
    	strcpy(msg2,"\033[ B\033[ C");   
    
    	msg2[2]= x + '0';
   	msg2[6]= y + '0';
   	
   	strcat(msg2, msg);
   	write(1, msg2, strlen(msg2));
}

int main(){
	
	char *msg = "hello";
	myprintf(2,7,msg);
	
	return 0;
}
