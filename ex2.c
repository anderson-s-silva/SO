/**
* @file ex2.c
* @author Anderson Soares da Silva
* @date 10 Mar 2022
* @brief  exercicio 2 aula posix
*/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

volatile sig_atomic_t flag = false;

void handler(int signum){
	flag = true;
}

void printsinc(char* msg, int segundos){
	signal(SIGALRM, handler);
	while(1){
		write(1, msg, strlen(msg));
		alarm(segundos);
		pause();
		flag = false;
	}
}

int main(){
	printsinc("rodando ", 4);
	
	return 0;	
}
